#include "gspace.h"

char *author	=AUTHOR;
char *package	=PACKAGE;

u64 isInit = 0;

var *V_src;
var *V_program;
var *V_buffer;
var *V_texture;
var *V_sampler;
var *V_vertexAttrib;
var *V_model;

void init(void)
{
	if _oF(!(V_src=base->create_vmat(root, S_src, 0, auth_read))) goto Err;
	if _oF(!(V_program=base->create_vmat(V_src, S_program, 0, auth_read))) goto Err;
	if _oF(!(V_buffer=base->create_vmat(V_src, S_buffer, 0, auth_read))) goto Err;
	if _oF(!(V_texture=base->create_vmat(V_src, S_texture, 0, auth_read))) goto Err;
	if _oF(!(V_sampler=base->create_vmat(V_src, S_sampler, 0, auth_read))) goto Err;
	if _oF(!(V_vertexAttrib=base->create_vmat(V_src, S_vertexAttrib, 0, auth_read))) goto Err;
	if _oF(!(V_model=base->create_vmat(V_src, S_model, 0, auth_read))) goto Err;
	
	
	return ;
	Err:
	throw_errid(errid_MemLess);
}

void uini(void)
{
	void FreeSrc_program(var *v);
	void FreeSrc_buffer(var *v);
	void FreeSrc_texture(var *v);
	void FreeSrc_sampler(var *v);
	void FreeSrc_vertexAttrib(var *v);
	
	base->clear_vmsrc(V_src, S_program, FreeSrc_program);
	base->clear_vmsrc(V_src, S_buffer, FreeSrc_buffer);
	base->clear_vmsrc(V_src, S_texture, FreeSrc_texture);
	base->clear_vmsrc(V_src, S_sampler, FreeSrc_sampler);
	base->clear_vmsrc(V_src, S_vertexAttrib, FreeSrc_vertexAttrib);
	
	base->var_delete(root, S_src);
}

void FreeSrc_program(var *v)
{
	glDeleteProgram(v->v.v_long);
}

void FreeSrc_buffer(var *v)
{
	GLuint b = v->v.v_long;
	glDeleteBuffers(1, &b);
}

void FreeSrc_texture(var *v)
{
	GLuint t = v->v.v_long;
	glDeleteTextures(1, &t);
}

void FreeSrc_sampler(var *v)
{
	GLuint t = v->v.v_long;
	glDeleteSamplers(1, &t);
}

void FreeSrc_vertexAttrib(var *v)
{
	var *vp;
	GLuint b;
	
	vp = base->var_find(v, S_handle);
	if _oT(vp)
	{
		b = vp->v.v_long;
		glDeleteVertexArrays(1, &b);
	}
}

