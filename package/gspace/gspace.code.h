#ifndef	_package_vya_gspace_code_h_
#define	_package_vya_gspace_code_h_

typedef void (*DeleteSrcFunc)(u64);

var* createSrc(var *src, u32 tlog, u64 *gen);
GLuint getHandle(var *src, u64 sid, u32 isok, u32 *mode);
var* getObject(var *src, u64 sid, u32 isok, u32 *mode);
GLuint getObjectHandle(var *o);
void setHandleOk(var *src, u64 sid);
u32 getHandleType(var *src, u64 sid);
void referSrcHandle(var *src, u64 sid);
int testSrcRefer(var *pool, u64 sid);
int createSrcRefer(var *pool, var *src, u64 sid, u64 value);
void deleteSrcRefer(var *pool, DeleteSrcFunc f, u64 sid);

// program
u64 createProgram(void);
void deleteProgram(u64 sid);
void deleteUserProgram(u64 sid);
int attachShader(GLuint program, GLenum type, u32 N, char *code[], char **error);
int linkProgram(GLuint program, char **error);
int useProgram(u64 sid);

// buffer
u64 createBuffer(u32 type);
void deleteBuffer(u64 sid);
void deleteUserBuffer(u64 sid);
int bufferSetData(GLuint buffer, u32 type, GLsizeiptr size, void *data, GLenum usage);
int bufferSubData(GLuint buffer, u32 type, GLintptr offset, GLsizeiptr size, void *data);
int useBuffer(u64 sid, u32 type);

// texture
u64 createTexture(u32 type);
void deleteTexture(u64 sid);
void deleteUserTexture(u64 sid);
int textureStorage(GLuint texture, u32 type, GLuint lsb, GLenum format, GLsizei w, GLsizei h, GLsizei d, GLboolean fixed);
int textureSubImage(GLuint texture, u32 type, GLint level, u32 cube, GLenum format, GLenum ft,
	GLvoid *pixels, u64 length, GLint x, GLint y, GLint z, GLsizei w, GLsizei h, GLsizei d);
int useTexture(u64 sid, u32 type, u32 active);

// vertexAttrib
u64 createVertexAttrib(void);
void deleteVertexAttrib(u64 sid);
void deleteUserVertexAttrib(u64 sid);
int vertexAttribPointer(var *v, u64 bid, GLuint index,
	GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer);
void setVertexAttrib(var *v, u32 enable, u64 index);
int useVertexAttrib(u64 sid);

// model
u64 createModel(u64 program);
void deleteModel(u64 sid);
void deleteUserModel(u64 sid);
void finalModel(u64 sid);
int modelLinkUniform(var *vp, char *name, var *sync, u32 type, u32 begin, u32 count, u32 transpose);
ulong* modelNewCommand(var *vp, u32 argc);
void modelDeleteCommand(var *vp, u32 n);
int modelRunScript(var *vp, char *script);
int modelSyncUniform(var *vp);
int modelSetBindBuffer(var *vp, u32 type, u64 buffer, u32 *ptype, u64 *plast);
int modelBindBuffer(var *vp, u64 buffer);
int modelSetVertexAttrib(var *vp, u64 va, u64 *plast);
int modelBindVertexAttrib(var *vp, u64 va);
int modelDrawArrays(var *vp, u32 mode, u32 begin, u32 count);
int modelDrawElements(var *vp, u32 mode, u32 begin, u32 count, u32 pbase);

int modelDraw(var *model);

// draw
int isDrawMode(GLenum m);

// system
void init_window(char *title, int width, int height);

#endif

