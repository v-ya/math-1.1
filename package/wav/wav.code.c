#include "wav.h"
#include "wav.code.h"
#include "wav.string.h"

static vya_wav_hi _tp_wav_hi = {
	.RIFF[0]='R',
	.RIFF[1]='I',
	.RIFF[2]='F',
	.RIFF[3]='F',
	.fsz=36,		// += wav.size*SamBytes
	.WAVE[0]='W',
	.WAVE[1]='A',
	.WAVE[2]='V',
	.WAVE[3]='E',
	.Label_fmt[0]='f',
	.Label_fmt[1]='m',
	.Label_fmt[2]='t',
	.Label_fmt[3]=' ',
	.fmt_sz=16,
	.format=1,
	.channel=1,
	.samp_f=SamFre,
	.samp_bf=SamFre*SamBytes,
	.align=SamBytes,
	.depth=SamBits,
	.Label_data[0]='d',
	.Label_data[1]='a',
	.Label_data[2]='t',
	.Label_data[3]='a',
	.data_sz=0,		// = wav.size*SamBytes
} ;

vya_wav* wav_new(double time)
{
	vya_wav *wav;
	if _oF(time<=0||time>_phy_maxtime) return NULL;
	wav=malloc(sizeof(vya_wav));
	if _oF(!wav) return NULL;
	wav->size=(u32)(time*SamFre);
	wav->data=malloc(wav->size*SamBytes);
	if _oF(!wav->data) 
	{
		free(wav);
		return NULL;
	}
	memset(wav->data,0,wav->size*SamBytes);
	return wav;
}

