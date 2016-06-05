#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_module.h>
#include <xan_loader_defines.h>
#include <xan_debug.h>
#include <xan_keyvaluepair.h>
#include <xan_console.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Module , rux );
	_module_handle = NULL;
	_rux_module_init = NULL;
	_module_free = NULL;
	_exception_filter = NULL;
	_is_rux_module = NULL;
	_module_ref = 1;	
	_loaded_modules_index = 0xffffffff;
end_implement_rux_class();
namespace rux
{	
	Module::~Module()
	{
		if( _module_handle )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_reregister_handler )
				::rux::engine::_globals->_rux_memory_reregister_handler();
			if( _exception_filter )
				::rux::debug::UnRegisterExceptionEvent( _exception_filter );
			if( _module_free )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT( _filename.str() , "dll_free" , _module_free );
			}
	#ifdef __WINDOWS__
			FreeLibrary( _module_handle );
	#endif
	#ifdef __UNIX__
	#ifndef VALGRIND_DEBUG_INFO
			dlclose( _module_handle );
	#endif
	#endif
		}	
	}
	::rux::String& Module::get_FileNameWithoutExtension( void )
	{
		CRITICAL_SECTION_LOCK( _cs_methods );
		if( _filename_without_extension.Length() == 0 )
		{
			_filename_without_extension.set_ByRef( _filename.ReplaceAll( '\\' , '/' ) );
			::rux::int32 index0 = _filename_without_extension.LastIndexOf( '/' );
			if( index0 > -1 && static_cast< ::rux::uint32>( index0 + 1 ) < _filename_without_extension.Length() )	
				_filename_without_extension.set_ByRef( _filename_without_extension.Substring( index0 + 1 ) );
			index0 = _filename_without_extension.LastIndexOf( '.' );
			if( index0 > -1 )
				_filename_without_extension.set_ByRef( _filename_without_extension.Substring( 0 , index0 ) );
		}
		CRITICAL_SECTION_UNLOCK( _cs_methods );
		return _filename_without_extension++;
	};
	void Module::private_Release( ::rux::int32 loadable_module_index ) const
	{	
		Module* _this = const_cast< Module* >( this );
		if( 	XInterlocked::CompareExchange( &_this->_module_ref , 0 , 0 ) != 0 )
			XInterlocked::Decrement( &_this->_module_ref );
		CRITICAL_SECTION_LOCK( _this->_cs_methods );
		for( ::rux::uint32 index0 = 0 ; index0 < _this->_loadable_module_indexes.Count() ; index0++ )
		{
			if( _this->_loadable_module_indexes[ index0 ] == loadable_module_index )
			{
				_this->_loadable_module_indexes.RemoveAt( index0 );
				::rux::engine::_globals->_loader_globals->_rux_loadable_decrement( loadable_module_index );
				break;
			}
		}
		CRITICAL_SECTION_UNLOCK( _this->_cs_methods );
	};
	bool Module::private_IsActive( void ) const
	{
	#ifdef __SOLARIS__
		return XInterlocked::CompareExchange( const_cast<volatile ::rux::uint32*>( &_module_ref ) , 0 , 0 ) != 0;
	#else
		return XInterlocked::CompareExchange( const_cast<volatile long*>( &_module_ref ) , 0 , 0 ) != 0;
	#endif
	};
	void Module::private_Free( void ) const
	{
		if( _module_handle )
		{
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_reregister_handler )
				::rux::engine::_globals->_rux_memory_reregister_handler();
			if( _exception_filter )
				::rux::debug::UnRegisterExceptionEvent( _exception_filter );
			if( _module_free )
			{
				CODE_LABELS_INITIALIZE();
				CODE_LABEL_ANOTHER_CONTEXT( _filename.str() , "dll_free" , _module_free );
			}
	#ifdef __WINDOWS__
			FreeLibrary( _module_handle );
	#endif
	#ifdef __UNIX__
	#ifndef VALGRIND_DEBUG_INFO
			dlclose( _module_handle );
	#endif
	#endif
			Module* _this = const_cast<Module*>( this );
			_this->_module_handle = NULL;
			CRITICAL_SECTION_LOCK( _this->_cs_methods );
			_this->_methods.Clear();
			CRITICAL_SECTION_UNLOCK( _this->_cs_methods );
		}
	};
	void* Module::get_MethodAddress( const char* method_name_ptr ) const
	{
		void* address = NULL;	
		if( _module_handle )
		{
			::rux::uint32 index = 0;
			Module* _this = const_cast<Module*>( this );
			CRITICAL_SECTION_LOCK( _this->_cs_methods );
			for( index = 0 ; index < _methods.Count() ; index++ )
			{
				if( _methods[ index ]._method_name == method_name_ptr )
				{
					address = _methods[ index ]._address;
					break;
				}
			}
			if( address == NULL )
			{			
	#ifdef __WINDOWS__		
				if( method_name_ptr )
					address = GetProcAddress( _module_handle , method_name_ptr );
	#endif
	#ifdef __UNIX__
				if( method_name_ptr )
					address = dlsym( _module_handle , method_name_ptr );
	#endif
				if( address )
					::rux::engine::_globals->_loader_globals->_rux_add_module_method_info( this , method_name_ptr , address );
				else
				{
					::rux::XString error( g_current_module_name );
					error += " module,function '";
					error += method_name_ptr;
					error += "' was not found in module '" + _filename + "'\n";
					::rux::XConsole::Print( stderr , XRed , error );
				}
			}
			CRITICAL_SECTION_UNLOCK( _this->_cs_methods );
		}
		else
		{
			::rux::XString error( g_current_module_name );
			error += " module,module '" + _filename + "' was not loaded\n";
			::rux::XConsole::Print( stderr , XRed , error );
		}
		return address;
	};
	::rux::String& Module::get_FileName( void ) const
	{
		return _filename++;
	};
	::rux::String& XModule::get_FileName( void ) const
	{
		return (*this)()->get_FileName();
	};
	::rux::String& XModule::get_FileNameWithoutExtension( void ) const
	{
		return (*this)()->get_FileNameWithoutExtension();
	};
	::rux::String& XModule::get_Directory( void )
	{
		::rux::XString local_file_name( (*this)()->_filename.ReplaceAll( '\\' , '/' ) );
		::rux::int32 index0 = local_file_name.LastIndexOf( '/' );
		if( index0 > -1 )
			local_file_name.set_ByRef( local_file_name.Substring( 0 , index0 ) );
		return local_file_name++;
	};
	Module& XModule::get_Executable( ::rux::XString& error )
	{
		error.Clear();
		::rux::XModule module;
	#ifdef __WINDOWS__
		HMODULE module_handle = GetModuleHandleA( NULL );
		if( module_handle )
		{
			wchar_t module_path[ 4096 ] = L"";
			if( GetModuleFileNameW( module_handle , module_path , 4096 ) != 0 )
			{
				module()->_filename = module_path;
				module()->_filename = module()->_filename.ReplaceAll( '\\' , '/' );			
			}
			else
				error = "Error : cannot get executable module file name";
		}
		else
			error = "Error : cannot get executable module handle";
	#endif
	#ifdef __UNIX__
		declare_stack_variable( char , module_path , 4096 );
	#ifdef __SOLARIS__
		declare_stack_variable( char , key_ptr , 64 );
		::rux::safe_sprintf( key_ptr , 64 , "/proc/%ld/path/a.out" , getpid() );
		if( readlink( key_ptr , module_path , 4095 ) > 0 )
	#elif defined( __LINUX__ )
		if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
	#endif
		{
			module()->_filename = module_path;
		}
		else
			error = "Error : cannot get executable module file name";
	#endif
		return module++;
	};
	Module& XModule::get_Current( ::rux::XString& error )
	{
		error.Clear();
		::rux::XModule module;
	#ifdef __WINDOWS__
		HMODULE module_handle = GetModuleHandleA( g_current_module_name );
		if( module_handle )
		{
			wchar_t module_path[ 4096 ] = L"";
			if( GetModuleFileNameW( module_handle , module_path , 4096 ) != 0 )
			{
				module()->_filename = module_path;
				module()->_filename = module()->_filename.ReplaceAll( '\\' , '/' );			
			}
			else
			{
				error = "Error : cannot get module '";
				error += g_current_module_name;
				error += "' file name";
			}
		}
		else
		{
			error = "Error : cannot get module '";
			error += g_current_module_name;
			error += "' 	handle";
		}
	#endif
	#ifdef __UNIX__
		Dl_info info;
		if( dladdr( (void*)g_current_module_name , &info ) != 0 )
		{        
			module()->_filename = info.dli_fname;
		}	
		else
		{		
			declare_stack_variable( char , module_path , 4096 );
	#ifdef __SOLARIS__
			declare_stack_variable( char , key_ptr , 64 );
			::rux::safe_sprintf( key_ptr , 64 , "/proc/%ld/path/a.out" , getpid() );
			if( readlink( key_ptr , module_path , 4095 ) > 0 )
	#elif defined( __LINUX__ )
			if( readlink( "/proc/self/exe" , module_path , 4095 ) > 0 )
	#endif
			{
				module()->_filename = module_path;
			}
			else
				error = "Error : cannot get executable module file name";
		}	
	#endif
		return module++;
	};
	void* XModule::get_MethodAddress( const ::rux::XString& method_name , ::rux::XString& error )
	{	
		void* address = NULL;
		error.Clear();	
		if( (*this)()->_module_handle )
		{
			::rux::uint32 index = 0;
			CRITICAL_SECTION_LOCK( (*this)()->_cs_methods );
			for( index = 0 ; index < (*this)()->_methods.Count() ; index++ )
			{
				if( (*this)()->_methods[ index ]._method_name == method_name )
				{
					address = (*this)()->_methods[ index ]._address;
					break;
				}
			}
			if( address == NULL )
			{			
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
				::rux::XString utf8;
				utf8.set_ByRef( method_name.ConvertToUTF8() );
				address = dlsym( (*this)()->_module_handle , utf8.str() );
	#endif
				if( address )
					::rux::engine::_globals->_loader_globals->_rux_add_module_method_info( (*this)() , method_name , address );
				else
					error = "Error : function '" + method_name + "' was not found in module '" + (*this)()->_filename + "'";
			}
			CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
		}
		else
			error = "Error : module '" + (*this)()->_filename + "' was not loaded";
		return address;
	};
	::rux::uint8 XModule::IsExists( const ::rux::XString& method_name , ::rux::XString& error )
	{
		::rux::uint8 is_exists = 0;
		error.Clear();	
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
				address = dlsym( (*this)()->_module_handle , method_name.ConvertToUTF8().str() );
	#endif
				if( address )
				{
					::rux::engine::_globals->_loader_globals->_rux_add_module_method_info( (*this)() , method_name , address );
					is_exists = 1;
				}
				else
					error = "Error : function '" + method_name + "' was not found in module '" + (*this)()->_filename + "'";
			}		
			else 
				is_exists = 1;
			CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
		}
		else
			error = "Error : module '" + (*this)()->_filename + "' was not loaded";
		return is_exists;
	};
	void XModule::UnLoad( void )
	{
		::rux::loader::UnLoadModule( *this );
	};
	::rux::uint8 XModule::IsLoaded( void )
	{
		return (*this)()->_module_handle ? 1 : 0;
	};
	void XModule::private_AddRef( ::rux::int32 loadable_module_index ) const
	{	
		if( 	XInterlocked::CompareExchange( &(*this)()->_module_ref , 0 , 0 ) != 0 )
		{
			XInterlocked::Increment( &(*this)()->_module_ref );	
			CRITICAL_SECTION_LOCK( (*this)()->_cs_methods );
			(*this)()->_loadable_module_indexes.Add( loadable_module_index );
			::rux::engine::_globals->_loader_globals->_rux_loadable_increment( loadable_module_index );
			CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
		}
	};
	void XModule::private_Release( ::rux::int32 loadable_module_index ) const
	{		
		(*this)()->private_Release( loadable_module_index );
	};
	bool XModule::private_IsActive( void ) const
	{
		return (*this)()->private_IsActive();
	};
	void XModule::private_Free( void ) const
	{
		if( (*this)()->_module_handle )
		{
			CODE_LABELS_INITIALIZE();
			if( ::rux::engine::_globals && ::rux::engine::_globals->_rux_memory_reregister_handler )
				::rux::engine::_globals->_rux_memory_reregister_handler();
			if( (*this)()->_exception_filter )
				::rux::debug::UnRegisterExceptionEvent( (*this)()->_exception_filter );
			if( (*this)()->_module_free )
				CODE_LABEL_ANOTHER_CONTEXT( (*this)()->_filename.str() , "dll_free" , (*this)()->_module_free );
	#ifndef VALGRIND_DEBUG_INFO
			dlclose( (*this)()->_module_handle );
	#endif
			(*this)()->_module_handle = NULL;
			CRITICAL_SECTION_LOCK( (*this)()->_cs_methods );
			(*this)()->_methods.Clear();
			CRITICAL_SECTION_UNLOCK( (*this)()->_cs_methods );
		}
	};
	void XModule::AddModuleMethodInfo( const Module*  module_ptr , const ::rux::XString& method_name , void* address_ptr )
	{
		::rux::engine::_globals->_loader_globals->_rux_add_module_method_info( module_ptr , method_name , address_ptr );
	};
};
