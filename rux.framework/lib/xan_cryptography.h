#ifndef XAN_CRYPTOGRAPHY_H
#define XAN_CRYPTOGRAPHY_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_array.h>
#include <xan_loader_defines.h>
#include <openssl/des.h>
#include <openssl/md4.h>
namespace rux
{	
	namespace cryptography
	{			
		class DES;
		class MD4;
		begin_declare_rux_class( Cryptography );
			friend class DES;
			friend class MD4;
			friend class XDES;
			~Cryptography();
		begin_declare_rux_class_members( Cryptography );
			rux::XModule _openssl_module;	
			DES_set_key_t _DES_set_key;
			ERR_clear_error_t _ERR_clear_error;
			DES_ecb_encrypt_t _DES_ecb_encrypt;
			MD4_t _MD4;
			DES_set_odd_parity_t _DES_set_odd_parity;
		end_declare_rux_class_members( Cryptography );
			static rux::uint64 Hash( unsigned char* data_ptr, rux::int32 data_size );
	private:
			static bool GostHashStep( rux::uint64& data );
			static bool GostBase( rux::uint64& data, rux::uint8 index );
		end_declare_rux_class();

		void initialize( void );
		void deinitialize( void );
	};
};
#endif
