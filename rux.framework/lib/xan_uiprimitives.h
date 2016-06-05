#ifndef XAN_UIPRIMITIVES_H
#define XAN_UIPRIMITIVES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uigroup.h>
#include <xan_uibutton.h>
#include <xan_uipath.h>
namespace rux
{
	namespace gui
	{
		class XPrimitives
		{
		public:
			static ::rux::gui::controls::Group& Cogwheel( void );
			static ::rux::gui::controls::Group& Plus( float left , float top , float width , float height , ::rux::gui::XEnum_PointUnit point_unit , ::rux::gui::ColorBase* background );
			static ::rux::gui::controls::Group& Star( float left , float top , float width , float height , ::rux::gui::XEnum_PointUnit point_unit , ::rux::gui::ColorBase* background );	
			static ::rux::gui::controls::Button& TransparentButton( void );
			static ::rux::gui::controls::Group& ReelOfFilm( void );
			static ::rux::gui::controls::Group& Camera( void );
			static ::rux::gui::controls::Group& Calendar( void );
			static ::rux::gui::controls::Group& LeftArrow( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& LeftArrowAndLine( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& LeftArrowAndTwoLines( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& LeftArrowAndThreeLines( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& RightArrowAndLine( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& RightArrowAndTwoLines( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& RightArrowAndThreeLines( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& RightArrow( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& DoubleRightArrow( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& UpArrow( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& DownArrow( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& Pause( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& Stop( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& VerticalDivider( ::rux::gui::XEnum_Alignment horizontal_alignment , float top , float height , ::rux::uint8 is_visible );
			static ::rux::gui::controls::Group& VerticalDivider( float left , float top , float height , ::rux::uint8 is_visible );
			static ::rux::gui::controls::Group& HorizontalDivider( ::rux::gui::XEnum_Alignment vertical_alignment , float top , float left , float width , ::rux::uint8 is_visible );
			static ::rux::gui::controls::Group& MuteOff( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& MuteOn( ::rux::gui::ColorBase* background_object );
			static ::rux::gui::controls::Group& Cross( ::rux::gui::Color* background_object , float thickness );
			static ::rux::gui::controls::Group& Minimize( ::rux::gui::Color* background_object , float thickness );
			static ::rux::gui::controls::Group& Maximize( ::rux::gui::Color* background_object );
			static ::rux::gui::controls::Group& UnMaximize( ::rux::gui::Color* background_object );
			static ::rux::gui::controls::Group& RightFlag( ::rux::gui::Color* background_object );
			static ::rux::gui::controls::Group& LeftFlag( ::rux::gui::Color* background_object );
			static ::rux::gui::controls::Group& Download( ::rux::gui::Color* background_object );
			static ::rux::gui::controls::Group& Loader( void );
			static void LoaderAnimation( ::rux::gui::controls::XGroup& loader , XGroupAnimation& group_animation , uint64 one_step_duration );	
			static ::rux::gui::controls::Group& VerticalMargin( float height , ::rux::gui::XEnum_Alignment vertical_alignment );
			static ::rux::gui::controls::Group& HorizontalMargin( float width , ::rux::gui::XEnum_Alignment horizontal_alignment );
		};
	};
};
#endif
