#ifndef XAN_UIRECTANGLE_H
#define XAN_UIRECTANGLE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_window.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class Rectangle : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XRectangle;
				friend class Window;
			private:
				DECLARE_BASE_UI_MEMBERS();
				float _border_width;	
				rux::gui::ColorBase* _background;
				rux::gui::Color* _border_color;
				rux::gui::Thickness _corner;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;		
				::rux::uint8 _smooth;
				DECLARE_RUX_BASE_UI_FUNCTIONS( Rectangle );
				declare_rux_property_without_member( XColorWrap , Background );
				declare_rux_property_without_member( XColorWrap , BorderColor );
			public:
				explicit Rectangle( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Rectangle();
				void set_Background( ::rux::gui::ColorBase* background );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				declare_content_size();
				::rux::gui::ColorBase* get_Background( void );
				::rux::gui::ColorBase* get_BorderColor( void );
				void set_BorderWidth(float border_width);
			};
			class XRectangle : public XGCHandle<Rectangle>
			{
				declare_rux_base_methods(Rectangle);
				friend class XGroup;
				friend class Group;				
			private:
			public:
				declare_rux_set_operators( Rectangle );
				XRectangle();
				XRectangle( const Rectangle& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XRectangle( const XRectangle& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XRectangle( const Rectangle& uirectangle );
				XRectangle( const XRectangle& uirectangle );
				explicit XRectangle( const XObject& object );
				explicit XRectangle( XGCRef* object );
				void set_Background( ::rux::gui::ColorBase* background );
				rux::gui::ColorBase* get_Background( void );
				rux::gui::Color* get_BorderColor( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Corner( const ::rux::gui::Thickness& corner );	
				DECLARE_BASE_UI_FUNCTIONS();
			};
		};
	};
};
#endif