vya_wav* wav_load(char **path)
{
	FILE *fp;
	u32 label,size,s_start,s_end,s_this,data;
	u32 samp_f,align,i;
	vya_wav *wav;
	double l1,l2,r,k;
	
	fp=fopen(*path,"rb");
	if _oF(!fp) goto Err_open;
	#define _ic4(c0,c1,c2,c3) ((u32)(c0)|(c1<<8)|(c2<<16)|(c3<<24))
	s_start=0;
	fseek(fp,0,SEEK_END);
	s_end=ftell(fp);
	fseek(fp,0,SEEK_SET);
	// check 'RIFF'
	if _oF(s_end<8) goto Err_format;
	if _oF(fread(&label,4,1,fp)<1) goto Err_open;
	if _oF(label!=_ic4('R','I','F','F')) goto Err_format;
	if _oF(fread(&size,4,1,fp)<1) goto Err_open;
	s_start=ftell(fp);
	if _oF((s_start+size)<s_end) s_end=s_start+size;
	// check 'WAVE'
	if _oF((s_this+4)>s_end) goto Err_format;
	if _oF(fread(&label,4,1,fp)<1) goto Err_open;
	if _oF(label!=_ic4('W','A','V','E')) goto Err_format;
	s_start=ftell(fp);
	if _oF((s_start+size)<s_end) s_end=s_start+size;
	// find first 'fmt '
	s_this=s_start;
	while(1)
	{
		if _oF((s_this+8)>s_end) goto Err_format;
		if _oF(fread(&label,4,1,fp)<1) goto Err_open;
		if _oF(fread(&size,4,1,fp)<1) goto Err_open;
		if _oT((label==_ic4('f','m','t',' '))&&(size==0x10)) break;
		s_this+=8+size;
	}
	s_start=s_this+8+size;
	// read 'fmt '
		// .format && .channel
	if _oF(fread(&data,4,1,fp)<1) goto Err_open;
		// .format==1 && .channel==1 (PCM && one channel)
	if _oF(data!=0x00010001) goto Err_format;
		// .samp_f, .samp_bf, .align & .depth
	if _oF(fread(&samp_f,4,1,fp)<1) goto Err_open;
	if _oF(fread(&data,4,1,fp)<1) goto Err_open;
	if _oF(fread(&align,4,1,fp)<1) goto Err_open;
		// check .samp_bf
	if _oF(data!=(samp_f*(align&0xffff))) goto Err_format;
	data=align>>16;
	align&=0xffff;
		// check .depth
	if _oF(data!=(align*8)) goto Err_format;
		// check .align
	if _oF(align!=1&&align!=2&&align!=4) goto Err_format;
	// find 'data'
	s_this=s_start;
	if _oF((s_this+8)>s_end) goto Err_format;
	fseek(fp,s_this,SEEK_SET);
	while(1)
	{
		if _oF((s_this+8)>s_end) goto Err_format;
		if _oF(fread(&label,4,1,fp)<1) goto Err_open;
		if _oF(fread(&size,4,1,fp)<1) goto Err_open;
		if _oT(label==_ic4('d','a','t','a')) break;
		s_this+=8+size;
	}
	s_start=s_this+8;
	s_end=s_start+size;
	// new wav
	size/=align;
		// check time
	if _oF(((double)size/samp_f)>_phy_maxtime) goto Err_time;
		// alloc wav
	wav=malloc(sizeof(vya_wav));
	if _oF(!wav) goto Err_mem;
		// calc wav->size
	wav->size=(u32)(((double)size/samp_f)*SamFre);
		// alloc wav->data
	wav->data=malloc(wav->size*SamBytes);
	if _oF(!wav->data) 
	{
		free(wav);
		goto Err_mem;
	}
	// read 'data' && set wav
	k=(double)samp_f/SamFre;
	s_this=0;
	fseek(fp,s_start,SEEK_SET);
	switch(align)
	{
		case 1:
			for(i=0;i<wav->size;i++)
			{
				r=i*k;
				align=(u32)r;
				r-=align;
				align+=2;
				if _oF(align>size)
				{
					align=size;
					r=1;
				}
				while(s_this<align)
				{
					data=getc(fp)<<24;
					l1=l2;
					l2=(s32)data;
					s_this++;
				}
				r=l1*(1-r)+l2*r;
				r=(r>0)?(r+0.5):(r<0)?(r-0.5):0;
				wav->data[i]=(s32)r;
			}
			break;
		case 2:
			for(i=0;i<wav->size;i++)
			{
				r=i*k;
				align=(u32)r;
				r-=align;
				align+=2;
				if _oF(align>size)
				{
					align=size;
					r=1;
				}
				while(s_this<align)
				{
					fread(&data,2,1,fp);
					data<<=16;
					l1=l2;
					l2=(s32)data;
					s_this++;
				}
				r=l1*(1-r)+l2*r;
				r=(r>0)?(r+0.5):(r<0)?(r-0.5):0;
				wav->data[i]=(s32)r;
			}
			break;
		case 4:
			for(i=0;i<wav->size;i++)
			{
				r=i*k;
				align=(u32)r;
				r-=align;
				align+=2;
				if _oF(align>size)
				{
					align=size;
					r=1;
				}
				while(s_this<align)
				{
					fread(&data,4,1,fp);
					l1=l2;
					l2=(s32)data;
					s_this++;
				}
				r=l1*(1-r)+l2*r;
				r=(r>0)?(r+0.5):(r<0)?(r-0.5):0;
				wav->data[i]=(s32)r;
			}
			break;
	}
	#undef _ic4
	return wav;
	Err:
	if (fp) fclose(fp);
	return NULL;
	Err_open:
	*path=error_load_path;
	goto Err;
	Err_format:
	*path=error_load_format;
	goto Err;
	Err_mem:
	*path=NULL;
	goto Err;
	Err_time:
	*path=error_load_time;
	goto Err;
}

void wav_free(vya_wav *wav)
{
	free(wav->data);
	free(wav);
}

int wav_write(vya_wav *wav, char *path)
{
	FILE *fp;
	u32 i;
	char *d;
	vya_wav_hi hi;
	memcpy(&hi,&_tp_wav_hi,sizeof(vya_wav_hi));
	hi.data_sz=wav->size*SamBytes;
	hi.fsz+=hi.data_sz;
	fp=fopen(path,"wb");
	if (!fp) return 1;
	fwrite(&hi,sizeof(vya_wav_hi),1,fp);
	d=(char*) wav->data;
	for(i=hi.data_sz;i>0;i--)
		putc(*(d++),fp);
	fclose(fp);
	return 0;
}

