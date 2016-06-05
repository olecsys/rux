#ifndef XAN_OPENGL_FUNCTIONS_H
#define XAN_OPENGL_FUNCTIONS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_memory_helper.h>
#ifdef __WINDOWS__
#include <win_GL.h>
#include <GdiPlus.h>
#include <win_glext.h>
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glx.h>
#endif
#ifdef __SOLARIS__
#include <GL/glext.h>
#else
#ifdef __ANDROID__
#else
#include <win_glext.h>
#endif
#endif
#endif
#ifndef GL_WRITE_ONLY
#ifdef __ANDROID__
#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#endif
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
#endif
#ifndef GL_FRAMEBUFFER_STATUS_ERROR_EXT
#define GL_FRAMEBUFFER_STATUS_ERROR_EXT                      0x8CDE
#endif
#ifndef GL_DEPTH24_STENCIL8
#define GL_DEPTH24_STENCIL8               0x88F0
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#define GL_TEXTURE_BASE_LEVEL             0x813C
#endif
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT 0x8CD8
#endif
#ifndef GL_TEXTURE_RECTANGLE
#define GL_TEXTURE_RECTANGLE              0x84F5
#endif
#ifndef GL_PIXEL_UNPACK_BUFFER
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#endif
#ifndef GL_TEXTURE_IMAGE_FORMAT
#define GL_TEXTURE_IMAGE_FORMAT           0x828F
#endif
#ifdef __WINDOWS__
typedef int (WINAPI * PFNWGLEXTGETSWAPINTERVALPROC)( void );
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC)( int interval );
#define glGetProcAddress( function ) rux::gui::engine::_wglGetProcAddress( function )
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#define glGetProcAddress( function ) rux::gui::engine::_eglGetProcAddress( function )
#else
typedef void (APIENTRYP PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
#define glGetProcAddress( function ) rux::gui::engine::_glXGetProcAddress( (GLubyte*)function )
#endif
#endif
#if !defined( GL_CLAMP_TO_EDGE )
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifdef __WINDOWS__
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C
#define WGL_SAMPLE_BUFFERS_ARB         0x2041
#define WGL_SAMPLES_ARB                0x2042
#endif
#ifdef __ANDROID__
	#ifndef APIENTRY
		#define APIENTRY GL_APIENTRY
	#endif
	#ifndef APIENTRYP
		#define APIENTRYP APIENTRY *
	#endif
	typedef GLuint (APIENTRYP PFNGLCREATESHADEROBJECTARBPROC)( GLenum shaderType );
	typedef void (APIENTRYP PFNGLSHADERSOURCEARBPROC)( GLuint shaderObj, GLsizei count, const GLchar* *string, const GLint *length);
	typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
	typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
	typedef void (APIENTRYP PFNGLLINKPROGRAMARBPROC) (GLuint programObj);
	typedef void (APIENTRYP PFNGLATTACHOBJECTARBPROC) (GLuint containerObj, GLuint obj);
	typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shaderObj);
	typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
	typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
	typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
	typedef void (APIENTRYP PFNGLUSEPROGRAMOBJECTARBPROC) (GLuint programObj);
	typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint programObj, const GLchar *name);
	typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);
	typedef PFNGLMAPBUFFEROESPROC PFNGLMAPBUFFERPROC;
	typedef PFNGLUNMAPBUFFEROESPROC PFNGLUNMAPBUFFERPROC;
	typedef void (APIENTRYP PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
	typedef void (APIENTRYP PFNGLDETACHOBJECTARBPROC) (GLuint containerObj, GLuint attachedObj);
	typedef void (APIENTRYP PFNGLDELETEOBJECTARBPROC) (GLuint obj);
	typedef PFNGLFRAMEBUFFERRENDERBUFFEROESPROC PFNGLFRAMEBUFFERRENDERBUFFERPROC;
	typedef PFNGLRENDERBUFFERSTORAGEOESPROC PFNGLRENDERBUFFERSTORAGEPROC;
	typedef PFNGLGENRENDERBUFFERSOESPROC PFNGLGENRENDERBUFFERSPROC;
	typedef PFNGLDELETERENDERBUFFERSOESPROC PFNGLDELETERENDERBUFFERSPROC;
	typedef PFNGLBINDRENDERBUFFEROESPROC PFNGLBINDRENDERBUFFERPROC;
	typedef PFNGLFRAMEBUFFERTEXTURE2DOESPROC PFNGLFRAMEBUFFERTEXTURE2DPROC;
	typedef PFNGLCHECKFRAMEBUFFERSTATUSOESPROC PFNGLCHECKFRAMEBUFFERSTATUSPROC;
	typedef PFNGLGENERATEMIPMAPOESPROC PFNGLGENERATEMIPMAPPROC;
	typedef PFNGLGENFRAMEBUFFERSOESPROC PFNGLGENFRAMEBUFFERSPROC;
	typedef PFNGLDELETEFRAMEBUFFERSOESPROC PFNGLDELETEFRAMEBUFFERSPROC;
	typedef PFNGLBINDFRAMEBUFFEROESPROC PFNGLBINDFRAMEBUFFERPROC;
	typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
	typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
	typedef void (APIENTRYP PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);
	typedef void (APIENTRYP PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	typedef void (APIENTRYP PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);
	typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
	typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint programObj, const GLchar *name);
	typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
	typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
#endif
typedef PFNGLDELETEBUFFERSPROC PFNglDeleteBuffersPROC;
typedef PFNGLACTIVESTENCILFACEEXTPROC PFNglActiveStencilFacePROC;
typedef PFNGLDISABLEVERTEXATTRIBARRAYPROC PFNglDisableVertexAttribArrayPROC;
typedef PFNGLGETATTRIBLOCATIONPROC PFNglGetAttribLocationPROC;
#ifdef __ANDROID__
#else
typedef PFNGLGETINFOLOGARBPROC PFNglGetInfoLogPROC;
typedef PFNGLGETOBJECTPARAMETERIVARBPROC PFNglGetObjectParameterivPROC;
#endif
typedef PFNGLGETUNIFORMLOCATIONPROC PFNglGetUniformLocationPROC; 
typedef PFNGLLINKPROGRAMARBPROC PFNglLinkProgramPROC;
typedef PFNGLUSEPROGRAMOBJECTARBPROC PFNglUseProgramPROC;
typedef PFNGLVALIDATEPROGRAMARBPROC PFNglValidateProgramPROC;
typedef PFNGLGETPROGRAMIVPROC PFNglGetProgramivPROC;
#ifdef __WINDOWS__
typedef PFNWGLCHOOSEPIXELFORMATARBPROC PFNwglChoosePixelFormatPROC;
typedef PFNWGLSWAPINTERVALEXTPROC PFNwglSwapIntervalPROC;
typedef PFNWGLEXTGETSWAPINTERVALPROC PFNwglGetSwapIntervalPROC; 
#endif
typedef PFNGLBINDFRAMEBUFFERPROC PFNglBindFramebufferPROC; 
typedef PFNGLMAPBUFFERPROC PFNglMapBufferPROC;
typedef PFNGLSTENCILOPSEPARATEPROC PFNglStencilOpSeparatePROC;
typedef PFNGLUNIFORM1FPROC PFNglUniform1fPROC;
typedef PFNGLUNIFORM1FVPROC PFNglUniform1fvPROC;
typedef PFNGLDELETEFRAMEBUFFERSPROC PFNglDeleteFramebuffersPROC;
typedef PFNGLACTIVETEXTUREPROC PFNglActiveTexturePROC;
typedef PFNGLBINDRENDERBUFFERPROC PFNglBindRenderbufferPROC;
typedef PFNGLCHECKFRAMEBUFFERSTATUSPROC PFNglCheckFramebufferStatusPROC;
typedef PFNGLDELETERENDERBUFFERSPROC PFNglDeleteRenderbuffersPROC;
typedef PFNGLFRAMEBUFFERRENDERBUFFERPROC PFNglFramebufferRenderbufferPROC;
typedef PFNGLFRAMEBUFFERTEXTURE2DPROC PFNglFramebufferTexture2DPROC;
typedef PFNGLGENERATEMIPMAPPROC PFNglGenerateMipmapPROC;
typedef PFNGLGENFRAMEBUFFERSPROC PFNglGenFramebuffersPROC;
typedef PFNGLGENRENDERBUFFERSPROC PFNglGenRenderbuffersPROC;
typedef PFNGLGETINTERNALFORMATIVPROC PFNglGetInternalformativPROC; 
typedef PFNGLRENDERBUFFERSTORAGEPROC PFNglRenderbufferStoragePROC;
typedef PFNGLUNIFORM4FVPROC PFNglUniform4fvPROC;
typedef PFNGLUNMAPBUFFERPROC PFNglUnmapBufferPROC;
typedef PFNGLCREATEPROGRAMOBJECTARBPROC PFNglCreateProgramPROC;
typedef PFNGLCREATESHADEROBJECTARBPROC PFNglCreateShaderPROC;
typedef PFNGLBINDBUFFERPROC PFNglBindBufferPROC;
typedef PFNGLBUFFERDATAPROC PFNglBufferDataPROC; 
typedef PFNGLBUFFERSUBDATAPROC PFNglBufferSubDataPROC;
typedef PFNGLCOMPILESHADERPROC PFNglCompileShaderPROC;
typedef PFNGLDELETEOBJECTARBPROC PFNglDeleteShaderPROC;
typedef PFNGLDETACHOBJECTARBPROC PFNglDetachShaderPROC;
typedef PFNGLENABLEVERTEXATTRIBARRAYPROC PFNglEnableVertexAttribArrayPROC;
typedef PFNGLBINDFRAGDATALOCATIONPROC PFNglBindFragDataLocationPROC;
typedef PFNGLGENBUFFERSPROC PFNglGenBuffersPROC;
typedef PFNGLSHADERSOURCEARBPROC PFNglShaderSourcePROC;
typedef PFNGLUNIFORM1IPROC PFNglUniform1iPROC;
typedef PFNGLUNIFORM4FPROC PFNglUniform4fPROC;
typedef PFNGLVERTEXATTRIBPOINTERPROC PFNglVertexAttribPointerPROC;
typedef PFNGLATTACHOBJECTARBPROC PFNglAttachShaderPROC;
namespace rux
{
	namespace gui
	{
		namespace engine
		{
			class GLContext;
#ifdef __ANDROID__
#define declare_opengl_function_with_return_typedef( FUNC , RETURN_TYPE ) typedef RETURN_TYPE (EGLAPIENTRY *PFN##FUNC##PROC)
#define declare_opengl_function_typedef( FUNC ) declare_opengl_function_with_return_typedef( FUNC , void )
#else
#define declare_opengl_function_with_return_typedef( FUNC , RETURN_TYPE ) typedef RETURN_TYPE (APIENTRYP PFN##FUNC##PROC)
#define declare_opengl_function_typedef( FUNC ) declare_opengl_function_with_return_typedef( FUNC , void )
#endif
#ifdef __WINDOWS__
			#define declare_wgl_function_with_return_typedef( FUNC , RETURN_TYPE ) typedef RETURN_TYPE (WINAPI *PFN##FUNC##PROC)
#else
#ifdef __ANDROID__
			#define declare_egl_function_with_return_typedef( FUNC , RETURN_TYPE ) typedef RETURN_TYPE (EGLAPIENTRY *PFN##FUNC##PROC)			
#else
			#define declare_xgl_function_with_return_typedef( FUNC , RETURN_TYPE ) typedef RETURN_TYPE (*PFN##FUNC##PROC)			
#endif
#endif
			declare_opengl_function_with_return_typedef( glGetError , GLenum )( void );			
			declare_opengl_function_with_return_typedef( glGetString , const GLubyte * )(GLenum name);

			declare_opengl_function_typedef( glColor4f )( GLfloat red , GLfloat green , GLfloat blue , GLfloat alpha );			
			declare_opengl_function_typedef( glRotatef )( GLfloat angle , GLfloat x , GLfloat y , GLfloat z );
			declare_opengl_function_typedef( glDisableClientState )(GLenum array);
			declare_opengl_function_typedef( glDrawArrays )(GLenum mode, GLint first, GLsizei count);
			declare_opengl_function_typedef( glMultiDrawArrays )( GLenum mode ,  GLint* first ,  GLsizei* count , GLsizei primcount );
			declare_opengl_function_typedef( glVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
			declare_opengl_function_typedef( glPixelStorei )(GLenum pname, GLint param);
			declare_opengl_function_typedef( glEnable )(GLenum cap);
			declare_opengl_function_typedef( glBindTexture )(GLenum target, GLuint texture);
			declare_opengl_function_typedef( glDisable )(GLenum cap);
			declare_opengl_function_typedef( glStencilFunc )(GLenum func, GLint ref, GLuint mask);
			declare_opengl_function_typedef( glColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
			declare_opengl_function_typedef( glStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
			declare_opengl_function_typedef( glBlendFunc )(GLenum sfactor, GLenum dfactor);
			declare_opengl_function_typedef( glGetFloatv )(GLenum pname, GLfloat *params);
			declare_opengl_function_typedef( glGetBooleanv )(GLenum pname, GLboolean *params);
			declare_opengl_function_typedef( glTexParameteri )(GLenum target, GLenum pname, GLint param);
			declare_opengl_function_typedef( glTexEnvf )(GLenum target, GLenum pname, GLfloat param);
			declare_opengl_function_typedef( glTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
			declare_opengl_function_typedef( glTexImage1D )( GLenum target , GLint level , GLint internalFormat , GLsizei width , GLint border , GLenum format , GLenum type , const GLvoid* data );
			declare_opengl_function_typedef( glGetIntegerv )(GLenum pname, GLint *params);
			declare_opengl_function_typedef( glGetShaderiv )( GLuint shader , GLenum pname , GLint *params );
			declare_opengl_function_typedef( glDeleteProgram )( GLuint program );

#ifdef __WINDOWS__
			declare_wgl_function_with_return_typedef( wglShareLists , BOOL )( HGLRC hglrc1 , HGLRC  hglrc2 );
			declare_wgl_function_with_return_typedef( wglGetProcAddress , PROC )( LPCSTR );
			declare_wgl_function_with_return_typedef( wglGetCurrentDC , HDC )( VOID );
			declare_wgl_function_with_return_typedef( wglCreateContext , HGLRC )( HDC );
			declare_wgl_function_with_return_typedef( wglDeleteContext , BOOL )( HGLRC );
			declare_wgl_function_with_return_typedef( wglMakeCurrent , BOOL )( HDC , HGLRC );
			declare_wgl_function_with_return_typedef( wglGetCurrentContext , HGLRC )(VOID);
#else
#ifdef __ANDROID__
			declare_egl_function_with_return_typedef( eglGetProcAddress , __eglMustCastToProperFunctionPointerType )( const char *procname );
#else
			typedef void(*PROC)();
			declare_xgl_function_with_return_typedef( glXQueryExtension , Bool )(Display *  dpy,  int *  errorBase,  int *  eventBase);
			declare_xgl_function_with_return_typedef( glXGetConfig , int )(Display *  dpy,  XVisualInfo *  vis,  int  attrib,  int *  value);
			declare_xgl_function_with_return_typedef( glXGetProcAddress , PROC )(const GLubyte *  procName);
			declare_xgl_function_with_return_typedef( glXCreateContext , GLXContext )(Display *  dpy,  XVisualInfo *  vis,  GLXContext  shareList,  Bool  direct);
			declare_xgl_function_with_return_typedef( glXDestroyContext , void )(Display *  dpy,  GLXContext  ctx);
			declare_xgl_function_with_return_typedef( glXSwapBuffers , void )(Display *  dpy,  GLXDrawable  drawable );
			declare_xgl_function_with_return_typedef( glXGetCurrentContext , GLXContext )();
			declare_xgl_function_with_return_typedef( glXMakeCurrent , Bool )(Display *  dpy,  GLXDrawable  drawable,  GLXContext  ctx);
#endif
#endif
			declare_opengl_function_typedef( glLoadIdentity )(void);
#ifdef __ANDROID__
			declare_opengl_function_typedef( glOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
#else
			declare_opengl_function_typedef( glOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
#endif
			declare_opengl_function_typedef( glGenTextures )(GLsizei n, GLuint *textures);			
			declare_opengl_function_typedef( glDeleteTextures )(GLsizei n, const GLuint *textures);
			declare_opengl_function_typedef( glEnableClientState )(GLenum array);
			declare_opengl_function_typedef( glTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
			declare_opengl_function_typedef( glClearStencil )(GLint s);
			declare_opengl_function_typedef( glColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
			declare_opengl_function_typedef( glTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
			declare_opengl_function_typedef( glTexSubImage1D )( GLenum target , GLint level , GLint xoffset , GLsizei width , GLenum format , GLenum type , const GLvoid* data );
			declare_opengl_function_typedef( glPushMatrix )(void);
			declare_opengl_function_typedef( glMatrixMode )(GLenum mode);
			declare_opengl_function_typedef( glDepthFunc )(GLenum func);
			declare_opengl_function_typedef( glClearDepthf )(GLclampf depth);
			declare_opengl_function_typedef( glClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
			declare_opengl_function_typedef( glShadeModel )(GLenum mode);
			declare_opengl_function_typedef( glViewport )(GLint x, GLint y, GLsizei width, GLsizei height);
			declare_opengl_function_typedef( glClear )(GLbitfield mask);
			declare_opengl_function_typedef( glScalef )(GLfloat x, GLfloat y, GLfloat z);
			declare_opengl_function_typedef( glHint )(GLenum target, GLenum mode);
			declare_opengl_function_typedef( glFlush )(void);
			declare_opengl_function_typedef( glFinish )(void);
			declare_opengl_function_typedef( glPopMatrix )(void);
			declare_opengl_function_typedef( glEnd )(void);
			declare_opengl_function_typedef( glVertex3f )(GLfloat x, GLfloat y, GLfloat z);
			declare_opengl_function_typedef( glTexCoord2f )(GLfloat s, GLfloat t);
			declare_opengl_function_typedef( glBegin )(GLenum mode);
			declare_opengl_function_typedef( glRasterPos3f )(GLfloat x, GLfloat y , GLfloat z);
			declare_opengl_function_typedef( glPixelZoom )(GLfloat xfactor, GLfloat yfactor);
			declare_opengl_function_typedef( glDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

#define declare_opengl_function_with_return_in_header_macros( FUNC , RETURN_TYPE ) static RETURN_TYPE APIENTRY FUNC
#define declare_opengl_function_in_header_macros( FUNC ) declare_opengl_function_with_return_in_header_macros( FUNC , void )

#define implement_opengl_function_with_return_in_header_macros( FUNC , RETURN_TYPE ) RETURN_TYPE OpenGL::FUNC
#define implement_opengl_function_in_header_macros( FUNC ) implement_opengl_function_with_return_in_header_macros( FUNC , void )

#define declare_opengl_function_object_in_header_macros( FUNC ) extern dll_internal rux::gui::engine::PFN##FUNC##PROC _##FUNC
#define declare_opengl_function_object_in_cpp_macros( FUNC ) dll_internal rux::gui::engine::PFN##FUNC##PROC _##FUNC = NULL

			extern dll_internal rux_volatile _gl_library_inited;
			extern dll_internal XCrtSect* _cs_gl_library;
#ifdef __WINDOWS__
			extern dll_internal HMODULE _gl_library;
#else
			extern dll_internal void* _gl_library;
#endif
			extern dll_internal rux::uint8 _log_enable;
			extern dll_internal rux::byte _opengl_one_thread_swap_buffers;
			extern dll_internal rux::byte _winapi_trace;
			extern dll_internal ::rux::byte _check_gl_error;
			extern dll_internal ::rux::byte _check_opengl_functions_timeout;
			extern dll_internal ::rux::byte _check_support_framebuffer;
			extern dll_internal ::rux::byte _check_support_arb_texture_rectangle;
			extern dll_internal ::rux::byte _check_support_old_yv12_to_rgb_shader;
			extern dll_internal ::rux::byte _check_support_old_yv12_to_rgb_shader2;
			extern dll_internal ::rux::byte _check_support_old_yuyv_to_rgb_shader;
			extern dll_internal ::rux::byte _check_support_yv12_to_rgb_shader;
			extern dll_internal ::rux::byte _check_support_yuyv_to_rgb_shader;
			extern dll_internal ::rux::byte _check_support_fxaa;
			extern dll_internal ::rux::byte _vsync;
			extern dll_internal ::rux::byte _use_gl_finish;
			extern dll_internal ::rux::byte _check_support_vbo;
			class RenderThread;
			declare_global_variable( char , _error_filename , 1024 );
			declare_global_variable( char , _log_filename , 1024 );
#ifdef __WINDOWS__
			declare_opengl_function_object_in_header_macros( wglShareLists );
			declare_opengl_function_object_in_header_macros( wglGetProcAddress );
			declare_opengl_function_object_in_header_macros( wglGetCurrentDC );
			declare_opengl_function_object_in_header_macros( wglCreateContext );
			declare_opengl_function_object_in_header_macros( wglDeleteContext );
			declare_opengl_function_object_in_header_macros( wglMakeCurrent );
			declare_opengl_function_object_in_header_macros( wglGetCurrentContext );
#else
#ifdef __ANDROID__
			declare_opengl_function_object_in_header_macros( eglGetProcAddress );
#else
			declare_opengl_function_object_in_header_macros( glXQueryExtension );
			declare_opengl_function_object_in_header_macros( glXGetConfig );
			declare_opengl_function_object_in_header_macros( glXGetProcAddress );
			declare_opengl_function_object_in_header_macros( glXCreateContext );
			declare_opengl_function_object_in_header_macros( glXDestroyContext );
			declare_opengl_function_object_in_header_macros( glXSwapBuffers );
			declare_opengl_function_object_in_header_macros( glXGetCurrentContext );
			declare_opengl_function_object_in_header_macros( glXMakeCurrent );
#endif
#endif
			declare_opengl_function_object_in_header_macros( glGetString );
			declare_opengl_function_object_in_header_macros( glColor4f );
			declare_opengl_function_object_in_header_macros( glRotatef );
			declare_opengl_function_object_in_header_macros( glGetError );
			declare_opengl_function_object_in_header_macros( glDisableClientState );
			declare_opengl_function_object_in_header_macros( glDrawArrays );
			declare_opengl_function_object_in_header_macros( glVertexPointer );
			declare_opengl_function_object_in_header_macros( glPixelStorei );
			declare_opengl_function_object_in_header_macros( glEnable );
			declare_opengl_function_object_in_header_macros( glBindTexture );
			declare_opengl_function_object_in_header_macros( glDisable );
			declare_opengl_function_object_in_header_macros( glStencilFunc );
			declare_opengl_function_object_in_header_macros( glColorMask );
			declare_opengl_function_object_in_header_macros( glStencilOp );
			declare_opengl_function_object_in_header_macros( glBlendFunc );
			declare_opengl_function_object_in_header_macros( glGetFloatv );
			declare_opengl_function_object_in_header_macros( glGetBooleanv );
			declare_opengl_function_object_in_header_macros( glTexParameteri );
			declare_opengl_function_object_in_header_macros( glTexEnvf );
			declare_opengl_function_object_in_header_macros( glTexImage2D );			
			declare_opengl_function_object_in_header_macros( glTexImage1D );
			declare_opengl_function_object_in_header_macros( glLoadIdentity );
#ifdef __ANDROID__
			declare_opengl_function_object_in_header_macros( glOrthof );
#else
			declare_opengl_function_object_in_header_macros( glOrtho );
#endif
			declare_opengl_function_object_in_header_macros( glGenTextures );
			declare_opengl_function_object_in_header_macros( glDeleteTextures );
			declare_opengl_function_object_in_header_macros( glEnableClientState );
			declare_opengl_function_object_in_header_macros( glTexCoordPointer );
			declare_opengl_function_object_in_header_macros( glClearStencil );
			declare_opengl_function_object_in_header_macros( glColorPointer );
			declare_opengl_function_object_in_header_macros( glTexSubImage2D );
			declare_opengl_function_object_in_header_macros( glTexSubImage1D );
			declare_opengl_function_object_in_header_macros( glPushMatrix );
			declare_opengl_function_object_in_header_macros( glMatrixMode );
			declare_opengl_function_object_in_header_macros( glDepthFunc );
			declare_opengl_function_object_in_header_macros( glClearDepthf );
			declare_opengl_function_object_in_header_macros( glClearColor );
			declare_opengl_function_object_in_header_macros( glShadeModel );
			declare_opengl_function_object_in_header_macros( glViewport );
			declare_opengl_function_object_in_header_macros( glClear );
			declare_opengl_function_object_in_header_macros( glScalef );
			declare_opengl_function_object_in_header_macros( glHint );
			declare_opengl_function_object_in_header_macros( glFlush );
			declare_opengl_function_object_in_header_macros( glFinish );
			declare_opengl_function_object_in_header_macros( glPopMatrix );
			declare_opengl_function_object_in_header_macros( glEnd );
			declare_opengl_function_object_in_header_macros( glVertex3f );
			declare_opengl_function_object_in_header_macros( glTexCoord2f );
			declare_opengl_function_object_in_header_macros( glBegin );
			declare_opengl_function_object_in_header_macros( glScalef );
			declare_opengl_function_object_in_header_macros( glGetIntegerv );			
			declare_opengl_function_object_in_header_macros( glRasterPos3f );
			declare_opengl_function_object_in_header_macros( glPixelZoom );
			declare_opengl_function_object_in_header_macros( glDrawPixels );
						
			class OpenGL
			{
			public:
#ifdef __WINDOWS__
				declare_opengl_function_with_return_in_header_macros( wglShareLists , BOOL )( HGLRC hglrc1 , HGLRC  hglrc2 );
				declare_opengl_function_with_return_in_header_macros( wglGetProcAddress , PROC )( LPCSTR );
				declare_opengl_function_with_return_in_header_macros( wglGetCurrentDC , HDC )( VOID );
				declare_opengl_function_with_return_in_header_macros( wglCreateContext , HGLRC )( HDC );
				declare_opengl_function_with_return_in_header_macros( wglDeleteContext , BOOL )( HGLRC );
				declare_opengl_function_with_return_in_header_macros( wglMakeCurrent , BOOL )( HDC , HGLRC );
				declare_opengl_function_with_return_in_header_macros( wglGetCurrentContext , HGLRC )(VOID);
#else
#ifdef __ANDROID__
				declare_opengl_function_with_return_in_header_macros( eglGetProcAddress , __eglMustCastToProperFunctionPointerType )( const char* procName );
#else
				declare_opengl_function_with_return_in_header_macros( glXQueryExtension , Bool )(Display *  dpy,  int *  errorBase,  int *  eventBase);
				declare_opengl_function_with_return_in_header_macros( glXGetConfig , int )(Display *  dpy,  XVisualInfo *  vis,  int  attrib,  int *  value);
				declare_opengl_function_with_return_in_header_macros( glXGetProcAddress , PROC )(const GLubyte *  procName);
				declare_opengl_function_with_return_in_header_macros( glXCreateContext , GLXContext )(Display *  dpy,  XVisualInfo *  vis,  GLXContext  shareList,  Bool  direct);
				declare_opengl_function_with_return_in_header_macros( glXDestroyContext , void )(Display *  dpy,  GLXContext  ctx , size_t ___rux__thread_index1986 );
				declare_opengl_function_with_return_in_header_macros( glXSwapBuffers , void )(Display *  dpy,  GLXDrawable  drawable , size_t ___rux__thread_index1986 );
				declare_opengl_function_with_return_in_header_macros( glXGetCurrentContext , GLXContext )();
				declare_opengl_function_with_return_in_header_macros( glXMakeCurrent , Bool )(Display *  dpy,  GLXDrawable  drawable,  GLXContext  ctx);
#endif
#endif
				declare_opengl_function_with_return_in_header_macros( glGetError , GLenum )( void );
				declare_opengl_function_with_return_in_header_macros( glGetString , const GLubyte * )(GLenum name);
				declare_opengl_function_in_header_macros( glColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glRotatef )( GLfloat angle , GLfloat x , GLfloat y , GLfloat z , ::rux::gui::engine::GLContext* , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDisableClientState )(GLenum array , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDrawArrays )(GLenum mode, GLint first, GLsizei count , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glMultiDrawArrays )( GLenum mode ,  GLint* first ,  GLsizei* count , GLsizei primcount , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glPixelStorei )(GLenum pname, GLint param , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glEnable )(GLenum cap , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glBindTexture )(GLenum target, GLuint texture , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDisable )(GLenum cap , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glStencilFunc )(GLenum func, GLint ref, GLuint mask , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glStencilOp )(GLenum fail, GLenum zfail, GLenum zpass , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glBlendFunc )(GLenum sfactor, GLenum dfactor , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glGetFloatv )(GLenum pname, GLfloat *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glGetBooleanv )(GLenum pname, GLboolean *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexParameteri )(GLenum target, GLenum pname, GLint param , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexEnvf )(GLenum target, GLenum pname, GLfloat param , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexImage1D )( GLenum target , GLint level , GLint internalFormat , GLsizei width , GLint border , GLenum format , GLenum type , const GLvoid* data , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glLoadIdentity )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#ifdef __ANDROID__
				declare_opengl_function_in_header_macros( glOrthof )(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#else
				declare_opengl_function_in_header_macros( glOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#endif
				declare_opengl_function_in_header_macros( glGenTextures )(GLsizei n, GLuint *textures , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDeleteTextures )(GLsizei n, const GLuint *textures , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glEnableClientState )(GLenum array , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glClearStencil )(GLint s , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexSubImage1D )( GLenum target , GLint level , GLint xoffset , GLsizei width , GLenum format , GLenum type , const GLvoid* data , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glPushMatrix )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glMatrixMode )(GLenum mode , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDepthFunc )(GLenum func , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glClearDepthf )(GLclampf depth , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glShadeModel )(GLenum mode , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glViewport )(GLint x, GLint y, GLsizei width, GLsizei height , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glClear )(GLbitfield mask , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glScalef )(GLfloat x, GLfloat y, GLfloat z , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glHint )(GLenum target, GLenum mode , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glFlush )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glFinish )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glPopMatrix )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glEnd )( const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glVertex3f )(GLfloat x, GLfloat y, GLfloat z , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glTexCoord2f )(GLfloat s, GLfloat t , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glBegin )(GLenum mode , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glGetIntegerv )(GLenum pname, GLint *params , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glGetShaderiv )( GLuint shader,  GLenum pname,  GLint *params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDeleteProgram )( GLuint program , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glRasterPos3f )(GLfloat x, GLfloat y, GLfloat z , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glPixelZoom )(GLfloat xfactor, GLfloat yfactor , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				declare_opengl_function_in_header_macros( glDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );

				static void glDeleteBuffers( GLsizei n , const GLuint* buffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glActiveTexture( GLenum texture , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUniform1i( GLint location , GLint v0 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGenBuffers( GLsizei n , GLuint* buffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glEnableVertexAttribArray( GLuint index , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBindBuffer( GLenum target , GLuint buffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glVertexAttribPointer( GLuint index , GLint size , GLenum type , GLboolean normalized , GLsizei stride , const GLvoid* pointer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBufferData( GLenum target , GLsizeiptr size , const GLvoid* data , GLenum usage , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glShaderSource( GLuint shaderObj , GLsizei count , const GLchar** string , const GLint* length , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glCompileShader( GLuint shaderObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glDeleteShader( GLuint shaderObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glDetachShader( GLuint containerObj, GLuint attachedObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glAttachShader( GLuint containerObj , GLuint obj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glLinkProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBindFragDataLocation( GLuint program , GLuint colorNumber , const char * name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLint glGetAttribLocation( GLuint programObj , const GLchar* name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUseProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glValidateProgram( GLuint programObj , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGetProgramiv( GLuint program , GLenum pname , GLint* params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLuint glCreateProgram( ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLuint glCreateShader( GLenum shaderType , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLint glGetUniformLocation( GLuint programObj , const GLchar* name , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glDisableVertexAttribArray( GLuint index , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUniform4fv( GLint location , GLsizei count , const GLfloat *value , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLvoid* glMapBuffer( GLenum target , GLenum access , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLboolean glUnmapBuffer( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUniform1f( GLint location , GLfloat v0 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUniform1fv( GLint location , GLsizei count , const GLfloat* value , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glUniform4f( GLint location , GLfloat v0 , GLfloat v1 , GLfloat v2 , GLfloat v3 , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#ifdef __ANDROID__
#else
				static void glGetObjectParameteriv( GLuint obj , GLenum pname , GLint *params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGetInfoLog( GLuint obj , GLsizei maxLength , GLsizei *length , GLchar *infoLog , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#endif
				static void glStencilOpSeparate( GLenum face , GLenum sfail , GLenum dpfail , GLenum dppass , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glActiveStencilFace( GLenum face , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBufferSubData( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBindFramebuffer( GLenum target , GLuint framebuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glDeleteFramebuffers( GLsizei n , const GLuint *framebuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGenFramebuffers( GLsizei n , GLuint *framebuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGenerateMipmap( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static GLenum glCheckFramebufferStatus( GLenum target , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glFramebufferTexture2D( GLenum target , GLenum attachment , GLenum textarget , GLuint texture , GLint level , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glBindRenderbuffer( GLenum target , GLuint renderbuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glDeleteRenderbuffers( GLsizei n , const GLuint *renderbuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGenRenderbuffers( GLsizei n , GLuint *renderbuffers , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glRenderbufferStorage( GLenum target , GLenum internalformat , GLsizei width , GLsizei height , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glFramebufferRenderbuffer( GLenum target , GLenum attachment , GLenum renderbuffertarget , GLuint renderbuffer , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static void glGetInternalformativ( GLenum target , GLenum internalformat , GLenum pname , GLsizei bufSize , GLint* params , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#ifdef __WINDOWS__
				static BOOL WINAPI wglChoosePixelFormat( HDC hdc , const int *piAttribIList , const FLOAT *pfAttribFList , UINT nMaxFormats , int *piFormats , UINT *nNumFormats , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static BOOL WINAPI wglSwapInterval( int interval , ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
				static int WINAPI wglGetSwapInterval( ::rux::gui::engine::GLContext* gl_context , const char* caller_function , ::rux::int32 caller_line , size_t ___rux__thread_index1986 );
#endif				
				static void initialize( void );
				static void deinitialize( void );
				static void write_error( const char* text );
				static void write_log( char* text );
				static void check_gl_error( const char* function , const char* caller_function , ::rux::int32 caller_line , const char* format , ... );
			};
		};
	};
};
#endif
