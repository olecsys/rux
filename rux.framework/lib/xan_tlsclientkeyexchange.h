#ifndef XAN_TLSCLIENTKEYEXCHANGE_H
#define XAN_TLSCLIENTKEYEXCHANGE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gchandle.h>
namespace rux
{
	namespace network
	{
		begin_declare_rux_class( TlsClientKeyExchange );
			friend class XTlsRecordLayerPacket;
			friend class XTls;
			~TlsClientKeyExchange();
		begin_declare_rux_class_members( TlsClientKeyExchange );
			rux::uint8 _tls_major_version;
			rux::uint8 _tls_minor_version;	
			rux::uint8 _pre_master_key[ 48 ];
			XObject _encrypter;
			rux::uint8* _encrypted_data_ptr;
			rux::uint32 _encrypted_data_ptr_size;
			rux::uint32 _encrypted_data_ptr_length;
		end_declare_rux_class_members( TlsClientKeyExchange );
			void set_Random( const rux::uint8 random[ 46 ] );
			rux::uint32 get_PacketSize( void );
			void set_Encrypter( const XObject& encrypter );	
		end_declare_rux_class();
	};
};
#endif
