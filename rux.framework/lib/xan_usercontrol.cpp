#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_usercontrol.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			UserControl::UserControl( void )
			{
			};
			float UserControl::get_Opacity( void )
			{
				return _group_main.get_Opacity();
			};
			void UserControl::set_Width( float width ) const
			{
				_group_main.set_Width( width );
			};
			void UserControl::StartDrag( void )
			{
				_group_main.StartDrag();
			};
			void UserControl::StopDrag( void )
			{
				_group_main.StopDrag();
			};
			rux::gui::XEnum_Cursors UserControl::get_Cursor( void )
			{
				return _group_main.get_Cursor();
			};
			void UserControl::set_Cursor( ::rux::gui::XEnum_Cursors cursor )
			{
				_group_main.set_Cursor( cursor );
			};
			rux::gui::Margin& UserControl::get_Margin( void )
			{
				return _group_main.get_Margin();
			};
			void UserControl::set_Margin( const ::rux::gui::Margin& margin )
			{
				_group_main.set_Margin( margin );
			};
			void UserControl::private_parent_reset_cache( void )
			{
				_group_main()->private_parent_reset_cache();
			};
			void UserControl::set_Height( float height ) const
			{
				_group_main.set_Height( height );
			};
			float UserControl::get_AssignedWidth( void )
			{
				return _group_main.get_AssignedWidth();
			};
			float UserControl::get_AssignedHeight( void )
			{
				return _group_main.get_AssignedHeight();
			};
			float UserControl::get_AssignedLeft( void )
			{
				return _group_main.get_AssignedLeft();
			};
			float UserControl::get_AssignedTop( void )
			{
				return _group_main.get_AssignedTop();
			};
			void UserControl::set_Left( float left ) const
			{
				_group_main.set_Left( left );
			};
			void UserControl::set_Top( float top ) const
			{
				_group_main.set_Top( top );
			};
			float UserControl::get_Left( ::rux::uint8 relative_to_parent , float* cache_left_ptr )
			{
				return _group_main.get_Left( relative_to_parent , cache_left_ptr );
			};
			float UserControl::get_Top( ::rux::uint8 relative_to_parent , float* cache_top_ptr )
			{
				return _group_main.get_Top( relative_to_parent , cache_top_ptr );
			};
			float UserControl::get_Width( float* cache_width_ptr )
			{
				return _group_main.get_Width( cache_width_ptr );
			};
			float UserControl::get_Height( float* cache_height_ptr )
			{
				return _group_main.get_Height( cache_height_ptr );
			};
			void UserControl::set_ParentControl( ::rux::gui::ParentBase* parent_control )
			{
				_group_main.set_ParentControl( parent_control );
			};
			rux::gui::XEnum_PointUnit UserControl::get_PointUnit( void )
			{
				return _group_main.get_PointUnit();
			};
			void UserControl::set_PointUnit( ::rux::gui::XEnum_PointUnit point_unit )
			{
				_group_main.set_PointUnit( point_unit );
			}
			void UserControl::set_VerticalAlignment( ::rux::gui::XEnum_Alignment alignment )
			{
				_group_main.set_VerticalAlignment( alignment );
			}
			void UserControl::set_HorizontalAlignment( ::rux::gui::XEnum_Alignment alignment )
			{
				_group_main.set_HorizontalAlignment( alignment );
			}
			::rux::gui::XEnum_Filling UserControl::get_VerticalFilling( void )
			{
				return _group_main.get_VerticalFilling();
			};
			::rux::gui::XEnum_Filling UserControl::get_HorizontalFilling( void )
			{
				return _group_main.get_HorizontalFilling();
			};
			::rux::gui::XEnum_Alignment UserControl::get_VerticalAlignment( void )
			{
				return _group_main.get_VerticalAlignment();
			};
			::rux::gui::XEnum_Alignment UserControl::get_HorizontalAlignment( void )
			{
				return _group_main.get_HorizontalAlignment();
			};
			void UserControl::set_VerticalFilling( ::rux::gui::XEnum_Filling filling )
			{
				_group_main.set_VerticalFilling( filling );
			}
			void UserControl::set_HorizontalFilling( ::rux::gui::XEnum_Filling filling )
			{
				_group_main.set_HorizontalFilling( filling );
			}
			void UserControl::set_Tag( const XObject& tag ) const
			{
				_group_main.set_Tag( tag );
			}
			Object& UserControl::get_Tag( void )
			{
				return _group_main.get_Tag();
			}
			void UserControl::private_set_Tag( const XObject& ) const
			{
			}
			Object& UserControl::private_get_Tag( void )
			{
				return XObject( "" , __FILE__ , __LINE__ )++;
			}
			::rux::gui::ParentBase* UserControl::get_ParentControl( ::rux::byte lock , ::rux::byte real_parent )
			{
				return _group_main.get_ParentControl( lock , real_parent );
			}
			void UserControl::set_Opacity( float opacity )
			{
				_group_main.set_Opacity( opacity );
			};
			::rux::int32 UserControl::get_ZIndex( void )
			{
				return _group_main.get_ZIndex();
			};
			void UserControl::set_ZIndex( size_t index )
			{
				_group_main.set_ZIndex( index );
			};
			void UserControl::set_IsVisible( ::rux::uint8 is_visible ) const
			{
				_group_main.set_IsVisible( is_visible );
			}
			::rux::int32 UserControl::get_IsVisible( void )
			{
				return _group_main.get_IsVisible();
			}
			void UserControl::raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt )
			{		
				_group_main()->raise_OnKeyChar( symbol , shift , control , alt );
			};
			void UserControl::raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 native_value , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnKeyDown( key , shift , control , alt , native_value , explicit_event );
			};
			void UserControl::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnRightMouseButtonDown( window_event , explicit_event );
			};
			void UserControl::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnRightMouseButtonDoubleClick( window_event , explicit_event );
			};
			void UserControl::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnRightMouseButtonUp( window_event , explicit_event );
			};
			void UserControl::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnLeftMouseButtonDown( window_event , explicit_event );
			};
			float UserControl::get_ContentWidth( void )
			{
				return _group_main()->get_ContentWidth();
			};
			void UserControl::Invalidate( void )
			{
				return _group_main()->Invalidate();
			};
			void UserControl::AddControl( const XObject& control , ::rux::XString& error )
			{
				_group_main.AddControl( control , error );
			};
			void UserControl::AddControl( const XGCRef& control , ::rux::XString& error )
			{
				_group_main.AddControl( control , error );
			};
			void UserControl::InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error )
			{
				_group_main.InsertControl( index , control , error );
			};
			void UserControl::RemoveControl( XGCRef& control )
			{
				_group_main.RemoveControl( control );
			};
			void UserControl::RemoveControl( XObject& control )
			{
				_group_main.RemoveControl( control );
			};
			void UserControl::ClearControls( void )
			{
				_group_main.ClearControls();
			};
			::rux::uint8 UserControl::get_IsTransparent( void )
			{
				return _group_main()->get_IsTransparent();
			};
			float UserControl::get_ContentHeight( void )
			{
				return _group_main()->get_ContentHeight();
			};
			::rux::uint8 UserControl::get_IsSupportContentSize( void )
			{
				return _group_main()->get_IsSupportContentSize();
			};
			void UserControl::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{					
				_group_main()->raise_OnLeftMouseButtonUp( window_event , explicit_event );
			};
			rux::gui::Window* UserControl::get_ParentWindow( void ) const
			{				
				return _group_main.get_ParentWindow();
			}
			void UserControl::set_ControlName( const ::rux::XString& name )
			{
				_group_main.set_ControlName( name );
			};
			void UserControl::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_group_main()->raise_OnMouseMove( window_event , explicit_event );
			};
			void UserControl::UpdateParentWindow( void )
			{
				_group_main()->UpdateParentWindow();
			};
			void UserControl::FreeResources( ::rux::byte from_removed_controls_schedule )
			{
				_group_main()->FreeResources( from_removed_controls_schedule );
			};
			::rux::byte UserControl::IsFreeSystemResourcesThread( void )
			{
				return _group_main()->IsFreeSystemResourcesThread();
			};
			void UserControl::set_FreeSystemResourcesThreadId( void )
			{
				_group_main()->set_FreeSystemResourcesThreadId();
			};
			void UserControl::BeforeFreeSystemResources( void )
			{
				_group_main()->BeforeFreeSystemResources();
			};
			void UserControl::WaitForFreeSystemResources( void )
			{
				_group_main()->WaitForFreeSystemResources();
			};
			void UserControl::FreeSystemResources( void )
			{
				_group_main()->FreeSystemResources();
			};
			void UserControl::private_ResetCache( void )
			{
				_group_main()->private_ResetCache();
			};			
			void UserControl::render( ::rux::gui::RenderContextBase* , float , float& , size_t )
			{					
			};
			void UserControl::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnMouseWheel( window_event , explicit_event );
			};
			void UserControl::Activate( void )
			{
				_group_main.Activate();
			};
			::rux::byte UserControl::get_IsMouseEntered( void )
			{
				return _group_main()->get_IsMouseEntered();
			};
			void UserControl::set_IsMouseEntered( ::rux::byte val )
			{
				_group_main()->set_IsMouseEntered( val );
			};
			void UserControl::RemoveControl( void )
			{
				_group_main.RemoveControl();
			};
			void* UserControl::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_UserControl )
				{
					::rux::gui::controls::UserControl* ptr = const_cast< UserControl* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 UserControl::get_IsGroup( void )
			{
				return _group_main()->get_IsGroup();
			};
			::rux::uint8 UserControl::get_IsTimePicker( void )
			{
				return _group_main()->get_IsTimePicker();
			};
			::rux::uint8 UserControl::get_IsButton( void )
			{
				return _group_main()->get_IsButton();
			};
			::rux::uint8 UserControl::get_IsWindow( void )
			{
				return _group_main()->get_IsWindow();
			};
			void UserControl::raise_OnActivate( void )
			{		
				_group_main()->raise_OnActivate();
			};
			void UserControl::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{	
				_group_main()->raise_OnDeactivate( activated_control );
			};
			void UserControl::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_group_main()->raise_OnLeftMouseButtonDoubleClick( window_event , explicit_event );
			};
			::rux::byte UserControl::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				return _group_main()->raise_OnMouseEnter( window_event , raised_event , explicit_event );
			};
			void UserControl::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_group_main()->raise_OnMouseLeave( window_event , explicit_event );
			};
			::rux::uint8 UserControl::get_IsUserControl( void )
			{
				return 1;
			};
			::rux::uint32 UserControl::get_TypeIndex( void )
			{
				return _group_main()->get_TypeIndex();
			};
			rux::String& UserControl::get_ControlName( void )
			{
				return _group_main.get_ControlName();
			}
			rux::uint32 UserControl::get_ControlNameHash( void )
			{
				return _group_main.get_ControlNameHash();
			}
			void UserControl::set_TagPtr( void* tag_ptr )
			{
				_group_main.set_TagPtr( tag_ptr );
			}
			void* UserControl::get_TagPtr( void ) const
			{
			return _group_main.get_TagPtr();
			}
			void UserControl::GCRefRelease( const char* file , ::rux::int32 line )
			{
				_group_main()->GCRefRelease( file , line );
			}
			void UserControl::GCRefAddRef( const char* file , ::rux::int32 line )
			{
				_group_main()->GCRefAddRef( file , line );
			}
			void UserControl::private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent )
			{
				_group_main()->private_ResetLocationAndSizeCache( left , top , width , height , reset_parent );
			};
			::rux::uint8 UserControl::IsPointInControl( ::rux::int32  x , ::rux::int32 y )
			{
				return _group_main()->IsPointInControl( x , y );
			};
			void UserControl::set_OnMouseEnter( ::rux::gui::events::on_event_t on_mouse_enter_callback )
			{
				_group_main.set_OnMouseEnter( on_mouse_enter_callback );
			};
			void UserControl::dispatch_on_resized( void* param )
			{
			};
			void UserControl::set_OnResized( ::rux::gui::events::on_event_t on_resized_callback )
			{
				_group_main.set_OnResized( on_resized_callback );
			};
			void UserControl::set_OnRelocated( ::rux::gui::events::on_event_t on_resized_callback )
			{
				_group_main.set_OnRelocated( on_resized_callback );
			};
			void UserControl::set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )
			{
				_group_main.set_OnLeftMouseButtonDown( on_left_mouse_button_down );
			}
			void UserControl::set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down )
			{
				_group_main.set_OnRightMouseButtonDown( on_left_mouse_button_down );
			}
			::rux::int32 UserControl::get_TabIndex( void )
			{
				return _group_main.get_TabIndex();
			}
			void UserControl::set_TabIndex( ::rux::int32 tab_index )
			{
				_group_main.set_TabIndex( tab_index );
			}
			void UserControl::set_OnDeactivatedAll( ::rux::gui::events::on_deactivated_event_t on_activated )
			{
				_group_main()->set_OnDeactivatedAll( on_activated );
			}
			void UserControl::set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated )
			{
				_group_main.set_OnDeactivated( on_activated );
			}
			void UserControl::set_OnActivated( ::rux::gui::events::on_event_t on_activated )
			{
				_group_main.set_OnActivated( on_activated );
			}
			void UserControl::set_IsForwardEvents( ::rux::byte value )
			{
				_group_main.set_IsForwardEvents( value );
			};
			::rux::byte UserControl::get_IsForwardEvents( void )
			{
				return _group_main.get_IsForwardEvents();
			};
			void UserControl::set_OnMouseDoubleClick( ::rux::gui::events::on_mouse_event_t on_mouse_double_click )
			{
				_group_main.set_OnMouseDoubleClick( on_mouse_double_click );
			}
			void UserControl::set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )
			{
				_group_main.set_OnMouseMove( on_mouse_leave_callback );
			}
			void UserControl::set_OnMouseWheel( ::rux::gui::events::on_mouse_event_t on_mouse_wheel_callback )
			{
				_group_main.set_OnMouseWheel( on_mouse_wheel_callback );
			}
			void UserControl::set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up )
			{
				_group_main.set_OnLeftMouseButtonUp( on_left_mouse_button_up );
			};
			void UserControl::set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_right_mouse_button_up )
			{
				_group_main.set_OnRightMouseButtonUp( on_right_mouse_button_up );
			}
			void UserControl::set_OnMouseLeave( ::rux::gui::events::on_mouse_event_t on_mouse_leave_callback )
			{
				_group_main.set_OnMouseLeave( on_mouse_leave_callback );
			};
			void UserControl::ResetRightSticker( void )
			{
				_group_main.ResetRightSticker();
			};
			void UserControl::ResetBottomSticker( void )
			{
				_group_main.ResetBottomSticker();
			};
			void UserControl::set_RightSticker( float val )
			{
				_group_main.set_RightSticker( val );
			};
			void UserControl::set_BottomSticker( float val )
			{
				_group_main.set_BottomSticker( val );
			};
		};
	};
};