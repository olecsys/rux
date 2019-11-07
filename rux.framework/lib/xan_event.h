#ifndef XAN_EVENT_H
#define XAN_EVENT_H
#include "xan_array.h"
#define OLD_RUX_EVENT 0
namespace rux
{
	namespace threading
	{
#ifndef INFINITE
		const ::rux::uint32 infinite = 0xFFFFFFFF;
#else
		const ::rux::uint32 infinite = INFINITE;
#endif
		begin_declare_rux_class( Event );
			~Event();
			void Set( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );	
			void Reset( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );
			::rux::uint8 IsSet( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );
			::rux::uint8 Wait( ::rux::uint32 milliseconds , const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 , ::rux::uint8 debug = 1 );
		begin_declare_rux_class_members( Event );
#if OLD_RUX_EVENT
#ifdef __WINDOWS__
			HANDLE _event_handle;
#else
			pthread_cond_t _pthread_cond;
			pthread_mutex_t _pthread_mutex;
			clockid_t _clockid;
			::rux::uint8 _is_set;
#endif
#else
			rux_volatile _is_set;
#endif
		end_declare_rux_class_members( Event );
			bool operator ==( const XEvent& event);
			void Set( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );
			void Reset( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );
			::rux::uint8 IsSet( const char* file = __FILE__ , ::rux::int32 line = __LINE__ );
			::rux::uint8 Wait( ::rux::uint32 milliseconds , const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 , ::rux::uint8 debug = 1 );	
		end_declare_rux_class();
	};
};
#endif
