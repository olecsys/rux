#ifndef XAN_RSAPUBLICKEY_H
#define XAN_RSAPUBLICKEY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rawdata.h>
namespace rux
{	
	namespace cryptography
	{			
		class XRSA;
		begin_declare_rux_class( RsaPublicKey );
			friend class XRSA;
		begin_declare_rux_class_members( RsaPublicKey );
			rux::XRawData _modulus;
			rux::XRawData _public_exponent;
		end_declare_rux_class_members( RsaPublicKey );
			void set_Modulus( const rux::XRawData& modulus );
			void set_PublicExponent( const rux::XRawData& public_exponent );
			void set_Modulus( const rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length );
			void set_PublicExponent( const rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length );
			void set_ModulusFromReverseOrder( const rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length );
			void set_PublicExponentFromReverseOrder( const rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length );
			void set_Modulus( rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length );
			void set_PublicExponent( rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length );
		end_declare_rux_class();
	};
};
#endif
