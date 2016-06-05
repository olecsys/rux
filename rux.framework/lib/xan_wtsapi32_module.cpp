#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_wtsapi32_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		wtsapi32_module::wtsapi32_module( void )
			: rux::system::module( "WtsApi32.dll" )
		{
			_WTSQueryUserToken = 0;
		};
		BOOL WINAPI wtsapi32_module::WTSQueryUserToken( ULONG SessionId , PHANDLE phToken )
		{
			load();
			return _WTSQueryUserToken( SessionId , phToken );
		};
		void wtsapi32_module::on_loaded( void )
		{
			system_module_dlsym( WTSQueryUserToken );
		};
#endif
	};
};