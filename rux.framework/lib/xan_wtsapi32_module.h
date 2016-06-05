#ifndef XAN_WTSAPI32_MODULE_H
#define XAN_WTSAPI32_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_system_module.h>
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef BOOL (WINAPI *WTSQueryUserToken_t)( ULONG SessionId , PHANDLE phToken );
		class wtsapi32_module : public rux::system::module
		{
		protected:
			WTSQueryUserToken_t _WTSQueryUserToken;
		public:
			wtsapi32_module( void );
			BOOL WINAPI WTSQueryUserToken( ULONG SessionId , PHANDLE phToken );
			virtual void on_loaded( void );
		};
#endif
	};
};
#endif
