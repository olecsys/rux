#ifndef XAN_TLSRECORDLAYERPACKET_H
#define XAN_TLSRECORDLAYERPACKET_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_tlsclienthello.h>
#include <xan_sslv2clienthello.h>
#include <xan_tlsserverhello.h>
#include <xan_tlscertificate.h>
#include <xan_tlsserverhellodone.h>
#include <xan_tlsclientkeyexchange.h>
#include <xan_tlschangecipherspec.h>
#include <xan_tlsfinished.h>
#include <xan_aes.h>
#include <xan_tlsapplicationdata.h>
namespace rux
{
	namespace network
	{
		class Tls;
		begin_declare_rux_class( TlsRecordLayerPacket );
		begin_declare_rux_class_members( TlsRecordLayerPacket );
			rux::uint8 _tls_major_version;
			rux::uint8 _tls_minor_version;	
			XObject _tls_content;	
			Tls* _tls;
		end_declare_rux_class_members( TlsRecordLayerPacket );
			XTlsRecordLayerPacket( Tls* tls );
			void set_TlsContent( const XObject& content );
			Object& get_TlsContent( void );
			static void NetworkFormat( const XObject& send_data , rux::uint8*& network_buffer_ptr , rux::uint32& network_buffer_size , rux::XString& error );
			static rux::uint32 NetworkParse( const XObject& receive_data , rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
			static rux::uint32 NetworkGetLength( rux::uint8* network_buffer_ptr , rux::uint32 network_buffer_size , rux::XString& error );
		end_declare_rux_class();
	};
};
#endif
