#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_rsapublickey.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( RsaPublicKey , rux::cryptography );
end_implement_rux_class();
namespace rux
{
	namespace cryptography
	{
		void XRsaPublicKey::set_Modulus( const rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length )
		{
			(*this)()->_modulus = rux::XRawData( modulus_ptr , modulus_ptr_length );
			(*this)()->_modulus.ResetForceSize();
		};
		void XRsaPublicKey::set_PublicExponent( const rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length )
		{
			(*this)()->_public_exponent = rux::XRawData( exponent_ptr , exponent_ptr_length );
		};
		void XRsaPublicKey::set_ModulusFromReverseOrder( const rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length )
		{
			(*this)()->_modulus = rux::XRawData::FromReverseOrder( modulus_ptr , modulus_ptr_length );
			(*this)()->_modulus.ResetForceSize();
		};
		void XRsaPublicKey::set_PublicExponentFromReverseOrder( const rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length )
		{
			(*this)()->_public_exponent = rux::XRawData::FromReverseOrder( exponent_ptr , exponent_ptr_length );
		};
		void XRsaPublicKey::set_Modulus( const rux::XRawData& modulus )
		{
			(*this)()->_modulus.Copy( modulus );
			(*this)()->_modulus.ResetForceSize();
		};
		void XRsaPublicKey::set_PublicExponent( const rux::XRawData& public_exponent )
		{
			(*this)()->_public_exponent.Copy( public_exponent );
		};
		void XRsaPublicKey::set_Modulus( rux::uint8* modulus_ptr , rux::uint32 modulus_ptr_length )
		{
			(*this)()->_modulus.set_RawData( modulus_ptr , modulus_ptr_length );
			(*this)()->_modulus.ResetForceSize();
		};
		void XRsaPublicKey::set_PublicExponent( rux::uint8* exponent_ptr , rux::uint32 exponent_ptr_length )
		{
			(*this)()->_public_exponent.set_RawData( exponent_ptr , exponent_ptr_length );
		};
	};
};