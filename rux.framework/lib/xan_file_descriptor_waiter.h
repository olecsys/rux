#ifndef XAN_FILE_DESCRIPTOR_WAITER_H
#define XAN_FILE_DESCRIPTOR_WAITER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_defines.h"
#ifdef __LINUX__
#include <sys/epoll.h>
#endif
namespace rux
{
	namespace io
	{
#define FILE_DESCRIPTOR_WAITER_FDS_MAX 64
		class file_descriptor_waiter
		{
			::rux::int32 _wait_res;
#ifdef __USE_EPOLL__
			::rux::int32 _epoll_fd;
			struct epoll_event _epoll_events[ FILE_DESCRIPTOR_WAITER_FDS_MAX ];
#else
			fd_set _read_fd_set;
			fd_set _write_fd_set;
			fd_set _except_fd_set;
#endif
			::rux::int32 _fds[ FILE_DESCRIPTOR_WAITER_FDS_MAX ];
			size_t _fds_count;
			::rux::byte _wait_read;
			::rux::byte _wait_write;
		public:
			file_descriptor_waiter( void );
			~file_descriptor_waiter( void );
			void add( ::rux::int32 fd );
			void clear( void );
			bool wait( ::rux::int32 timeout_ms , ::rux::byte wait_read , ::rux::byte wait_write );
			::rux::int32 get_read_fd( void );
			::rux::int32 get_write_fd( void );
			::rux::int32 get_except_fd( void );
		};
	};
};
#endif