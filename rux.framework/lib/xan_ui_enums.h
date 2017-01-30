#ifndef XAN_UI_ENUMS_H
#define XAN_UI_ENUMS_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_object.h>
#include <xan_string.h>
#include <xan_gui_defines.h>
#include <xan_gui_events.h>
#include <xan_event_handler.h>
#ifndef BOOLDOG_NETWORK
#define BOOLDOG_NETWORK
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include <booldog/boo_rdwrlock.h>
namespace rux
{
	namespace gui
	{
		class Window;
		enum XEnum_ClassType
		{
			XEnum_ClassType_ControlBase , 
			XEnum_ClassType_ParentBase , 
			XEnum_ClassType_UserControl
		};
		enum XEnum_AnimationSetValueType
		{
			XEnum_AnimationSetValueType_Gradual ,
			XEnum_AnimationSetValueType_Instant
		};
		enum XEnum_AnimationType
		{
			XEnum_AnimationType_Repeat , 
			XEnum_AnimationType_OneTime
		};
		enum XEnum_StartPosition
		{
			XEnum_StartPosition_Manual , 
			XEnum_StartPosition_Center
		};
		enum XEnum_Filling
		{
			XEnum_Filling_Auto ,
			XEnum_Filling_Manual , 
			XEnum_Filling_FromContent , 
			XEnum_Filling_FromParent
		};
		enum XEnum_Alignment
		{
			XEnum_Alignment_LeftOrTop , 
			XEnum_Alignment_Center ,
			XEnum_Alignment_RightOrBottom , 
			XEnum_Alignment_Manual , 
			XEnum_Alignment_Stick , 
			XEnum_Alignment_ManualAndLocationIsControlCenter
		};
		class ParentBase;
		class CommonBase : public XGCRef
		{
		public:
			CommonBase( ::rux::get_static_Type_t get_static_Type_ )
				: XGCRef( get_static_Type_ )
			{
			};
			virtual void GCRefRelease( const char* file , ::rux::int32 line ) = 0;
			virtual void GCRefAddRef( const char* file , ::rux::int32 line ) = 0;
			virtual ::rux::uint8 IsPointInControl( ::rux::int32  x , ::rux::int32 y ) = 0;
			virtual ::rux::byte get_IsMouseEntered( void ) = 0;
			virtual void set_IsForwardEvents( ::rux::byte value ) = 0;
			virtual ::rux::byte get_IsForwardEvents( void ) = 0;
			virtual void set_IsMouseEntered( ::rux::byte val ) = 0;
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual ::rux::gui::XEnum_Cursors get_Cursor( void ) = 0;
			virtual ::rux::uint8 get_IsWindow( void ) = 0;
			virtual ::rux::uint8 get_IsButton( void ) = 0;
			virtual ::rux::uint8 get_IsGroup( void ) = 0;
			virtual void* DynamicCast( ::rux::int32 class_type ) const = 0;
			virtual ::rux::uint8 get_IsTimePicker( void ) = 0;
			virtual void Invalidate( void ) = 0;
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 ) = 0;
			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnActivate( void ) = 0;
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control ) = 0;
			virtual void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt ) = 0;
			virtual void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event ) = 0;
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;			
			virtual void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual void set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated ) = 0;
		};
		class ControlBase : public CommonBase
		{
		public:
			ControlBase( ::rux::get_static_Type_t get_static_Type_ )
				: CommonBase( get_static_Type_ )
			{
			};
			virtual ::rux::int32 get_IsVisible( void ) = 0;
			virtual void render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux_thread_index1986 ) = 0;
			virtual void private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent = 1 ) = 0;	
			virtual ::rux::uint8 get_IsGroup( void ) = 0;	
			virtual ::rux::uint8 get_IsSupportContentSize( void ) = 0;	
			virtual ::rux::uint8 get_IsButton( void ) = 0;	
			virtual ::rux::uint8 get_IsTimePicker( void ) = 0;
			virtual ::rux::uint8 get_IsTransparent( void ) = 0;
			virtual ::rux::uint8 IsPointInControl( ::rux::int32  x , ::rux::int32 y ) = 0;
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void set_IsForwardEvents( ::rux::byte value ) = 0;
			virtual ::rux::byte get_IsForwardEvents( void ) = 0;
			virtual ::rux::byte get_IsMouseEntered( void ) = 0;
			virtual void set_IsMouseEntered( ::rux::byte val ) = 0;
			virtual void FreeResources( ::rux::byte from_removed_controls_schedule ) = 0;
			virtual void UpdateParentWindow( void ) = 0;
			virtual void FreeSystemResources( void ) = 0;
			virtual void BeforeFreeSystemResources( void ) = 0;
			virtual void set_FreeSystemResourcesThreadId( void ) = 0;
			virtual ::rux::byte IsFreeSystemResourcesThread( void ) = 0;
			virtual void WaitForFreeSystemResources( void ) = 0;
			virtual void raise_OnActivate( void ) = 0;
			virtual void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt ) = 0;
			virtual void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control ) = 0;
			virtual void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void GCRefRelease( const char* file , ::rux::int32 line ) = 0;
			virtual void GCRefAddRef( const char* file , ::rux::int32 line ) = 0;
			virtual ::rux::int32 get_TabIndex( void ) = 0;
			virtual void set_ParentControl( ::rux::gui::ParentBase* parent_control ) = 0;
			virtual void private_ResetCache( void ) = 0;
			virtual ::rux::String& get_ControlName( void ) = 0;
			virtual ::rux::uint32 get_ControlNameHash( void ) = 0;
			virtual ::rux::uint32 get_TypeIndex( void ) = 0;
			virtual ::rux::uint8 get_IsUserControl( void ) = 0;
			virtual void RemoveControl( void ) = 0;
			virtual void Activate( void ) = 0;
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 ) = 0;
			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual ::rux::gui::XEnum_Cursors get_Cursor( void ) = 0;
			virtual float get_Width( float* cache_width_ptr = NULL ) = 0;
			virtual float get_Height( float* cache_height_ptr = NULL ) = 0;
			virtual float get_Left( ::rux::uint8 relative_to_parent = 0 , float* cache_left_ptr = NULL ) = 0;
			virtual float get_Top( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL ) = 0;
			virtual ::rux::gui::XEnum_Filling get_VerticalFilling( void ) = 0;
			virtual ::rux::gui::XEnum_Filling get_HorizontalFilling( void ) = 0;
			virtual ::rux::gui::XEnum_Alignment get_VerticalAlignment( void ) = 0;
			virtual ::rux::gui::XEnum_Alignment get_HorizontalAlignment( void ) = 0;
			virtual ::rux::gui::Margin& get_Margin( void ) = 0;
			virtual ::rux::gui::XEnum_PointUnit get_PointUnit( void ) = 0;
			virtual Window* get_ParentWindow( void ) const = 0;
			virtual void Invalidate( void ) = 0;
			virtual void set_Left( float left ) const = 0;
			virtual void set_Top( float top ) const = 0;
			virtual Object& get_Tag( void ) = 0;
			virtual void* get_TagPtr( void ) const = 0;
			virtual void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual void* DynamicCast( ::rux::int32 class_type ) const = 0;
			virtual void private_set_Tag( const XObject& tag ) const = 0;
			virtual Object& private_get_Tag( void ) = 0;
		};
		class ParentBase : public ControlBase
		{
		public:
			ParentBase( ::rux::get_static_Type_t get_static_Type_ )
				: ControlBase( get_static_Type_ )
			{
			};
			virtual void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 ) = 0;
			virtual void RemoveControl( XGCRef* control ) = 0;
			virtual void RemoveTabControls( void ) = 0;
			virtual void AddTabControls( void ) = 0;
			virtual void private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index = 0 ) = 0;
			virtual ::rux::byte Exists( ::rux::gui::CommonBase* control ) = 0;
			virtual ::rux::uint8 get_IsWindow( void ) = 0;
			virtual ::rux::uint8 get_IsButton( void ) = 0;
			virtual ::rux::uint8 get_IsGroup( void ) = 0;
			virtual void* DynamicCast( ::rux::int32 class_type ) const = 0;
			virtual ::rux::uint8 get_IsTimePicker( void ) = 0;
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 ) = 0;
			virtual ::rux::byte get_IsMouseEntered( void ) = 0;
			virtual void set_IsForwardEvents( ::rux::byte value ) = 0;
			virtual ::rux::byte get_IsForwardEvents( void ) = 0;
			virtual void set_IsMouseEntered( ::rux::byte val ) = 0;
			virtual void Invalidate( void ) = 0;
			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnActivate( void ) = 0;
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control ) = 0;
			virtual void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt ) = 0;
			virtual void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event ) = 0;
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;			
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual Object& get_Tag( void ) = 0;
			virtual void set_ScrollXOffset( float scroll_x_offset ) = 0;
			virtual void set_ScrollYOffset( float scroll_y_offset ) = 0;
			virtual float get_ScrollXOffset( void ) = 0;
			virtual float get_ScrollYOffset( void ) = 0;
			virtual ::rux::int32 IndexOf( XGCRef* control ) = 0;
			virtual void set_ZIndex( XGCRef* control , size_t index ) = 0;
			virtual void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event ) = 0;
			virtual void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event) = 0;
			virtual void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event) = 0;
			virtual Window* get_ParentWindow( void ) const = 0;
			virtual float get_Width( float* cache_width_ptr = NULL ) = 0;
			virtual float get_Height( float* cache_height_ptr = NULL ) = 0;
			virtual float get_Left( ::rux::uint8 relative_to_parent = 0 , float* cache_left_ptr = NULL ) = 0;
			virtual float get_Top( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL ) = 0;
		};
