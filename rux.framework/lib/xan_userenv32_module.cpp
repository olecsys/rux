#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_userenv32_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		userenv_module::userenv_module( void )
			: rux::system::module( "userenv.dll" )
		{
			_DestroyEnvironmentBlock = 0;
			_CreateEnvironmentBlock = 0;
			_GetUserProfileDirectoryW = 0;
		};
		void userenv_module::on_loaded( void )
		{
			system_module_dlsym( DestroyEnvironmentBlock );
			system_module_dlsym( CreateEnvironmentBlock );
			system_module_dlsym( GetUserProfileDirectoryW );
		};
		BOOL userenv_module::DestroyEnvironmentBlock( LPVOID lpEnvironment )
		{
			load();
			return _DestroyEnvironmentBlock( lpEnvironment );
		};
		BOOL userenv_module::CreateEnvironmentBlock( LPVOID* lpEnvironment , HANDLE hToken , BOOL bInherit )
		{
			load();
			return _CreateEnvironmentBlock( lpEnvironment , hToken , bInherit );
		};
		BOOL userenv_module::GetUserProfileDirectoryW( HANDLE hToken , wchar_t* lpProfileDir , LPDWORD lpcchSize )
		{
			load();
			return _GetUserProfileDirectoryW( hToken , lpProfileDir , lpcchSize );
		};
#endif
	};
};