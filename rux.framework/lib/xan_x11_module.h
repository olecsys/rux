#ifndef XAN_X11_MODULE_H
#define XAN_X11_MODULE_H
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
#endif
#endif
namespace rux
{
	namespace system
	{
#ifdef __UNIX__
		typedef Bool (*predicate_t)( Display* d , XEvent* e , char* arg );
		typedef int (*XSetErrorHandler_handler_t)(Display *, XErrorEvent *);
		typedef int (*XSetIOErrorHandler_handler_t)(Display *);
		typedef int (*XSetErrorHandler_t)( XSetErrorHandler_handler_t );
		typedef int (*XSetIOErrorHandler_t)( XSetIOErrorHandler_handler_t handler );
		typedef ::Display* (*XOpenDisplay_t)( char* display_name );
		typedef int (*XCloseDisplay_t)( ::Display* display );
		typedef int (*XDestroyWindow_t)(::Display *display, ::Window w);
		typedef Status (*XSendEvent_t)(::Display *display, ::Window w, Bool propagate, long event_mask, ::XEvent *event_send);
		typedef ::Atom (*XInternAtom_t)(::Display *display, const char *atom_name, Bool only_if_exists);
		typedef int (*XIfEvent_t)(::Display *display, ::XEvent *event_return, predicate_t predicate , ::XPointer arg);
		typedef int (*XMapWindow_t)(::Display *display, ::Window w);
		typedef int (*XMapRaised_t)(::Display *display, ::Window w);
		typedef int (*XConfigureWindow_t)(::Display *display, ::Window w, unsigned value_mask, ::XWindowChanges *changes);
		typedef int (*XMoveWindow_t)(::Display *display, ::Window w, int x, int y);
		typedef int (*XResizeWindow_t)(::Display *display, ::Window w, unsigned width, unsigned height);
		typedef int (*XMoveResizeWindow_t)(::Display *display, ::Window w, int x, int y, unsigned width, unsigned height);
		typedef int (*XFlush_t)(::Display *display);
		typedef int (*XSync_t)(::Display *display, Bool discard);
		typedef int (*XEventsQueued_t)(::Display *display, int mode);
		typedef int (*XPending_t)(::Display *display);
		typedef ::Cursor (*XCreateFontCursor_t)(::Display *display, unsigned int shape);
		typedef int (*XDefineCursor_t)(::Display *display, ::Window w, ::Cursor cursor);
		typedef int (*XUndefineCursor_t)(::Display *display, ::Window w);
		typedef Bool (*XQueryPointer_t)(::Display *display, ::Window w, ::Window *root_return, ::Window *child_return, int *root_x_return, int *root_y_return, int *win_x_return, int *win_y_return, unsigned int *mask_return);
		typedef int (*XUnmapWindow_t)(::Display *display, ::Window w);
		typedef int (*XGrabPointer_t)(::Display *display, ::Window grab_window, Bool owner_events, unsigned int event_mask, int pointer_mode, int keyboard_mode, ::Window confine_to, ::Cursor cursor, ::Time time);
		typedef int (*XUngrabPointer_t)(::Display *display, ::Time time);
		typedef Status (*XInitThreads_t)(void);
		typedef void (*XLockDisplay_t)(::Display *display);
		typedef void (*XUnlockDisplay_t)(::Display *display);
		typedef XVisualInfo* (*XGetVisualInfo_t)(::Display *display, long vinfo_mask, XVisualInfo* vinfo_template, int *nitems_return);
		typedef int (*XFree_t)(void *data);
		typedef int (*XSelectInput_t)(::Display *display, ::Window w, long event_mask);
		typedef int (*XNextEvent_t)(::Display *display, ::XEvent *event_return);
		typedef int (*XPeekEvent_t)(::Display *display, ::XEvent *event_return);
		typedef int (*XWindowEvent_t)(::Display *display, ::Window w, long event_mask, ::XEvent *event_return);
		typedef Bool (*XCheckWindowEvent_t)(::Display *display, ::Window w, long event_mask, ::XEvent *event_return);
		typedef int (*XMaskEvent_t)(::Display *display, long event_mask, ::XEvent *event_return);
		typedef Bool (*XCheckMaskEvent_t)(::Display *display, long event_mask, ::XEvent *event_return);
		typedef Bool (*XCheckTypedEvent_t)(::Display *display, int event_type, ::XEvent *event_return);
		typedef Bool (*XCheckTypedWindowEvent_t)(::Display *display, ::Window w, int event_type, ::XEvent *event_return);
		typedef void (*XPutBackEvent_t)(::Display *display, ::XEvent *event);
		typedef Colormap (*XCreateColormap_t)(::Display *display, ::Window w, ::Visual *visual, int alloc);
		typedef ::Window (*XCreateWindow_t)(::Display *display, ::Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int _class, ::Visual *visual, unsigned long valuemask, ::XSetWindowAttributes *attributes);
		typedef int (*XChangeProperty_t)(::Display *display, ::Window w, ::Atom prop, ::Atom type, int format, int mode, unsigned char *data, int nelements);
		typedef int (*XGetWindowProperty_t)(::Display *display, ::Window w, ::Atom prop, long long_offset, long long_length, Bool is_delete, ::Atom req_type, ::Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
		typedef Status (*XSetWMProtocols_t)(::Display *display, ::Window w, ::Atom *protocols, int count);
		typedef char *(*XGetAtomName_t)(::Display *display, ::Atom atom);
		typedef int (*XLookupString_t)(::XKeyEvent *event_struct, char *buffer_return, int bytes_buffer, ::KeySym *keysym_return, XComposeStatus *status_in_out);
		typedef Status (*XGetWindowAttributes_t)(Display *display, Window w, XWindowAttributes *window_attributes_return);
		typedef int (*XGetErrorText_t)(Display *display, int code, char *buffer_return, int length);
		class x11_module : public rux::system::module
		{
		protected:
			XOpenDisplay_t _XOpenDisplay;
			XCloseDisplay_t _XCloseDisplay;
			XDestroyWindow_t _XDestroyWindow;
			XSendEvent_t _XSendEvent;
			XInternAtom_t _XInternAtom;
			XIfEvent_t _XIfEvent;
			XMapWindow_t _XMapWindow;
			XMapRaised_t _XMapRaised;
			XConfigureWindow_t _XConfigureWindow;
			XMoveWindow_t _XMoveWindow;
			XResizeWindow_t _XResizeWindow;
			XMoveResizeWindow_t _XMoveResizeWindow;
			XFlush_t _XFlush;
			XSync_t _XSync;
			XEventsQueued_t _XEventsQueued;
			XPending_t _XPending;
			XCreateFontCursor_t _XCreateFontCursor;
			XDefineCursor_t _XDefineCursor;
			XUndefineCursor_t _XUndefineCursor;
			XQueryPointer_t _XQueryPointer;
			XUnmapWindow_t _XUnmapWindow;
			XGrabPointer_t _XGrabPointer;
			XUngrabPointer_t _XUngrabPointer;
			XInitThreads_t _XInitThreads;
			XLockDisplay_t _XLockDisplay;
			XUnlockDisplay_t _XUnlockDisplay;
			XGetVisualInfo_t _XGetVisualInfo;
			XFree_t _XFree;
			XSelectInput_t _XSelectInput;
			XNextEvent_t _XNextEvent;
			XPeekEvent_t _XPeekEvent;
			XWindowEvent_t _XWindowEvent;
			XCheckWindowEvent_t _XCheckWindowEvent;
			XMaskEvent_t _XMaskEvent;
			XCheckMaskEvent_t _XCheckMaskEvent;
			XCheckTypedEvent_t _XCheckTypedEvent;
			XCheckTypedWindowEvent_t _XCheckTypedWindowEvent;
			XPutBackEvent_t _XPutBackEvent;
			XCreateColormap_t _XCreateColormap;
			XCreateWindow_t _XCreateWindow;
			XChangeProperty_t _XChangeProperty;
			XGetWindowProperty_t _XGetWindowProperty;
			XSetWMProtocols_t _XSetWMProtocols;
			XGetAtomName_t _XGetAtomName;
			XLookupString_t _XLookupString;
			XGetWindowAttributes_t _XGetWindowAttributes;
			XGetErrorText_t _XGetErrorText;
			XSetErrorHandler_t _XSetErrorHandler;
			XSetIOErrorHandler_t _XSetIOErrorHandler;
		public:
			x11_module( void );
			virtual void on_loaded( void );
			::Display* XOpenDisplay( char* display_name );
			int XCloseDisplay( ::Display* display );
			int XDestroyWindow(::Display *display, ::Window w);
			Status XSendEvent(::Display *display, ::Window w, Bool propagate, long event_mask, ::XEvent *event_send);
			::Atom XInternAtom(::Display *display, const char *atom_name, Bool only_if_exists);
			int XIfEvent(::Display *display, ::XEvent *event_return, predicate_t predicate , ::XPointer arg);
			int XMapWindow(::Display *display, ::Window w);
			int XMapRaised(::Display *display, ::Window w);
			int XConfigureWindow(::Display *display, ::Window w, unsigned value_mask, ::XWindowChanges *changes);
			int XMoveWindow(::Display *display, ::Window w, int x, int y);
			int XResizeWindow(::Display *display, ::Window w, unsigned width, unsigned height);
			int XMoveResizeWindow(::Display *display, ::Window w, int x, int y, unsigned width, unsigned height);
			int XFlush(::Display *display);
			int XSync(::Display *display, Bool discard);
			int XEventsQueued(::Display *display, int mode);
			int XPending(::Display *display);
			::Cursor XCreateFontCursor(::Display *display, unsigned int shape);
			int XDefineCursor(::Display *display, ::Window w, ::Cursor cursor);
			int XUndefineCursor(::Display *display, ::Window w);
			Bool XQueryPointer(::Display *display, ::Window w, ::Window *root_return, ::Window *child_return, int *root_x_return, int *root_y_return, int *win_x_return, int *win_y_return, unsigned int *mask_return);
			int XUnmapWindow(::Display *display, ::Window w);
			int XGrabPointer(::Display *display, ::Window grab_window, Bool owner_events, unsigned int event_mask, int pointer_mode, int keyboard_mode, ::Window confine_to, ::Cursor cursor, ::Time time);
			int XUngrabPointer(::Display *display, ::Time time);
			Status XInitThreads(void);
			void XLockDisplay(::Display *display);
			void XUnlockDisplay(::Display *display);
			XVisualInfo* XGetVisualInfo(::Display *display, long vinfo_mask, XVisualInfo* vinfo_template, int *nitems_return);
			int XFree(void *data);
			int XSelectInput(::Display *display, ::Window w, long event_mask);
			int XNextEvent(::Display *display, ::XEvent *event_return);
			int XPeekEvent(::Display *display, ::XEvent *event_return);
			int XWindowEvent(::Display *display, ::Window w, long event_mask, ::XEvent *event_return);
			Bool XCheckWindowEvent(::Display *display, ::Window w, long event_mask, ::XEvent *event_return);
			int XMaskEvent(::Display *display, long event_mask, ::XEvent *event_return);
			Bool XCheckMaskEvent(::Display *display, long event_mask, ::XEvent *event_return);
			Bool XCheckTypedEvent(::Display *display, int event_type, ::XEvent *event_return);
			Bool XCheckTypedWindowEvent(::Display *display, ::Window w, int event_type, ::XEvent *event_return);
			void XPutBackEvent(::Display *display, ::XEvent *event);
			Colormap XCreateColormap(::Display *display, ::Window w, ::Visual *visual, int alloc);
			::Window XCreateWindow(::Display *display, ::Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int _class, ::Visual *visual, unsigned long valuemask, ::XSetWindowAttributes *attributes);
			int XChangeProperty(::Display *display, ::Window w, ::Atom prop, ::Atom type, int format, int mode, unsigned char *data, int nelements);
			int XGetWindowProperty(::Display *display, ::Window w, ::Atom prop, long long_offset, long long_length, Bool is_delete, ::Atom req_type, ::Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
			Status XSetWMProtocols(::Display *display, ::Window w, ::Atom *protocols, int count);
			char *XGetAtomName(::Display *display, ::Atom atom);
			int XLookupString(::XKeyEvent *event_struct, char *buffer_return, int bytes_buffer, ::KeySym *keysym_return, ::XComposeStatus *status_in_out);
			Status XGetWindowAttributes( Display *display , Window w , XWindowAttributes* window_attributes_return );
			int XGetErrorText(Display *display, int code, char *buffer_return, int length);
			int XSetErrorHandler( XSetErrorHandler_handler_t handler );
			int XSetIOErrorHandler( XSetIOErrorHandler_handler_t handler );
		};
#endif
	};
};
#endif