#define implement_rux_color_property( rux_class , color ) XGCRef& rux_class::static_get_##color( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return XColorWrap( obj->get_##color() )++;\
			};\
			void rux_class::static_set_##color( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				XColorWrap val;\
				val.set_ByRef( value );\
				obj->set_##color( val()->_color );\
			}
		#define end_implement_rux_base_ui_min_functions() end_register_rux_property()
		#define begin_implement_rux_base_ui_min_functions( rux_class ) void rux_class::set_ParentControl( ::rux::gui::ParentBase* parent_control )\
			{\
				::rux::gui::controls::set_parent_control< rux_class >( this , parent_control );\
			};\
			void rux_class::private_parent_reset_cache( void )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					parent_copy->Invalidate();\
			};\
			rux::gui::XEnum_PointUnit rux_class::get_PointUnit( void )\
			{\
				return _point_unit;\
			};\
			void rux_class::set_Tag( const XObject& tag ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				_this->_tag = tag;\
			}\
			Object& rux_class::get_Tag( void )\
			{\
				return _tag++;\
			}\
			void rux_class::private_set_Tag( const XObject& tag ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				_this->_private_tag = tag;\
			}\
			Object& rux_class::private_get_Tag( void )\
			{\
				return _private_tag++;\
			}\
			::rux::gui::ParentBase* rux_class::get_ParentControl( ::rux::byte lock , ::rux::byte real_parent )\
			{\
				if( lock )\
					WaitForFreeSystemResources();\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( real_parent == 0 && parent_copy && parent_copy->get_IsGroup() )\
				{\
					::rux::gui::ParentBase* parent_base = parent_copy->get_ParentControl( lock , 1 );\
					if( parent_base && parent_base->get_IsWindow() )\
						return parent_base;\
				}\
				return parent_copy;\
			}\
			void rux_class::set_Opacity( float opacity )\
			{\
				if( _opacity != opacity )\
				{\
					_opacity = opacity;\
					private_ResetCache();\
					Invalidate();\
				}\
			}\
			float rux_class::get_Opacity( void )\
			{\
				return _opacity;\
			};\
			::rux::int32 rux_class::get_ZIndex( void )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					return parent_copy->IndexOf( this );\
				else\
					return -1;\
			};\
			void rux_class::set_ZIndex( size_t index )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					parent_copy->set_ZIndex( this , index );\
			};\
			void rux_class::set_IsVisible( ::rux::uint8 is_visible ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _is_visible != is_visible )\
				{\
					_this->_is_visible = is_visible;\
					_this->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
					if( is_visible == 0 )\
					{\
						::rux::gui::Window* window = get_ParentWindow();\
						if( window )\
						{\
							WRITE_LOCK( window->_cs_over_control );\
							if( window->_over_control == ((CommonBase*)_this) )\
							{\
								rux::gui::CommonBase* over_control = window->_over_control;\
								window->GCRefAddRef( __FILE__ , __LINE__ );\
								window->_over_control = window;\
								window->_cs_over_control.WriteUnlock();\
								::rux::gui::controls::rux_change_over_control( 0 , over_control , NULL );\
								over_control->GCRefRelease( __FILE__ , __LINE__ );\
							}\
							else\
								window->_cs_over_control.WriteUnlock();\
						}\
					}\
				}\
			}\
			::rux::int32 rux_class::get_IsVisible( void )\
			{\
				return _is_visible;\
			}\
			rux::gui::Window* rux_class::get_ParentWindow( void ) const\
			{\
				return ::rux::gui::controls::get_parent_window< rux_class >( const_cast< rux_class* >( this ) );\
			}\
			void rux_class::set_ControlName( const ::rux::XString& name )\
			{\
				::rux::XString utf8_name;\
				utf8_name.set_ByRef( name.ConvertToUTF8() );\
				if( utf8_name.Size() <= 128 )\
					::memcpy( _utf8_control_name , utf8_name.str() , utf8_name.Size() );\
				else\
				{\
					::memcpy( _utf8_control_name , utf8_name.str() , 127 );\
					_utf8_control_name[ 127 ] = 0;\
				}\
				_utf8_control_name_hash = ::rux::cryptography::hash::times33_hash( _utf8_control_name , SIZE_MAX );\
			}\
			void rux_class::Activate( void )\
			{\
				rux::gui::Window* window = get_ParentWindow();\
				if( window )\
					window->set_ActiveControl( this );\
			};\
			void rux_class::RemoveControl( void )\
			{\
				set_ParentControl( NULL );\
			}\
			::rux::uint8 rux_class::get_IsUserControl( void )\
			{\
				if( get_IsGroup() == 1 )\
				{\
					::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)this;\
					return group->_is_user_control;\
				}\
				else\
					return 0;\
			};\
			::rux::uint32 rux_class::get_TypeIndex( void )\
			{\
				return get_RuxTypeIndex();\
			};\
			::rux::uint8 rux_class::get_IsWindow( void )\
			{\
				return 0;\
			};\
			rux::String& rux_class::get_ControlName( void )\
			{\
				return ::rux::XString( _utf8_control_name , XEnumCodePage_UTF8 )++;\
			}\
			rux::uint32 rux_class::get_ControlNameHash( void )\
			{\
				return _utf8_control_name_hash;\
			};\
			void rux_class::set_TagPtr( void* tag_ptr )\
			{\
				_tag_ptr = tag_ptr;\
			}\
			void* rux_class::get_TagPtr( void ) const\
			{\
				return _tag_ptr;\
			};\
			::rux::byte rux_class::get_IsForwardEvents( void )\
			{\
				return _is_forward_events;\
			};\
			void rux_class::set_IsForwardEvents( ::rux::byte value )\
			{\
				_is_forward_events = value;\
			};\
			::rux::byte rux_class::get_IsMouseEntered( void )\
			{\
				return _is_mouse_entered;\
			};\
			void rux_class::set_IsMouseEntered( ::rux::byte val )\
			{\
				_is_mouse_entered = val;\
			};\
			void rux_class::GCRefRelease( const char* file , ::rux::int32 line )\
			{\
				Release( file , line );\
			}\
			void rux_class::GCRefAddRef( const char* file , ::rux::int32 line )\
			{\
				AddRef( file , line );\
			}\
			void rux_class::private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent )\
			{\
				if( left || top )\
				{\
					Invalidate();\
				}\
				else if( ( width || height ) && _point_unit != ::rux::gui::XEnum_PointUnit_Pixel )\
				{\
					Invalidate();\
				}\
			};\
			void rux_class::set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback )\
			{\
				_on_mouse_enter = on_mouse_enter_callback;\
			};\
			void rux_class::dispatch_on_resized( void* param )\
			{\
				rux_class* control = (rux_class*)param;\
				XInterlocked::Exchange( &control->_already_dispatch_on_resized , 0 );\
				control->_on_resized_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *control , 1 ) );\
				control->Release();\
			};\
			void rux_class::dispatch_on_relocated( void* param )\
			{\
				rux_class* control = (rux_class*)param;\
				XInterlocked::Exchange( &control->_already_dispatch_on_relocated , 0 );\
				control->_on_relocated_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *control , 1 ) );\
				control->Release();\
			};\
			void rux_class::set_OnResized( ::rux::gui::events::on_event_t on_resized_callback )\
			{\
				_on_resized_callback = on_resized_callback;\
			};\
			void rux_class::set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback )\
			{\
				_on_relocated_callback = on_relocated_callback;\
			};\
			void rux_class::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )\
			{\
				_on_left_mouse_button_down = on_left_mouse_button_down;\
			}\
			void rux_class::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_down )\
			{\
				_on_right_mouse_button_down = on_right_mouse_button_down;\
			}\
			void rux_class::set_OnMouseWheelDown(::rux::gui::events::on_mouse_event_t on_mouse_wheel_down)\
			{\
				_on_mouse_wheel_down = on_mouse_wheel_down;\
			}\
			void rux_class::set_OnMouseWheelUp(::rux::gui::events::on_mouse_event_t on_mouse_wheel_up)\
			{\
				_on_mouse_wheel_up = on_mouse_wheel_up;\
			}\
			::rux::int32 rux_class::get_TabIndex( void )\
			{\
				return _tab_index;\
			}\
			void rux_class::set_TabIndex( ::rux::int32 tab_index )\
			{\
				if( _tab_index != tab_index )\
				{\
					_tab_index = tab_index;\
					rux::gui::XWindow window( get_ParentWindow() );\
					window()->private_InsertTabControl( _tab_index , ((::rux::gui::ControlBase*)this) );\
				}\
			}\
			void rux_class::set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated )\
			{\
				_on_deactivated = on_activated;\
			}\
			void rux_class::set_OnActivated( ::rux::gui::events::on_event_t on_activated )\
			{\
				_on_activated = on_activated;\
			}\
			void rux_class::set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_mouse_double_click )\
			{\
				_on_mouse_double_click = on_mouse_double_click;\
			}\
			void rux_class::set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				_on_mouse_move = on_mouse_leave_callback;\
			}\
			void rux_class::set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback )\
			{\
				_on_mouse_wheel = on_mouse_wheel_callback;\
			}\
			void rux_class::set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up )\
			{\
				_on_left_mouse_button_up = on_left_mouse_button_up;\
			}\
			void rux_class::set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up )\
			{\
				_on_right_mouse_button_up = on_right_mouse_button_up;\
			}\
			void rux_class::set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				_on_mouse_leave_callback = on_mouse_leave_callback;\
			};\
			void rux_class::ResetRightSticker( void )\
			{\
				_right_sticker = FLT_MAX;\
			};\
			void rux_class::ResetBottomSticker( void )\
			{\
				_bottom_sticker = FLT_MAX;\
			};\
			void rux_class::set_RightSticker( float val )\
			{\
				if( _right_sticker != val )\
				{\
					_right_sticker = val;\
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			void rux_class::set_BottomSticker( float val )\
			{\
				if( _bottom_sticker != val )\
				{\
					_bottom_sticker = val;\
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			XGCRef& rux_class::static_get_Opacity( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Opacity() )++;\
			};\
			void rux_class::static_set_Opacity( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				obj->set_Opacity( ::rux::gcref_to_float( value ) );\
			};\
			XGCRef& rux_class::static_get_Visible( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XBoolean( obj->get_IsVisible() != 0 )++;\
			};\
			void rux_class::static_set_Visible( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				obj->set_IsVisible( ::rux::gcref_to_byte( value ) );\
			};\
			begin_register_rux_property( rux_class )\
				register_rux_property( ::rux::XBoolean , Visible );\
				register_rux_property( ::rux::XFloat , Opacity )
		#define begin_implement_rux_base_ui_functions( rux_class ) void rux_class::set_Width( float width ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _width != width )\
				{\
					_this->_width = width;\
					_this->private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , 0 , 1 , 0 );\
				}\
			};\
			::rux::byte rux_class::IsFreeSystemResourcesThread( void )\
			{\
				return _free_system_resources_thread_id == ::rux::threading::XThread::get_CurrentThreadId() ? 1 : 0;\
			};\
			void rux_class::set_FreeSystemResourcesThreadId( void )\
			{\
				_free_system_resources_thread_id = ::rux::threading::XThread::get_CurrentThreadId();\
			};\
			void rux_class::BeforeFreeSystemResources( void )\
			{\
				::rux::gui::controls::before_free_system_resources< rux_class >( this );\
			};\
			void rux_class::WaitForFreeSystemResources( void )\
			{\
				::rux::gui::controls::wait_for_free_system_resources< rux_class >( this );\
			};\
			void rux_class::FreeSystemResources( void )\
			{\
				::rux::gui::controls::free_system_resources< rux_class >( this );\
			};\
			void rux_class::StartDrag( void )\
			{\
				rux::gui::Window* parent = get_ParentWindow();\
				if( parent )\
				{\
					rux::XString error;\
					parent->StartDrag( XObject( this , 1 ) , error );\
				}\
			};\
			void rux_class::StopDrag( void )\
			{\
				rux::gui::Window* parent = get_ParentWindow();\
				if( parent )\
					parent->StopDrag();\
			};\
			rux::gui::XEnum_Cursors rux_class::get_Cursor( void )\
			{\
				return _cursor;\
			};\
			void rux_class::set_Cursor( ::rux::gui::XEnum_Cursors cursor )\
			{\
				if( _cursor != cursor )\
				{\
					_cursor = cursor;\
					rux::gui::Window* window = get_ParentWindow();\
					if( window )\
					{\
						WRITE_LOCK( window->_cs_over_control );\
						if( window->_over_control == ((::rux::gui::CommonBase*)this) )\
						{\
							window->_cs_over_control.WriteUnlock();\
							window->set_Cursor( cursor );\
						}\
						else\
							window->_cs_over_control.WriteUnlock();\
					}\
				}\
			};\
			rux::gui::Margin& rux_class::get_Margin( void )\
			{\
				return _margin;\
			};\
			void rux_class::set_Margin( const ::rux::gui::Margin& margin )\
			{\
				if( _margin != margin )\
				{\
					_margin = margin;\
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			void rux_class::private_parent_reset_cache( void )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					parent_copy->Invalidate();\
			};\
			void rux_class::set_Height( float height ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _height != height )\
				{\
					_this->_height = height;\
					_this->private_ResetLocationAndSizeCache( 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , 0 , 1 );\
				}\
			};\
			float rux_class::get_AssignedWidth( void )\
			{\
				return _width;\
			};\
			float rux_class::get_AssignedHeight( void )\
			{\
				return _height;\
			};\
			float rux_class::get_AssignedLeft( void )\
			{\
				return _left;\
			};\
			float rux_class::get_AssignedTop( void )\
			{\
				return _top;\
			};\
			void rux_class::set_Left( float left ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _left != left )\
				{\
					_this->_left = left;\
					_this->private_ResetLocationAndSizeCache( 1 , 0 , 0 , 0 );\
				}\
			};\
			void rux_class::set_Top( float top ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _top != top )\
				{\
					_this->_top = top;\
					_this->private_ResetLocationAndSizeCache( 0 , 1 , 0 , 0 );\
				}\
			};\
			float rux_class::get_Left( ::rux::uint8 relative_to_parent , float* cache_left_ptr )\
			{\
				return rux_try_get_left< rux_class >( this , relative_to_parent , cache_left_ptr );\
			};\
			float rux_class::get_Top( ::rux::uint8 relative_to_parent , float* cache_top_ptr )\
			{\
				return rux_try_get_top< rux_class >( this , relative_to_parent , cache_top_ptr );\
			};\
			float rux_class::get_Width( float* cache_width_ptr )\
			{\
				return rux_try_get_width< rux_class >( this , cache_width_ptr );\
			};\
			float rux_class::get_Height( float* cache_height_ptr )\
			{\
				return rux_try_get_height< rux_class >( this , cache_height_ptr );\
			};\
			void rux_class::set_ParentControl( ::rux::gui::ParentBase* parent_control )\
			{\
				::rux::gui::controls::set_parent_control< rux_class >( this , parent_control );\
			};\
			rux::gui::XEnum_PointUnit rux_class::get_PointUnit( void )\
			{\
				return _point_unit;\
			};\
			void rux_class::set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit )\
			{\
				if( _point_unit != point_unit )\
				{\
					_point_unit = point_unit;\
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			}\
			void rux_class::set_VerticalAlignment( ::rux::gui::XEnum_Alignment alignment )\
			{\
				if( _vertical_alignment != alignment )\
				{\
					_vertical_alignment = alignment;\
					private_ResetLocationAndSizeCache( 1 , 0 , 0 , 0 );\
				}\
			}\
			void rux_class::set_HorizontalAlignment( ::rux::gui::XEnum_Alignment alignment )\
			{\
				if( _horizontal_alignment != alignment )\
				{\
					_horizontal_alignment = alignment;\
					private_ResetLocationAndSizeCache( 0 , 1 , 0 , 0 );\
				}\
			}\
			::rux::gui::XEnum_Filling rux_class::get_VerticalFilling( void )\
			{\
				return _vertical_filling;\
			};\
			::rux::gui::XEnum_Filling rux_class::get_HorizontalFilling( void )\
			{\
				return _horizontal_filling;\
			};\
			::rux::gui::XEnum_Alignment rux_class::get_VerticalAlignment( void )\
			{\
				return _vertical_alignment;\
			};\
			::rux::gui::XEnum_Alignment rux_class::get_HorizontalAlignment( void )\
			{\
				return _horizontal_alignment;\
			};\
			void rux_class::set_VerticalFilling( ::rux::gui::XEnum_Filling filling )\
			{\
				if( _vertical_filling != filling )\
				{\
					_vertical_filling = filling;\
					private_ResetLocationAndSizeCache( 0 , 0 , 0 , 1 );\
				}\
			}\
			void rux_class::set_HorizontalFilling( ::rux::gui::XEnum_Filling filling )\
			{\
				if( _horizontal_filling != filling )\
				{\
					_horizontal_filling = filling;\
					private_ResetLocationAndSizeCache( 0 , 0 , 1 , 0 );\
				}\
			}\
			void rux_class::set_Tag( const XObject& tag ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				_this->_tag = tag;\
			}\
			Object& rux_class::get_Tag( void )\
			{\
				return _tag++;\
			}\
			void rux_class::private_set_Tag( const XObject& tag ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				_this->_private_tag = tag;\
			}\
			Object& rux_class::private_get_Tag( void )\
			{\
				return _private_tag++;\
			}\
			::rux::gui::ParentBase* rux_class::get_ParentControl( ::rux::byte lock , ::rux::byte real_parent )\
			{\
				if( lock )\
					WaitForFreeSystemResources();\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( real_parent == 0 && parent_copy && parent_copy->get_IsGroup() )\
				{\
					::rux::gui::ParentBase* parent_base = parent_copy->get_ParentControl( lock , 1 );\
					if( parent_base && parent_base->get_IsWindow() )\
						return parent_base;\
				}\
				return parent_copy;\
			}\
			void rux_class::set_Opacity( float opacity )\
			{\
				if( _opacity != opacity )\
				{\
					_opacity = opacity;\
					private_ResetCache();\
					Invalidate();\
				}\
			};\
			float rux_class::get_Opacity( void )\
			{\
				return _opacity;\
			};\
			::rux::int32 rux_class::get_ZIndex( void )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					return parent_copy->IndexOf( this );\
				else\
					return -1;\
			};\
			void rux_class::set_ZIndex( size_t index )\
			{\
				READ_LOCK( _cs_parent_control );\
				::rux::gui::ParentBase* parent_copy = _parent_control;\
				_cs_parent_control.ReadUnlock();\
				if( parent_copy )\
					parent_copy->set_ZIndex( this , index );\
			};\
			void rux_class::set_IsVisible( ::rux::uint8 is_visible ) const\
			{\
				rux_class* _this = const_cast< rux_class* >( this );\
				if( _is_visible != is_visible )\
				{\
					_this->_is_visible = is_visible;\
					_this->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
					if( is_visible == 0 )\
					{\
						::rux::gui::Window* window = get_ParentWindow();\
						if( window )\
						{\
							WRITE_LOCK( window->_cs_over_control );\
							if( window->_over_control == ((CommonBase*)_this) )\
							{\
								rux::gui::CommonBase* over_control = window->_over_control;\
								window->GCRefAddRef( __FILE__ , __LINE__ );\
								window->_over_control = window;\
								window->_cs_over_control.WriteUnlock();\
								::rux::gui::controls::rux_change_over_control( 0 , over_control , NULL );\
								over_control->GCRefRelease( __FILE__ , __LINE__ );\
							}\
							else\
								window->_cs_over_control.WriteUnlock();\
						}\
					}\
				}\
			}\
			::rux::int32 rux_class::get_IsVisible( void )\
			{\
				return _is_visible;\
			}\
			rux::gui::Window* rux_class::get_ParentWindow( void ) const\
			{\
				return ::rux::gui::controls::get_parent_window< rux_class >( const_cast< rux_class* >( this ) );\
			}\
			void rux_class::set_ControlName( const ::rux::XString& name )\
			{\
				::rux::XString utf8_name;\
				utf8_name.set_ByRef( name.ConvertToUTF8() );\
				if( utf8_name.Size() <= 128 )\
					::memcpy( _utf8_control_name , utf8_name.str() , utf8_name.Size() );\
				else\
				{\
					::memcpy( _utf8_control_name , utf8_name.str() , 127 );\
					_utf8_control_name[ 127 ] = 0;\
				}\
				_utf8_control_name_hash = ::rux::cryptography::hash::times33_hash( _utf8_control_name , SIZE_MAX );\
			}\
			void rux_class::Activate( void )\
			{\
				rux::gui::Window* window = get_ParentWindow();\
				if( window )\
					window->set_ActiveControl( this );\
			};\
			void rux_class::RemoveControl( void )\
			{\
				set_ParentControl( NULL );\
			}\
			::rux::uint8 rux_class::get_IsUserControl( void )\
			{\
				if( get_IsGroup() == 1 )\
				{\
					::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)this;\
					return group->_is_user_control;\
				}\
				else\
					return 0;\
			};\
			::rux::uint32 rux_class::get_TypeIndex( void )\
			{\
				return get_RuxTypeIndex();\
			};\
			::rux::uint8 rux_class::get_IsWindow( void )\
			{\
				return 0;\
			};\
			rux::String& rux_class::get_ControlName( void )\
			{\
				return ::rux::XString( _utf8_control_name , XEnumCodePage_UTF8 )++;\
			}\
			::rux::uint32 rux_class::get_ControlNameHash( void )\
			{\
				return _utf8_control_name_hash;\
			};\
			void rux_class::set_TagPtr( void* tag_ptr )\
			{\
				_tag_ptr = tag_ptr;\
			}\
			void* rux_class::get_TagPtr( void ) const\
			{\
				return _tag_ptr;\
			}\
			::rux::byte rux_class::get_IsForwardEvents( void )\
			{\
				return _is_forward_events;\
			};\
			void rux_class::set_IsForwardEvents( ::rux::byte value )\
			{\
				_is_forward_events = value;\
			};\
			::rux::byte rux_class::get_IsMouseEntered( void )\
			{\
				return _is_mouse_entered;\
			};\
			void rux_class::set_IsMouseEntered( ::rux::byte val )\
			{\
				_is_mouse_entered = val;\
			};\
			void rux_class::GCRefRelease( const char* file , ::rux::int32 line )\
			{\
				Release( file , line );\
			}\
			void rux_class::GCRefAddRef( const char* file , ::rux::int32 line )\
			{\
				AddRef( file , line );\
			}\
			void rux_class::private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent )\
			{\
				rux_try_reset_cache< rux_class >( this , left , top , width , height , reset_parent );\
			};\
			::rux::uint8 rux_class::IsPointInControl( ::rux::int32  x , ::rux::int32 y )\
			{\
				return rux_is_point_in_control< rux_class >( this , x , y );\
			};\
			void rux_class::set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback )\
			{\
				_on_mouse_enter = on_mouse_enter_callback;\
			};\
			void rux_class::dispatch_on_resized( void* param )\
			{\
				rux_class* control = (rux_class*)param;\
				XInterlocked::Exchange( &control->_already_dispatch_on_resized , 0 );\
				control->_on_resized_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *control , 1 ) );\
				control->Release();\
			};\
			void rux_class::dispatch_on_relocated( void* param )\
			{\
				rux_class* control = (rux_class*)param;\
				XInterlocked::Exchange( &control->_already_dispatch_on_relocated , 0 );\
				control->_on_relocated_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *control , 1 ) );\
				control->Release();\
			};\
			void rux_class::set_OnResized( ::rux::gui::events::on_event_t on_resized_callback )\
			{\
				_on_resized_callback = on_resized_callback;\
			};\
			void rux_class::set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback )\
			{\
				_on_relocated_callback = on_relocated_callback;\
			};\
			void rux_class::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )\
			{\
				_on_left_mouse_button_down = on_left_mouse_button_down;\
			}\
			void rux_class::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_down )\
			{\
				_on_right_mouse_button_down = on_right_mouse_button_down;\
			}\
			void rux_class::set_OnMouseWheelDown(::rux::gui::events::on_mouse_event_t on_mouse_wheel_down)\
			{\
				_on_mouse_wheel_down = on_mouse_wheel_down;\
			}\
			void rux_class::set_OnMouseWheelUp(::rux::gui::events::on_mouse_event_t on_mouse_wheel_up)\
			{\
				_on_mouse_wheel_up = on_mouse_wheel_up;\
			}\
			::rux::int32 rux_class::get_TabIndex( void )\
			{\
				return _tab_index;\
			}\
			void rux_class::set_TabIndex( ::rux::int32 tab_index )\
			{\
				if( _tab_index != tab_index )\
				{\
					_tab_index = tab_index;\
					rux::gui::XWindow window( get_ParentWindow() );\
					window()->private_InsertTabControl( _tab_index , this );\
				}\
			}\
			void rux_class::set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated )\
			{\
				_on_deactivated = on_activated;\
			}\
			void rux_class::set_OnActivated( ::rux::gui::events::on_event_t on_activated )\
			{\
				_on_activated = on_activated;\
			}\
			void rux_class::set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_mouse_double_click )\
			{\
				_on_mouse_double_click = on_mouse_double_click;\
			}\
			void rux_class::set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				_on_mouse_move = on_mouse_leave_callback;\
			}\
			void rux_class::set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback )\
			{\
				_on_mouse_wheel = on_mouse_wheel_callback;\
			}\
			void rux_class::set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up )\
			{\
				_on_left_mouse_button_up = on_left_mouse_button_up;\
			}\
			void rux_class::set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up )\
			{\
				_on_right_mouse_button_up = on_right_mouse_button_up;\
			}\
			void rux_class::set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				_on_mouse_leave_callback = on_mouse_leave_callback;\
			};\
			void rux_class::ResetRightSticker( void )\
			{\
				_right_sticker = FLT_MAX;\
				_horizontal_alignment = XEnum_Alignment_Manual;\
			};\
			void rux_class::ResetBottomSticker( void )\
			{\
				_bottom_sticker = FLT_MAX;\
				_vertical_alignment = XEnum_Alignment_Manual;\
			};\
			void rux_class::set_RightSticker( float val )\
			{\
				_right_sticker = val;\
				_horizontal_alignment = XEnum_Alignment_Stick;\
				private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
			};\
			void rux_class::set_BottomSticker( float val )\
			{\
				_bottom_sticker = val;\
				_vertical_alignment = XEnum_Alignment_Stick;\
				private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
			};\
			XGCRef& rux_class::static_get_Opacity( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Opacity() )++;\
			};\
			void rux_class::static_set_Opacity( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				obj->set_Opacity( ::rux::gcref_to_float( value ) );\
			};\
			XGCRef& rux_class::static_get_Visible( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XBoolean( obj->get_IsVisible() != 0 )++;\
			};\
			void rux_class::static_set_Visible( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				obj->set_IsVisible( ::rux::gcref_to_byte( value ) );\
			};\
			void rux_class::static_set_MarginLeft( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				float val = ::rux::gcref_to_float( value );\
				if( obj->_margin._left_offset != val )\
				{\
					obj->_margin._left_offset = val;\
					obj->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			XGCRef& rux_class::static_get_MarginLeft( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->_margin._left_offset )++;\
			};\
			void rux_class::static_set_MarginTop( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				float val = ::rux::gcref_to_float( value );\
				if( obj->_margin._top_offset != val )\
				{\
					obj->_margin._top_offset = val;\
					obj->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			XGCRef& rux_class::static_get_MarginTop( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->_margin._top_offset )++;\
			};\
			void rux_class::static_set_MarginRight( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				float val = ::rux::gcref_to_float( value );\
				if( obj->_margin._right_offset != val )\
				{\
					obj->_margin._right_offset = val;\
					obj->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			XGCRef& rux_class::static_get_MarginRight( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->_margin._right_offset )++;\
			};\
			void rux_class::static_set_MarginBottom( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				float val = ::rux::gcref_to_float( value );\
				if( obj->_margin._bottom_offset != val )\
				{\
					obj->_margin._bottom_offset = val;\
					obj->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );\
				}\
			};\
			XGCRef& rux_class::static_get_MarginBottom( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->_margin._bottom_offset )++;\
			};\
			XGCRef& rux_class::static_get_Left( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Left() )++;\
			};\
			void rux_class::static_set_Left( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				if( rux_is_object_ptr( value , ::rux::XFloat ) )\
				{\
					obj->set_HorizontalAlignment( XEnum_Alignment_Manual );\
					obj->set_Left( ::rux::gcref_to_float( value ) );\
				}\
				else if( rux_is_object_ptr( value , ::rux::XString ) )\
				{\
					rux::XString string_value;\
					string_value.set_ByRef( value );\
					string_value.set_ByRef( string_value.ToLower() );\
					if( string_value.Equals( "left" , 4 ) )\
						obj->set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );\
					else if( string_value.Equals( "center" , 6 ) )\
						obj->set_HorizontalAlignment( XEnum_Alignment_Center );\
					else if( string_value.Equals( "right" , 6 ) )\
						obj->set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );\
					else if( string_value.Equals( "stick" , 5 ) )\
						obj->set_HorizontalAlignment( XEnum_Alignment_Stick );\
				}\
			};\
			XGCRef& rux_class::static_get_Top( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Top() )++;\
			};\
			void rux_class::static_set_Top( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				if( rux_is_object_ptr( value , ::rux::XFloat ) )\
				{\
					obj->set_VerticalAlignment( XEnum_Alignment_Manual );\
					obj->set_Top( ::rux::gcref_to_float( value ) );\
				}\
				else if( rux_is_object_ptr( value , ::rux::XString ) )\
				{\
					rux::XString string_value;\
					string_value.set_ByRef( value );\
					string_value.set_ByRef( string_value.ToLower() );\
					if( string_value.Equals( "top" , 3 ) )\
						obj->set_VerticalAlignment( XEnum_Alignment_LeftOrTop );\
					else if( string_value.Equals( "center" , 6 ) )\
						obj->set_VerticalAlignment( XEnum_Alignment_Center );\
					else if( string_value.Equals( "bottom" , 6 ) )\
						obj->set_VerticalAlignment( XEnum_Alignment_RightOrBottom );\
					else if( string_value.Equals( "stick" , 5 ) )\
						obj->set_VerticalAlignment( XEnum_Alignment_Stick );\
				}\
			};\
			XGCRef& rux_class::static_get_ZIndex( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XInt32( obj->get_ZIndex() )++;\
			};\
			void rux_class::static_set_ZIndex( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				obj->set_ZIndex( ::rux::gcref_to_int32( value ) );\
			};\
			XGCRef& rux_class::static_get_Width( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Width() )++;\
			};\
			void rux_class::static_set_Width( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				if( rux_is_object_ptr( value , ::rux::XFloat ) )\
				{\
					obj->set_HorizontalFilling( XEnum_Filling_Manual );\
					obj->set_Width( ::rux::gcref_to_float( value ) );\
				}\
			};\
			XGCRef& rux_class::static_get_Height( const XGCRef* object )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				return ::rux::XFloat( obj->get_Height() )++;\
			};\
			void rux_class::static_set_Height( const XGCRef* object , const XGCRef* value )\
			{\
				rux_class* obj = (rux_class*)object->get_ObjectAddress();\
				if( rux_is_object_ptr( value , ::rux::XFloat ) )\
				{\
					obj->set_VerticalFilling( XEnum_Filling_Manual );\
					obj->set_Height( ::rux::gcref_to_float( value ) );\
				}\
			};\
			begin_register_rux_property( rux_class )\
				register_any_class_rux_property( Left );\
				register_any_class_rux_property( Top );\
				register_any_class_rux_property( Width );\
				register_any_class_rux_property( Height );\
				register_rux_property( ::rux::XBoolean , Visible );\
				register_rux_property( ::rux::XFloat , Opacity );\
				register_rux_property( ::rux::XFloat , MarginLeft );\
				register_rux_property( ::rux::XFloat , MarginRight );\
				register_rux_property( ::rux::XFloat , MarginTop );\
				register_rux_property( ::rux::XFloat , MarginBottom )
