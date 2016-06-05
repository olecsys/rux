#ifndef XAN_UDPSERVER_H
#define XAN_UDPSERVER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_pipe.h>
#include <xan_networkaddress.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( UdpServer );
			~UdpServer();
			void StartListen( const rux::network::XAddress& network_address , rux::XString& error );
			void StopListen( void );	
			static void listen_thread( void* param , size_t ___rux__thread_index1986 );
		begin_declare_rux_class_members( UdpServer );
			XCrtSect _cs_listen_thread;
			rux::threading::XThread _listen_thread;
			XCrtSect _cs_buffer_size;
			rux::uint32 _buffer_size;	
			rux_on_pipe_callback _on_receive_callback;
			XAddress _network_address;
			SOCKET* _server_sockets;
			size_t _server_sockets_count;
			rux::uint8 _is_broadcast;
		end_declare_rux_class_members( UdpServer );
			void StartListen( const rux::network::XAddress& network_address , rux::XString& error );
			void StopListen( void );	
			void set_OnReceived( rux_on_pipe_callback on_pipe_callback );
			void set_IsBroadcast( rux::uint8 value );	
		end_declare_rux_class();
	};
};
#endif
