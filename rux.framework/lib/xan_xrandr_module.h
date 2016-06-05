#ifndef XAN_XRANDR_MODULE_H
#define XAN_XRANDR_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
#ifdef __UNIX__
#ifdef __ANDROID__
#else
#define XLIB_ILLEGAL_ACCESS
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>
#include <GL/glx.h>
#include <X11/extensions/Xrandr.h>
#endif
#endif
namespace rux
{
	namespace system
	{
#ifdef __UNIX__
		typedef XRRScreenSize* (*XRRSizes_t)( Display* dpy , int screen , int* nsizes );
		typedef XRRScreenConfiguration* (*XRRGetScreenInfo_t)( Display* dpy , Window window );
		typedef void (*XRRFreeScreenConfigInfo_t)( XRRScreenConfiguration* config );
		typedef short (*XRRConfigCurrentRate_t)( XRRScreenConfiguration* config );
		typedef SizeID (*XRRConfigCurrentConfiguration_t)( XRRScreenConfiguration* config , Rotation* rotation );
		typedef XRRScreenResources* (*XRRGetScreenResources_t)( Display* dpy , Window window );
		typedef void (*XRRFreeScreenResources_t)( XRRScreenResources* resources );
		typedef XRRCrtcInfo* (*XRRGetCrtcInfo_t)( Display* dpy , XRRScreenResources* resources , RRCrtc crtc );
		typedef void (*XRRFreeCrtcInfo_t)( XRRCrtcInfo* crtcInfo );
		class xrandr_module : public rux::system::module
		{
		public:
			XRRSizes_t _XRRSizes;
			XRRGetScreenInfo_t _XRRGetScreenInfo;
			XRRConfigCurrentRate_t _XRRConfigCurrentRate;
			XRRConfigCurrentConfiguration_t _XRRConfigCurrentConfiguration;
			XRRGetScreenResources_t _XRRGetScreenResources;
			XRRFreeScreenResources_t _XRRFreeScreenResources;
			XRRFreeCrtcInfo_t _XRRFreeCrtcInfo;
			XRRGetCrtcInfo_t _XRRGetCrtcInfo;
			XRRFreeScreenConfigInfo_t _XRRFreeScreenConfigInfo;
		public:
			xrandr_module( void );
			virtual void on_loaded( void );
			XRRScreenSize* XRRSizes( Display* dpy , int screen , int* nsizes );
			XRRScreenConfiguration* XRRGetScreenInfo( Display* dpy , Window window );
			void XRRFreeScreenConfigInfo( XRRScreenConfiguration* config );
			short XRRConfigCurrentRate( XRRScreenConfiguration* config );
			SizeID XRRConfigCurrentConfiguration( XRRScreenConfiguration* config , Rotation* rotation );
			XRRScreenResources* XRRGetScreenResources( Display* dpy , Window window );
			void XRRFreeScreenResources( XRRScreenResources* resources );
			XRRCrtcInfo* XRRGetCrtcInfo( Display* dpy , XRRScreenResources* resources , RRCrtc crtc );
			void XRRFreeCrtcInfo( XRRCrtcInfo* crtcInfo );
		};
#endif
	};
};
#endif
