#ifndef XAN_RAWDATA_H
#define XAN_RAWDATA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_string.h"
#include "xan_io_defines.h"
namespace rux
{
	class RawData;
	class XRawData : public XGCHandle<RawData>
	{
		friend class RawData;
		declare_rux_base_methods( RawData );
	private:
	public:
		declare_rux_set_operators( RawData );
		XRawData( void );
		XRawData( const RawData& raw_data );
		XRawData( const XRawData& raw_data );
		explicit XRawData( const XObject& object );
		XRawData( const ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size );
		static RawData& FromReverseOrder( const ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size );
		bool operator == ( const XRawData& raw_data );
		bool operator == ( ::rux::uint32 number );
		bool operator != ( ::rux::uint32 number );
		bool operator != ( XRawData& raw_data );
		bool operator > ( XRawData& raw_data );
		bool operator > ( ::rux::uint32 number ) const;
		bool operator < ( XRawData& raw_data );
		bool operator < ( ::rux::uint32 number ) const;
		bool operator >= ( XRawData& raw_data );
		bool operator >= ( RawData& raw_data );
		bool operator >= ( ::rux::uint32 number );
		bool operator <= ( XRawData& raw_data );
		bool operator <= ( ::rux::uint32 number );
		void operator = ( const ::rux::uint32 number );
		void operator = ( ::rux::XString& number );
		RawData& operator + ( XRawData& raw_data );
		RawData& operator + ( ::rux::uint32 number );
		RawData& operator - ( XRawData& raw_data );
		RawData& operator - ( ::rux::uint32 number ) const;	
		XRawData& operator << ( const ::rux::int32 shift );
		XRawData& operator >> ( const ::rux::int32 shift );
		RawData& operator * ( XRawData& raw_data );
		RawData& operator * ( const XRawData& raw_data ) const;
		RawData& operator * ( ::rux::uint32 number );	
		RawData& operator % ( XRawData& raw_data );	
		RawData& operator % ( ::rux::uint32 number );	
		RawData& operator / ( XRawData& raw_data );
		RawData& operator / ( ::rux::uint32 number );
		RawData& operator ^ ( XRawData& raw_data );
		XRawData& operator -= (XRawData& raw_data );
		XRawData& operator -= (RawData& raw_data );
		XRawData& operator -= ( ::rux::uint32 number );
		XRawData& operator += ( XRawData& raw_data );
		XRawData& operator += ( const XRawData& raw_data );
		bool get_IsZero( void );
		RawData& Sqrt( void );
		::rux::uint32 get_BitsCount( void ) const;
		RawData& Sqr( XRawData& raw_data );		
		RawData& ModularInverse( const XRawData& raw_data ) const;	
		bool get_IsPrimeNumber( void ) const;
		bool get_IsOddNumber( void );
		void set_OddNumber( void );
		void set_EvenNumber( void );
		::rux::uint8 get_ByteAt( ::rux::uint32 index );
		::rux::uint32 get_AlignedSize( void ) const;
		::rux::uint32 get_Size( void );
		::rux::uint32 get_SizeInUlong32Count( void ) const;	
		::rux::String& I2OSP_ToString( ::rux::uint32 octet_string_length , ::rux::XString& error );
		RawData& I2OSP( ::rux::uint32 octet_string_length , ::rux::XString& error );
		static String& ToHexString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_length );
		static RawData& FromOctetString( const ::rux::XString& octet_string , ::rux::XString& error );
		static RawData& FromReverseOctetString( const ::rux::XString& octet_string , ::rux::XString& error );
		static RawData& OS2IP( const ::rux::XString& i2osp , ::rux::XString& error );
		static RawData& OS2IP( const ::rux::uint8* i2osp_ptr , ::rux::uint32 i2osp_ptr_length , ::rux::XString& error );
		static RawData& OS2IP( rux::io::file& i2osp_file , ::rux::XString& error );
		static String& ToString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_size );
		static void FromString( const ::rux::XString& data , ::rux::uint8*& data_ptr , ::rux::uint32& data_ptr_size );
		static void FromReverseHexString( const ::rux::XString& data , ::rux::uint8*& data_ptr , ::rux::uint32& data_ptr_size );
		::rux::String& ToString( void );
		static String& ToReverseHexString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_size );	
		static String& ToOctetString( ::rux::uint8* data_ptr , size_t data_ptr_length );
		::rux::String& ToOctetString( void );
		static String& ToReverseOctetString( ::rux::uint8* data_ptr , ::rux::uint32 data_ptr_length );
		::rux::String& ToReverseOctetString( void );
		void set_RawData( ::rux::uint8* raw_data_ptr , ::rux::uint32 raw_data_ptr_size );
		static ::rux::Array<XString>& SplitOctetStringByLength( const ::rux::XString& octet_string , ::rux::uint32 octet_string_length );
		friend RawData& operator % ( RawData& raw_data1 , XRawData& raw_data2 );	
		friend RawData& operator % ( XRawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator % ( RawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator * ( RawData& raw_data1 , XRawData& raw_data2 );	
		friend RawData& operator * ( XRawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator - ( XRawData& raw_data1 , RawData& raw_data2 );
		friend RawData& operator + ( XRawData& raw_data1 , RawData& raw_data2 );	
		static RawData& GCD( const XRawData& raw_data1 , const XRawData& raw_data2 );
		friend RawData& operator * ( RawData& raw_data1 , RawData& raw_data2 );	
		friend bool operator > ( XRawData& raw_data1 , RawData& raw_data2 );
		friend bool operator == ( RawData& raw_data1 , ::rux::uint32 number );
		void Copy( const XRawData& raw_data );
		void Copy( RawData& raw_data );
		::rux::uint8* get_RawData( void );	
		size_t get_AllocatedSize( void );
		void ResetForceSize( void );
		void FastRightShift( void );
		void FastLeftShift( void );	
	private:
		::rux::uint32 private_Divide( ::rux::uint32 number );
		::rux::uint32 private_VirtualDivide( ::rux::uint32 number );	
		::rux::uint8 private_IsGreater( XRawData& raw_data );
		::rux::uint8 private_IsGreater( const XRawData& raw_data );
		::rux::uint8 private_IsGreater( RawData& raw_data );
		::rux::uint8 private_IsLess( ::rux::uint32 number );
		static void minus( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len );
		static void plus( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len );	
		static ::rux::int32 compare( ::rux::uint8* buffer_ptr0 , ::rux::uint8* buffer_ptr1 , ::rux::uint32 len );		
	};
	class RawData : public XGCRef
	{
		friend class XRawData;
		friend class XRSA;
		declare_rux_base_internal_methods();
	private:		
		::rux::uint32 _max_data_length;
		::rux::uint8* _raw_data_ptr;
		::rux::uint32 _raw_data_ptr_size;	
		::rux::int32 _is_carry;
		::rux::uint8 _is_negative;
		::rux::uint32 _bits_count;
		size_t _force_size;
	public:
		friend RawData& operator * ( RawData& raw_data1 , XRawData& raw_data2 );	
		friend RawData& operator * ( XRawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator - ( XRawData& raw_data1 , RawData& raw_data2 );
		friend RawData& operator % ( RawData& raw_data1 , XRawData& raw_data2 );	
		friend RawData& operator % ( XRawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator % ( RawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator * ( RawData& raw_data1 , RawData& raw_data2 );	
		friend RawData& operator + ( XRawData& raw_data1 , RawData& raw_data2 );
		friend bool operator > ( XRawData& raw_data1 , RawData& raw_data2 );
		friend bool operator == ( RawData& raw_data1 , ::rux::uint32 number );	
		RawData& operator / ( RawData& raw_data );
		explicit RawData( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		~RawData();
		::rux::uint32 get_AlignedSize( void );
		::rux::uint32 get_BitsCount( void );
		::rux::uint32 get_SizeInUlong32Count( void );	
		::rux::uint8 getbit( size_t bit_index );	
		void ResetForceSize( void );
		void modExp( RawData& e, RawData& m );	
		RawData& operator / ( ::rux::uint32 number );
		RawData& operator % ( ::rux::uint32 number );
		void modular( RawData& number , RawData& modular );
		void modular( RawData& modular );
		void modular( RawData& raw_data , ::rux::uint32 number );
		void modmult( RawData& number , RawData& m , RawData& temp_a , RawData& temp_b );
		void divide( RawData& number , RawData& divider );	
		void multiplication( RawData& number , RawData& divider );
		bool operator >= ( RawData& raw_data );	
		void FastLeftShift( void );
		void FastRightShift( void );
		bool get_IsZero( void );
		RawData& operator -= ( RawData& raw_data );
		RawData& operator += ( ::rux::uint32 number );
		::rux::uint8 private_IsGreater( RawData& raw_data );
		void increase_memory( size_t size );
		void Copy( RawData& raw_data );
		bool operator >= ( ::rux::uint32 number );
		RawData& operator -= ( ::rux::uint32 number );
		bool operator < ( ::rux::uint32 number );
		::rux::String& ToString( void );
		bool operator > ( ::rux::uint32 number );
		void operator = ( const ::rux::uint32 number );
		::rux::uint8 private_IsLess( ::rux::uint32 number );
		RawData& operator += (XRawData& raw_data );
		RawData& operator %= (RawData& raw_data );
		RawData& operator += (RawData& raw_data );
		static void test();
	};
};
#endif