// 用户输入值转响度幅值
s32 loudness(double f)
{
	static s32 _max_plus=0x7fffffff, _max_neg=0x80000000;
	extern var *_loudmode_;
	s32 value;
	switch(_loudmode_->v.v_int)
	{
		case loudmode_no:
			label:
			value=(s32) f;
			value=(f<0)?((f<_max_neg)?_max_neg:value):((f>_max_plus)?_max_plus:value);
			return value;
		case loudmode_ra:
			f=(f<-1)?(-1):((f>1)?1:f);
			value=(s32) (f*_max_plus);
			return value;
		case loudmode_db_atte:
			if _oF(f<0)
			{
				f=-f;
				value=-1;
			}
			else value=1;
			f=pow(10,f/20);
			value*=(s32) (_max_plus/f);
			return value;
		default:
			goto label;
	}
}

// 响度幅值转用户期待值
double loudexpe(s32 value)
{
	static s32 _max_plus=0x7fffffff;
	extern var *_loudmode_;
	double f;
	if _oF(value==0x80000000) value++;
	switch(_loudmode_->v.v_int)
	{
		case loudmode_no:
			label:
			return value;
		case loudmode_ra:
			f=value;
			f/=_max_plus;
			return f;
		case loudmode_db_atte:
			if _oF(value>0) f=1;
			else if _oF(value<0)
			{
				f=-1;
				value=-value;
			}
			else return 20*log10((double)_max_plus*2);
			f*=20*log10((double)_max_plus/value);
			return f;
		default:
			goto label;
	}
}


void clear(vya_wav *wav, u32 b, s32 t)
{
	s32 *d;
	u32 e;
	d=wav->data;
	e=b+t;
	if _oT(t>0)
	{
		if _oF(e<b||e>wav->size) e=wav->size;
		while(b<e)
		{
			d[b]=0;
			b++;
		}
	}
	else if _oT(t<0)
	{
		if _oF(e>b)
		{
			while(b>=0)
			{
				d[b]=0;
				b--;
			}
		}
		while(b>e)
		{
			d[b]=0;
			b--;
		}
	}
}

static double _pi2=M_PI*2;

void rsin_inc(vya_wav *wav, u32 n, s32 loud, u32 b, s32 t)
{
	s32 *d;
	u32 e,i=0;
	double w;
	d=wav->data;
	e=b+t;
	if _oT(t>0)
	{
		w=_pi2/t*n;
		if _oF(e<b||e>wav->size) e=wav->size;
		while(b<e)
		{
			d[b]+=loud*sin(w*i++);
			b++;
		}
	}
	else if _oT(t<0)
	{
		w=_pi2/(-t)*n;
		if _oF(e>b) e=0;
		while(b>e)
		{
			d[b]+=loud*sin(w*i++);
			b--;
		}
	}
}

void rcos_inc(vya_wav *wav, u32 n, s32 loud, u32 b, s32 t)
{
	s32 *d;
	u32 e,i=0;
	double w;
	d=wav->data;
	e=b+t;
	if _oT(t>0)
	{
		w=_pi2/t*n;
		if _oF(e<b||e>wav->size) e=wav->size;
		while(b<e)
		{
			d[b]+=loud*cos(w*i++);
			b++;
		}
	}
	else if _oT(t<0)
	{
		w=_pi2/(-t)*n;
		if _oF(e>b) e=0;
		while(b>e)
		{
			d[b]+=loud*cos(w*i++);
			b--;
		}
	}
}

s32 rsin_get(vya_wav *wav, u32 n, u32 b, s32 t)
{
	s32 *d;
	u32 e,i=0;
	double w,loud=0;
	d=wav->data;
	e=b+t;
	if _oF(!n) return 0;
	if _oT(t>0)
	{
		w=_pi2/t*n;
		if _oF(e<b||e>wav->size) e=wav->size;
		while(b<e)
		{
			loud+=d[b]*sin(w*i++);
			b++;
		}
		loud=loud/t*2;
	}
	else if _oT(t<0)
	{
		w=_pi2/(-t)*n;
		if _oF(e>b) e=0;
		while(b>e)
		{
			loud+=d[b]*sin(w*i++);
			b--;
		}
		loud=loud/(-t)*2;
	}
	return (s32) loud;
}

