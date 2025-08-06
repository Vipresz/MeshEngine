#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

/* Function pointer types */
typedef void (APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
typedef void (APIENTRYP PFNGLCLEARCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRYP PFNGLENABLEPROC) (GLenum cap);
typedef void (APIENTRYP PFNGLDISABLEPROC) (GLenum cap);
typedef void (APIENTRYP PFNGLBLENDFUNCPROC) (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRYP PFNGLVIEWPORTPROC) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLGETINTEGERVPROC) (GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLMATRIXMODEPROC) (GLenum mode);
typedef void (APIENTRYP PFNGLLOADIDENTITYPROC) (void);
typedef void (APIENTRYP PFNGLPUSHMATRIXPROC) (void);
typedef void (APIENTRYP PFNGLPOPMATRIXPROC) (void);
typedef void (APIENTRYP PFNGLORTHOPROC) (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
typedef void (APIENTRYP PFNGLBEGINPROC) (GLenum mode);
typedef void (APIENTRYP PFNGLENDPROC) (void);
typedef void (APIENTRYP PFNGLVERTEX2FPROC) (GLfloat x, GLfloat y);
typedef void (APIENTRYP PFNGLCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (APIENTRYP PFNGLLINEWIDTHPROC) (GLfloat width);
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);

/* GLAD function pointers */
static void* (*glad_glGetProcAddress)(const char* name) = NULL;

/* OpenGL function pointers */
static PFNGLCLEARPROC glad_glClear = NULL;
static PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
static PFNGLENABLEPROC glad_glEnable = NULL;
static PFNGLDISABLEPROC glad_glDisable = NULL;
static PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
static PFNGLVIEWPORTPROC glad_glViewport = NULL;
static PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;
// Legacy OpenGL 1.x function pointers removed to avoid conflicts
static PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
static PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
static PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
static PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
static PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
static PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
static PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
static PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
static PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
static PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
static PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
static PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
static PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
static PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
static PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
static PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
static PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
static PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
static PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
static PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
static PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
static PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
static PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
static PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
static PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
static PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
static PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
static PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
static PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;

/* GLAD loader function */
int gladLoadGL(void* (*load)(const char*)) {
    glad_glGetProcAddress = (void* (*)(const char*))load;
    
    /* Load OpenGL functions */
    glad_glClear = (PFNGLCLEARPROC)load("glClear");
    glad_glClearColor = (PFNGLCLEARCOLORPROC)load("glClearColor");
    glad_glEnable = (PFNGLENABLEPROC)load("glEnable");
    glad_glDisable = (PFNGLDISABLEPROC)load("glDisable");
    glad_glBlendFunc = (PFNGLBLENDFUNCPROC)load("glBlendFunc");
    glad_glViewport = (PFNGLVIEWPORTPROC)load("glViewport");
    glad_glGetIntegerv = (PFNGLGETINTEGERVPROC)load("glGetIntegerv");
    // Legacy OpenGL 1.x function loading removed to avoid conflicts
    glad_glCreateShader = (PFNGLCREATESHADERPROC)load("glCreateShader");
    glad_glShaderSource = (PFNGLSHADERSOURCEPROC)load("glShaderSource");
    glad_glCompileShader = (PFNGLCOMPILESHADERPROC)load("glCompileShader");
    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)load("glGetShaderiv");
    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load("glGetShaderInfoLog");
    glad_glDeleteShader = (PFNGLDELETESHADERPROC)load("glDeleteShader");
    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)load("glCreateProgram");
    glad_glAttachShader = (PFNGLATTACHSHADERPROC)load("glAttachShader");
    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)load("glLinkProgram");
    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load("glGetProgramiv");
    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load("glGetProgramInfoLog");
    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load("glDeleteProgram");
    glad_glUseProgram = (PFNGLUSEPROGRAMPROC)load("glUseProgram");
    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load("glGetUniformLocation");
    glad_glUniform1i = (PFNGLUNIFORM1IPROC)load("glUniform1i");
    glad_glUniform1f = (PFNGLUNIFORM1FPROC)load("glUniform1f");
    glad_glUniform2fv = (PFNGLUNIFORM2FVPROC)load("glUniform2fv");
    glad_glUniform3fv = (PFNGLUNIFORM3FVPROC)load("glUniform3fv");
    glad_glUniform4fv = (PFNGLUNIFORM4FVPROC)load("glUniform4fv");
    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load("glUniformMatrix4fv");
    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load("glGenVertexArrays");
    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load("glBindVertexArray");
    glad_glGenBuffers = (PFNGLGENBUFFERSPROC)load("glGenBuffers");
    glad_glBindBuffer = (PFNGLBINDBUFFERPROC)load("glBindBuffer");
    glad_glBufferData = (PFNGLBUFFERDATAPROC)load("glBufferData");
    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load("glVertexAttribPointer");
    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load("glEnableVertexAttribArray");
    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)load("glDrawArrays");
    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load("glDeleteVertexArrays");
    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load("glDeleteBuffers");
    
    return 1; // Success
}

