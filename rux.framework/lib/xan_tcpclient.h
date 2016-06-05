#ifndef XAN_TCPCLIENT_H
#define XAN_TCPCLIENT_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_pipe.h>
#include <xan_networkaddress.h>
#include <xan_data.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TcpClient );
			~TcpClient();
			void set_NetworkAddress( const rux::network::XAddress& network_address );
			void Close( void );
			void Open( rux::XString& error );
			void Send( char* send_data , size_t send_data_size , rux::XString& error );
			void Receive( XData** recv_data , rux::XString& error );
			rux::network::Address& get_NetworkAddress( void );
		begin_declare_rux_class_members( TcpClient );
			XAddress _network_address;
			SOCKET _client_socket;
			XData _recv_data;
		end_declare_rux_class_members( TcpClient );	
			void set_NetworkAddress( const rux::network::XAddress& network_address );
			void Close( void );
			void Open( rux::XString& error );			
			void Send( char* send_data , size_t send_data_size , rux::XString& error );
			void Receive( XData** recv_data , rux::XString& error );
			rux::network::Address& get_NetworkAddress( void );
		end_declare_rux_class();
	};
};
#endif