s32 rcos_get(vya_wav *wav, u32 n, u32 b, s32 t)
{
	s32 *d;
	u32 e,i=0;
	double w,loud=0;
	d=wav->data;
	e=b+t;
	if _oT(t>0)
	{
		w=_pi2/t*n;
		if _oF(e<b||e>wav->size) e=wav->size;
		while(b<e)
		{
			loud+=d[b]*cos(w*i++);
			b++;
		}
		loud=loud/t*2;
	}
	else if _oT(t<0)
	{
		w=_pi2/(-t)*n;
		if _oF(e>b) e=0;
		while(b>e)
		{
			loud+=d[b]*cos(w*i++);
			b--;
		}
		loud=loud/(-t)*2;
	}
	if _oF(!n) return (s32) (loud/2);
	return (s32) loud;
}

double _noise_k(u32 j)
{
	u32 g,h,i;
	double k,k1;
	k=0;
	for(i=0;i<=j;i++)
	{
		g=(2*i>j)?i:j-i;
		k1=1;
		for(h=g+1;h<=j;h++)
			k1*=h;
		for(h=j-g;h>0;h--)
			k1/=h;
		k+=((double)k1)*k1;
	}
	return k;
}

double noise(double *sa, u32 n)
{
	double e=0,e1=0,e2,ei,k=1;
	u32 ni,i,j=0;
	extern var *_lim_noise_ni;
	extern var *_lim_noise_ri;
	extern var *_lim_noise_ei;
	
	ni=n*fabs(_lim_noise_ri->v.v_float);
	if _oF(_lim_noise_ni->v.v_int<ni) ni=_lim_noise_ni->v.v_int;
	ei=fabs(_lim_noise_ei->v.v_float);
	if _oF(!ni) ni++;
	
	while(j<ni)
	{
		e2=e1;
		e1=e;
		if _oF(!(--n)) return e;
		k=sqrt(_noise_k(j)/_noise_k(j+1));
		j++;
		for(i=0;i<n;i++)
			sa[i]=(sa[i+1]-sa[i])*k;
		e=0;
		for(i=0;i<n;i++)
			e+=sa[i]*sa[i];
		e=sqrt(e/n);
		if _oF((fabs(e-e1)/e<ei)&&((fabs(e-e2)/e<ei))) break;
	}
	return e;
}

double find_nextzero(s32 *data, double start, u32 end)
{
	u32 i;
	s32 v;
	i=floor(start)+1;
	if _oF(i>=end) goto Not;
	if _oF(data[i]==0) return i;
	v=(i-start)*data[i-1]+(start+1-i)*data[i];
	if _oT(v>0) goto _pos;
	else if _oT(v<0) goto _neg;
	if _oF(data[i]<0) goto _neg;
	_pos:
	for(;i<end;i++)
		if _oF(data[i]<=0) goto End;
	goto Not;
	_neg:
	for(;i<end;i++)
		if _oF(data[i]>=0) goto End;
	goto Not;
	End:
	if _oF(data[i]==0) return i;
	start=data[i]-data[i-1];
	return i-(data[i]/start);
	Not:
	return -1;
}

double aloud(s32 *data, double start, double end)
{
	u32 s,e;
	s=ceil(start);
	e=ceil(end);
	if _oF(s>=e) return 0;
	end=e-s;
	for(start=0;s<e;s++)
		start+=(double)data[s]*data[s];
	return sqrt(start/end);
}

double spwav_like(s32 *data, double t0, double t1, double t2, double a1, double a2)
{
	u32 i,n,j;
	double like,tk,i1,v1;
	i=ceil(t1);
	n=ceil(t2);
	if _oF(i>=n) return 0;
	tk=(t1-t0)/(t2-t1);
	for(like=0;i<n;i++)
	{
		i1=(i-t1)*tk+t0;
		j=i1;
		v1=(i1-j)*data[j+1]+(j+1-i1)*data[j];
		v1=(data[i]/a2)-(v1/a1);
		like+=v1*v1;
	}
	like/=n-ceil(t1);
	return sqrt(like);
}

