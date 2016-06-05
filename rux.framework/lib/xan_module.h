#ifndef XAN_MODULE_H
#define XAN_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_methodinfo.h>
#include <xan_crtsect.h>
class ModuleManager;
namespace rux
{	
	begin_declare_rux_class( Module );
		friend class ::ModuleManager;
		::rux::XString _filename;	
		XArray< ::rux::XMethodInfo > _methods;
		XCrtSect _cs_methods;
#ifdef __WINDOWS__
		HMODULE _module_handle;
#else
		void* _module_handle;
#endif
		rux_volatile _module_ref;
		rux_module_init_t _rux_module_init;
		rux_module_free_t _module_free;
		::rux::rux_exception_event_t _exception_filter;
		is_rux_module_t _is_rux_module;		
		XMallocArray< ::rux::int32 , 5 > _loadable_module_indexes;
public:
		::rux::XString _filename_without_extension;
		::rux::uint32 _loaded_modules_index;	
		~Module();
		void* get_MethodAddress( const char* method_name_ptr ) const;
private:
		void private_Release( ::rux::int32 loadable_module_index ) const;
		bool private_IsActive( void ) const;
		void private_Free( void ) const;
public:
		::rux::String& get_FileName( void ) const;
		::rux::String& get_FileNameWithoutExtension( void );
		template< typename F , typename P0 >
		::rux::uint8 Invoke( const char* method_name , P0 param0 )
		{
			::rux::uint8 result = 0;
			void* address = get_MethodAddress( method_name );
			if( address )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER(	_filename.str() , method_name , ( (F)address ) , param0 );
				result = 1;
			}
			return result;
		};
		template< typename F , typename P0 , typename R >
		R InvokeResult( const char* method_name , P0 param0 )
		{
			R result = 0;
			void* address = get_MethodAddress( method_name );
			if( address )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( _filename.str() , method_name , ( (F)address ) , param0 , result );
			}
			return result;
		};
		template< typename F , typename P0 , typename P1 >
		::rux::uint8 Invoke( const char* method_name , P0 param0 , P1 param1 )
		{
			::rux::uint8 result = 0;
			void* address = get_MethodAddress( method_name );
			if( address )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( _filename.str() , method_name , ( (F)address ) , param0 , param1 );
				result = 1;
			}
			return result;
		};
		template< typename F , typename P0 , typename P1 , typename P2 >
		::rux::uint8 Invoke( const char* method_name , P0 param0 , P1 param1 , P2 param2 )
		{
			::rux::uint8 result = 0;
			void* address = get_MethodAddress( method_name );
			if( address )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_3_PARAMETERS( _filename.str() , method_name , ( (F)address ) , param0 , param1 , param2 );
				result = 1;
			}
			return result;
		};
	begin_declare_rux_class_members( Module );
	end_declare_rux_class_members( Module );
		friend class ::ModuleManager;
		::rux::String& get_FileName( void ) const;	
		::rux::String& get_FileNameWithoutExtension( void ) const;	
		::rux::String& get_Directory( void );	
		void UnLoad( void );	
		::rux::uint8 IsLoaded( void );
		::rux::uint8 IsExists( const ::rux::XString& method_name , ::rux::XString& error );
		void* get_MethodAddress( const ::rux::XString& method_name , ::rux::XString& error );
		static Module& get_Current( ::rux::XString& error );
		static Module& get_Executable( ::rux::XString& error );
	private:
		void private_AddRef( ::rux::int32 loadable_module_index ) const;
		void private_Release( ::rux::int32 loadable_module_index ) const;
		bool private_IsActive( void ) const;
		void private_Free( void ) const;
	public:
		template< typename F , typename P0 >
		::rux::uint8 Invoke( const ::rux::XString& method_name , P0 param0 )
		{
			::rux::uint8 result = 0;
			::rux::XString error;
			void* address = get_MethodAddress( method_name , error );
			if( error.Length() == 0 )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER(	(*this)()->_filename.str() , method_name , ( (F)address ) , param0 );
				result = 1;
			}
			return result;
		};
		template< typename F , typename P0 , typename R >
		R InvokeResult( const ::rux::XString& method_name , P0 param0 )
		{
			R result = 0;
			::rux::XString error;
			void* address = get_MethodAddress( method_name , error );
			if( error.Length() == 0 )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_PARAMETER_RESULT( (*this)()->_filename.str() , method_name , ( (F)address ) , param0 , result );
			}
			return result;
		};
		template< typename F , typename P0 , typename P1 >
		::rux::uint8 Invoke( const ::rux::XString& method_name , P0 param0 , P1 param1 )
		{
			::rux::uint8 result = 0;
			::rux::XString error;
			void* address = get_MethodAddress( method_name , error );
			if( error.Length() == 0 )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_2_PARAMETERS( (*this)()->_filename.str() , method_name , ( (F)address ) , param0 , param1 );
				result = 1;
			}
			return result;
		};
		template< typename F , typename P0 , typename P1 , typename P2 >
		::rux::uint8 Invoke( const ::rux::XString& method_name , P0 param0 , P1 param1 , P2 param2 )
		{
			::rux::uint8 result = 0;
			::rux::XString error;
			void* address = get_MethodAddress( method_name , error );
			if( error.Length() == 0 )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT_3_PARAMETERS( (*this)()->_filename.str() , method_name , 	( (F)address ) , param0 , param1 , param2 );
				result = 1;
			}
			return result;
		};
		static void AddModuleMethodInfo( const Module*  module_ptr , const ::rux::XString& method_name , void* address_ptr );
		template< typename T >
	#ifdef __x86__
		T RuntimeInvoke( const ::rux::XString& method_name , rux_runtime_param_t* params_ptr , ::rux::uint32 params_count , ::rux::XString& error )
	#else
		T RuntimeInvoke( const ::rux::XString& method_name , rux_runtime_param_t* params_ptr , ::rux::uint64 params_count , ::rux::XString& error )
	#endif
		{
			error.Clear();
			T value;
			if( (*this)()->_module_handle )
			{
				::rux::uint32 index = 0;
				CRITICAL_SECTION_LOCK( (*this)()->_cs_methods );
				for( index = 0 ; index < (*this)()->_methods.Count() ; index++ )
				{
					if( (*this)()->_methods[ index ]._method_name == method_name )
						break;
				}
				if( index >= (*this)()->_methods.Count() )
				{
					void* address = NULL;
	#ifdef __WINDOWS__		
					if( method_name()->_code_page == XEnumCodePage_UTF16 )
					{
						::rux::XString ansi = method_name.ConvertToAnsi();
						address = GetProcAddress( (*this)()->_module_handle , ansi.str() );
					}
					else
						address = GetProcAddress( (*this)()->_module_handle , method_name.str() );
	#endif
	#ifdef __UNIX__
					address = dlsym( (*this)()->_module_handle , method_name.str() );
	#endif
					if( address )
						AddModuleMethodInfo( (*this)() , method_name , address );
					else
						error = "Error : function '" + method_name + "' was not found in module '" + (*this)()->_filename + "'";
				}
				XMethodInfo method_info;
				if( index < (*this)()->_methods.Count() )
				{
					method_info = (*this)()->_methods[ index ];
					CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
					value = method_info.RuntimeInvoke< T >( params_ptr , params_count );			
				}
				else
					CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
			}
			else
				error = "Error : module '" + (*this)()->_filename + "' was not loaded";
			return value;
		};
	end_declare_rux_class();
};
#endif
