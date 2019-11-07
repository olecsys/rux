#ifndef XAN_USERENV_MODULE_H
#define XAN_USERENV_MODULE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_system_module.h"
namespace rux
{
	namespace system
	{
#ifdef __WINDOWS__
		typedef BOOL (WINAPI *DestroyEnvironmentBlock_t)( LPVOID lpEnvironment );
		typedef BOOL (WINAPI *CreateEnvironmentBlock_t)( LPVOID* lpEnvironment , HANDLE hToken , BOOL bInherit );
		typedef BOOL (WINAPI *GetUserProfileDirectoryW_t)( HANDLE hToken , wchar_t* lpProfileDir , LPDWORD lpcchSize );
		class userenv_module : public rux::system::module
		{
		protected:
			DestroyEnvironmentBlock_t _DestroyEnvironmentBlock;
			CreateEnvironmentBlock_t _CreateEnvironmentBlock;
			GetUserProfileDirectoryW_t _GetUserProfileDirectoryW;
		public:
			userenv_module( void );
			virtual void on_loaded( void );
			BOOL WINAPI DestroyEnvironmentBlock( LPVOID lpEnvironment );
			BOOL WINAPI CreateEnvironmentBlock( LPVOID* lpEnvironment , HANDLE hToken , BOOL bInherit );
			BOOL WINAPI GetUserProfileDirectoryW( HANDLE hToken , wchar_t* lpProfileDir , LPDWORD lpcchSize );
		};
#endif
	};
};
#endif
