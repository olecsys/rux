#ifndef XAN_UIIMAGE_H
#define XAN_UIIMAGE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_image.h>
#include <xan_ui_enums.h>
#include <xan_window.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class Image : public ::rux::gui::ControlBase
			{
				declare_rux_base_internal_methods();
				friend class XImage;
			private:
				DECLARE_BASE_UI_MEMBERS();
				::rux::threading::RdWrLock _cs_frame;
				rux::media::XFrame _frame;
				rux_volatile _is_new_image;
				rux_volatile _already_empty_image_rendered;
				::rux::uint8 _is_loaded;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_loaded;
				::rux::EventHandler< ::rux::gui::events::on_event_t > _on_empty_image_rendered;
				::rux::uint8 _is_copying_to_video_card_buffer;
				rux::gui::RenderCacheBase* _ui_cache;
				::rux::int32 _fourcc;
				float _brightness;
				float _contrast;
				::rux::uint8 _is_alpha;
				rux::XString _url;
				::rux::int32 _image_left;
				::rux::int32 _image_top;
				::rux::int32 _image_width;
				::rux::int32 _image_height;
				DECLARE_RUX_BASE_UI_FUNCTIONS( Image );
			public:
				explicit Image( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Image();
				void set_FrameData( char* data_ptr , ::rux::uint32 width , ::rux::uint32 height , ::rux::media::XEnum_Fourcc fcc , ::rux::int16 bit_count = 0 , ::rux::byte copy = 1 );
				void set_Frame( const ::rux::media::XFrame& frame );
				void set_EmptyData( void );	
				declare_content_size();
				void set_Brightness( float brightness );
				void set_Contrast( float contrast );
				float get_Brightness( void );
				float get_Contrast( void );
				void set_IsAlpha( ::rux::uint8 alpha );
				void set_URL( const ::rux::XString& url );
				void get_Frame( ::rux::media::XFrame& frame );
				void set_ImageLeft( ::rux::int32 left );
				void set_ImageTop( ::rux::int32 top );
				void set_ImageWidth( ::rux::int32 width );
				void set_ImageHeight( ::rux::int32 height );
				static void dispatch_on_empty_image_rendered( void* param );
				void set_OnEmptyImageRendered( ::rux::gui::events::on_event_t on_empty_image_rendered );
			};
			class XImage : public XGCHandle<Image>
			{
				declare_rux_base_methods(Image);
				friend class XGroup;
				friend class Group;				
			private:
			public:
				declare_rux_set_operators( Image );
				XImage();
				XImage( const Image& uiimage );
				XImage( const XImage& uiimage );
				explicit XImage( const XObject& object );
				explicit XImage( XGCRef* object );
				void set_FrameData( char* data_ptr , ::rux::uint32 width , ::rux::uint32 height , ::rux::media::XEnum_Fourcc fcc , ::rux::int16 bit_count = 0 , ::rux::byte copy = 1 );
				void set_Frame( const ::rux::media::XFrame& frame );
				void get_Frame( ::rux::media::XFrame& frame );
				void set_EmptyData( void );
				void set_OnLoaded( ::rux::gui::events::on_event_t on_loaded );
				void set_OnEmptyImageRendered( ::rux::gui::events::on_event_t on_empty_image_rendered );
				void set_Brightness( float brightness );
				void set_Contrast( float contrast );
				void set_IsAlpha( ::rux::uint8 alpha );
				void set_URL( const ::rux::XString& url );
				void set_ImageLeft( ::rux::int32 left );
				void set_ImageTop( ::rux::int32 top );
				void set_ImageWidth( ::rux::int32 width );
				void set_ImageHeight( ::rux::int32 height );
				DECLARE_BASE_UI_FUNCTIONS();
			public:	
			};
		};
	};
};
#endif
