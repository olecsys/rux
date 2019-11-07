#ifndef XAN_NETWORK_FUNCTIONS_H
#define XAN_NETWORK_FUNCTIONS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
namespace rux
{
	namespace network
	{
		dll_internal int bind( SOCKET s, const struct sockaddr *name , int namelen );
		dll_internal int setsockopt( SOCKET s , int level , int optname , const char *optval , int optlen );
		dll_internal SOCKET socket( int af , int type , int protocol );
		dll_internal int sendto( SOCKET s , const char *buf , int len , int flags , const struct sockaddr *to , int tolen );
		dll_internal int getsockname( SOCKET s , struct sockaddr *name , socklen_t *namelen );
		dll_internal int recvfrom( SOCKET s , char *buf , int len , int flags , struct sockaddr *from , socklen_t *fromlen );			
		dll_internal int gethostname( char* name , int namelen );
		dll_internal u_long htonl( u_long hostlong );
		dll_internal char* inet_ntoa( struct in_addr in );
		dll_internal struct hostent* gethostbyname( const char *name );
		dll_internal int select( int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds , struct timeval *timeout );
		dll_internal u_short htons( u_short hostshort );
		dll_internal int listen( SOCKET s , int backlog );
		dll_internal SOCKET accept( SOCKET s , struct sockaddr *addr , socklen_t *addrlen );
		dll_internal int send( SOCKET s , const char *buf , int len , int flags );
		dll_internal int recv( SOCKET s , char *buf , int len , int flags );
	};
};
#endif
