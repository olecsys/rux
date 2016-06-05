#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsclientkeyexchange.h>
#include <xan_rsa.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsClientKeyExchange , rux::network );
	memset( _pre_master_key , 0 , 48 );
	_tls_major_version = 3;
	_tls_minor_version = 1;	
	_encrypted_data_ptr = NULL;
	_encrypted_data_ptr_size = 0;
	_encrypted_data_ptr_length = 0;
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		TlsClientKeyExchange::~TlsClientKeyExchange()
		{
			if( _encrypted_data_ptr ) 
				rux::engine::free_mem( _encrypted_data_ptr );
		};
		void XTlsClientKeyExchange::set_Encrypter( const XObject& encrypter )
		{
			(*this)()->_encrypter = encrypter;
		};
		void XTlsClientKeyExchange::set_Random( const rux::uint8 random[ 46 ] )
		{
			memcpy( &(*this)()->_pre_master_key[ 2 ] , random , 46 );
		};
		rux::uint32 XTlsClientKeyExchange::get_PacketSize( void )
		{
			rux::uint32 size = 0;
			if( rux_is_object( (*this)()->_encrypter , rux::cryptography::XRSA ) )
			{
				rux::cryptography::XRSA rsa( (*this)()->_encrypter );		
				(*this)()->_pre_master_key[ 0 ] = (*this)()->_tls_major_version;
				(*this)()->_pre_master_key[ 1 ] = (*this)()->_tls_minor_version;		
				rux::XString error;
				rsa.Encrypt( (*this)()->_pre_master_key , 48 , (*this)()->_encrypted_data_ptr , (*this)()->_encrypted_data_ptr_size , (*this)()->_encrypted_data_ptr_length , error );
				if( error.Length() == 0 )
					size = (*this)()->_encrypted_data_ptr_length + 6;
			}
			return size;
		};
	};
};