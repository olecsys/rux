#ifndef XAN_IPHLPAPI_MODULE_H
#define XAN_IPHLPAPI_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
#ifdef __WINDOWS__
#include <IPHlpApi.h>
#endif
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef ULONG (WINAPI *GetAdaptersAddresses_t)( ULONG Family , ULONG Flags , PVOID Reserved , PIP_ADAPTER_ADDRESSES AdapterAddresses , PULONG SizePointer );
		typedef DWORD (*AddIPAddress_t)( IPAddr Address , IPMask IpMask , DWORD IfIndex , PULONG NTEContext , PULONG NTEInstance );
		class iphlpapi_module : public rux::system::module
		{
		protected:
			GetAdaptersAddresses_t _GetAdaptersAddresses;
			AddIPAddress_t _AddIPAddress;
		public:
			iphlpapi_module( void );
			virtual void on_loaded( void );
			ULONG WINAPI GetAdaptersAddresses( ULONG Family , ULONG Flags , PVOID Reserved , PIP_ADAPTER_ADDRESSES AdapterAddresses , PULONG SizePointer );
			DWORD AddIPAddress( IPAddr Address , IPMask IpMask , DWORD IfIndex , PULONG NTEContext , PULONG NTEInstance );
		};
#endif
	};
};
#endif
