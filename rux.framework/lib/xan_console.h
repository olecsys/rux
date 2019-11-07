#ifndef XAN_CONSOLE_H
#define XAN_CONSOLE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
namespace rux
{
	class XConsole;
};
enum XColors
{
	XWhite,
	XGreen,
	XRed
};
namespace rux
{
	class XConsole
	{
	private:	
	public:
			::rux::uint8 _is_exit;
		XConsole();	
		::rux::String& Read( void );
		static void WaitForEnter( void );
		static void Initialize( rux_start_event_t start_event , before_exit_event_t before_exit_event );
		static void DeInitialize( void );
		void Process( LPXPROCESSFN pfnProcess , int argc , char ** args );
		void Exit( void );
		::rux::uint8 get_IsExit( void );
		static void __cdecl Print( FILE* file_ptr , XColors color , const char* format );
		static void __cdecl Print( FILE* file_ptr , XColors color , const char* format , ... );
		static void __cdecl Print( FILE* file_ptr , XColors color , const ::rux::XString& string );
	#if !defined( __SOLARIS__ )	
		static void console_posix_ctrl_c_signal( ::rux::int32 signum );
	#endif	
		static void WriteToStdOut( const char* text_ptr );
		static void WriteToStdErr( const char* text_ptr );
		static void WriteToStdIn( const char* text_ptr );
		static void ShowWindow( void );
		static void HideWindow( void );
	};
	#if defined( __SOLARIS__ )
	extern "C" void console_posix_ctrl_c_signal( ::rux::int32 signum );
	#endif
};
#endif
