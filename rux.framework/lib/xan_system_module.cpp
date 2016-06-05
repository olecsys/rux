#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
#include <xan_defines.h>
#include <xan_io_defines.h>
namespace rux
{
	namespace system
	{
		module::module( const char* library , const char* alter_library , ::rux::byte begin_from_exe_dir )
		{
			_begin_from_exe_dir = begin_from_exe_dir;
			_need_lock = 1;
			_loaded = 0;
			::rux::safe_strncpy( _library , library , 128 );
			if( alter_library )
				::rux::safe_strncpy( _alter_library , alter_library , 128 );
			else
				_alter_library[ 0 ] = 0;
			_handle = 0;
		};
		module::~module( void )
		{
			_need_lock = 0;
			unload();
		};
		void module::load( void )
		{
			if( XInterlocked::CompareExchange( &_loaded , 2 , 0 ) != 1 )
			{					
				CRITICAL_SECTION_LOCK( _cs );
				if( _handle == 0 )
				{
					if( XInterlocked::CompareExchange( &_loaded , 0 , 0 ) == 2 )
					{
						if( _begin_from_exe_dir == 0 )
							_handle = dlopen( _library , RTLD_LAZY );
						if( _handle == 0 )
						{
							if( _begin_from_exe_dir == 0 )
							{
								if( _alter_library[ 0 ] != 0 )
									_handle = dlopen( _alter_library , RTLD_LAZY );
							}
						}
						if( _handle == 0 )
						{
							char module_name[ 4096 ] = {0};
							::rux::safe_strncpy( module_name , ::rux::engine::_globals->_executable_directory , 4096 );
							::rux::string::replace_all( module_name , '\\' , '/' );
							size_t len = strlen( module_name );
							module_name[ len ] = '/';
							::memcpy( &module_name[ len + 1 ] , "lib/" , 4 );
							module_name[ len + 5 ] = 0;
							::rux::safe_strncat( module_name , _library , 4096 );
							if( ::rux::io::file::exists( module_name , 1 ) == 1 )
								_handle = dlopen( module_name , RTLD_LAZY );
						}
						if( _handle == 0 )
						{
							if( _alter_library[ 0 ] != 0 )
							{
								char module_name[ 4096 ] = {0};
								::rux::safe_strncpy( module_name , ::rux::engine::_globals->_executable_directory , 4096 );
								::rux::string::replace_all( module_name , '\\' , '/' );
								size_t len = strlen( module_name );
								module_name[ len ] = '/';
								::memcpy( &module_name[ len + 1 ] , "lib/" , 4 );
								module_name[ len + 5 ] = 0;
								::rux::safe_strncat( module_name , _alter_library , 4096 );
								if( ::rux::io::file::exists( module_name , 1 ) == 1 )
									_handle = dlopen( module_name , RTLD_LAZY );
							}
						}
						if( _handle == 0 )
						{
							char module_name[ 4096 ] = {0};
							for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_env_variables_count ; index0++ )
							{
								::rux::safe_strncpy( module_name , ::rux::engine::_globals->_env_variables[ index0 ] , 4096 );
								::rux::string::replace_all( module_name , '\\' , '/' );
								size_t len = strlen( module_name );
								module_name[ len ] = '/';
								module_name[ len + 1 ] = 0;
								::rux::safe_strncat( module_name , _library , 4096 );
								if( ::rux::io::file::exists( module_name , 1 ) == 1 )
								{
									_handle = dlopen( module_name , RTLD_LAZY );
									if( _handle )
										break;
								}
								if( _alter_library[ 0 ] != 0 )
								{
									::rux::safe_strncpy( module_name , ::rux::engine::_globals->_env_variables[ index0 ] , 4096 );
									::rux::string::replace_all( module_name , '\\' , '/' );
									size_t len = strlen( module_name );
									module_name[ len ] = '/';
									module_name[ len + 1 ] = 0;
									::rux::safe_strncat( module_name , _alter_library , 4096 );
									if( ::rux::io::file::exists( module_name , 1 ) == 1 )
									{
										_handle = dlopen( module_name , RTLD_LAZY );
										if( _handle )
											break;
									}
								}
							}
						}
						if( _handle == 0 )
						{
							if( _begin_from_exe_dir == 1 )
								_handle = dlopen( _library , RTLD_LAZY );
						}
						if( _handle == 0 )
						{
							if( _begin_from_exe_dir == 1 )
							{
								if( _alter_library[ 0 ] != 0 )
									_handle = dlopen( _alter_library , RTLD_LAZY );
							}
						}
						if( _handle )
						{
							on_loaded();
							XInterlocked::CompareExchange( &_loaded , 1 , 2 );
						}
						else
							XInterlocked::CompareExchange( &_loaded , 0 , 2 );
					}					
				}		
				_cs.UnLock();
			}
		};
		void module::unload( void )
		{	
			if( XInterlocked::CompareExchange( &_loaded , 2 , 1 ) == 1 )
			{
				if( _need_lock )
					CRITICAL_SECTION_LOCK( _cs );
				if( _handle )
				{
					dlclose( _handle );
					_handle = NULL;							
				}
				XInterlocked::CompareExchange( &_loaded , 0 , 2 );
				if( _need_lock )
					_cs.UnLock();
			}
		};
		void module::on_loaded( void )
		{
		};
		bool module::loaded( void )
		{
			return _handle ? true : false;
		};
	};
};