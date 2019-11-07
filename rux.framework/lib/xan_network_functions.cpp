#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_network_functions.h"
#include "xan_global_variables.h"
#include "xan_engine_globals.h"
namespace rux
{
	namespace network
	{
		int bind( SOCKET s, const struct sockaddr *name , int namelen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.bind( s, name , namelen );
#else
			return ::bind( s, name , namelen );
#endif
		};
		int setsockopt( SOCKET s , int level , int optname , const char *optval , int optlen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.setsockopt( s , level , optname , optval , optlen );
#else
			return ::setsockopt( s , level , optname , optval , optlen );
#endif
		};
		SOCKET socket( int af , int type , int protocol )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.socket( af , type , protocol );
#else
			return ::socket( af , type , protocol );
#endif
		};
		int sendto( SOCKET s , const char *buf , int len , int flags , const struct sockaddr *to , int tolen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.sendto( s , buf , len , flags , to , tolen );
#else
			return ::sendto( s , buf , len , flags , to , tolen );
#endif
		};
		int getsockname( SOCKET s , struct sockaddr *name , socklen_t *namelen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.getsockname( s , name , namelen );
#else
			return ::getsockname( s , name , namelen );
#endif
		};
		int recvfrom( SOCKET s , char *buf , int len , int flags , struct sockaddr *from , socklen_t *fromlen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.recvfrom( s , buf , len , flags , from , fromlen );
#else
			return ::recvfrom( s , buf , len , flags , from , fromlen );
#endif
		};
		int gethostname( char* name , int namelen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.gethostname( name , namelen );
#else
			return ::gethostname( name , namelen );
#endif
		};
		u_long htonl( u_long hostlong )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.htonl( hostlong );
#else
			return ::htonl( hostlong );
#endif
		};
		char* inet_ntoa( struct in_addr in )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.inet_ntoa( in );
#else
			return ::inet_ntoa( in );
#endif
		};
		struct hostent* gethostbyname( const char *name )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.gethostbyname( name );
#else
			return ::gethostbyname( name );
#endif
		};
		int select( int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds , struct timeval *timeout )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.select( nfds , readfds , writefds , exceptfds , timeout );
#else
			return ::select( nfds , readfds , writefds , exceptfds , timeout );
#endif
		};
		u_short htons( u_short hostshort )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.htons( hostshort );
#else
			return ::htons( hostshort );
#endif
		};
		int listen( SOCKET s , int backlog )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.listen( s , backlog );
#else
			return ::listen( s , backlog );
#endif
		};
		SOCKET accept( SOCKET s , struct sockaddr *addr , socklen_t *addrlen )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.accept( s , addr , addrlen );
#else
			return ::accept( s , addr , addrlen );
#endif
		};
		int send( SOCKET s , const char *buf , int len , int flags )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.send( s , buf , len , flags );
#else
			return ::send( s , buf , len , flags );
#endif
		};
		int recv( SOCKET s , char *buf , int len , int flags )
		{
#ifdef __WINDOWS__
			return ::rux::engine::_globals->_ws2_32_module.recv( s , buf , len , flags );
#else
			return ::recv( s , buf , len , flags );
#endif
		};
	};
};