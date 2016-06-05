#include <xan_console.h>
#include <xan_tcpserver.h>
#include <xan_udpserver.h>
#include <xan_udpclient.h>
#include <xan_tcpclient.h>
#include <xan_icmpclient.h>
#include <xan_smb2client.h>
#include <xan_smbclient.h>
#include <xan_network_information.h>
#include <xan_networkaddress.h>
#include <xan_malloc_thread.h>
#include <xan_asn_defines.h>
const char* g_current_module_name = "rux.network.test.exe";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
rux::network::XTcpServer* _tcp_server = NULL;
rux::network::XUdpServer* _udp_server = NULL;
rux::network::XUdpClient* _udp_client = NULL;
rux::network::XTcpClient* _tcp_client = NULL;
rux::network::XIcmpClient* _icmp_client = NULL;
rux::network::XUdpServer* _udp_broadcast_server = NULL;
rux::network::SmbClient* _smb_client = NULL;
void on_http_received( XData& recv , XData& send , const char* extern_ip , const char* intern_ip )
{
	rux::XString interface_string , error;
	interface_string.set_ByRef( rux::network::XNetworkInformation::get_Interface( intern_ip , error ) );
	printf( "Extern ip = '%s'\nIntern ip = '%s'\nInterface = '%s'\n" , extern_ip , intern_ip , interface_string()->_array_ptr );
	const char* send_data = "HELLO,MY FRIEND!!!\n";
	send.Copy( (void*)send_data , strlen( send_data ) + 1 );
};
void on_received( XData& recv , XData& send , const char* extern_ip , const char* intern_ip )
{
	rux::XString interface_string , error;
	interface_string.set_ByRef( rux::network::XNetworkInformation::get_Interface( intern_ip , error ) );
	printf( "Extern ip = '%s'\nIntern ip = '%s'\nInterface = '%s'\n" , extern_ip , intern_ip , interface_string()->_array_ptr );
	const char* send_data = "HELLO,MY FRIEND!!!\n";
	send.Copy( (void*)send_data , strlen( send_data ) + 1 );
};
void on_udp_broadcast( XData& recv , XData& send , const char* extern_ip , const char* intern_ip );
void process( char* line_ptr , rux::XConsole* console_ptr )
{	
	rux::XString error;
	rux::XString line( line_ptr );
	line.ToLower();
	if( line.ToLower() == "quit"
		|| line.ToLower() == "exit"
		|| line.ToLower() == "q" )
	{
		console_ptr->Exit();
	}
	else if( line.ToLower() == "tcpsrv"
		|| line.ToLower() == "tcpsrv\n" )
	{	
		if( _tcp_server == NULL )
		{
			_tcp_server = rux::engine::alloc_object< rux::network::XTcpServer >();
			_tcp_server->StartListen( rux::network::XAddress( "0.0.0.0" , 19860 , error ) , error );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
			_tcp_server->set_OnReceived( on_received );
		}
	}
	else if( line.ToLower() == "smbcli"
		|| line.ToLower() == "smbcli\n" )
	{			
		if( _smb_client == NULL )
		{	
			declare_stack_variable( char , exec_dir , 1024 );
			rux_get_executable_directory( exec_dir );
#ifdef __WINDOWS__
#ifdef __x86__
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win32" );
#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../../extern_lib/windows/win64" );
#endif
#else
#ifdef __x86__
			rux_concatenation( exec_dir , "/../extern_lib/linux/x86" );
#elif defined( __x64__ )
			rux_concatenation( exec_dir , "/../extern_lib/linux/x64" );
#endif
#endif
			rux_add_search_directory( exec_dir );
			
			_smb_client = rux::engine::alloc_object< rux::network::SmbClient >();
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
		}

		char challenge[ 8 ] = {0x10 , 0x2D , 0xB5 , 0xDF , 0x08 , 0x5D , 0x30 , 0x41} , response[ 24 ] = {0};
		rux::network::ntlm::LmChallengeResponse( challenge , "MyPw" , response );
		rux::network::ntlm::NtChallengeResponse( challenge , "MyPw" , response );
		char nt_challenge_response[] = { 0x4E , 0x9D , 0x3C , 0x8F , 0x9C , 0xFD , 0x38 , 0x5D 
			, 0x5B , 0xF4 , 0xD3 , 0x24 , 0x67 , 0x91 , 0x95 , 0x6C
			, 0xA4 , 0xC3 , 0x51 , 0xAB , 0x40 , 0x9A , 0x3D , 0x61 };
		if( memcmp( response , nt_challenge_response , sizeof( nt_challenge_response ) ) == 0 )
		{

		}

		if( _smb_client->Logon( "test" , "test" , "192.168.40.88" , error ) == 1 )
		{
			rux::uint16 TID = 0;
			if( _smb_client->TreeConnect( "\\\\192.168.40.88\\IntegraVideo7" , TID , error ) == 1 )
			{
				rux::uint16 FID = 0;
				if( _smb_client->CreateFile( "\\EXCEPTIONS\\SMBTEST" , TID , FID , error ) == 1 )
				{
					_smb_client->WriteFile( TID , FID , (rux::uint8*)"GROUP" , 5 , 0 , error );

					_smb_client->CloseFile( TID , FID , error );
				}
				else
					rux::XConsole::Print( stderr , XRed , error );
			}
			_smb_client->Logoff();
		}
		rux::network::SmbClient::CopyFile( "D:\\video-server-7.0.325-x86.exe" , "\\\\192.168.40.90\\linux_docs" , "rux-olecsys\\rux" , "4058olecsys" , error );
	}
	else if( line.ToLower() == "smb2cli"
		|| line.ToLower() == "smb2cli\n" )
	{	
		if( _tcp_client == NULL )
		{			
			_tcp_client = rux::engine::alloc_object< rux::network::XTcpClient >();
			_tcp_client->set_NetworkAddress( rux::network::XAddress( "192.168.40.88" , 445 , error ) );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
		}
		rux::network::smb2_negotiate_request smb2_negotiate_request;
		_tcp_client->Send( (char*)&smb2_negotiate_request , sizeof( smb2_negotiate_request ) , error );
		if( error.Length() == 0 )
		{
			XData* recv_data = NULL;
			_tcp_client->Receive( &recv_data , error );
			if( recv_data )
			{
				printf( "%p\n" , recv_data );
			}
		}

		rux::network::smb2_session_setup_request smb2_session_setup_request;		
		_tcp_client->Send( (char*)&smb2_session_setup_request , sizeof( smb2_session_setup_request ) , error );
		if( error.Length() == 0 )
		{
			XData* recv_data = NULL;
			_tcp_client->Receive( &recv_data , error );
			if( recv_data )
			{
				printf( "%p\n" , recv_data );
			}
		}
	}
	else if( line.ToLower() == "httpcli"
		|| line.ToLower() == "httpcli\n" )
	{			
		if( _tcp_client == NULL )
		{
			rux::XArray< rux::XString > addrs;
			addrs.set_ByRef( rux::network::XNetworkInformation::get_IPAddresses( "www.google.com" , error ) );
			for( size_t index0 = 0 ; index0 < addrs.Count() ; index0++ )
			{
				addrs[ index0 ] += "\n";
				rux::XConsole::Print( stdout , XWhite , addrs[ index0 ] );
			}
			_tcp_client = rux::engine::alloc_object< rux::network::XTcpClient >();
			_tcp_client->set_NetworkAddress( rux::network::XAddress( addrs[ 0 ] , 80 , error ) );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
		}
		const char* send_data = "GET / HTTP/1.1\r\n"
			"Host: www.google.com\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 5.1; rv:25.0) Gecko/20100101 Firefox/25.0\r\n"
			"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
			"Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Cookie: doxygen_width=210; plg__width=300; cppcms_session=Cq8zj9Ar9FV_lXGd9KTKs9K1b8wIQ7qVSBUeCkTEHKOVjEJf2ftljLybxTrrLn0GJa660xe5Xf8z4M0KUe_o146RLRPTBfeyzeuEKnnNbv1J6pAlWZVA2Qnk7w0VzACYa6wMcEw7PhzQsH7YABurXuelgHqtjrl88VMgRJlSfF3YCUaT70ZaenFoYQ2Ph5kJ6Z1XJgfA6oVMt5umGswD8Y0loaz4; back=\"/admin/plugins/profiles\"; flash=\r\n"
			"Connection: keep-alive\r\n"
			"\r\n";
		XData* recv_data = NULL;
		_tcp_client->Send( (char*)send_data , strlen( send_data ) , error );
		if( error.Length() > 0 )
			printf( "error = '%s'\n" , error()->_array_ptr );
		else if( recv_data )
		{
			rux::uint32 data_size = recv_data->_data_size;
			char* data = (char*)recv_data->Read( data_size );
			if( data )
			{
				data[ data_size - 1 ] = 0;
				char* begin = data , * ptr = data;
				do
				{
					ptr = strstr( ptr , "\r\n" );
					if( ptr )
					{
						*ptr = 0;

						printf( "%s\n\n" , begin );

						char* field_name = begin;
						begin = ptr;
						if( ptr - field_name > 1 )
						{
							ptr = strstr( field_name , ":" );
							if( ptr )
							{
								ptr++;
								char* field_value = ptr;
								printf( "%s\n\n" , field_value );
							}
						}
						else
						{
							ptr++;
							char* field_content = ptr;
							printf( "%s\n\n" , field_content );
						}						
						begin += 2;						
						if( begin[ 0 ] == 0 )
							break;
						ptr = begin;
					}
				}
				while( ptr );
			}
		}
	}
	else if( line.ToLower() == "icmpcli"
		|| line.ToLower() == "icmpcli\n" )
	{			
		rux::XString ip = "192.168.40.102";
		if( _icmp_client == NULL )
		{
			rux::XArray< rux::XString > addrs;
			addrs.set_ByRef( rux::network::XNetworkInformation::get_IPAddresses( "www.google.com" , error ) );
			for( size_t index0 = 0 ; index0 < addrs.Count() ; index0++ )
			{
				addrs[ index0 ] += "\n";
				rux::XConsole::Print( stdout , XWhite , addrs[ index0 ] );
			}
			_icmp_client = rux::engine::alloc_object< rux::network::XIcmpClient >();
			ip = addrs[ 0 ];
			_icmp_client->set_NetworkAddress( rux::network::XAddress( ip , 80 , error ) );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
		}
		size_t packet_count = 4 , received_packets = 0;
		rux::uint32 min_time = 0 , max_time = 0 , avg_time = 0;
		_icmp_client->Echo( 32 , packet_count , received_packets , max_time , min_time , avg_time , error );
		if( error.Length() > 0 )
			printf( "error = '%s'\n" , error()->_array_ptr );
		else
		{
			printf( "\nStatistic Ping for %s:\n\tPackets: sended = %u, received = %u, lost = %u (%u%% loses),\n\tmin time = %u ms. , max time = %u ms. , average time = %u ms.\n" ,
				ip()->_array_ptr , packet_count , received_packets , packet_count - received_packets , (rux::uint32)( 100.f * (float)( packet_count - received_packets ) / (float)packet_count ) , min_time , max_time , avg_time );
		}
	}
	else if( line.ToLower() == "httpsrv"
		|| line.ToLower() == "httpsrv\n" )
	{	
		if( _tcp_server == NULL )
		{
			_tcp_server = rux::engine::alloc_object< rux::network::XTcpServer >();
			_tcp_server->StartListen( rux::network::XAddress( "0.0.0.0" , 19860 , error ) , error );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
			_tcp_server->set_OnReceived( on_http_received );
		}
	}
	else if( line.ToLower() == "udpsrv"
		|| line.ToLower() == "udpsrv\n" )
	{	
		if( _udp_server == NULL )
		{
			_udp_server = rux::engine::alloc_object< rux::network::XUdpServer >();
			_udp_server->StartListen( rux::network::XAddress( "0.0.0.0" , 19860 , error ) , error );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );
			_udp_server->set_OnReceived( on_received );
		}
	}
	else if( line.ToLower() == "udpcli"
		|| line.ToLower() == "udpcli\n" )
	{	
		if( _udp_client == NULL )
		{
			_udp_client = rux::engine::alloc_object< rux::network::XUdpClient >();
			_udp_client->set_NetworkAddress( rux::network::XAddress( "192.168.40.64" , 19860 , error ) );
			if( error.Length() > 0 )
				printf( "error = '%s'\n" , error()->_array_ptr );

		}
		const char* send_data = "PRIVET MEDVED!!!\n";
		XData* recv_data = NULL;
		_udp_client->Send( (char*)send_data , strlen( send_data ) + 1 , &recv_data , error );
		if( error.Length() > 0 )
			printf( "error = '%s'\n" , error()->_array_ptr );
		else if( recv_data )
			printf( "recv msg = '%s'\n" , (char*)recv_data->_data );
	}
	else if( line.ToLower() == "interfaces"
		|| line.ToLower() == "interfaces\n" )
	{
		rux::XArray< rux::XString > interfaces;
		interfaces.set_ByRef( rux::network::XNetworkInformation::get_Interfaces( error ) );
		if( error.Length() == 0 )
		{
			for( size_t index0 = 0 ; index0 < interfaces.Count() ; index0++ )			
				printf( "'%s'\n" , interfaces[ index0 ]._array_ptr );
		}
	}
	else if( line.ToLower() == "add_ipv4_address"
		|| line.ToLower() == "add_ipv4_address\n" )
	{
		rux::XArray< rux::XString > interfaces;
		interfaces.set_ByRef( rux::network::XNetworkInformation::get_Interfaces( error ) );
		if( error.Length() == 0 )
		{
			for( size_t index0 = 0 ; index0 < interfaces.Count() ; index0++ )		
			{
				if( interfaces[ index0 ] != "MS TCP Loopback interface" 
					&& interfaces[ index0 ] != "lo"
					&& interfaces[ index0 ] != "pan0" )
				{
					rux::network::AddNetworkIpv4Address( interfaces[ index0 ] , "192.168.40.64" , error );
					if( error.Length() > 0 )
						printf( "error = '%s'\n" , error()->_array_ptr );
					break;
				}
			}
		}
	}
	else if( line.ToLower() == "connect_zavis"
		|| line.ToLower() == "connect_zavis\n" )
	{
		if( _udp_broadcast_server == NULL )
		{
			_udp_broadcast_server = rux::engine::alloc_object< rux::network::XUdpServer >();
			rux::XString error;
			_udp_broadcast_server->set_IsBroadcast( 1 );
			_udp_broadcast_server->set_OnReceived( on_udp_broadcast );
			_udp_broadcast_server->StartListen( rux::network::XAddress( "0.0.0.0" , 20036 , error ) , error );
		}
	}
}
XCrtSect _cs_commands_thread;
XMallocThread _download_or_check_registered_thread;
void Test( void )
{
#ifdef __WINDOWS__
	int ret = -1;
	socklen_t sz = sizeof( sockaddr_in );
	sockaddr_in sa127;
	memset( &sa127 , 0 , sizeof( sockaddr_in ) );
	sa127.sin_family=AF_INET;
	sa127.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
	rux::int32 SignalSocket = socket( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	if( SignalSocket == -1 )
		return;
	printf( "TEST SOCKET BEFORE rconfig %p %u\n" , ::connect , rux::threading::XThread::get_CurrentThreadId() );
	ret = ::connect( SignalSocket , (SOCKADDR*)&sa127 , sizeof( sockaddr_in ) );
	printf( "TEST SOCKET AFTER rconfig %p %u\n" , ::connect , rux::threading::XThread::get_CurrentThreadId() );
	if( ret == -1 )
	{
		closesocket( SignalSocket );
		ret=WSAGetLastError();
		return;
	};
	ret = getsockname( SignalSocket , (SOCKADDR*)&sa127 , &sz );
	if( ret == -1 )
	{
		closesocket( SignalSocket );
		ret = WSAGetLastError();
		return;
	};
	closesocket( SignalSocket );
#endif
};
void download_thread( void* param )
{	
	Test();
};
void on_udp_broadcast( XData& recv , XData& send , const char* extern_ip , const char* intern_ip )
{	
	if( recv._data_size > 0 )
	{
		size_t data_size = recv._data_size;
		char* json = (char*)recv.Read( data_size );
		if( json[ data_size - 1 ] == 0 )
		{
			CRITICAL_SECTION_LOCK( _cs_commands_thread );
			if( _download_or_check_registered_thread.get_IsStarted() == 0 )
			{
				_download_or_check_registered_thread.set_ThreadMode( XThreadMode_OneTime );
				//_download_or_check_registered_thread.set_Timeout( 30000 );
				_download_or_check_registered_thread.set_ThreadName( "booter download" );
				_download_or_check_registered_thread.Start();
			}
			_download_or_check_registered_thread.Push( download_thread , _udp_broadcast_server );
			CRITICAL_SECTION_UNLOCK( _cs_commands_thread );

			/*CRITICAL_SECTION_LOCK( (*g_booter)()->_cs_commands_thread );
			if( (*g_booter)()->_commands_thread.get_IsStarted() == 0 )
			{
				(*g_booter)()->_commands_thread.set_ThreadMode( XThreadMode_OneTime );	
				(*g_booter)()->_commands_thread.set_ThreadName( "booter commands" );
				(*g_booter)()->_commands_thread.Start();
			}
			(*g_booter)()->_commands_thread.Push( private_Commands , 0 );	
			CRITICAL_SECTION_UNLOCK( (*g_booter)()->_cs_commands_thread );*/
			

			//Test();
			/*rux::XString error;
			XObject json_object( XJsonSerializer::Deserialize( json , 1 , error ) );
			if( error.Length() == 0 )
			{				
				if( rux_is_object( json_object , XDataObject ) )
				{
					XDataObject object( json_object );
					XDataObject data_object0;
					rux::XArray< rux::XKeyValuePair< XObject > > pairs0;
					pairs0.set_ByRef( data_object0.get_KeyValuePairs() );
					pairs0.Add( rux::XKeyValuePair< XObject >( "instruction" , rux::XString( "change_central_server" ) ) );
					pairs0.Add( rux::XKeyValuePair< XObject >( "info" , object ) );
					pairs0.Add( rux::XKeyValuePair< XObject >( "extern_ip" , rux::XString( extern_ip , XEnumCodePage_UTF8 , 1 , NULL , __FILE__ , __LINE__ ) ) );
					CRITICAL_SECTION_LOCK( (*g_booter)()->_cs_commands );
					(*g_booter)()->_commands.Add( data_object0 );				
					CRITICAL_SECTION_UNLOCK( (*g_booter)()->_cs_commands );
					CRITICAL_SECTION_LOCK( (*g_booter)()->_cs_commands_thread );
					if( (*g_booter)()->_commands_thread.get_IsStarted() == 0 )
					{
						(*g_booter)()->_commands_thread.set_ThreadMode( XThreadMode_OneTime );	
						(*g_booter)()->_commands_thread.set_ThreadName( "booter commands" );
						(*g_booter)()->_commands_thread.Start();
					}
					CRITICAL_SECTION_UNLOCK( (*g_booter)()->_cs_commands_thread );
					(*g_booter)()->_commands_thread.Push( private_Commands , 0 );					
				}
			}*/
		}
	}
};
int main( int , char** )
{
	rux::XConsole console;
	console.Process( process , 0 , NULL , NULL );	
	if( _tcp_server )
	{
		rux::engine::free_object< rux::network::XTcpServer >( _tcp_server );
		_tcp_server = NULL;
	}
	if( _udp_server )
	{
		rux::engine::free_object< rux::network::XUdpServer >( _udp_server );
		_udp_server = NULL;
	}
	if( _udp_client )
	{
		rux::engine::free_object< rux::network::XUdpClient >( _udp_client );
		_udp_client = NULL;
	}
	if( _smb_client )
	{
		rux::engine::free_object< rux::network::SmbClient >( _smb_client );
		_smb_client = NULL;
	}
	if( _tcp_client )
	{
		rux::engine::free_object< rux::network::XTcpClient >( _tcp_client );
		_tcp_client = NULL;
	}
	if( _icmp_client)
	{
		rux::engine::free_object< rux::network::XIcmpClient >( _icmp_client );
		_icmp_client = NULL;
	}
	rux::deinitialize();
	return 0;
}
