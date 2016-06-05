#ifndef XAN_VSRCRESOURCES_H
#define XAN_VSRCRESOURCES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_int64.h>
#include <xan_keyvaluepair.h>
#define _rux_installer_zip 19859
namespace rux
{
	namespace resources
	{
		class XVSRCResources
		{
		public:
			static uint8 ExtractInFile( char* file_name_ptr , int32 rc_id , rux::alloc_t alloc , char** error_ptr_ptr );
			static uint8 ExtractInFile( char* file_name_ptr , const XInt64& rc_id , rux::alloc_t alloc , char** error_ptr_ptr );
			static rux::Array< XInt64 >& GetResources( void );
			static rux::Array< XInt64 >& GetStringResources( void );
			static void InstallFromResources( const XString& install_path , XString& error );
			static void UnInstallFromResources( const XString& install_path , XArray< XString >& exclude_files , XString& error );
		#ifdef __WINDOWS__
			static BOOL CALLBACK EnumResName( HMODULE module , const char* type , char* name , LONG_PTR param );
		#endif
		};
	};
};
#endif