void spwav_get(vya_wav_spwav *sp)
{
	double etmax,eamax,elmax,size;
	double a,ts,te,tne,tne_a,tne_b;
	double a1,l1,te1,at,lt;
	s32 *data;
	
	if _oF(*(sp->status)<0) return ;
	
	data=sp->wav->data;
	etmax=*(sp->etmax);
	eamax=*(sp->eamax);
	elmax=*(sp->elmax);
	size=*(sp->tb);
	ts=*(sp->ts);
	te=*(sp->te);
	a=aloud(data,ts,te);
	
	if _oF(etmax<0||eamax<0||elmax<0||te>=size) goto Not;
	tne_a=te+(te-ts)*(1.0-etmax);
	tne_b=te+(te-ts)*(1.0+etmax);
	if _oF(tne_a<te) tne_a=te;
	if _oF(tne_b>size) tne_b=size;
	
	te1=-1;
	for(tne=find_nextzero(data,te,tne_b);tne>0;tne=find_nextzero(data,tne,tne_b))
	
	if _oT(tne>=tne_a)
	{
		at=aloud(data,te,tne);
		if _oT(fabs(at/a-1)<=eamax)
		{
			lt=spwav_like(data,ts,te,tne,a,at);
			if _oF(lt>elmax) continue;
			if _oF(te1<0||lt<l1)
			{
				_set:
				te1=tne;
				a1=at;
				l1=lt;
			}
			else if _oF(lt==l1)
			{
				if _oF(fabs(tne-te*2+ts)<fabs(te1-te*2+ts)) goto _set;
			}
		}
	}
	
	if _oT(te1<0) goto Not;
	*(sp->ts)=te;
	*(sp->te)=te1;
	*(sp->aloud)=loudexpe(a1);
	*(sp->like)=l1;
	*(sp->et)=(te1-te)/(te-ts)-1;
	*(sp->ea)=a1/a-1;
	return ;
	Not:
	*(sp->status)=-1;
	return ;
}

void spwav_init(vya_wav_spwav *sp)
{
	double ts,te,size,like,tmax,tmin,amin,te1;
	s32 *data;
	
	data=sp->wav->data;
	ts=*(sp->ta);
	size=*(sp->tb);
	tmax=*(sp->tmax);
	tmin=*(sp->tmin);
	amin=loudness(*(sp->amin));
	if _oF(ts<0) ts=0;
	if _oF(ts>size) goto Not;
	
	ts=find_nextzero(data,ts,size);
	while((te=find_nextzero(data,ts,size))>0)
	{
		if (aloud(data,ts,te)>amin) break;
		ts=te;
	}
	
	like=-1;
	tmin+=ts;
	tmax+=ts;
	if _oF(tmax>size) tmax=size;
	for(;te>0;te=find_nextzero(data,te,tmax))
	if _oT(te>tmin)
	{
		*(sp->status)=0;
		*(sp->ts)=ts;
		*(sp->te)=te;
		spwav_get(sp);
		if _oF(*(sp->status)<0) continue;
		if _oF(like<0||*(sp->like)<like)
		{
			te1=te;
			like=*(sp->like);
		}
	}
	
	if _oF(like<0) goto Not;
	*(sp->status)=0;
	*(sp->ts)=ts;
	*(sp->te)=te1;
	*(sp->aloud)=loudexpe(aloud(data,ts,te1));
	*(sp->like)=like;
	*(sp->et)=0;
	*(sp->ea)=0;
	return ;
	Not:
	*(sp->status)=-1;
	*(sp->ts)=*(sp->ta);
	*(sp->te)=size;
	*(sp->aloud)=loudexpe(aloud(data,*(sp->ta),size));
	*(sp->like)=0;
	*(sp->et)=0;
	*(sp->ea)=0;
	return ;
}

