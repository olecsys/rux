#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_xrandr_module.h"
namespace rux
{
	namespace system
	{
#ifdef __UNIX__
		xrandr_module::xrandr_module( void )
			: rux::system::module( "libXrandr.so.2" )
		{
			_XRRSizes = 0;
			_XRRGetScreenInfo = 0;
			_XRRConfigCurrentRate = 0;
			_XRRConfigCurrentConfiguration = 0;
			_XRRGetScreenResources = 0;
			_XRRFreeScreenResources = 0;
			_XRRFreeCrtcInfo = 0;
			_XRRGetCrtcInfo = 0;
			_XRRFreeScreenConfigInfo = 0;
		};
		void xrandr_module::on_loaded( void )
		{
			system_module_dlsym( XRRSizes );
			system_module_dlsym( XRRGetScreenInfo );
			system_module_dlsym( XRRConfigCurrentRate );
			system_module_dlsym( XRRConfigCurrentConfiguration );
			system_module_dlsym( XRRGetScreenResources );
			system_module_dlsym( XRRFreeScreenResources );
			system_module_dlsym( XRRFreeCrtcInfo );
			system_module_dlsym( XRRGetCrtcInfo );
			system_module_dlsym( XRRFreeScreenConfigInfo );
		};
		XRRScreenSize* xrandr_module::XRRSizes( Display* dpy , int screen , int* nsizes )
		{
			load();
			return _XRRSizes( dpy , screen , nsizes );
		};
		XRRScreenConfiguration* xrandr_module::XRRGetScreenInfo( Display* dpy , Window window )
		{
			load();
			return _XRRGetScreenInfo( dpy , window );
		};
		short xrandr_module::XRRConfigCurrentRate( XRRScreenConfiguration* config )
		{
			load();
			return _XRRConfigCurrentRate( config );
		};
		void xrandr_module::XRRFreeScreenConfigInfo( XRRScreenConfiguration* config )
		{
			load();
			_XRRFreeScreenConfigInfo( config );
		};
		SizeID xrandr_module::XRRConfigCurrentConfiguration( XRRScreenConfiguration* config , Rotation* rotation )
		{
			load();
			return _XRRConfigCurrentConfiguration( config , rotation );
		};
		XRRScreenResources* xrandr_module::XRRGetScreenResources( Display* dpy , Window window )
		{
			load();
			return _XRRGetScreenResources( dpy , window );
		};
		void xrandr_module::XRRFreeScreenResources( XRRScreenResources* resources )
		{
			load();
			_XRRFreeScreenResources( resources );
		};
		XRRCrtcInfo* xrandr_module::XRRGetCrtcInfo( Display* dpy , XRRScreenResources* resources , RRCrtc crtc )
		{
			load();
			return _XRRGetCrtcInfo( dpy , resources , crtc );
		};
		void xrandr_module::XRRFreeCrtcInfo( XRRCrtcInfo* crtcInfo )
		{
			load();
			_XRRFreeCrtcInfo( crtcInfo );
		};
#endif
	};
};