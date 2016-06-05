#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_file_descriptor_waiter.h>
#include <xan_error.h>
#include <xan_log.h>
#include <xan_network_functions.h>
#ifdef __WINDOWS__
#define RUX_FD_ISSET(fd, set) ::rux::engine::_globals->_ws2_32_module.__WSAFDIsSet((SOCKET)(fd), (fd_set FAR *)(set))
#else
#define RUX_FD_ISSET(fd, set) FD_ISSET( fd , set )
#endif
#include <xan_thread.h>
namespace rux
{
	namespace io
	{
		file_descriptor_waiter::file_descriptor_waiter( void )
		{	
			_wait_read = 0;
			_wait_write = 0;
#ifdef __USE_EPOLL__
			_epoll_fd = -1;
#endif
			clear();
		};
		file_descriptor_waiter::~file_descriptor_waiter( void )
		{
#ifdef __USE_EPOLL__
			if( _epoll_fd != -1 )
				close( _epoll_fd );
#endif
		};
		void file_descriptor_waiter::add( ::rux::int32 fd )
		{
			_fds[ _fds_count ] = fd;
			_fds_count++;
		};
		void file_descriptor_waiter::clear( void )
		{
#ifdef __USE_EPOLL__
			if( _epoll_fd != -1 )
			{
				close( _epoll_fd );
				_epoll_fd = -1;
			}
#endif
			_wait_res = -1;
			_fds_count = 0;
		};
		bool file_descriptor_waiter::wait( ::rux::int32 timeout_ms , ::rux::byte wait_read , ::rux::byte wait_write )
		{
			_wait_res = -1;
#ifdef __USE_EPOLL__
			if( _wait_read != wait_read || _wait_write != wait_write )
			{
				if( _epoll_fd != -1 )
				{
					close( _epoll_fd );
					_epoll_fd = -1;
				}
			}
			if( _epoll_fd == -1 )
			{
				_epoll_fd = epoll_create( FILE_DESCRIPTOR_WAITER_FDS_MAX );
				if( _epoll_fd == -1 )
				{
					declare_stack_variable( char , error_str , 1024 );
					XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno , error_str );
					::rux::log::WriteError( "%s, %s:%d, epoll_create, error '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , error_str );
				}
				else
				{
					for( size_t index0 = 0 ; index0 < _fds_count ; index0++ )
					{
						struct epoll_event epoll_ev;
						epoll_ev.data.fd = _fds[ index0 ];
						epoll_ev.data.ptr = 0;
						epoll_ev.data.u32 = 0;
						epoll_ev.data.u64 = 0;
						if( wait_read && wait_write )
							epoll_ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLPRI | EPOLLET;
						else if( wait_read )
							epoll_ev.events = EPOLLIN | EPOLLRDHUP | EPOLLPRI | EPOLLET;
						else if( wait_write )
							epoll_ev.events = EPOLLOUT | EPOLLRDHUP | EPOLLET;
						else
							epoll_ev.events = EPOLLRDHUP | EPOLLET;
						if( epoll_ctl( _epoll_fd , EPOLL_CTL_ADD , _fds[ index0 ] , &epoll_ev ) != 0 )
						{
							declare_stack_variable( char , error_str , 1024 );
							XError::ConvertErrorCodeToString( errno , XEnum_ErrorType_Errno , error_str );
							::rux::log::WriteError( "%s, %s:%d, epoll_ctl, error '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , error_str );
						}
					}
				}
			}
			if( _epoll_fd != -1 )
			{
				::rux::int32 errno_value = 0 , tries = 0;
				do
				{
					_wait_res = epoll_wait( _epoll_fd , _epoll_events , _fds_count , timeout_ms );
					tries++;
				}
				while( _wait_res == -1 && ( errno_value = errno ) == EINTR && tries <= 100 );
				if( _wait_res == -1 && errno_value == EINTR )
				{
					::rux::threading::XThread::Sleep( 10 );
					_wait_res = epoll_wait( _epoll_fd , _epoll_events , _fds_count , timeout_ms );
				}
				if( _wait_res == -1 )
				{					
					declare_stack_variable( char , error_str , 1024 );
					XError::ConvertErrorCodeToString( errno_value , XEnum_ErrorType_Errno , error_str );
					::rux::log::WriteError( "%s, %s:%d, epoll_wait, error '%s'" , __FUNCTION__ , __FILE__ , __LINE__ , error_str );
					if( errno_value == EINTR )
						_wait_res = 0;
				}
			}
#else
			timeval socket_select_timewait;
			socket_select_timewait.tv_sec = timeout_ms / 1000;
			socket_select_timewait.tv_usec = 1000 * ( timeout_ms % 1000 );
			if( wait_read )
				FD_ZERO( &_read_fd_set );
			if( wait_write )
				FD_ZERO( &_write_fd_set );
			FD_ZERO( &_except_fd_set );
			::rux::int32 max_fd = -1;
			for( size_t index0 = 0 ; index0 < _fds_count ; index0++ )
			{
				if( _fds[ index0 ] > max_fd )
					max_fd = _fds[ index0 ];
				if( wait_read )
					FD_SET( _fds[ index0 ] , &_read_fd_set );
				if( wait_write )
					FD_SET( _fds[ index0 ] , &_write_fd_set );
				FD_SET( _fds[ index0 ] , &_except_fd_set );
			}
			_wait_res = ::rux::network::select( max_fd + 1 , wait_read ? &_read_fd_set : 0 , wait_write ? &_write_fd_set : 0 , &_except_fd_set , &socket_select_timewait );
#endif
			_wait_read = wait_read;
			_wait_write = wait_write;
			if( _wait_res >= 0 )
				return true;
			else
				return false;
		};
		::rux::int32 file_descriptor_waiter::get_read_fd( void )
		{
			::rux::int32 fd = -1;
			if( _wait_res > 0 )
			{				
#ifdef __USE_EPOLL__
				for( size_t index0 = 0 ; index0 < _wait_res ; ++index0 )
				{
					 if( ( _epoll_events[ index0 ].events & EPOLLIN )
						 || ( _epoll_events[ index0 ].events & EPOLLPRI ) )
					 {
						fd = _epoll_events[ index0 ].data.fd;
						break;
					 }
				}
#else		
				for( size_t index0 = 0 ; index0 < _fds_count ; index0++ )
				{
					if( RUX_FD_ISSET( _fds[ index0 ] , &_read_fd_set ) )
					{
						fd = _fds[ index0 ];
						break;
					}
				}
#endif
			}
			return fd;
		};
		::rux::int32 file_descriptor_waiter::get_write_fd( void )
		{
			::rux::int32 fd = -1;
			if( _wait_res > 0 )
			{
#ifdef __USE_EPOLL__
				for( size_t index0 = 0 ; index0 < _wait_res ; ++index0 )
				{
					 if( _epoll_events[ index0 ].events & EPOLLOUT )
					 {
						fd = _epoll_events[ index0 ].data.fd;
						break;
					 }
				}
#else		
				for( size_t index0 = 0 ; index0 < _fds_count ; index0++ )
				{
					if( RUX_FD_ISSET( _fds[ index0 ] , &_write_fd_set ) )
					{
						fd = _fds[ index0 ];
						break;
					}
				}
#endif
			}
			return fd;
		};
		::rux::int32 file_descriptor_waiter::get_except_fd( void )
		{
			::rux::int32 fd = -1;
			if( _wait_res > 0 )
			{
#ifdef __USE_EPOLL__
				for( size_t index0 = 0 ; index0 < _wait_res ; ++index0 )
				{
					 if( ( _epoll_events[ index0 ].events & EPOLLRDHUP ) 
						 || ( _epoll_events[ index0 ].events & EPOLLERR ) )
					 {
						fd = _epoll_events[ index0 ].data.fd;
						break;
					 }
				}
#else			
				for( size_t index0 = 0 ; index0 < _fds_count ; index0++ )
				{
					if( RUX_FD_ISSET( _fds[ index0 ] , &_except_fd_set ) )
					{
						fd = _fds[ index0 ];
						break;
					}
				}
#endif
			}
			return fd;
		};
	};
};