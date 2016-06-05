#ifndef XAN_UIPATH_H
#define XAN_UIPATH_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_window.h>
#include <xan_gui_defines.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class XPath;
			class Path : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class ::rux::gui::controls::XPath;
			private:	
				DECLARE_BASE_UI_MEMBERS();
				rux::gui::ColorBase* _background;
				XCrtSect _cs_points;
				XMallocArray< ::rux::gui::Point* > _points;
				rux::gui::RenderCacheBase* _ui_cache;	
				::rux::uint8 _is_smooth;
				DECLARE_RUX_BASE_UI_FUNCTIONS( Path );
				declare_rux_property_without_member( XColorWrap , Background );
			public:
				explicit Path( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Path();
				void set_Background( ::rux::gui::ColorBase* background );
				declare_content_size();
				::rux::gui::ColorBase* get_Background( void );
			};
			class XPath : public XGCHandle<Path>
			{
				declare_rux_base_methods(Path);
				friend class XGroup;
				friend class Group;				
			private:
			public:
				declare_rux_set_operators( Path );
				XPath();
				XPath( const Path& uipath );
				XPath( const XPath& uipath );
				explicit XPath( const XObject& object );
				explicit XPath( XGCRef* object );
				void set_Background( ::rux::gui::ColorBase* background );
				rux::gui::ColorBase* get_Background( void );
				void AddPoint( const ::rux::gui::Point& point );
				void set_IsSmooth( ::rux::uint8 smooth );
				DECLARE_BASE_UI_FUNCTIONS();
			};
		};
	};
};
#endif
