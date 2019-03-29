
int isDrawMode(GLenum m)
{
	switch(m)
	{
		case GL_POINTS:
		case GL_LINE_STRIP:
		case GL_LINE_LOOP:
		case GL_LINES:
		case GL_TRIANGLE_STRIP:
		case GL_TRIANGLE_FAN:
		case GL_TRIANGLES:
		case GL_LINE_STRIP_ADJACENCY:
		case GL_LINES_ADJACENCY:
		case GL_TRIANGLE_STRIP_ADJACENCY:
		case GL_TRIANGLES_ADJACENCY:
		case GL_PATCHES:
			return 1;
		default:
			return 0;
	}
}

