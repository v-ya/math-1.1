#include "wav.h"
#include "wav.code.h"

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
	wav->size=(u32)time*SamFre;
	wav->data=malloc(wav->size*SamBytes);
	if _oF(!wav->data) 
	{
		free(wav);
		return NULL;
	}
	memset(wav->data,0,wav->size*SamBytes);
	return wav;
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


