#include "gspace.h"

var* createSrc(var *src, u32 tlog, u64 *gen)
{
	u64 sid;
	var *vp;
	
	*gen = 0;
	do
	{
		sid=base->get_sid();
	}
	while(base->var_find_index(src, sid));
	vp = base->create_var(src, NULL, sid, tlog, 0, auth_read);
	
	*gen = sid;
	if _oF(!vp) return NULL;
	return vp;
}

GLuint getHandle(var *src, u64 sid, u32 isok, u32 *mode)
{
	var *vp;
	if _oF(!(vp=base->var_find_index(src, sid))) return 0;
	
	if _oF(isok & F_type)
	{
		if _oF((vp->mode&F_type) != (isok&F_type)) return 0;
	}
	if _oT(isok & F_isok)
	{
		if _oF(!(vp->mode&F_isok)) return 0;
	}
	else
	{
		if _oF(vp->mode&F_isok) return 0;
	}
	
	if _oF(mode) *mode = vp->mode;
	
	return vp->v.v_long;
}

void setHandleOk(var *src, u64 sid)
{
	var *vp;
	if _oT(vp=base->var_find_index(src, sid))
	{
		vp->mode|=F_isok;
	}
}

u32 getHandleType(var *src, u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(src, sid);
	if _oT(vp) return F_type_g(vp->mode);
	else return (u32)-1;
}

void referSrcHandle(var *src, u64 sid)
{
	var *vp;
	
	vp = base->var_find_index(src, sid);
	if _oT(vp) base->var_save(vp);
}

int testSrcRefer(var *pool, u64 sid)
{
	return base->var_find_index(pool, sid)?1:0;
}

int createSrcRefer(var *pool, var *src, u64 sid, u64 value)
{
	var *vp;
	
	if _oF(base->var_find_index(pool, sid)) return 0;
	if _oT(base->create_ulong(pool, NULL, sid, auth_read, value))
	{
		if _oT(src) referSrcHandle(src, sid);
		return 1;
	}
	else return -1;
}

void deleteSrcRefer(var *pool, DeleteSrcFunc f, u64 sid)
{
	if _oT(base->var_find_index(pool, sid))
	{
		base->var_delete_index(pool, sid);
		if _oT(f) f(sid);
	}
}

#include "code.program.c"
#include "code.buffer.c"
#include "code.vertexAttributes.c"
#include "code.model.c"
#include "code.draw.c"

void init_window(char *title, int width, int height)
{
	static int argc=1;
	static char *argv[1]={"Gspace"};
	glutInit(&argc,argv);
	glutInitDisplayMode(VAR_DisplayMode->v.v_long);
	glutInitWindowSize(width,height);
	glutInitContextVersion(VAR_OpenglMajor->v.v_long, VAR_OpenglMinor->v.v_long);
	glutInitContextProfile(VAR_OpenglProfile->v.v_long);
	if (title) glutCreateWindow(title);
	else glutCreateWindow(argv[0]);
	glewInit();
	// 深度检测
	glEnable(GL_DEPTH_TEST);
	// 重启动
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xffffffff);
	// 边缘线不被遮挡
	glPolygonOffset(2.0,2.0);
	glLineWidth(2.0);
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glFlush();
	
	INFO_OpenglVendor->v.v_string			= (char *) glGetString(GL_VENDOR);
	INFO_OpenglRenderer->v.v_string			= (char *) glGetString(GL_RENDERER);
	INFO_OpenglVersion->v.v_string			= (char *) glGetString(GL_VERSION);
	INFO_OpenglShadingLanguageVersion->v.v_string	= (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
	INFO_OpenglExtensions->v.v_string		= (char *) glGetString(GL_EXTENSIONS);
	
	isInit = 1;
}

