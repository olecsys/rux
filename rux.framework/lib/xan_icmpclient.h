#ifndef XAN_ICMPCLIENT_H
#define XAN_ICMPCLIENT_H
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
		begin_declare_rux_class( IcmpClient );
			~IcmpClient();
			void set_NetworkAddress( const rux::network::XAddress& network_address );
			void Close( void );
			void Open( rux::XString& error );
			void Receive( XData** recv_data , rux::XString& error );
			void Send( char* send_data , size_t send_data_size , rux::XString& error );
			rux::uint16 checksum( rux::uint16* buffer , rux::int32 size );
			void Echo( size_t datasize , size_t packet_count , size_t& received_packets , rux::uint32& max_time , rux::uint32& min_time , rux::uint32& average_time , rux::XString& error );
		begin_declare_rux_class_members( IcmpClient );
			XAddress _network_address;
			SOCKET _client_socket;
			rux::uint8 _is_broadcast;
			XData _recv_data;
		end_declare_rux_class_members( IcmpClient );			
			void set_NetworkAddress( const rux::network::XAddress& network_address );
			void Close( void );
			void Open( rux::XString& error );
			void set_IsBroadcast( rux::uint8 value );	
			void Send( char* send_data , size_t send_data_size , rux::XString& error );
			void Receive( XData** recv_data , rux::XString& error );
			void Echo( size_t datasize , size_t packet_count , size_t& received_packets , rux::uint32& max_time , rux::uint32& min_time , rux::uint32& average_time , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
