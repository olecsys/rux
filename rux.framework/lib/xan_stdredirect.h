#ifndef XAN_STDREDIRECT_H
#define XAN_STDREDIRECT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_thread.h>
namespace rux
{	
	begin_declare_rux_class( StdRedirect );
		~StdRedirect();
	begin_declare_rux_class_members( StdRedirect );
		size_t _buffer_size;
		::rux::XString _buffer;
		XCrtSect _cs_buffer;
		::rux::threading::XThread _redirect;
		XCrtSect _cs_redirect;
		rux_simple_t _on_changed;
	end_declare_rux_class_members( StdRedirect );
		void Start( void );
		void set_OnChanged( rux_simple_t on_changed );
		void Stop( void );
		static void redirect_thread( void* param , size_t ___rux__thread_index1986 );
		::rux::String& get_Buffer( void );
	end_declare_rux_class();
};
#endif