#define end_implement_rux_base_ui_functions() end_register_rux_property()
		#define declare_content_size() ::rux::uint8 get_IsSupportContentSize( void )
		#define implement_empty_content_size( rux_class ) float rux_class::get_ContentWidth( void )\
			{\
				return 0.f;\
			};\
			float rux_class::get_ContentHeight( void )\
			{\
				return 0.f;\
			};\
			::rux::uint8 rux_class::get_IsSupportContentSize( void )\
			{\
				return 0;\
			}
		#define DECLARE_RUX_BASE_UI_MIN_FUNCTIONS() private:\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel;\
			virtual void set_ParentControl( ::rux::gui::ParentBase* parent_control );\
			virtual ::rux::uint8 get_IsGroup( void );\
			virtual void* DynamicCast( ::rux::int32 class_type ) const;\
			virtual ::rux::uint8 get_IsButton( void );\
			virtual ::rux::uint8 get_IsWindow( void );\
			virtual ::rux::uint8 get_IsTimePicker( void );\
			virtual void GCRefRelease( const char* file , ::rux::int32 line );\
			virtual void GCRefAddRef( const char* file , ::rux::int32 line );\
			public:\
			virtual ::rux::byte get_IsForwardEvents( void );\
			virtual void set_IsForwardEvents( ::rux::byte value );\
			virtual ::rux::byte get_IsMouseEntered( void );\
			virtual void set_IsMouseEntered( ::rux::byte val );\
			virtual ::rux::int32 get_IsVisible( void );\
			virtual ::rux::uint8 get_IsTransparent( void );\
			virtual void set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit );\
			virtual ::rux::gui::XEnum_PointUnit get_PointUnit( void );\
			virtual void raise_OnActivate( void );\
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control );\
			virtual void set_Tag( const XObject& tag ) const;\
			virtual Object& get_Tag( void );\
			virtual void private_set_Tag( const XObject& tag ) const;\
			virtual Object& private_get_Tag( void );\
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 );\
			virtual void set_Opacity( float opacity );\
			virtual float get_Opacity( void );\
			virtual ::rux::int32 get_ZIndex( void );\
			virtual void set_ZIndex( size_t index );\
			virtual void set_IsVisible( ::rux::uint8 is_visible ) const;\
			virtual void private_parent_reset_cache( void );\
			virtual Window* get_ParentWindow( void ) const;\
			virtual void set_ControlName( const ::rux::XString& name );\
			virtual ::rux::uint32 get_TypeIndex( void );\
			virtual ::rux::uint8 get_IsUserControl( void );\
			virtual void RemoveControl( void );\
			virtual void Activate( void );\
			virtual ::rux::String& get_ControlName( void );\
			virtual ::rux::uint32 get_ControlNameHash( void );\
			virtual void set_TagPtr( void* tag_ptr );\
			virtual void* get_TagPtr( void ) const;\
			protected:\
			virtual void FreeResources( ::rux::byte from_removed_controls_schedule );\
			virtual void UpdateParentWindow( void );\
			virtual void FreeSystemResources( void );\
			virtual void BeforeFreeSystemResources( void );\
			virtual void set_FreeSystemResourcesThreadId( void );\
			virtual ::rux::byte IsFreeSystemResourcesThread( void );\
			virtual void WaitForFreeSystemResources( void );\
			virtual void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);\
			virtual void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event\
				, ::rux::byte explicit_event);\
			virtual void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);\
			virtual void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt );\
			virtual void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event );\
			virtual void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 );\
			virtual void private_ResetCache( void );\
			template< class T >\
			friend rux_inline void rux_try_reset_cache( T* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent );\
			template< class T >\
			friend rux_inline rux::gui::Window* get_parent_window( T* _this );\
			template< class T >\
			friend rux_inline void set_parent_control( T* _this , ::rux::gui::ParentBase* parent_control );\
			template< class T >\
			friend rux_inline void wait_for_free_system_resources( T* _this );\
			template< class T >\
			friend rux_inline void before_free_system_resources( T* _this );\
			template< class T >\
			friend rux_inline void free_system_resources( T* _this );\
			virtual void private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent = 1 );\
			public:\
			virtual void Invalidate( void );\
			public:\
			virtual ::rux::uint8 IsPointInControl( ::rux::int32  x , ::rux::int32 y );\
			public:\
			virtual void set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback );\
			static void dispatch_on_resized( void* param );\
			static void dispatch_on_relocated( void* param );\
			virtual void set_OnResized( ::rux::gui::events::on_event_t on_resized_callback );\
			virtual void set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback );\
			virtual void set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			virtual void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnActivated( ::rux::gui::events::on_event_t on_activated );\
			virtual void set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated );\
			virtual void set_TabIndex( ::rux::int32 tab_index );\
			virtual ::rux::int32 get_TabIndex( void );\
			virtual void set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );\
			virtual void set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			virtual void set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback );\
			virtual void set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			virtual void set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			virtual void set_OnMouseWheelUp(::rux::gui::events::on_mouse_event_t on_left_mouse_button_up);\
			void set_RightSticker( float val );\
			void set_BottomSticker( float val );\
			void ResetRightSticker( void );\
			void ResetBottomSticker( void )
		#define DECLARE_RUX_BASE_UI_FUNCTIONS( rux_class ) protected:\
			virtual void set_ParentControl( ::rux::gui::ParentBase* parent_control );\
			virtual ::rux::uint8 get_IsGroup( void );\
			virtual void* DynamicCast( ::rux::int32 class_type ) const;\
			virtual ::rux::uint8 get_IsButton( void );\
			virtual ::rux::uint8 get_IsWindow( void );\
			virtual ::rux::uint8 get_IsTimePicker( void );\
			virtual void GCRefRelease( const char* file , ::rux::int32 line );\
			virtual void GCRefAddRef( const char* file , ::rux::int32 line );\
			private:\
			rux_class( const rux_class& );\
			void operator =( const rux_class& );\
			public:\
			virtual ::rux::byte get_IsForwardEvents( void );\
			virtual void set_IsForwardEvents( ::rux::byte value );\
			virtual ::rux::byte get_IsMouseEntered( void );\
			virtual void set_IsMouseEntered( ::rux::byte val );\
			virtual float get_Left( ::rux::uint8 relative_to_parent = 0 , float* cache_left_ptr = NULL );\
			virtual float get_Top( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL );\
			virtual float get_Width( float* cache_width_ptr = NULL );\
			virtual float get_ContentWidth( void );\
			virtual float get_ContentHeight( void );\
			virtual float get_Height( float* cache_height_ptr = NULL );\
			virtual void set_Left( float left ) const;\
			virtual float get_AssignedWidth( void );\
			virtual float get_AssignedHeight( void );\
			virtual float get_AssignedLeft( void );\
			virtual float get_AssignedTop( void );\
			virtual void set_Top( float top ) const;\
			virtual void set_Width( float width ) const;\
			virtual void StartDrag( void );\
			virtual void StopDrag( void );\
			virtual void set_Cursor( ::rux::gui::XEnum_Cursors cursor );\
			virtual ::rux::gui::XEnum_Cursors get_Cursor( void );\
			virtual ::rux::gui::Margin& get_Margin( void );\
			virtual void set_Margin( const ::rux::gui::Margin& margin );\
			virtual void set_Height( float height ) const;\
			virtual ::rux::int32 get_IsVisible( void );\
			virtual ::rux::uint8 get_IsTransparent( void );\
			virtual void raise_OnActivate( void );\
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control );\
			virtual void set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit );\
			virtual ::rux::gui::XEnum_PointUnit get_PointUnit( void );\
			virtual void set_VerticalAlignment( ::rux::gui::XEnum_Alignment alignment );\
			virtual void set_HorizontalAlignment( ::rux::gui::XEnum_Alignment alignment );\
			virtual void set_VerticalFilling( ::rux::gui::XEnum_Filling filling );\
			virtual void set_HorizontalFilling( ::rux::gui::XEnum_Filling filling );\
			virtual ::rux::gui::XEnum_Filling get_VerticalFilling( void );\
			virtual ::rux::gui::XEnum_Filling get_HorizontalFilling( void );\
			virtual ::rux::gui::XEnum_Alignment get_VerticalAlignment( void );\
			virtual ::rux::gui::XEnum_Alignment get_HorizontalAlignment( void );\
			virtual void set_Tag( const XObject& tag ) const;\
			virtual Object& get_Tag( void );\
			virtual void private_set_Tag( const XObject& tag ) const;\
			virtual Object& private_get_Tag( void );\
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 );\
			virtual void set_Opacity( float opacity );\
			virtual float get_Opacity( void );\
			virtual ::rux::int32 get_ZIndex( void );\
			virtual void set_ZIndex( size_t index );\
			virtual void set_IsVisible( ::rux::uint8 is_visible ) const;\
			virtual void private_parent_reset_cache( void );\
			virtual Window* get_ParentWindow( void ) const;\
			virtual void set_ControlName( const ::rux::XString& name );\
			virtual ::rux::uint32 get_TypeIndex( void );\
			virtual ::rux::uint8 get_IsUserControl( void );\
			virtual void RemoveControl( void );\
			virtual void Activate( void );\
			virtual ::rux::String& get_ControlName( void );\
			virtual ::rux::uint32 get_ControlNameHash( void );\
			virtual void set_TagPtr( void* tag_ptr );\
			virtual void* get_TagPtr( void ) const;\
			protected:\
			virtual void FreeResources( ::rux::byte from_removed_controls_schedule );\
			virtual void UpdateParentWindow( void );\
			virtual void FreeSystemResources( void );\
			virtual void BeforeFreeSystemResources( void );\
			virtual void set_FreeSystemResourcesThreadId( void );\
			virtual ::rux::byte IsFreeSystemResourcesThread( void );\
			virtual void WaitForFreeSystemResources( void );\
			virtual void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);\
			virtual void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event\
				, ::rux::byte explicit_event);\
			virtual void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);\
			virtual void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt );\
			virtual void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event );\
			virtual void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );\
			virtual void render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 );\
			virtual void private_ResetCache( void );\
			template< class T >\
			friend rux_inline void rux_try_reset_cache( T* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent );\
			template< class T >\
			friend rux_inline rux::gui::Window* get_parent_window( T* _this );\
			template< class T >\
			friend rux_inline void set_parent_control( T* _this , ::rux::gui::ParentBase* parent_control );\
			template< class T >\
			friend rux_inline void wait_for_free_system_resources( T* _this );\
			template< class T >\
			friend rux_inline void before_free_system_resources( T* _this );\
			template< class T >\
			friend rux_inline void free_system_resources( T* _this );\
			virtual void private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent = 1 );\
			public:\
			virtual void Invalidate( void );\
			public:\
			virtual ::rux::uint8 IsPointInControl( ::rux::int32  x , ::rux::int32 y );\
			public:\
			virtual void set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback );\
			static void dispatch_on_resized( void* param );\
			static void dispatch_on_relocated( void* param );\
			virtual void set_OnResized( ::rux::gui::events::on_event_t on_resized_callback );\
			virtual void set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback );\
			virtual void set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			virtual void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			virtual void set_OnActivated( ::rux::gui::events::on_event_t on_activated );\
			virtual void set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated );\
			virtual void set_TabIndex( ::rux::int32 tab_index );\
			virtual ::rux::int32 get_TabIndex( void );\
			virtual void set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );\
			virtual void set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			virtual void set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback );\
			virtual void set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			virtual void set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			virtual void set_OnMouseWheelUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			template< class T >\
			friend rux_inline float rux_try_get_left( T* control , ::rux::uint8 relative_to_parent , float* cache_left_ptr );\
			template< class T >\
			friend rux_inline float rux_try_get_top( T* control , ::rux::uint8 relative_to_parent , float* cache_top_ptr );\
			template< class T >\
			friend rux_inline float rux_try_get_width( T* control , float* cache_width_ptr );\
			template< class T >\
			friend rux_inline float rux_try_get_height( T* control , float* cache_height_ptr );\
			void set_RightSticker( float val );\
			void ResetRightSticker( void );\
			void ResetBottomSticker( void );\
			void set_BottomSticker( float val )
		#define IMPLEMENT_BASE_UI_MEMBERS() _utf8_control_name[ 0 ] = 0;\
			_utf8_control_name_hash = 0;\
			_parent_control = NULL;\
			_parent_window = 0;\
			_free_system_resources_accomplished = 1;\
			_free_system_resources_thread_id = 0;\
			_left = 0;\
			_top = 0;\
			_width = 120;\
			_cursor = ::rux::gui::XEnum_Cursors_Arrow;\
			_height = 90;\
			_point_unit = ::rux::gui::XEnum_PointUnit_Pixel;\
			_horizontal_alignment = XEnum_Alignment_Manual;\
			_vertical_alignment = XEnum_Alignment_Manual;\
			_vertical_filling = XEnum_Filling_Manual;\
			_horizontal_filling = XEnum_Filling_Manual;\
			_opacity = 1.0;\
			_is_visible = 1;\
			_is_mouse_entered = 0;\
			_tag_ptr = NULL;\
			_is_forward_events = 0;\
			_cache_left_inited = 0;\
			_cache_top_inited = 0;\
			_cache_width_inited = 0;\
			_cache_height_inited = 0;\
			_cache_left = 0.f;\
			_cache_top = 0.f;\
			_cache_width = 0.f;\
			_cache_height = 0.f;\
			_tab_index = -1;\
			_right_sticker = FLT_MAX;\
			_bottom_sticker = FLT_MAX;\
			_already_dispatch_on_resized = 0;\
			_already_dispatch_on_relocated = 0
		#define DECLARE_BASE_UI_MEMBERS() ::rux::gui::ParentBase* _parent_control;\
			::rux::threading::RdWrLock _cs_parent_control;\
			::rux::gui::Window* _parent_window;\
			public:\
			rux_volatile _free_system_resources_accomplished;\
			private:\
			::rux::pid_t _free_system_resources_thread_id;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel;\
			rux_volatile _cache_left_inited;\
			::rux::pid_t _cache_left_thread;\
			rux_volatile _cache_top_inited;\
			::rux::pid_t _cache_top_thread;\
			rux_volatile _cache_width_inited;\
			::rux::pid_t _cache_width_thread;\
			rux_volatile _cache_height_inited;\
			::rux::pid_t _cache_height_thread;\
			rux_volatile _already_dispatch_on_resized;\
			rux_volatile _already_dispatch_on_relocated;\
			float _cache_left;\
			float _cache_top;\
			float _cache_width;\
			float _cache_height;\
			float _left;\
			float _top;\
			float _width;\
			float _height;\
			rux::gui::XEnum_Cursors _cursor;\
			rux::gui::Margin _margin;\
			rux::gui::XEnum_PointUnit _point_unit;\
			::rux::EventHandler< ::rux::gui::events::on_drop_t > _on_drop_callback;\
			::rux::gui::XEnum_Alignment _vertical_alignment;\
			::rux::gui::XEnum_Alignment _horizontal_alignment;\
			::rux::gui::XEnum_Filling _vertical_filling;\
			::rux::gui::XEnum_Filling _horizontal_filling;\
			XObject _tag;\
			XObject _private_tag;\
			float _opacity;\
			::rux::uint8 _is_visible;\
			::rux::byte _is_mouse_entered;\
			char _utf8_control_name[ 128 ];\
			::rux::uint32 _utf8_control_name_hash;\
			void* _tag_ptr;\
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_mouse_enter;\
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_resized_callback;\
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_relocated_callback;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_leave_callback;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_double_click;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_down;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_double_click;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_down;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_down;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_double_click;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_up;\
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_activated;\
			::rux::EventHandler< ::rux::gui::events::on_deactivated_event_t > _on_deactivated;\
			::rux::int32 _tab_index;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_up;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_up;\
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_move;\
			::rux::byte _is_forward_events;\
			float _right_sticker;\
			float _bottom_sticker;\
			::booldog::threading::rdwrlock _cs_drawing_elements;\
			void initialize_members( const char* variable_name , const char* __file__ , ::rux::int32 __line__ );\
			declare_register_rux_property();\
			declare_rux_property_without_member( ::rux::XFloat , Left );\
			declare_rux_property_without_member( ::rux::XInt32 , ZIndex );\
			declare_rux_property_without_member( ::rux::XFloat , Top );\
			declare_rux_property_without_member( ::rux::XFloat , Width );\
			declare_rux_property_without_member( ::rux::XFloat , Height );\
			declare_rux_property_without_member( ::rux::XBoolean , Visible );\
			declare_rux_property_without_member( ::rux::XFloat , Opacity );\
			declare_rux_property_without_member( ::rux::XFloat , MarginLeft );\
			declare_rux_property_without_member( ::rux::XFloat , MarginRight );\
			declare_rux_property_without_member( ::rux::XFloat , MarginTop );\
			declare_rux_property_without_member( ::rux::XFloat , MarginBottom )
		#define DECLARE_BASE_UI_FUNCTIONS() void set_Left( float left ) const;\
			void set_Top( float top ) const;\
			void set_Width( float width ) const;\
			float get_AssignedWidth( void );\
			float get_AssignedHeight( void );\
			float get_AssignedLeft( void );\
			float get_AssignedTop( void );\
			void StartDrag( void );\
			void StopDrag( void );\
			void set_Cursor( ::rux::gui::XEnum_Cursors cursor );\
			rux::gui::XEnum_Cursors get_Cursor( void );\
			rux::gui::Margin& get_Margin( void );\
			void set_Margin( const ::rux::gui::Margin& margin );\
			void set_Height( float height ) const;\
			void set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit );\
			rux::gui::XEnum_PointUnit get_PointUnit( void );\
			float get_Left( ::rux::uint8 relative_to_parent = 0 , float* cache_left_ptr = NULL );\
			float get_Top( ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL );\
			float get_ContentWidth( void );\
			float get_ContentHeight( void );\
			float get_Width( float* cache_width_ptr = NULL );\
			float get_Height( float* cache_height_ptr = NULL );\
			void set_ParentControl( ::rux::gui::ParentBase* parent_control );\
			void set_OnDrop( ::rux::gui::events::on_drop_t on_drop_callback );\
			void set_VerticalAlignment( ::rux::gui::XEnum_Alignment alignment );\
			void set_HorizontalAlignment( ::rux::gui::XEnum_Alignment alignment );\
			void set_VerticalFilling( ::rux::gui::XEnum_Filling filling );\
			void set_HorizontalFilling( ::rux::gui::XEnum_Filling filling );\
			::rux::gui::XEnum_Filling get_VerticalFilling( void );\
			::rux::gui::XEnum_Filling get_HorizontalFilling( void );\
			::rux::gui::XEnum_Alignment get_VerticalAlignment( void );\
			::rux::gui::XEnum_Alignment get_HorizontalAlignment( void );\
			void set_Tag( const XObject& tag ) const;\
			Object& get_Tag( void );\
			void private_set_Tag( const XObject& tag ) const;\
			Object& private_get_Tag( void );\
			::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 );\
			void set_Opacity( float opacity );\
			float get_Opacity( void );\
			::rux::int32 get_ZIndex( void );\
			void set_ZIndex( size_t index );\
			void set_IsVisible( ::rux::uint8 is_visible ) const;\
			virtual ::rux::int32 get_IsVisible( void );\
			rux::gui::Window* get_ParentWindow( void ) const;\
			void set_ControlName( const ::rux::XString& name );\
			rux::String& get_ControlName( void );\
			::rux::uint32 get_ControlNameHash( void );\
			void RemoveControl( void );\
			void Activate( void );\
			void set_TagPtr( void* tag_ptr );\
			void* get_TagPtr( void ) const;\
			void set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback );\
			void set_OnResized( ::rux::gui::events::on_event_t on_resized_callback );\
			void set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback );\
			void set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click );\
			void set_OnActivated( ::rux::gui::events::on_event_t on_activated );\
			void set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated );\
			void set_TabIndex( ::rux::int32 tab_index );\
			::rux::int32 get_TabIndex( void );\
			::rux::byte get_IsForwardEvents( void );\
			void set_IsForwardEvents( ::rux::byte value );\
			void set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down );\
			void set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback );\
			void set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback );\
			void set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			void set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			void set_OnMouseWheelUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up );\
			::rux::uint8 get_IsTransparent( void );\
			void set_RightSticker( float val );\
			void ResetRightSticker( void );\
			void ResetBottomSticker( void );\
			void set_BottomSticker( float val )
		#define IMPLEMENT_BASE_UI_FUNCTIONS( rux_class ) void rux_class::set_Width( float width ) const\
			{\
				(*this)()->set_Width( width );\
			};\
			void rux_class::StartDrag( void )\
			{\
				(*this)()->StartDrag();\
			};\
			void rux_class::StopDrag( void )\
			{\
				(*this)()->StopDrag();\
			};\
			rux::gui::XEnum_Cursors rux_class::get_Cursor( void )\
			{\
				return (*this)()->get_Cursor();\
			};\
			void rux_class::set_Cursor( ::rux::gui::XEnum_Cursors cursor )\
			{\
				(*this)()->set_Cursor( cursor );\
			};\
			rux::gui::Margin& rux_class::get_Margin( void )\
			{\
				return (*this)()->get_Margin();\
			};\
			void rux_class::set_Margin( const ::rux::gui::Margin& margin )\
			{\
				(*this)()->set_Margin( margin );\
			};\
			void rux_class::set_Height( float height ) const\
			{\
				(*this)()->set_Height( height );\
			};\
			float rux_class::get_AssignedWidth( void )\
			{\
				return (*this)()->get_AssignedWidth();\
			};\
			float rux_class::get_AssignedHeight( void )\
			{\
				return (*this)()->get_AssignedHeight();\
			};\
			float rux_class::get_AssignedLeft( void )\
			{\
				return (*this)()->get_AssignedLeft();\
			};\
			float rux_class::get_AssignedTop( void )\
			{\
				return (*this)()->get_AssignedTop();\
			};\
			void rux_class::set_Left( float left ) const\
			{\
				(*this)()->set_Left( left );\
			};\
			void rux_class::set_Top( float top ) const\
			{\
				(*this)()->set_Top( top );\
			};\
			float rux_class::get_Left( ::rux::uint8 relative_to_parent , float* cache_left_ptr )\
			{\
				return (*this)()->get_Left( relative_to_parent , cache_left_ptr );\
			};\
			float rux_class::get_Top( ::rux::uint8 relative_to_parent , float* cache_top_ptr )\
			{\
				return (*this)()->get_Top( relative_to_parent , cache_top_ptr );\
			};\
			float rux_class::get_Width( float* cache_width_ptr )\
			{\
				return (*this)()->get_Width( cache_width_ptr );\
			};\
			float rux_class::get_Height( float* cache_height_ptr )\
			{\
				return (*this)()->get_Height( cache_height_ptr );\
			};\
			void rux_class::Activate( void )\
			{\
				(*this)()->Activate();\
			};\
			void rux_class::RemoveControl( void )\
			{\
				(*this)()->RemoveControl();\
			}\
			void rux_class::set_ParentControl( ::rux::gui::ParentBase* parent_control )\
			{\
				(*this)()->set_ParentControl( parent_control );\
			};\
			rux::gui::XEnum_PointUnit rux_class::get_PointUnit( void )\
			{\
				return (*this)()->get_PointUnit();\
			};\
			void rux_class::set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit )\
			{\
				(*this)()->set_PointUnit( point_unit );\
			}\
			void rux_class::set_VerticalAlignment( ::rux::gui::XEnum_Alignment alignment )\
			{\
				(*this)()->set_VerticalAlignment( alignment );\
			}\
			void rux_class::set_HorizontalAlignment( ::rux::gui::XEnum_Alignment alignment )\
			{\
				(*this)()->set_HorizontalAlignment( alignment );\
			}\
			::rux::gui::XEnum_Alignment rux_class::get_VerticalAlignment( void )\
			{\
				return (*this)()->get_VerticalAlignment();\
			};\
			::rux::gui::XEnum_Alignment rux_class::get_HorizontalAlignment( void )\
			{\
				return (*this)()->get_HorizontalAlignment();\
			};\
			::rux::gui::XEnum_Filling rux_class::get_VerticalFilling( void )\
			{\
				return (*this)()->get_VerticalFilling();\
			};\
			::rux::gui::XEnum_Filling rux_class::get_HorizontalFilling( void )\
			{\
				return (*this)()->get_HorizontalFilling();\
			};\
			void rux_class::set_VerticalFilling( ::rux::gui::XEnum_Filling filling )\
			{\
				(*this)()->set_VerticalFilling( filling );\
			}\
			void rux_class::set_HorizontalFilling( ::rux::gui::XEnum_Filling filling )\
			{\
				(*this)()->set_HorizontalFilling( filling );\
			}\
			void rux_class::set_Tag( const XObject& tag ) const\
			{\
				(*this)()->set_Tag( tag );\
			}\
			Object& rux_class::get_Tag( void )\
			{\
				return (*this)()->get_Tag();\
			}\
			::rux::gui::ParentBase* rux_class::get_ParentControl( ::rux::byte lock , ::rux::byte real_parent )\
			{\
				return (*this)()->get_ParentControl( lock , real_parent );\
			}\
			void rux_class::set_Opacity( float opacity )\
			{\
				(*this)()->set_Opacity( opacity );\
			};\
			float rux_class::get_Opacity( void )\
			{\
				return (*this)()->get_Opacity();\
			}\
			::rux::int32 rux_class::get_ZIndex( void )\
			{\
				return (*this)()->get_ZIndex();\
			};\
			void rux_class::set_ZIndex( size_t index )\
			{\
				(*this)()->set_ZIndex( index );\
			};\
			void rux_class::set_IsVisible( ::rux::uint8 is_visible ) const\
			{\
				(*this)()->set_IsVisible( is_visible );\
			}\
			::rux::int32 rux_class::get_IsVisible( void )\
			{\
				return (*this)()->get_IsVisible();\
			}\
			rux::gui::Window* rux_class::get_ParentWindow( void ) const\
			{\
				return (*this)()->get_ParentWindow();\
			}\
			void rux_class::set_ControlName( const ::rux::XString& name )\
			{\
				(*this)()->set_ControlName( name );\
			}\
			rux::String& rux_class::get_ControlName( void )\
			{\
				return (*this)()->get_ControlName();\
			}\
			::rux::uint32 rux_class::get_ControlNameHash( void )\
			{\
				return (*this)()->get_ControlNameHash();\
			};\
			void rux_class::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click )\
			{\
				(*this)()->set_OnLeftMouseButtonDown( on_mouse_double_click );\
			}\
			void rux_class::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_mouse_double_click )\
			{\
				(*this)()->set_OnRightMouseButtonDown( on_mouse_double_click );\
			}\
			void rux_class::set_OnMouseWheelDown(::rux::gui::events::on_mouse_event_t on_mouse_double_click)\
			{\
				(*this)()->set_OnMouseWheelDown(on_mouse_double_click);\
			}\
			::rux::int32 rux_class::get_TabIndex( void )\
			{\
				return (*this)()->get_TabIndex();\
			}\
			void rux_class::set_TabIndex( ::rux::int32 tab_index )\
			{\
				(*this)()->set_TabIndex( tab_index );\
			}\
			void rux_class::set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated )\
			{\
				(*this)()->set_OnDeactivated( on_activated );\
			}\
			void rux_class::set_OnActivated( ::rux::gui::events::on_event_t on_activated )\
			{\
				(*this)()->set_OnActivated( on_activated );\
			}\
			void rux_class::set_IsForwardEvents( ::rux::byte value )\
			{\
				(*this)()->set_IsForwardEvents( value );\
			};\
			::rux::byte rux_class::get_IsForwardEvents( void )\
			{\
				return (*this)()->get_IsForwardEvents();\
			};\
			void rux_class::set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )\
			{\
				(*this)()->set_OnMouseDoubleClick( on_left_mouse_button_down );\
			}\
			void rux_class::set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				(*this)()->set_OnMouseMove( on_mouse_leave_callback );\
			}\
			void rux_class::set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback )\
			{\
				(*this)()->set_OnMouseWheel( on_mouse_wheel_callback );\
			}\
			void rux_class::set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up )\
			{\
				(*this)()->set_OnLeftMouseButtonUp( on_left_mouse_button_up );\
			}\
			void rux_class::set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up )\
			{\
				(*this)()->set_OnRightMouseButtonUp( on_right_mouse_button_up );\
			}\
			void rux_class::set_OnMouseWheelUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up )\
			{\
				(*this)()->set_OnMouseWheelUp( on_right_mouse_button_up );\
			}\
			void rux_class::set_OnResized( ::rux::gui::events::on_event_t on_resized_callback )\
			{\
				(*this)()->set_OnResized( on_resized_callback );\
			};\
			void rux_class::set_OnRelocated( ::rux::gui::events::on_event_t on_relocated_callback )\
			{\
				(*this)()->set_OnRelocated( on_relocated_callback );\
			};\
			void rux_class::set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback )\
			{\
				(*this)()->set_OnMouseEnter( on_mouse_enter_callback );\
			};\
			void rux_class::set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )\
			{\
				(*this)()->set_OnMouseLeave( on_mouse_leave_callback );\
			};\
			void rux_class::set_TagPtr( void* tag_ptr )\
			{\
				(*this)()->set_TagPtr( tag_ptr );\
			}\
			void* rux_class::get_TagPtr( void ) const\
			{\
				return (*this)()->get_TagPtr();\
			};\
			void rux_class::ResetRightSticker( void )\
			{\
				(*this)()->ResetRightSticker();\
			};\
			void rux_class::ResetBottomSticker( void )\
			{\
				(*this)()->ResetBottomSticker();\
			};\
			void rux_class::set_RightSticker( float val )\
			{\
				(*this)()->set_RightSticker( val );\
			};\
			void rux_class::set_BottomSticker( float val )\
			{\
				(*this)()->set_BottomSticker( val );\
			}
	};
};
#endif
