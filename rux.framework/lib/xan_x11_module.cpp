#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_x11_module.h>
namespace rux
{
	namespace system
	{
#ifdef __UNIX__
		x11_module::x11_module( void )
			: rux::system::module( "libX11.so" , "libX11.so.6" )
		{
			_XOpenDisplay = 0;
			_XCloseDisplay = 0;
			_XDestroyWindow = 0;
			_XSendEvent = 0;
			_XInternAtom = 0;
			_XIfEvent = 0;
			_XMapWindow = 0;
			_XMapRaised = 0;
			_XConfigureWindow = 0;
			_XMoveWindow = 0;
			_XResizeWindow = 0;
			_XMoveResizeWindow = 0;
			_XFlush = 0;
			_XSync = 0;
			_XEventsQueued = 0;
			_XPending = 0;
			_XCreateFontCursor = 0;
			_XDefineCursor = 0;
			_XUndefineCursor = 0;
			_XQueryPointer = 0;
			_XUnmapWindow = 0;
			_XGrabPointer = 0;
			_XUngrabPointer = 0;
			_XInitThreads = 0;
			_XLockDisplay = 0;
			_XUnlockDisplay = 0;
			_XGetVisualInfo = 0;
			_XFree = 0;
			_XSelectInput = 0;
			_XNextEvent = 0;
			_XPeekEvent = 0;
			_XWindowEvent = 0;
			_XCheckWindowEvent = 0;
			_XMaskEvent = 0;
			_XCheckMaskEvent = 0;
			_XCheckTypedEvent = 0;
			_XCheckTypedWindowEvent = 0;
			_XPutBackEvent = 0;
			_XCreateColormap = 0;
			_XCreateWindow = 0;
			_XChangeProperty = 0;
			_XGetWindowProperty = 0;
			_XSetWMProtocols = 0;
			_XGetAtomName = 0;
			_XLookupString = 0;
			_XGetWindowAttributes = 0;
			_XGetErrorText = 0;
			_XSetErrorHandler = 0;
			_XSetIOErrorHandler = 0;
		};
		void x11_module::on_loaded( void )
		{
			system_module_dlsym( XOpenDisplay );
			system_module_dlsym( XCloseDisplay );
			system_module_dlsym( XDestroyWindow );
			system_module_dlsym( XSendEvent );
			system_module_dlsym( XInternAtom );
			system_module_dlsym( XIfEvent );
			system_module_dlsym( XMapWindow );
			system_module_dlsym( XMapRaised );
			system_module_dlsym( XConfigureWindow );
			system_module_dlsym( XMoveWindow );
			system_module_dlsym( XResizeWindow );
			system_module_dlsym( XMoveResizeWindow );
			system_module_dlsym( XFlush );
			system_module_dlsym( XSync );
			system_module_dlsym( XEventsQueued );
			system_module_dlsym( XPending );
			system_module_dlsym( XCreateFontCursor );
			system_module_dlsym( XDefineCursor );
			system_module_dlsym( XUndefineCursor );
			system_module_dlsym( XQueryPointer );
			system_module_dlsym( XUnmapWindow );
			system_module_dlsym( XGrabPointer );
			system_module_dlsym( XUngrabPointer );
			system_module_dlsym( XInitThreads );
			system_module_dlsym( XLockDisplay );
			system_module_dlsym( XUnlockDisplay );
			system_module_dlsym( XGetVisualInfo );
			system_module_dlsym( XFree );
			system_module_dlsym( XSelectInput );
			system_module_dlsym( XNextEvent );
			system_module_dlsym( XPeekEvent );
			system_module_dlsym( XWindowEvent );
			system_module_dlsym( XCheckWindowEvent );
			system_module_dlsym( XMaskEvent );
			system_module_dlsym( XCheckMaskEvent );
			system_module_dlsym( XCheckTypedEvent );
			system_module_dlsym( XCheckTypedWindowEvent );
			system_module_dlsym( XPutBackEvent );
			system_module_dlsym( XCreateColormap );
			system_module_dlsym( XCreateWindow );
			system_module_dlsym( XChangeProperty );
			system_module_dlsym( XGetWindowProperty );
			system_module_dlsym( XSetWMProtocols );
			system_module_dlsym( XGetAtomName );
			system_module_dlsym( XLookupString );
			system_module_dlsym( XGetWindowAttributes );
			system_module_dlsym( XGetErrorText );
			system_module_dlsym( XSetErrorHandler );
			system_module_dlsym( XSetIOErrorHandler );
			system_module_dlsym(XTranslateCoordinates);
		};
		::Display* x11_module::XOpenDisplay( char* display_name )
		{
			load();
			return _XOpenDisplay( display_name );
		};
		int x11_module::XCloseDisplay( ::Display* display )
		{
			load();
			return _XCloseDisplay( display );
		};
		int x11_module::XDestroyWindow(::Display *display, ::Window w)
		{
			load();
			return _XDestroyWindow(display, w);
		};
		Status x11_module::XSendEvent(::Display *display, ::Window w, Bool propagate, long event_mask, ::XEvent *event_send)
		{
			load();
			return _XSendEvent(display, w,  propagate, event_mask, event_send);
		};
		::Atom x11_module::XInternAtom(::Display *display, const char *atom_name, Bool only_if_exists)
		{
			load();
			return _XInternAtom(display, atom_name, only_if_exists);
		};
		int x11_module::XIfEvent(::Display *display, ::XEvent *event_return, predicate_t predicate , ::XPointer arg)
		{
			load();
			return _XIfEvent(display, event_return, predicate, arg);
		};
		int x11_module::XMapWindow(::Display *display, ::Window w)
		{
			load();
			return _XMapWindow(display, w);
		};
		int x11_module::XMapRaised(::Display *display, ::Window w)
		{
			load();
			return _XMapRaised(display, w);
		};
		int x11_module::XConfigureWindow(::Display *display, ::Window w, unsigned value_mask, ::XWindowChanges *changes)
		{
			load();
			return _XConfigureWindow(display, w, value_mask, changes);
		};
		int x11_module::XMoveWindow(::Display *display, ::Window w, int x, int y)
		{
			load();
			return _XMoveWindow(display, w, x, y);
		};
		int x11_module::XResizeWindow(::Display *display, ::Window w, unsigned width, unsigned height)
		{
			load();
			return _XResizeWindow(display, w, width, height);
		};
		int x11_module::XMoveResizeWindow(::Display *display, ::Window w, int x, int y, unsigned width, unsigned height)
		{
			load();
			return _XMoveResizeWindow(display, w, x, y, width, height);
		};
		int x11_module::XFlush(::Display *display)
		{
			load();
			return _XFlush(display);
		};
		int x11_module::XSync(::Display *display, Bool discard)
		{
			load();
			return _XSync(display, discard);
		};
		int x11_module::XEventsQueued(::Display *display, int mode)
		{
			load();
			return _XEventsQueued(display, mode);
		};
		int x11_module::XPending(::Display *display)
		{
			load();
			return _XPending(display);
		};
		::Cursor x11_module::XCreateFontCursor(::Display *display, unsigned int shape)
		{
			load();
			return _XCreateFontCursor(display, shape);
		};
		int x11_module::XDefineCursor(::Display *display, ::Window w, ::Cursor cursor)
		{
			load();
			return _XDefineCursor(display, w, cursor);
		};
		int x11_module::XUndefineCursor(::Display *display, ::Window w)
		{
			load();
			return _XUndefineCursor(display, w);
		};
		Bool x11_module::XQueryPointer(::Display *display, ::Window w, ::Window *root_return, ::Window *child_return, int *root_x_return, int *root_y_return, int *win_x_return, int *win_y_return, unsigned int *mask_return)
		{
			load();
			return _XQueryPointer(display, w, root_return, child_return, root_x_return, root_y_return, win_x_return, win_y_return, mask_return);
		};
		int x11_module::XUnmapWindow(::Display *display, ::Window w)
		{
			load();
			return _XUnmapWindow(display, w);
		};
		int x11_module::XGrabPointer(::Display *display, ::Window grab_window, Bool owner_events, unsigned int event_mask, int pointer_mode, int keyboard_mode, ::Window confine_to, ::Cursor cursor, ::Time time)
		{
			load();
			return _XGrabPointer(display, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor, time);
		};
		int x11_module::XUngrabPointer(::Display *display, ::Time time)
		{
			load();
			return _XUngrabPointer(display, time);
		};
		Status x11_module::XInitThreads(void)
		{
			load();
			return _XInitThreads();
		};
		void x11_module::XLockDisplay(::Display *display)
		{
			load();
			_XLockDisplay( display );
		};
		void x11_module::XUnlockDisplay(::Display *display)
		{
			load();
			XUnlockDisplay( display );
		};
		::XVisualInfo *x11_module::XGetVisualInfo(::Display *display, long vinfo_mask, ::XVisualInfo *vinfo_template, int *nitems_return)
		{
			load();
			return _XGetVisualInfo( display , vinfo_mask , vinfo_template , nitems_return );
		};
		int x11_module::XFree(void *data)
		{
			load();
			return _XFree( data );
		};
		int x11_module::XSelectInput(::Display *display, ::Window w, long event_mask)
		{
			load();
			return _XSelectInput( display, w , event_mask);
		};
		int x11_module::XNextEvent(::Display *display, ::XEvent *event_return)
		{
			load();
			return _XNextEvent( display , event_return );
		};
		int x11_module::XPeekEvent(::Display *display, ::XEvent *event_return)
		{
			load();
			return _XPeekEvent( display, event_return);
		};
		int x11_module::XWindowEvent(::Display *display, ::Window w, long event_mask, ::XEvent *event_return)
		{
			load();
			return _XWindowEvent( display , w , event_mask , event_return );
		};
		Bool x11_module::XCheckWindowEvent(::Display *display, ::Window w, long event_mask, ::XEvent *event_return)
		{
			load();
			return _XCheckWindowEvent( display , w , event_mask , event_return );
		};
		int x11_module::XMaskEvent(::Display *display, long event_mask, ::XEvent *event_return)
		{
			load();
			return _XMaskEvent( display , event_mask , event_return );
		};
		Bool x11_module::XCheckMaskEvent(::Display *display, long event_mask, ::XEvent *event_return)
		{
			load();
			return _XCheckMaskEvent( display, event_mask , event_return );
		};
		Bool x11_module::XCheckTypedEvent(::Display *display, int event_type, ::XEvent *event_return)
		{
			load();
			return _XCheckTypedEvent( display, event_type, event_return );
		};
		Bool x11_module::XCheckTypedWindowEvent(::Display *display, ::Window w, int event_type, ::XEvent *event_return)
		{
			load();
			return _XCheckTypedWindowEvent( display, w, event_type, event_return);
		};
		void x11_module::XPutBackEvent(::Display *display, ::XEvent *event)
		{
			load();
			_XPutBackEvent( display, event );
		};
		Colormap x11_module::XCreateColormap(::Display *display, ::Window w, ::Visual *visual, int alloc)
		{
			load();
			return _XCreateColormap( display, w, visual, alloc);
		};
		::Window x11_module::XCreateWindow(::Display *display, ::Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int _class, ::Visual *visual, unsigned long valuemask, ::XSetWindowAttributes *attributes)
		{
			load();
			return _XCreateWindow( display, parent,  x, y, width, height, border_width, depth, _class , visual , valuemask, attributes);
		};
		int x11_module::XChangeProperty(::Display *display, ::Window w, ::Atom prop, ::Atom type, int format, int mode, unsigned char *data, int nelements)
		{
			load();
			return _XChangeProperty( display, w, prop, type, format, mode, data, nelements);
		};
		int x11_module::XGetWindowProperty(::Display *display, ::Window w, ::Atom prop, long long_offset, long long_length, Bool is_delete, ::Atom req_type, ::Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
		{
			load();
			return _XGetWindowProperty( display, w, prop , long_offset , long_length, is_delete , req_type , actual_type_return , actual_format_return , nitems_return , bytes_after_return , prop_return );
		};
		Status x11_module::XSetWMProtocols(::Display *display, ::Window w, ::Atom *protocols, int count)
		{
			load();
			return _XSetWMProtocols( display , w , protocols , count );
		};
		char *x11_module::XGetAtomName(::Display *display, ::Atom atom)
		{
			load();
			return _XGetAtomName( display , atom );
		};
		int x11_module::XLookupString(::XKeyEvent *event_struct, char *buffer_return, int bytes_buffer, ::KeySym *keysym_return, ::XComposeStatus *status_in_out)
		{
			load();
			return _XLookupString( event_struct , buffer_return , bytes_buffer , keysym_return , status_in_out );
		};
		Status x11_module::XGetWindowAttributes( Display *display , Window w , XWindowAttributes* window_attributes_return )
		{
			load();
			return _XGetWindowAttributes( display , w , window_attributes_return );
		};
		int x11_module::XGetErrorText(Display *display, int code, char *buffer_return, int length)
		{
			load();
			return _XGetErrorText( display , code , buffer_return , length );
		};
		int x11_module::XSetErrorHandler( XSetErrorHandler_handler_t handler )
		{
			load();
			return _XSetErrorHandler( handler );
		};
		int x11_module::XSetIOErrorHandler( XSetIOErrorHandler_handler_t handler )
		{
			load();
			return _XSetIOErrorHandler( handler );
		};
		Bool x11_module::XTranslateCoordinates(Display *display, Window src_w, Window dest_w, int src_x, int src_y
			, int *dest_x_return, int* dest_y_return, Window* child_return)
		{
			load();
			return _XTranslateCoordinates(display, src_w, dest_w, src_x, src_y, dest_x_return, dest_y_return, child_return);
		}
#endif
	};
};