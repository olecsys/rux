#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_tlsfinished.h>
#include <xan_tls.h>
#include <xan_sha1.h>
#include <xan_md5.h>
#include <xan_hmac.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( TlsFinished , rux::network );
	memset( _data_ptr , 0 , 16 );
end_implement_rux_class();
namespace rux
{
	namespace network
	{
		void XTlsFinished::set_VerifyData( Tls* tls_ptr , XEnum_TlsConnectionEnd connection_end , rux::XString& error )
		{
			if( tls_ptr )
			{
				if( tls_ptr->_mac_algorithm == XEnum_TlsMACAlgorithm_sha )
				{
					rux::uint8 hash[ 36 ] = {0};
					if( tls_ptr->_handshake_messages )
					{
						if( tls_ptr->_handshake_messages_length > 0 )
						{
							(*this)()->_data_ptr[ 0 ] = XEnum_TlsHandshakeType_finished;
							rux_from_big_endian_to_little_endian( (rux::uint8*)&_verify_data_length , &(*this)()->_data_ptr[ 1 ] , 3 );
							if( tls_ptr->_major_version == 3 
								&& tls_ptr->_minor_version == 1 )
							{
								rux::cryptography::XMD5::Hash( tls_ptr->_handshake_messages , tls_ptr->_handshake_messages_length , hash );
								rux::cryptography::XSha1::Hash( tls_ptr->_handshake_messages , tls_ptr->_handshake_messages_length , &hash[ 16 ] );
								if( connection_end == XEnum_TlsConnectionEnd_client )
									tls_ptr->PRF( tls_ptr->_master_secret , 48 , "client finished" , hash , 36 , &(*this)()->_data_ptr[ 4 ] , _verify_data_length , error );					
								else if( connection_end == XEnum_TlsConnectionEnd_server )
									tls_ptr->PRF( tls_ptr->_master_secret , 48 , "server finished" , hash , 36 , &(*this)()->_data_ptr[ 4 ] , _verify_data_length , error );					
							}
						}
						else
							error = "Handshake messages length is equal 0";
					}
					else
						error = "Handshake message pointer is NULL";
				}
			}
			else
				error = "Tls pointer is NULL";	
		};
	};
};