/* OpenGL function implementations */
void glClear(GLbitfield mask) {
    if (glad_glClear) glad_glClear(mask);
}

void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    if (glad_glClearColor) glad_glClearColor(red, green, blue, alpha);
}

void glEnable(GLenum cap) {
    if (glad_glEnable) glad_glEnable(cap);
}

void glDisable(GLenum cap) {
    if (glad_glDisable) glad_glDisable(cap);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
    if (glad_glBlendFunc) glad_glBlendFunc(sfactor, dfactor);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    if (glad_glViewport) glad_glViewport(x, y, width, height);
}

void glGetIntegerv(GLenum pname, GLint* params) {
    if (glad_glGetIntegerv) glad_glGetIntegerv(pname, params);
}

// Legacy OpenGL 1.x functions removed to avoid conflicts with system OpenGL
// These functions are not needed for modern OpenGL 3.3 rendering

GLuint glCreateShader(GLenum type) {
    if (glad_glCreateShader) return glad_glCreateShader(type);
    return 0;
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length) {
    if (glad_glShaderSource) glad_glShaderSource(shader, count, string, length);
}

void glCompileShader(GLuint shader) {
    if (glad_glCompileShader) glad_glCompileShader(shader);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) {
    if (glad_glGetShaderiv) glad_glGetShaderiv(shader, pname, params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {
    if (glad_glGetShaderInfoLog) glad_glGetShaderInfoLog(shader, bufSize, length, infoLog);
}

void glDeleteShader(GLuint shader) {
    if (glad_glDeleteShader) glad_glDeleteShader(shader);
}

GLuint glCreateProgram(void) {
    if (glad_glCreateProgram) return glad_glCreateProgram();
    return 0;
}

void glAttachShader(GLuint program, GLuint shader) {
    if (glad_glAttachShader) glad_glAttachShader(program, shader);
}

void glLinkProgram(GLuint program) {
    if (glad_glLinkProgram) glad_glLinkProgram(program);
}

void glGetProgramiv(GLuint program, GLenum pname, GLint* params) {
    if (glad_glGetProgramiv) glad_glGetProgramiv(program, pname, params);
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog) {
    if (glad_glGetProgramInfoLog) glad_glGetProgramInfoLog(program, bufSize, length, infoLog);
}

void glDeleteProgram(GLuint program) {
    if (glad_glDeleteProgram) glad_glDeleteProgram(program);
}

void glUseProgram(GLuint program) {
    if (glad_glUseProgram) glad_glUseProgram(program);
}

GLint glGetUniformLocation(GLuint program, const GLchar* name) {
    if (glad_glGetUniformLocation) return glad_glGetUniformLocation(program, name);
    return -1;
}

void glUniform1i(GLint location, GLint v0) {
    if (glad_glUniform1i) glad_glUniform1i(location, v0);
}

void glUniform1f(GLint location, GLfloat v0) {
    if (glad_glUniform1f) glad_glUniform1f(location, v0);
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat* value) {
    if (glad_glUniform2fv) glad_glUniform2fv(location, count, value);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat* value) {
    if (glad_glUniform3fv) glad_glUniform3fv(location, count, value);
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat* value) {
    if (glad_glUniform4fv) glad_glUniform4fv(location, count, value);
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
    if (glad_glUniformMatrix4fv) glad_glUniformMatrix4fv(location, count, transpose, value);
}

void glGenVertexArrays(GLsizei n, GLuint* arrays) {
    if (glad_glGenVertexArrays) glad_glGenVertexArrays(n, arrays);
}

void glBindVertexArray(GLuint array) {
    if (glad_glBindVertexArray) glad_glBindVertexArray(array);
}

void glGenBuffers(GLsizei n, GLuint* buffers) {
    if (glad_glGenBuffers) glad_glGenBuffers(n, buffers);
}

void glBindBuffer(GLenum target, GLuint buffer) {
    if (glad_glBindBuffer) glad_glBindBuffer(target, buffer);
}

void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
    if (glad_glBufferData) glad_glBufferData(target, size, data, usage);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
    if (glad_glVertexAttribPointer) glad_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void glEnableVertexAttribArray(GLuint index) {
    if (glad_glEnableVertexAttribArray) glad_glEnableVertexAttribArray(index);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    if (glad_glDrawArrays) glad_glDrawArrays(mode, first, count);
}

void glDeleteVertexArrays(GLsizei n, const GLuint* arrays) {
    if (glad_glDeleteVertexArrays) glad_glDeleteVertexArrays(n, arrays);
}

void glDeleteBuffers(GLsizei n, const GLuint* buffers) {
    if (glad_glDeleteBuffers) glad_glDeleteBuffers(n, buffers);
} 