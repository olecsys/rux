#ifndef XAN_WS2_32_MODULE_H
#define XAN_WS2_32_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef int (*gethostname_t)( char* name , int namelen );
		typedef int (*WSAStartup_t)( WORD wVersionRequested , LPWSADATA lpWSAData );
		typedef int (*WSACleanup_t)( void );
		typedef int (*WSAGetLastError_t)( void );
		typedef char* (FAR *inet_ntoa_t)( struct in_addr in );
		typedef struct hostent* (FAR *gethostbyname_t)( const char *name );
		typedef INT (WSAAPI *WSAAddressToStringA_t)( LPSOCKADDR lpsaAddress , DWORD dwAddressLength , LPWSAPROTOCOL_INFOA lpProtocolInfo , const char* lpszAddressString , LPDWORD lpdwAddressStringLength );
		typedef INT (WSAAPI *WSAStringToAddressA_t)( const char* AddressString , INT AddressFamily , LPWSAPROTOCOL_INFOA lpProtocolInfo , LPSOCKADDR lpAddress , LPINT lpAddressLength );
		typedef u_long (WSAAPI *htonl_t)( u_long hostlong );
		typedef int (*closesocket_t)( SOCKET s );
		typedef int (*WSAIoctl_t)( SOCKET s , DWORD dwIoControlCode , LPVOID lpvInBuffer , DWORD cbInBuffer , LPVOID lpvOutBuffer , DWORD cbOutBuffer , LPDWORD lpcbBytesReturned , LPWSAOVERLAPPED lpOverlapped , LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine );
		typedef int (*bind_t)( SOCKET s, const struct sockaddr *name , int namelen );
		typedef int (*setsockopt_t)( SOCKET s , int level , int optname , const char *optval , int optlen );
		typedef SOCKET (WSAAPI *socket_t)( int af , int type , int protocol );
		typedef int (*sendto_t)( SOCKET s , const char *buf , int len , int flags , const struct sockaddr *to , int tolen );
		typedef int (*getsockname_t)( SOCKET s , struct sockaddr *name , int *namelen );
		typedef int (*recvfrom_t)( SOCKET s , char *buf , int len , int flags , struct sockaddr *from , int *fromlen );
		typedef int (*ioctlsocket_t)( SOCKET s , long cmd , u_long *argp );
		typedef unsigned long (*inet_addr_t)( const char *cp );
		typedef int (*__WSAFDIsSet_t)( SOCKET fd , fd_set *set );
		typedef int (*select_t)( int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds , const struct timeval *timeout );
		typedef u_short (WSAAPI *htons_t)( u_short hostshort );
		typedef int (*listen_t)( SOCKET s , int backlog );
		typedef SOCKET (*accept_t)( SOCKET s , struct sockaddr *addr , int *addrlen );
		typedef int (*send_t)( SOCKET s , const char *buf , int len , int flags );
		typedef int (*recv_t)( SOCKET s , char *buf , int len , int flags );
		typedef int (*connect_t)( SOCKET s , const struct sockaddr* name , int namelen );
		class ws2_32_module : public rux::system::module
		{
		protected:
			gethostname_t _gethostname;
			WSAStartup_t _WSAStartup;
			WSACleanup_t _WSACleanup;
			WSAGetLastError_t _WSAGetLastError;
			inet_ntoa_t _inet_ntoa;
			gethostbyname_t _gethostbyname;
			WSAAddressToStringA_t _WSAAddressToStringA;
			WSAStringToAddressA_t _WSAStringToAddressA;
			htonl_t _htonl;
			closesocket_t _closesocket;
			WSAIoctl_t _WSAIoctl;
			bind_t _bind;
			setsockopt_t _setsockopt;
			socket_t _socket;
			sendto_t _sendto;
			getsockname_t _getsockname;
			recvfrom_t _recvfrom;
			ioctlsocket_t _ioctlsocket;
			inet_addr_t _inet_addr;
			__WSAFDIsSet_t ___WSAFDIsSet;
			select_t _select;
			htons_t _htons;
			listen_t _listen;
			accept_t _accept;
			send_t _send;
			recv_t _recv;
			connect_t _connect;
		public:
			ws2_32_module( void );
			virtual void on_loaded( void );
			int WSAStartup( WORD wVersionRequested , LPWSADATA lpWSAData );
			int WSACleanup( void );
			int WSAGetLastError( void );
			INT WSAAPI WSAAddressToStringA( LPSOCKADDR lpsaAddress , DWORD dwAddressLength , LPWSAPROTOCOL_INFOA lpProtocolInfo , const char* lpszAddressString , LPDWORD lpdwAddressStringLength );
			INT WSAAPI WSAStringToAddressA( const char* AddressString , INT AddressFamily , LPWSAPROTOCOL_INFOA lpProtocolInfo , LPSOCKADDR lpAddress , LPINT lpAddressLength );			
			int closesocket( SOCKET s );
			int WSAIoctl( SOCKET s , DWORD dwIoControlCode , LPVOID lpvInBuffer , DWORD cbInBuffer , LPVOID lpvOutBuffer , DWORD cbOutBuffer , LPDWORD lpcbBytesReturned , LPWSAOVERLAPPED lpOverlapped , LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine );
			int ioctlsocket( SOCKET s , long cmd , u_long *argp );
			int __WSAFDIsSet( SOCKET fd , fd_set *set );
			
			
			unsigned long inet_addr( const char *cp );
			int bind( SOCKET s, const struct sockaddr *name , int namelen );
			int setsockopt( SOCKET s , int level , int optname , const char *optval , int optlen );
			SOCKET WSAAPI socket( int af , int type , int protocol );
			int sendto( SOCKET s , const char *buf , int len , int flags , const struct sockaddr *to , int tolen );
			int getsockname( SOCKET s , struct sockaddr *name , socklen_t *namelen );
			int recvfrom( SOCKET s , char *buf , int len , int flags , struct sockaddr *from , socklen_t *fromlen );			
			int gethostname( char* name , int namelen );
			u_long WSAAPI htonl( u_long hostlong );
			char* FAR inet_ntoa( struct in_addr in );
			struct hostent* FAR gethostbyname( const char *name );
			int select( int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds , struct timeval *timeout );
			u_short WSAAPI htons( u_short hostshort );
			int listen( SOCKET s , int backlog );
			SOCKET accept( SOCKET s , struct sockaddr *addr , socklen_t *addrlen );
			int send( SOCKET s , const char *buf , int len , int flags );
			int recv( SOCKET s , char *buf , int len , int flags );
			int connect( SOCKET s , const struct sockaddr* name , int namelen );
		};
#endif
	};
};
#endif
