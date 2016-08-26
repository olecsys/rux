#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uiimage.h>
#include <xan_uiapplication.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_keyvaluepair.h>
#include <xan_window.h>
begin_implement_rux_class_with_properties_ns_base_class( Image , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_is_new_image = 1;
	_is_loaded = 0;
	_on_loaded = NULL;	
	_is_copying_to_video_card_buffer = 1;	
	_fourcc = ::rux::media::XEnum_Fourcc_Unknown;	
	_brightness = 0.0f;
	_contrast = 1.0f;
	_is_alpha = 0;
	_image_left = 0;
	_image_top = 0;
	_image_width = INT32_MAX;
	_image_height = INT32_MAX;
	_already_empty_image_rendered = 0;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			Image::~Image()
			{
				FreeResources( 0 );
			};
			begin_implement_rux_base_ui_functions( Image )
			end_implement_rux_base_ui_functions();
			void Image::set_IsAlpha( ::rux::uint8 alpha )
			{
				_is_alpha = alpha;
			};
			void XImage::set_URL( const ::rux::XString& url )
			{
				(*this)()->set_URL( url );
			};
			void Image::set_URL( const ::rux::XString& url )
			{
				if( _url != url )
				{
					_url = url;
					rux::XString error;
					WRITE_LOCK( _cs_frame );
					_frame.LockData();
					_frame.set_ImageData( _url , error );		
					_frame.UnLockData();
					_cs_frame.WriteUnlock();
					XInterlocked::Exchange( &_is_new_image , 1 );
					if( _is_loaded == 0 )
					{
						_is_loaded = 1;
						_on_loaded.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
					}
				}
			};
			void XImage::set_IsAlpha( ::rux::uint8 alpha )
			{
				(*this)()->set_IsAlpha( alpha );
			};
			implement_duplicate_internal_function_1( Image , set_ImageLeft , ::rux::int32 );
			void Image::set_ImageLeft( ::rux::int32 left )
			{
				_image_left = left;
			};
			implement_duplicate_internal_function_1( Image , set_ImageTop , ::rux::int32 );
			void Image::set_ImageTop( ::rux::int32 top )
			{
				_image_top = top;
			};
			implement_duplicate_internal_function_1( Image , set_ImageWidth , ::rux::int32 );
			void Image::set_ImageWidth( ::rux::int32 width )
			{
				_image_width = width;
			};
			implement_duplicate_internal_function_1( Image , set_ImageHeight , ::rux::int32 );
			void Image::set_ImageHeight( ::rux::int32 height )
			{
				_image_height = height;
			};
			void* Image::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Image* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Image::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 Image::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Image::get_IsButton( void )
			{
				return 0;
			};
			::rux::uint8 Image::get_IsTransparent( void )
			{	
				WRITE_LOCK( _cs_frame );
				::rux::byte res = _frame.get_ImageSize() > 0 && _opacity > 0 ? 0 : 1;
				_cs_frame.WriteUnlock();
				return res;
			};
			void Image::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{
				::rux::memory::extended_memory_info* extended_memory_info = 0;
				::rux::engine::_globals->_get_memory_extended_info( this , extended_memory_info );
				if( _ui_cache == NULL )
				{
					if( extended_memory_info && extended_memory_info->_file_name )
						render_context->DrawImage( _fourcc , get_Left() , get_Top() , get_Width() , get_Height() , _opacity * opacity , &_ui_cache , _brightness , _contrast , _selected_z_index , _is_alpha , _image_left , _image_top , _image_width , _image_height , extended_memory_info->_file_name , extended_memory_info->_line , ___rux__thread_index1986 );
					else
						render_context->DrawImage( _fourcc , get_Left() , get_Top() , get_Width() , get_Height() , _opacity * opacity , &_ui_cache , _brightness , _contrast , _selected_z_index , _is_alpha , _image_left , _image_top , _image_width , _image_height , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				}
				::rux::byte empty_image_rendered = 1;
				::rux::byte new_image = 0;
				::rux::media::Frame* frame = 0;
				READ_LOCK( _cs_frame );				
				frame = cpp_ptr_cast( ::rux::media::Frame* , _frame.get_GCRef( 0 , 1 ) );
				if( frame )
				{
					frame->AddRef( __FILE__ , __LINE__ );
					_cs_frame.ReadUnlock();

					frame->LockData();
					if( frame->get_ImageSize() == 0 )
						frame->UnLockData();
					else
					{
						empty_image_rendered = 0;
						::rux::byte* image_buffer = frame->get_ImageData( new_image );				
						if( image_buffer && ( new_image || XInterlocked::CompareExchange( &_is_new_image , 0 , 1 ) == 1 ) )
						{					
							_fourcc = frame->get_Fourcc();
							if( _ui_cache )
								_ui_cache->CopyToTexture( image_buffer , frame->get_ImageWidth() , frame->get_ImageHeight() , frame->get_ImageSize() , frame->get_BitCount() , _fourcc , _opacity * opacity , ___rux__thread_index1986 );
						}
						frame->UnLockData();
						if( extended_memory_info && extended_memory_info->_file_name )
							render_context->DrawImage( _fourcc , get_Left() , get_Top() , get_Width() , get_Height() , _opacity * opacity , &_ui_cache , _brightness , _contrast , _selected_z_index , _is_alpha , _image_left , _image_top , _image_width , _image_height , extended_memory_info->_file_name , extended_memory_info->_line , ___rux__thread_index1986 );
						else
							render_context->DrawImage( _fourcc , get_Left() , get_Top() , get_Width() , get_Height() , _opacity * opacity , &_ui_cache , _brightness , _contrast , _selected_z_index , _is_alpha , _image_left , _image_top , _image_width , _image_height , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					}
					frame->Release( __FILE__ , __LINE__ );
				}
				else
					_cs_frame.ReadUnlock();
				if( empty_image_rendered )
				{
					rux::gui::Window* window = get_ParentWindow();
					if( window )
					{													
						if( XInterlocked::CompareExchange( &_already_empty_image_rendered , 1 , 0 ) == 0 )
						{
							AddRef();
							window->Dispatch( Image::dispatch_on_empty_image_rendered , this , 0 );
						}
					}
				}
			};
			void Image::dispatch_on_empty_image_rendered( void* param )
			{
				Image* control = (Image*)param;
				control->_on_empty_image_rendered.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *control , 1 ) );
				XInterlocked::Exchange( &control->_already_empty_image_rendered , 0 );
				control->Release();
			};
			void Image::Invalidate( void )
			{
				XInterlocked::Exchange( &_is_new_image , 1 );
			};
			void Image::private_ResetCache( void )
			{	
				XInterlocked::Exchange( &_is_new_image , 1 );
			};
			void Image::set_EmptyData( void )
			{	
				WRITE_LOCK( _cs_frame );
				_frame.LockData();
				_frame.set_EmptyData();	
				_frame.UnLockData();
				_cs_frame.WriteUnlock();
			};
			void XImage::set_EmptyData( void )
			{
				(*this)()->set_EmptyData();
			};
			void XImage::set_Brightness( float brightness )
			{
				(*this)()->set_Brightness( brightness );
			};
			void Image::set_Brightness( float brightness )
			{
				if( brightness < -1.f )
					brightness = -1.f;
				else if( brightness > 1.f )
					brightness = 1.f;
				_cs_drawing_elements.wlock( debuginfo_macros );
				_brightness = brightness;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				XInterlocked::Exchange( &_is_new_image , 1 );
			};
			implement_empty_content_size( Image );
			float Image::get_Brightness( void )
			{
				return _brightness;
			};
			float Image::get_Contrast( void )
			{
				return _contrast;
			};
			void XImage::set_Contrast( float contrast )
			{
				(*this)()->set_Contrast( contrast );
			};
			void Image::set_Contrast( float contrast )	
			{
				if( contrast < 0 )
					contrast = 0;
				else if( contrast > 10 )
					contrast = 10;
				_cs_drawing_elements.wlock( debuginfo_macros );	
				_contrast = contrast;
				_cs_drawing_elements.wunlock( debuginfo_macros );
				XInterlocked::Exchange( &_is_new_image , 1 );
			};
			void XImage::set_FrameData( char* data_ptr , ::rux::uint32 width , ::rux::uint32 height , ::rux::media::XEnum_Fourcc fcc , ::rux::int16 bit_count , ::rux::byte copy )
			{
				(*this)()->set_FrameData( data_ptr , width , height , fcc , bit_count , copy );	
			};
			implement_duplicate_internal_function_1( Image , get_Frame , ::rux::media::XFrame& );
			void Image::get_Frame( ::rux::media::XFrame& frame )
			{
				WRITE_LOCK( _cs_frame );
				frame.CopyFrom( _frame );
				_cs_frame.WriteUnlock();
			};
			void Image::set_Frame( const ::rux::media::XFrame& frame )
			{
				WRITE_LOCK( _cs_frame );
				_frame.set_ByRef( frame );
				_cs_frame.WriteUnlock();
			};
			void XImage::set_Frame( const ::rux::media::XFrame& frame )
			{
				(*this)()->set_Frame( frame );
			};
			void Image::set_FrameData( char* data_ptr , ::rux::uint32 width , ::rux::uint32 height , ::rux::media::XEnum_Fourcc fcc , ::rux::int16 bit_count , ::rux::byte copy )
			{
				READ_LOCK( _cs_frame );				
				::rux::media::Frame* frame = _frame();
				frame->AddRef( __FILE__ , __LINE__ );
				_cs_frame.ReadUnlock();
				frame->set_ImageData( data_ptr , width , height , fcc , bit_count , copy );
				frame->Release( __FILE__ , __LINE__ );
				XInterlocked::Exchange( &_is_new_image , 1 );
				if( _is_loaded == 0 )
				{
					_is_loaded = 1;
					_on_loaded.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
				}
			};
			implement_duplicate_internal_function_1( Image , set_OnEmptyImageRendered , ::rux::gui::events::on_event_t );
			void Image::set_OnEmptyImageRendered( ::rux::gui::events::on_event_t on_empty_image_rendered )
			{
				_on_empty_image_rendered = on_empty_image_rendered;
			};
			void XImage::set_OnLoaded( ::rux::gui::events::on_event_t on_loaded )
			{
				(*this)()->_on_loaded = on_loaded;
			};
			void Image::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			void Image::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void Image::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
			};
			::rux::byte Image::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{	
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void Image::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_move.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void Image::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Image::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Image::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Image::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Image::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
			};
			void Image::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Image::raise_OnActivate( void )
			{		
			};
			void Image::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XImage );
		};
	};
};