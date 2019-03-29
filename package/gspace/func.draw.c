
func(drawClear)
{
	static char *label=label_name("drawClear");
	static u32 type_1[1]={type_void|type_znum};
	var *vp;
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	vp = argv->v;
	
	if _oT(vp->type&type_void) glClear(VAR_ClearBit->v.v_long);
	else glClear(vp->v.v_long);
	
	ret->type=type_void;
	return ret;
}

func(drawFlush)
{
	static char *label=label_name("drawFlush");
	static u32 type_1[1]={type_void};
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	glFlush();
	
	ret->type=type_void;
	return ret;
}

func(drawFinish)
{
	static char *label=label_name("drawFinish");
	static u32 type_1[1]={type_void};
	
	if _oF(argc!=1||base->check_varlist(argv,1,type_1)) return base->get_error(errid_FunArgvType,label);
	
	glFinish();
	
	ret->type=type_void;
	return ret;
}

func(drawArrays)
{
	static char *label=label_name("drawArrays");
	static u32 type_3[3]={type_znum, type_znum, type_znum};
	static u32 type_4[4]={type_znum, type_znum, type_znum, type_znum};
	u64 sid;
	u32 begin, count, mode;
	
	switch(argc)
	{
		case 3:
			if _oF(base->check_varlist(argv,3,type_3)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			begin = 0;
			break;
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			sid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	ret->type=type_void;
	
	if _oF(!isDrawMode(mode)) return ret;
	if _oF(useVertexAttrib(sid)) return ret;
	
	glDrawArrays(mode, begin, count);
	
	return ret;
}

func(drawElements)
{
	static char *label=label_name("drawElements");
	static u32 type_4[4]={type_znum, type_znum, type_znum, type_znum};
	static u32 type_5[5]={type_znum, type_znum, type_znum, type_znum, type_znum};
	static u32 type_6[6]={type_znum, type_znum, type_znum, type_znum, type_znum, type_znum};
	u64 vid, eid;
	u32 begin, count, mode;
	s32 pbase;
	
	switch(argc)
	{
		case 4:
			if _oF(base->check_varlist(argv,4,type_4)) return base->get_error(errid_FunArgvType,label);
			vid = argv->v->v.v_long;
			argv = argv->r;
			eid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			begin = 0;
			break;
		case 5:
			if _oF(base->check_varlist(argv,5,type_5)) return base->get_error(errid_FunArgvType,label);
			vid = argv->v->v.v_long;
			argv = argv->r;
			eid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			break;
		case 6:
			if _oF(base->check_varlist(argv,6,type_6)) return base->get_error(errid_FunArgvType,label);
			vid = argv->v->v.v_long;
			argv = argv->r;
			eid = argv->v->v.v_long;
			argv = argv->r;
			mode = argv->v->v.v_long;
			argv = argv->r;
			begin = argv->v->v.v_long;
			argv = argv->r;
			count = argv->v->v.v_long;
			argv = argv->r;
			pbase = argv->v->v.v_long;
			break;
		default:
			return base->get_error(errid_FunArgvType,label);
	}
	
	ret->type=type_void;
	
	if _oF(!isDrawMode(mode)) return ret;
	if _oF(useVertexAttrib(vid)) return ret;
	if _oF(useBuffer(eid, srcBufferTypeVertexIndices)) return ret;
	
	if _oT(argc<6) glDrawElements(mode, count, GL_UNSIGNED_INT, (GLvoid *)(((u64)begin) * sizeof(GLuint)));
	else glDrawElementsBaseVertex(mode, count, GL_UNSIGNED_INT, (GLvoid *)(((u64)begin) * sizeof(GLuint)), pbase);
	
	return ret;
}

