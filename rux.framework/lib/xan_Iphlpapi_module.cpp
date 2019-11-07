#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_Iphlpapi_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		iphlpapi_module::iphlpapi_module( void )
			: rux::system::module( "Iphlpapi.dll" )
		{
			_GetAdaptersAddresses = 0;
			_AddIPAddress = 0;
		};
		void iphlpapi_module::on_loaded( void )
		{
			system_module_dlsym( GetAdaptersAddresses );
			system_module_dlsym( AddIPAddress );
		};
		ULONG iphlpapi_module::GetAdaptersAddresses( ULONG Family , ULONG Flags , PVOID Reserved , PIP_ADAPTER_ADDRESSES AdapterAddresses , PULONG SizePointer )
		{
			load();
			return _GetAdaptersAddresses( Family , Flags , Reserved , AdapterAddresses , SizePointer );
		};
		DWORD iphlpapi_module::AddIPAddress( IPAddr Address , IPMask IpMask , DWORD IfIndex , PULONG NTEContext , PULONG NTEInstance )
		{
			load();
			return _AddIPAddress( Address , IpMask , IfIndex , NTEContext , NTEInstance );
		};
#endif
	};
};