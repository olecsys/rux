#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ws2_32_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		ws2_32_module::ws2_32_module( void )
			: rux::system::module( "ws2_32.dll" )
		{
			_gethostname = 0;
			_WSAStartup = 0;
			_WSACleanup = 0;
			_WSAGetLastError = 0;
			_inet_ntoa = 0;
			_gethostbyname = 0;
			_WSAAddressToStringA = 0;
			_WSAStringToAddressA = 0;
			_htonl = 0;
			_closesocket = 0;
			_WSAIoctl = 0;
			_bind = 0;
			_setsockopt = 0;
			_socket = 0;
			_sendto = 0;
			_getsockname = 0;
			_recvfrom = 0;
			_ioctlsocket = 0;
			_inet_addr = 0;
			___WSAFDIsSet = 0;
			_select = 0;
			_htons = 0;
			_listen = 0;
			_send = 0;
			_recv = 0;
			_connect = 0;
		};
		void ws2_32_module::on_loaded( void )
		{
			system_module_dlsym( gethostname );
			system_module_dlsym( WSAStartup );
			system_module_dlsym( WSACleanup );
			system_module_dlsym( WSAGetLastError );
			system_module_dlsym( inet_ntoa );
			system_module_dlsym( gethostbyname );
			system_module_dlsym( WSAAddressToStringA );
			system_module_dlsym( WSAStringToAddressA );
			system_module_dlsym( htonl );
			system_module_dlsym( closesocket );
			system_module_dlsym( WSAIoctl );
			system_module_dlsym( bind );
			system_module_dlsym( setsockopt );
			system_module_dlsym( socket );
			system_module_dlsym( sendto );
			system_module_dlsym( getsockname );
			system_module_dlsym( recvfrom );
			system_module_dlsym( ioctlsocket );
			system_module_dlsym( inet_addr );
			system_module_dlsym( __WSAFDIsSet );
			system_module_dlsym( select );
			system_module_dlsym( htons );
			system_module_dlsym( listen );
			system_module_dlsym( accept );
			system_module_dlsym( send );
			system_module_dlsym( recv );
			system_module_dlsym( connect );
		};
		int ws2_32_module::gethostname( char* name , int namelen )
		{
			load();
			return _gethostname( name , namelen );
		};
		int ws2_32_module::WSAStartup( WORD wVersionRequested , LPWSADATA lpWSAData )
		{
			load();
			return _WSAStartup( wVersionRequested , lpWSAData );
		};
		int ws2_32_module::WSACleanup( void )
		{
			load();
			return _WSACleanup();
		};
		int ws2_32_module::WSAGetLastError( void )
		{
			load();
			return _WSAGetLastError();
		};
		char* ws2_32_module::inet_ntoa( struct in_addr in )
		{
			load();
			return _inet_ntoa( in );
		};
		struct hostent* ws2_32_module::gethostbyname( const char *name )
		{
			load();
			return _gethostbyname( name );
		};
		INT WSAAPI ws2_32_module::WSAAddressToStringA( LPSOCKADDR lpsaAddress , DWORD dwAddressLength , LPWSAPROTOCOL_INFOA lpProtocolInfo , const char* lpszAddressString , LPDWORD lpdwAddressStringLength )
		{
			load();
			return _WSAAddressToStringA( lpsaAddress , dwAddressLength , lpProtocolInfo , lpszAddressString , lpdwAddressStringLength );
		};
		INT WSAAPI ws2_32_module::WSAStringToAddressA( const char* AddressString , INT AddressFamily , LPWSAPROTOCOL_INFOA lpProtocolInfo , LPSOCKADDR lpAddress , LPINT lpAddressLength )
		{
			load();
			return _WSAStringToAddressA( AddressString , AddressFamily , lpProtocolInfo , lpAddress , lpAddressLength );
		};
		u_long WSAAPI ws2_32_module::htonl( u_long hostlong )
		{
			load();
			return _htonl( hostlong );
		};
		int ws2_32_module::closesocket( SOCKET s )
		{
			load();
			return _closesocket( s );
		};
		int ws2_32_module::WSAIoctl( SOCKET s , DWORD dwIoControlCode , LPVOID lpvInBuffer , DWORD cbInBuffer , LPVOID lpvOutBuffer , DWORD cbOutBuffer , LPDWORD lpcbBytesReturned , LPWSAOVERLAPPED lpOverlapped , LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine )
		{
			load();
			return _WSAIoctl( s , dwIoControlCode , lpvInBuffer , cbInBuffer , lpvOutBuffer , cbOutBuffer , lpcbBytesReturned , lpOverlapped , lpCompletionRoutine );
		};
		int ws2_32_module::bind( SOCKET s, const struct sockaddr *name , int namelen )
		{
			load();
			return _bind( s , name , namelen );
		};
		int ws2_32_module::setsockopt( SOCKET s , int level , int optname , const char *optval , int optlen )
		{
			load();
			return _setsockopt( s , level , optname , optval , optlen );
		};
		SOCKET ws2_32_module::socket( int af , int type , int protocol )
		{
			load();
			return _socket( af , type , protocol );
		};
		int ws2_32_module::sendto( SOCKET s , const char *buf , int len , int flags , const struct sockaddr *to , int tolen )
		{
			load();
			return _sendto( s , buf , len , flags , to , tolen );
		};
		int ws2_32_module::getsockname( SOCKET s , struct sockaddr *name , socklen_t *namelen )
		{
			load();
			return _getsockname( s , name , namelen );
		};
		int ws2_32_module::recvfrom( SOCKET s , char *buf , int len , int flags , struct sockaddr *from , socklen_t *fromlen )
		{
			load();
			return _recvfrom( s , buf , len , flags , from , fromlen );
		};
		int ws2_32_module::ioctlsocket( SOCKET s , long cmd , u_long *argp )
		{
			load();
			return _ioctlsocket( s , cmd , argp );
		};
		unsigned long ws2_32_module::inet_addr( const char *cp )
		{
			load();
			return _inet_addr( cp );
		};
		int ws2_32_module::__WSAFDIsSet( SOCKET fd , fd_set *set )
		{
			load();
			return ___WSAFDIsSet( fd , set );
		};
		int ws2_32_module::select( int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds , struct timeval *timeout )
		{
			load();
			return _select( nfds , readfds , writefds , exceptfds , timeout );
		};
		u_short ws2_32_module::htons( u_short hostshort )
		{
			load();
			return _htons( hostshort );
		};
		int ws2_32_module::listen( SOCKET s , int backlog )
		{
			load();
			return _listen( s , backlog );
		};
		SOCKET ws2_32_module::accept( SOCKET s , struct sockaddr *addr , socklen_t *addrlen )
		{
			load();
			return _accept( s , addr , addrlen );
		};
		int ws2_32_module::send( SOCKET s , const char *buf , int len , int flags )
		{
			load();
			return _send( s , buf , len , flags );
		};
		int ws2_32_module::recv( SOCKET s , char *buf , int len , int flags )
		{
			load();
			return _recv( s , buf , len , flags );
		};
		int ws2_32_module::connect( SOCKET s , const struct sockaddr* name , int namelen )
		{
			load();
			return _connect( s , name , namelen );
		};
#endif
	};
};