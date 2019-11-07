#ifndef XAN_NATIVE_DIAGNOSTICS_H
#define XAN_NATIVE_DIAGNOSTICS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
namespace rux
{
	namespace diagnostics
	{
		class process_info
		{
		public:
			static ::rux::int64 working_set(::rux::pid_t pid, ::rux::XString* error);
			static ::rux::int64 virtual_bytes(::rux::pid_t pid, ::rux::XString* error);
			static ::rux::int64 working_set( const char* process_name , ::rux::XString* error );
			static ::rux::int64 virtual_bytes( const char* process_name , ::rux::XString* error );
			static pid_t pidof( const char* process_name , ::rux::pid_t include_pid = 0 , ::rux::pid_t except_pid = 0 
				, bool check_only_fullname = false );
			static ::rux::int64 threads_count( ::rux::pid_t pid , ::rux::XString* error );
			static ::rux::int64 descriptors_count( ::rux::pid_t pid , ::rux::XString* error );
		};
	};
};
#endif