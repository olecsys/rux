#ifndef XAN_UIPROGRESSBAR_H
#define XAN_UIPROGRESSBAR_H
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
			class ProgressBar : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XProgressBar;
			private:
				DECLARE_BASE_UI_MEMBERS();
				rux::gui::ColorBase* _background_object;
				rux::gui::Color* _background_border_color_object;
				rux::gui::ColorBase* _progress_color_object;
				rux::gui::Color* _progress_border_color_object;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;	
				rux::gui::RenderCacheBase* _ui_progress_border_cache;
				rux::gui::RenderCacheBase* _ui_progress_cache;	
				float _value;
				float _min_value;
				float _max_value;
				DECLARE_RUX_BASE_UI_FUNCTIONS( ProgressBar );
				declare_rux_property_without_member( ::rux::XFloat , Value );
			public:
				explicit ProgressBar( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~ProgressBar();
				void set_Value( float value );
				declare_content_size();
				float get_Value( void );				
			};
			class XProgressBar : public XGCHandle<ProgressBar>
			{
				declare_rux_base_methods(ProgressBar);
				friend class XGroup;
				friend class Group;				
			private:
			public:
				declare_rux_set_operators( ProgressBar );
				XProgressBar();
				XProgressBar( const ProgressBar& uiprogressbar );
				XProgressBar( const XProgressBar& uiprogressbar );
				explicit XProgressBar( const XObject& object );
				explicit XProgressBar( XGCRef* object );
				void set_Value( float value );
				void set_MaxValue( float max_value );
				void set_MinValue( float min_value );
				DECLARE_BASE_UI_FUNCTIONS();	
			};
		};
	};
};
#endif
