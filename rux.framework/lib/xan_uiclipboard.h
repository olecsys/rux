#ifndef XAN_UICLIPBOARD_H
#define XAN_UICLIPBOARD_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	namespace gui
	{
		class Clipboard : public XGCRef
		{
			declare_rux_base_internal_methods();
			friend class XClipboard;
		private:
		public:
			explicit Clipboard( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		};
		class XClipboard : public XGCHandle<Clipboard>
		{
			declare_rux_base_methods(Clipboard);
		private:
		public:
			declare_rux_set_operators( Clipboard );
			XClipboard();
			XClipboard( const Clipboard& uiclipboard );
			XClipboard( const XClipboard& uiclipboard );
			explicit XClipboard( const XObject& object );
			static ::rux::String& GetStringData( ::rux::XString& error );
			static void SetStringData( const ::rux::XString& data , ::rux::XString& error );
		};
	};
};
#endif
