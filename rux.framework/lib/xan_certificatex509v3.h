#ifndef XAN_CERTIFICATEX509V3_H
#define XAN_CERTIFICATEX509V3_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rawdata.h>
namespace rux
{	
	namespace cryptography
	{			
		begin_declare_rux_class( CertificateX509v3 );
			Object& get_EncryptionPublicKey( void );
		begin_declare_rux_class_members( CertificateX509v3 );
			char _version;
			rux::XRawData _serial_number;
			rux::XRawData _signature;
			rux::uint8 _tag0_length;
			rux::XString _ca_signature_algorithm;
			rux::XString _signature_algorithm;
			rux::XString _encryption_algorithm;
			rux::XArray<rux::XString> _issuer;
			rux::XString _not_before;
			rux::XString _not_after;
			rux::XArray<rux::XString> _subject;
			XObject _encryption_public_key;
		end_declare_rux_class_members( CertificateX509v3 );
			static CertificateX509v3& Parse( rux::uint8* data_ptr , rux::uint32 data_ptr_length , rux::XString& error );
			Object& get_EncryptionPublicKey( void );
		end_declare_rux_class();
	};
};
#endif
