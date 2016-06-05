#ifndef XAN_TCPSERVER_H
#define XAN_TCPSERVER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_pipe.h>
#include <xan_networkaddress.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TcpServer );
			~TcpServer();
			void StartListen( const rux::network::XAddress& network_address , rux::XString& error );
			void StopListen( void );
		begin_declare_rux_class_members( TcpServer );
			XCrtSect _cs_listen_thread;
			rux::threading::XThread _listen_thread;
			XCrtSect _cs_buffer_size;
			rux::uint32 _buffer_size;
			rux_volatile _threads_count;
			rux_on_pipe_callback _on_receive_callback;
			XAddress _network_address;
			SOCKET* _server_sockets;
			size_t _server_sockets_count;
		end_declare_rux_class_members( TcpServer );	
			void StartListen( const rux::network::XAddress& network_address , rux::XString& error );
			void StopListen( void );	
			void set_OnReceived( rux_on_pipe_callback on_pipe_callback );
			static void listen_thread( void* param , size_t ___rux__thread_index1986 );	
			static void client_thread( void* param , size_t ___rux__thread_index1986 );	
		end_declare_rux_class();
	};
};
#endif
