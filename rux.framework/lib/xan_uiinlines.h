#ifndef XAN_UIINLINES_H
#define XAN_UIINLINES_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_window.h>
#include <xan_uigroup.h>
#include <xan_uitimepicker.h>
#include <xan_uibutton.h>
namespace rux
{
	namespace gui
	{		
		namespace controls
		{
			rux_inline void rux_change_over_control( ::rux::gui::WindowMouseEvent* window_event , ::rux::gui::CommonBase* old_control , ::rux::gui::CommonBase* new_control , ::rux::byte lock )
			{
				if( old_control )
				{
					if( old_control->get_IsMouseEntered() )
					{
						old_control->set_IsMouseEntered( 0 );
						if( window_event )
							old_control->raise_OnMouseLeave( window_event , 1 );
						else
						{
							::rux::gui::WindowMouseEvent imitation( ::rux::gui::XEnum_EventType_WindowMouseMove , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
							old_control->raise_OnMouseLeave( &imitation , 1 );
						}
					}
					::rux::gui::ParentBase* parent = old_control->get_ParentControl( lock );
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl( lock );
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( new_control )
							{
								if( parent->Exists( new_control ) )
								{
									if( parents_index <= parents.Count() )
									{
										for( ; ; )
										{
											if( parents_index > 0 )
												parent->Release( __FILE__ , __LINE__ );
											if( parents_index < parents.Count() )
												parent = parents[ parents_index++ ];
											else
												break;
										}
									}
									break;
								}
							}
							if( parent->get_IsForwardEvents() == 0 )
							{
								if( parent->get_IsMouseEntered() )
								{
									parent->set_IsMouseEntered( 0 );
									if( window_event )
										parent->raise_OnMouseLeave( window_event , 0 );
									else
									{
										::rux::gui::WindowMouseEvent imitation( ::rux::gui::XEnum_EventType_WindowMouseMove , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
										parent->raise_OnMouseLeave( &imitation , 0 );
									}
								}
							}
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
					}
				}
				if( new_control )
				{
					if( new_control->get_IsGroup() )
					{
						::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)new_control->get_ObjectAddress();
						if( new_control->get_IsMouseEntered() == 0 )
						{
							new_control->set_IsMouseEntered( 1 );
							group->_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *group , 1 ) );
						}
					}
					else
					{
						if( new_control->get_IsMouseEntered() == 0 )
						{
							new_control->set_IsMouseEntered( 1 );
							::rux::byte raised_event = 1;
							if( window_event )
								new_control->raise_OnMouseEnter( window_event , raised_event , 1 );
							else
							{
								::rux::gui::WindowMouseEvent imitation( ::rux::gui::XEnum_EventType_WindowMouseMove , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
								new_control->raise_OnMouseEnter( &imitation , raised_event , 1 );
							}
						}						
					}
					::rux::gui::ParentBase* parent = new_control->get_ParentControl( lock );
					if( parent && parent->get_IsWindow() == 0 )
					{
						::rux::gui::ParentBase* new_parent = parent;
						XMallocArray< ::rux::gui::ParentBase* > parents;
						for( ; ; )
						{
							new_parent = new_parent->get_ParentControl( lock );
							if( new_parent && new_parent->get_IsWindow() == 0 )
							{
								new_parent->AddRef( __FILE__ , __LINE__ );
								parents.Add( new_parent );
							}
							else
								break;
						}
						size_t parents_index = 0;
						for( ; ; )
						{
							if( parent->get_IsForwardEvents() == 0 )
							{
								if( parent->get_IsGroup() )
								{
									::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)parent->get_ObjectAddress();
									if( parent->get_IsMouseEntered() == 0 )
									{
										parent->set_IsMouseEntered( 1 );
										group->_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *group , 0 ) );
									}
								}
								else
								{
									if( parent->get_IsMouseEntered() == 0 )
									{
										parent->set_IsMouseEntered( 1 );
										::rux::byte raised_event = 1;
										if( window_event )
											parent->raise_OnMouseEnter( window_event , raised_event , 0 );
										else
										{
											::rux::gui::WindowMouseEvent imitation( ::rux::gui::XEnum_EventType_WindowMouseMove , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
											parent->raise_OnMouseEnter( &imitation , raised_event , 0 );
										}
									}
								}
							}
							if( parents_index > 0 )
								parent->Release( __FILE__ , __LINE__ );
							if( parents_index < parents.Count() )
								parent = parents[ parents_index++ ];
							else
								break;
						}
					}
				}
			};
			template< class T >
			rux_inline void set_parent_control( T* _this , ::rux::gui::ParentBase* parent_control )
			{
				READ_LOCK( _this->_cs_parent_control );
				::rux::gui::ParentBase* parent_copy = _this->_parent_control;
				_this->_cs_parent_control.ReadUnlock();
				if( parent_copy )
					parent_copy->RemoveControl( _this );
				WRITE_LOCK( _this->_cs_parent_control );
				_this->_parent_control = parent_control;
				_this->_cs_parent_control.WriteUnlock();
				if( _this->_tab_index >= 0 )
				{
					rux::gui::Window* window = _this->get_ParentWindow();
					if( window )
						window->private_InsertTabControl( _this->_tab_index , _this );
				}
				else if( _this->get_IsGroup() == 1 )
					((::rux::gui::ParentBase*)_this)->AddTabControls();
				_this->Invalidate();
			};
			template< class T >
			rux_inline rux::gui::Window* get_parent_window( T* _this )
			{
				READ_LOCK( _this->_cs_parent_control );
				::rux::gui::ParentBase* parent_copy = _this->_parent_control;
				_this->_cs_parent_control.ReadUnlock();
				if( _this->_parent_window )
				{
					if( ::XInterlocked::CompareExchange( &const_cast< T* >( _this )->_free_system_resources_accomplished , 0 , 0 ) == 0 )
						return 0;
					else
						return _this->_parent_window;
				}
				else if( parent_copy )
					return parent_copy->get_ParentWindow();
				else
					return 0;
			};
			template< class T >
			rux_inline void before_free_system_resources( T* _this )
			{
				if( _this->get_IsGroup() == 1 )
				{
					rux::gui::controls::Group* group = ((rux::gui::controls::Group*)_this);
					rux::gui::Window* window = group->_parent_window;
					group->_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					for( size_t index0 = 0 ; index0 < group->_render_common_bases.Count() ; index0++ )
					{
						if( window )
						{
							::rux::gui::CommonBase* temp = group->_render_common_bases[ index0 ];
							WRITE_LOCK( window->_cs_active_control );
							if( window->_active_control == temp )
							{
								window->_active_control = NULL;
								temp->raise_OnDeactivate( 0 );
								temp->GCRefRelease( __FILE__ , __LINE__ );
							}
							window->_cs_active_control.WriteUnlock();
							WRITE_LOCK( window->_cs_over_control );
							if( window->_over_control == temp )
							{
								window->GCRefAddRef( __FILE__ , __LINE__ );
								window->_over_control = window;
								::rux::gui::controls::rux_change_over_control( 0 , temp , NULL , 0 );
								temp->GCRefRelease( __FILE__ , __LINE__ );
							}
							window->_cs_over_control.WriteUnlock();
							WRITE_LOCK( window->_cs_pressed_control );
							if( window->_pressed_control == temp )
							{
								window->_pressed_control = NULL;
								temp->GCRefRelease( __FILE__ , __LINE__ );
							}
							window->_cs_pressed_control.WriteUnlock();
						}
						group->_render_common_bases[ index0 ]->BeforeFreeSystemResources();
					}
					group->_cs_render_common_bases.ReadUnlock();
				}
				::XInterlocked::Exchange( &_this->_free_system_resources_accomplished , 0 );
			};
			template< class T >
			rux_inline void wait_for_free_system_resources( T* _this )
			{
				while( ::XInterlocked::CompareExchange( &_this->_free_system_resources_accomplished , 0 , 0 ) != 1 )
					::rux::threading::XThread::Sleep( 1 );
			};
			template< class T >
			rux_inline void free_system_resources( T* _this )
			{
				_this->_private_tag = 0;
				_this->_tag = 0;
				WRITE_LOCK( _this->_cs_parent_control );
				_this->_parent_control = NULL;
				_this->_cs_parent_control.WriteUnlock();
				_this->_parent_window = 0;
				_this->_cache_left_inited = 0;
				_this->_cache_top_inited = 0;
				_this->_cache_width_inited = 0;
				_this->_cache_height_inited = 0;
				_this->_cache_left = 0.f;
				_this->_cache_top = 0.f;
				_this->_cache_width = 0.f;
				_this->_cache_height = 0.f;
				_this->_free_system_resources_thread_id = 0;
				::XInterlocked::Exchange( &_this->_free_system_resources_accomplished , 1 );
			};
			template< class T >
			rux_inline void rux_try_reset_cache( T* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent )
			{	
				if( left )
					::XInterlocked::Exchange( &control->_cache_left_inited , 0 );
				if( top )
					::XInterlocked::Exchange( &control->_cache_top_inited , 0 );
				if( width )
					::XInterlocked::Exchange( &control->_cache_width_inited , 0 );
				if( height )
					::XInterlocked::Exchange( &control->_cache_height_inited , 0 );
				if( control->get_IsGroup() == 1 )
					((rux::gui::controls::Group*)control)->private_ResetChildLocationAndSizeCache( 1 , NULL , left , top , width , height );
				else if( control->get_IsButton() == 1 )
					((rux::gui::controls::Button*)control)->private_ResetChildLocationAndSizeCache( 1 , NULL , left , top , width , height );
				else if( control->get_IsTimePicker() == 1 )
					((rux::gui::controls::TimePicker*)control)->private_ResetChildLocationAndSizeCache( 1 , NULL , left , top , width , height );
				else
					control->Invalidate();
				if( reset_parent == 1 )
				{
					READ_LOCK( control->_cs_parent_control );
					::rux::gui::ParentBase* parent_copy = control->_parent_control;
					control->_cs_parent_control.ReadUnlock();
					if( parent_copy )
						parent_copy->private_ResetChildLocationAndSizeCache( 0 , control , left , top , width , height );
				}
			};			
			rux_inline void rux_try_mouse_deactivate( ::rux::gui::Window* window , ::rux::gui::CommonBase* control )
			{	
				WRITE_LOCK( window->_cs_active_control );
				if( window->_active_control )
				{
					rux::gui::CommonBase* active_control = window->_active_control;
					window->_active_control = NULL;
					window->_cs_active_control.WriteUnlock();
					active_control->raise_OnDeactivate( control );
					active_control->GCRefRelease( __FILE__ , __LINE__ );
				}
				else
					window->_cs_active_control.WriteUnlock();
			};
			rux_inline ::rux::uint8 rux_try_mouse_move( ::rux::gui::Window* window , ::rux::gui::ControlBase* control , ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& enter_raised )
			{
				control->GCRefAddRef( __FILE__ , __LINE__ );
				if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names && ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Count() )
				{
					rux::uint32 utf8_control_name_hash = control->get_ControlNameHash();
					for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Count() ; index0++ )
					{
						if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->operator[]( index0 ) == utf8_control_name_hash )
							break;
					}
				}
				::rux::uint8 result = 0;		
				if( control->get_IsVisible() == 1 
					&& control->get_IsForwardEvents() == 0
					&& control->get_IsTransparent() == 0 
					&& control->IsPointInControl( window_event->_x , window_event->_y ) == 1 )
				{
					WRITE_LOCK( window->_cs_over_control );
					if( window->_over_control != ((::rux::gui::CommonBase*)control) )
					{
						if( control->get_IsGroup() == 0 )
						{
							enter_raised = 1;
							rux::gui::CommonBase* over_control = window->_over_control;
							control->GCRefAddRef( __FILE__ , __LINE__ );
							window->_over_control = control;
							window->_cs_over_control.WriteUnlock();
							::rux::gui::controls::rux_change_over_control( window_event , over_control , control );
							over_control->GCRefRelease( __FILE__ , __LINE__ );
							window->set_Cursor( control->get_Cursor() );							
							result = 1;
						}
						else
						{
							window->_cs_over_control.WriteUnlock();
							result = 1;
							enter_raised = control->raise_OnMouseEnter( window_event , result , 1 );
						}
					}
					else if( control->get_IsGroup() == 1 )
					{			
						window->_cs_over_control.WriteUnlock();
						result = 1;
						enter_raised = control->raise_OnMouseEnter( window_event , result , 1 );
					}
					else
					{
						enter_raised = 0;
						window->_cs_over_control.WriteUnlock();
						control->raise_OnMouseMove( window_event , 1 );
						result = 1;
					}
				}	
				control->GCRefRelease( __FILE__ , __LINE__ );
				return result;
			};
			template< class T >
			rux_inline void rux_try_render_controls( T* parent , XMallocArray< ::rux::gui::ControlBase* >& render_objects , float opacity , ::rux::gui::RenderContextBase* render_context , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				::rux::gui::ControlBase* ui_common_base = NULL;	
				rux::gui::Rectangle* cliper = NULL;
				parent->get_Clip( cliper );
				parent->_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < render_objects.Count() ; index0++ )
				{		
					ui_common_base = render_objects[ index0 ];
					if( ui_common_base->get_IsVisible() )
					{
						ui_common_base->GCRefAddRef( __FILE__ , __LINE__ );
						if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names && ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Count() )
						{
							rux::uint32 utf8_control_name_hash = ui_common_base->get_ControlNameHash();
							for( size_t index0 = 0 ; index0 < ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->Count() ; index0++ )
							{
								if( ::rux::engine::_globals->_gui_globals->_debug_utf8_control_names->operator[]( index0 ) == utf8_control_name_hash )
									break;
							}
						}
						parent->_cs_render_common_bases.ReadUnlock();
						rux::gui::Rectangle common_base_clip( (::rux::int32)ui_common_base->get_Left() , (::rux::int32)ui_common_base->get_Top() , (::rux::int32)ui_common_base->get_Width() , (::rux::int32)ui_common_base->get_Height() );
						if( cliper->IsIntersects( common_base_clip ) )
						{
							render_context->set_Clip( *cliper );
							ui_common_base->render( render_context , opacity , _selected_z_index , ___rux__thread_index1986 );
							if( ui_common_base->get_IsGroup() == 0 )
								_selected_z_index += 1.f;
						}
						parent->_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						ui_common_base->GCRefRelease( __FILE__ , __LINE__ );
					}
				}
				parent->_cs_render_common_bases.ReadUnlock();
			};
			template< class T >
			rux_inline void rux_try_reset_cache( T* parent )
			{
				parent->_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < parent->_render_common_bases.Count() ; index0++ )
					parent->_render_common_bases[ index0 ]->private_ResetCache();
				parent->_cs_render_common_bases.ReadUnlock();
			};
			template< class T >
			rux_inline ::rux::uint8 rux_is_point_in_control( T* control , ::rux::int32 x , ::rux::int32 y )
			{
				if( ::XInterlocked::CompareExchange( &const_cast< T* >( control )->_free_system_resources_accomplished , 0 , 0 ) == 0 )
					return 0;
				rux::gui::Rectangle* cliper = NULL;
				::rux::gui::ParentBase* parent_control = control->get_ParentControl();
				if( parent_control )
					parent_control->get_Clip( cliper );
				return cliper && cliper->IsPointInside( x , y ) == 1 && (int32)control->get_Left() <= x
					&& (int32)( control->get_Left() + control->get_Width() ) >= x
					&& (int32)control->get_Top() <= y
					&& (int32)( control->get_Top() + control->get_Height() ) >= y ? 1 : 0;
			};
			template< class T >
			rux_inline float rux_try_get_left( T* control , ::rux::uint8 relative_to_parent = 0 , float* cache_left_ptr = NULL )
			{	
				rux_interlocked_return inited_res = 0;
				float val = 0.f;
				if( relative_to_parent == 1 || ( inited_res = ::XInterlocked::CompareExchange( &control->_cache_left_inited , 2 , 0 ) ) == 0 )
				{
					if( relative_to_parent == 0 )
						control->_cache_left_thread = ::rux::threading::XThread::get_CurrentThreadId();
					float* cache_left = cache_left_ptr , temp = 0.f;
					if( cache_left == NULL )
						cache_left = &temp;
					READ_LOCK( control->_cs_parent_control );
					::rux::gui::ParentBase* parent_copy = control->_parent_control;
					control->_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() )
						{
							::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)parent_copy;
							if( group->_child_location_type == XEnum_Child_Location_Type_Manual
								|| group->_child_location_type == XEnum_Child_Location_Type_VerticalStack )
							{
								if( control->_horizontal_alignment == XEnum_Alignment_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_left = control->_left + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_left = control->_left * group->get_Width() + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else
										*cache_left = 0;
								}
								else if( control->_horizontal_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_left = control->_left + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_left = control->_left * group->get_Width() + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else
										*cache_left = 0;
									*cache_left -= control->get_Width() / 2;
								}
								else if( control->_horizontal_alignment == XEnum_Alignment_LeftOrTop )
									*cache_left = ( relative_to_parent == 0 ? group->get_Left() : 0 );
								else if( control->_horizontal_alignment == XEnum_Alignment_Center )
									*cache_left = ( group->get_Width() - control->get_Width() ) / 2 + ( relative_to_parent == 0 ? group->get_Left() : 0 );
								else if( control->_horizontal_alignment == XEnum_Alignment_RightOrBottom )
									*cache_left = group->get_Width() - control->get_Width() + ( relative_to_parent == 0 ? group->get_Left() : 0 );
								else if( control->_horizontal_alignment == XEnum_Alignment_Stick )
									*cache_left = ( relative_to_parent == 0 ? group->get_Left() : 0 ) + control->_right_sticker - control->get_Width();
								else
									*cache_left = 0;
								/* margin begin */
								if( control->_horizontal_alignment != XEnum_Alignment_Center )
								{
									if( control->_horizontal_alignment == XEnum_Alignment_RightOrBottom )
										*cache_left -= control->_margin._right_offset;
									else
										*cache_left += control->_margin._left_offset;
								}
								/* margin end */
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_HorizontalStack )
							{
								if( control->_horizontal_alignment == XEnum_Alignment_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_left = control->_left + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_left = control->_left * group->get_Width() + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else
										*cache_left = 0;
								}
								else if( control->_horizontal_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_left = control->_left + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_left = control->_left * group->get_Width() + ( relative_to_parent == 0 ? group->get_Left() : 0 );
									else
										*cache_left = 0;
									*cache_left -= control->get_Width() / 2;
								}
								else if( control->_horizontal_alignment == XEnum_Alignment_LeftOrTop )
									*cache_left = ( relative_to_parent == 0 ? group->get_Left() : 0 ) + group->get_LeftStackOffset( control ) + control->_margin._left_offset;
								else if( control->_horizontal_alignment == XEnum_Alignment_Center )
									*cache_left = ( group->get_Width() - group->get_RightStackOffset( control ) - group->get_LeftStackOffset( control ) - control->get_Width() ) / 2 + ( relative_to_parent == 0 ? group->get_Left() : 0 ) + group->get_LeftStackOffset( control );
								else if( control->_horizontal_alignment == XEnum_Alignment_RightOrBottom )
									*cache_left = ( relative_to_parent == 0 ? group->get_Left() : 0 ) + group->get_Width() - control->get_Width() - group->get_RightStackOffset( control ) - control->_margin._right_offset - control->_margin._left_offset;
								else if( control->_horizontal_alignment == XEnum_Alignment_Stick )
									*cache_left = ( relative_to_parent == 0 ? group->get_Left() : 0 ) + control->_right_sticker - control->get_Width();
								else
									*cache_left = 0;
								/* margin begin */
								if( control->_horizontal_alignment != XEnum_Alignment_Center
									&& control->_horizontal_alignment != XEnum_Alignment_LeftOrTop 
									&& control->_horizontal_alignment != XEnum_Alignment_RightOrBottom )
									*cache_left += control->_margin._left_offset;
								/* margin end */
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_Wrap )
							{
								*cache_left = group->get_WrapLeftOffset( control ) + ( relative_to_parent == 0 ? group->get_Left() : 0 );
								/* margin begin */
								if( control->_horizontal_alignment != XEnum_Alignment_Center
									&& control->_horizontal_alignment != XEnum_Alignment_LeftOrTop 
									&& control->_horizontal_alignment != XEnum_Alignment_RightOrBottom )
									*cache_left += control->_margin._left_offset;
								/* margin end */
							}
						}
						else if( parent_copy->get_IsWindow() )
						{
							rux::gui::Window* window = (rux::gui::Window*)parent_copy;
							if( control->_horizontal_alignment == XEnum_Alignment_Manual )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									*cache_left = control->_left;
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									*cache_left = control->_left * window->get_Width();
								else
									*cache_left = 0;
							}
							else if( control->_horizontal_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									*cache_left = control->_left;
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									*cache_left = control->_left * window->get_Width();
								else
									*cache_left = 0;
								*cache_left -= control->get_Width() / 2;
							}
							else if( control->_horizontal_alignment == XEnum_Alignment_LeftOrTop )
								*cache_left = 0;
							else if( control->_horizontal_alignment == XEnum_Alignment_Center )
								*cache_left = ( window->get_Width() - control->get_Width() ) / 2;
							else if( control->_horizontal_alignment == XEnum_Alignment_RightOrBottom )
								*cache_left = window->get_Width() - control->get_Width();
							else if( control->_horizontal_alignment == XEnum_Alignment_Stick )
								*cache_left = control->_right_sticker - control->get_Width();
							else
								*cache_left = 0;
							/* margin begin */
							if( control->_horizontal_alignment != XEnum_Alignment_Center )
							{
								if( control->_horizontal_alignment == XEnum_Alignment_RightOrBottom )
									*cache_left -= control->_margin._right_offset;
								else
									*cache_left += control->_margin._left_offset;
							}
							/* margin end */
						}
						else if( parent_copy )
						{
							if( parent_copy->get_IsWindow() == 0 )
								*cache_left = control->_left + parent_copy->get_Left();
						}
					}
					if( relative_to_parent == 0 )
					{
						val = *cache_left;
						if( temp != control->_cache_left )
						{
							if( control->get_IsVisible() == 1 )
							{
								control->_cache_left = temp;
								::XInterlocked::Exchange( &control->_cache_left_inited , 1 );
								rux::gui::Window* window = control->get_ParentWindow();
								if( window )
								{
									if( XInterlocked::CompareExchange( &control->_already_dispatch_on_relocated , 1 , 0 ) == 0 )
									{
										control->AddRef();
										window->Dispatch( T::dispatch_on_relocated , control , 0 );
									}
								}
							}
						}
						else
						{
							if( control->get_IsVisible() == 1 )
								::XInterlocked::Exchange( &control->_cache_left_inited , 1 );
						}
					}
					*cache_left = ::rux::math::round( *cache_left , 0 );
					::XInterlocked::CompareExchange( &control->_cache_left_inited , 0 , 2 );
				}									
				else
				{
					if( inited_res == 2 && cache_left_ptr == 0 )
					{
						while( ::XInterlocked::CompareExchange( &control->_cache_left_inited , 0 , 0 ) == 2 )
						{
							if( control->_cache_left_thread == ::rux::threading::XThread::get_CurrentThreadId() )
								break;
							::rux::threading::XThread::Sleep( 1 );
						}
					}
					val = control->_cache_left;
				}
				READ_LOCK( control->_cs_parent_control );
				::rux::gui::ParentBase* parent_copy = control->_parent_control;
				control->_cs_parent_control.ReadUnlock();
				if( parent_copy )
					val -= parent_copy->get_ScrollXOffset();
				return val;
			};
			template< class T >
			rux_inline float rux_try_get_top( T* control , ::rux::uint8 relative_to_parent = 0 , float* cache_top_ptr = NULL )
			{	
				rux_interlocked_return inited_res = 0;
				float val = 0.f;
				if( relative_to_parent == 1 || ( inited_res = ::XInterlocked::CompareExchange( &control->_cache_top_inited , 2 , 0 ) ) == 0 )
				{
					if( relative_to_parent == 0 )
						control->_cache_top_thread = ::rux::threading::XThread::get_CurrentThreadId();
					float* cache_top = cache_top_ptr , temp = 0.f;
					if( cache_top == NULL )
						cache_top = &temp;
					READ_LOCK( control->_cs_parent_control );
					::rux::gui::ParentBase* parent_copy = control->_parent_control;
					control->_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() )
						{
							::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)parent_copy;
							if( group->_child_location_type == XEnum_Child_Location_Type_Manual
								|| group->_child_location_type == XEnum_Child_Location_Type_HorizontalStack )
							{
								if( control->_vertical_alignment == XEnum_Alignment_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_top = control->_top + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_top = control->_top * group->get_Height() + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else
										*cache_top = 0;
								}
								else if( control->_vertical_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_top = control->_top + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_top = control->_top * group->get_Height() + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else
										*cache_top = 0;
									*cache_top -= control->get_Height() / 2;
								}
								else if( control->_vertical_alignment == XEnum_Alignment_LeftOrTop )
									*cache_top = ( relative_to_parent == 0 ? group->get_Top() : 0 );
								else if( control->_vertical_alignment == XEnum_Alignment_Center )
									*cache_top = ( group->get_Height() - control->get_Height() ) / 2 + ( relative_to_parent == 0 ? group->get_Top() : 0 );
								else if( control->_vertical_alignment == XEnum_Alignment_RightOrBottom )
									*cache_top = group->get_Height() - control->get_Height() + ( relative_to_parent == 0 ? group->get_Top() : 0 );
								else if( control->_vertical_alignment == XEnum_Alignment_Stick )
									*cache_top = ( relative_to_parent == 0 ? group->get_Top() : 0 ) + control->_bottom_sticker - control->get_Height();
								else
									*cache_top = 0;
								/* margin begin */
								if( control->_vertical_alignment != XEnum_Alignment_Center )
								{
									if( control->_vertical_alignment == XEnum_Alignment_RightOrBottom )
										*cache_top -= control->_margin._bottom_offset;
									else
										*cache_top += control->_margin._top_offset;
								}
								/* margin end */
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_VerticalStack )
							{
								if( control->_vertical_alignment == XEnum_Alignment_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_top = control->_top + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_top = control->_top * group->get_Height() + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else
										*cache_top = 0;
								}
								else if( control->_vertical_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
										*cache_top = control->_top + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
										*cache_top = control->_top * group->get_Height() + ( relative_to_parent == 0 ? group->get_Top() : 0 );
									else
										*cache_top = 0;
									*cache_top -= control->get_Height() / 2;
								}
								else if( control->_vertical_alignment == XEnum_Alignment_LeftOrTop )
									*cache_top = ( relative_to_parent == 0 ? group->get_Top() : 0 ) + group->get_TopStackOffset( control ) + control->_margin._top_offset;
								else if( control->_vertical_alignment == XEnum_Alignment_Center )
									*cache_top = ( group->get_Height() - group->get_BottomStackOffset( control ) - group->get_TopStackOffset( control ) - control->get_Height() ) / 2 + ( relative_to_parent == 0 ? group->get_Top() : 0 ) + group->get_TopStackOffset( control );
								else if( control->_vertical_alignment == XEnum_Alignment_RightOrBottom )
									*cache_top = ( relative_to_parent == 0 ? group->get_Top() : 0 ) + group->get_Height() - control->get_Height() - group->get_BottomStackOffset( control ) - control->_margin._bottom_offset - control->_margin._top_offset;
								else if( control->_vertical_alignment == XEnum_Alignment_Stick )
									*cache_top = ( relative_to_parent == 0 ? group->get_Top() : 0 ) + control->_bottom_sticker - control->get_Height();
								else
									*cache_top = 0;
								/* margin begin */
								if( control->_vertical_alignment != XEnum_Alignment_Center
									&& control->_vertical_alignment != XEnum_Alignment_LeftOrTop 
									&& control->_vertical_alignment != XEnum_Alignment_RightOrBottom )
									*cache_top += control->_margin._top_offset;
								/* margin end */
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_Wrap )
							{
								*cache_top = group->get_WrapTopOffset( control ) + ( relative_to_parent == 0 ? group->get_Top() : 0 );
								/* margin begin */
								if( control->_vertical_alignment != XEnum_Alignment_Center
									&& control->_vertical_alignment != XEnum_Alignment_LeftOrTop 
									&& control->_vertical_alignment != XEnum_Alignment_RightOrBottom )
									*cache_top += control->_margin._top_offset;
								/* margin end */
							}
						}
						else if( parent_copy->get_IsWindow() )
						{
							rux::gui::Window* window = (rux::gui::Window*)parent_copy;
							if( control->_vertical_alignment == XEnum_Alignment_Manual )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									*cache_top = control->_top;
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									*cache_top = control->_top * window->get_Height();
								else
									*cache_top = 0;
							}
							else if( control->_vertical_alignment == XEnum_Alignment_ManualAndLocationIsControlCenter )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									*cache_top = control->_top;
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									*cache_top = control->_top * window->get_Height();
								else
									*cache_top = 0;
								*cache_top -= control->get_Height() / 2;
							}
							else if( control->_vertical_alignment == XEnum_Alignment_LeftOrTop )
								*cache_top = 0;
							else if( control->_vertical_alignment == XEnum_Alignment_Center )
								*cache_top = ( window->get_Height() - control->get_Height() ) / 2;
							else if( control->_vertical_alignment == XEnum_Alignment_RightOrBottom )
								*cache_top = window->get_Height() - control->get_Height();
							else if( control->_vertical_alignment == XEnum_Alignment_Stick )
								*cache_top = control->_bottom_sticker - control->get_Height();
							else
								*cache_top = 0;
							/* margin begin */
							if( control->_vertical_alignment != XEnum_Alignment_Center )
							{
								if( control->_vertical_alignment == XEnum_Alignment_RightOrBottom )
									*cache_top -= control->_margin._bottom_offset;
								else
									*cache_top += control->_margin._top_offset;
							}
							/* margin end */
						}
						else if( parent_copy )
						{
							if( parent_copy->get_IsWindow() == 0 )
								*cache_top = control->_top + parent_copy->get_Top();
						}
					}
					if( relative_to_parent == 0 )
					{
						val = *cache_top;
						if( temp != control->_cache_top )
						{
							if( control->get_IsVisible() == 1 )
							{
								control->_cache_top = temp;
								::XInterlocked::Exchange( &control->_cache_top_inited , 1 );
								rux::gui::Window* window = control->get_ParentWindow();
								if( window )
								{													
									if( XInterlocked::CompareExchange( &control->_already_dispatch_on_relocated , 1 , 0 ) == 0 )
									{
										control->AddRef();
										window->Dispatch( T::dispatch_on_relocated , control , 0 );
									}
								}
							}
						}
						else
						{
							if( control->get_IsVisible() == 1 )
								::XInterlocked::Exchange( &control->_cache_top_inited , 1 );
						}
					}
					*cache_top = ::rux::math::round( *cache_top , 0 );
					::XInterlocked::CompareExchange( &control->_cache_top_inited , 0 , 2 );
				}								
				else
				{
					if( inited_res == 2 && cache_top_ptr == 0 )
					{
						while( ::XInterlocked::CompareExchange( &control->_cache_top_inited , 0 , 0 ) == 2 )
						{
							if( control->_cache_top_thread == ::rux::threading::XThread::get_CurrentThreadId() )
								break;
							::rux::threading::XThread::Sleep( 1 );
						}
					}
					val = control->_cache_top;
				}
				READ_LOCK( control->_cs_parent_control );
				::rux::gui::ParentBase* parent_copy = control->_parent_control;
				control->_cs_parent_control.ReadUnlock();
				if( parent_copy )
					val -= parent_copy->get_ScrollYOffset();
				return val;
			};
			#define add_width_margin() if( *cache_width > control->_margin._right_offset + control->_margin._left_offset )\
					*cache_width -= control->_margin._right_offset + control->_margin._left_offset;\
				else\
					*cache_width = 0
			template< class T >
			rux_inline float rux_try_get_width( T* control , float* cache_width_ptr = NULL )
			{	
				rux_interlocked_return inited_res = 0;
				float val = 0.f;
				if( cache_width_ptr || ( inited_res = ::XInterlocked::CompareExchange( &control->_cache_width_inited , 2 , 0 ) ) == 0 )
				{
					if( cache_width_ptr == 0 )
						control->_cache_width_thread = ::rux::threading::XThread::get_CurrentThreadId();
					float* cache_width = cache_width_ptr;
					if( cache_width == NULL )
						cache_width = &control->_cache_width;
					if( control->get_IsSupportContentSize() == 0 
						&& control->_horizontal_filling == XEnum_Filling_FromContent )
						control->_horizontal_filling = XEnum_Filling_Manual;
					READ_LOCK( control->_cs_parent_control );
					::rux::gui::ParentBase* parent_copy = control->_parent_control;
					control->_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() )
						{
							::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)parent_copy;
							if( group->_child_location_type == XEnum_Child_Location_Type_Manual
								|| group->_child_location_type == XEnum_Child_Location_Type_VerticalStack
								|| group->_child_location_type == XEnum_Child_Location_Type_Wrap )
							{
								if( group->_child_location_type == XEnum_Child_Location_Type_Wrap
									|| control->_horizontal_filling == XEnum_Filling_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
									 	|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									{
										if( *cache_width != control->_width )
										{
											*cache_width = control->_width;
											if( group->_child_location_type != XEnum_Child_Location_Type_Wrap )
												add_width_margin();
											val = *cache_width;
											if( cache_width_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_width;
											if( cache_width_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										}
									}
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									{
										float width = control->_width * group->get_Width();
										if( *cache_width != width )
										{
											*cache_width = width;
											add_width_margin();
											val = *cache_width;
											if( cache_width_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_width;
											if( cache_width_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										}
									}
									else
									{
										*cache_width = 0;
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_FromParent )
								{
									float width = group->get_Width();
									if( *cache_width != width )
									{
										*cache_width = width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_Auto )
								{
									float width = 0.0;
									if( control->_horizontal_alignment != XEnum_Alignment_Center
										&& control->_horizontal_alignment != XEnum_Alignment_RightOrBottom
										&& control->_horizontal_alignment != XEnum_Alignment_Stick 
										&& control->_horizontal_alignment != XEnum_Alignment_LeftOrTop )
										width = group->get_Width() - ( control->get_Left() - group->get_Left() );
									else
										width = group->get_Width();
									if( *cache_width != width )
									{
										*cache_width = width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_FromContent )
								{
									if( *cache_width != control->get_ContentWidth() )
									{
										*cache_width = control->get_ContentWidth();
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else
								{
									*cache_width = 0;
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_HorizontalStack )
							{
								if( control->_horizontal_filling == XEnum_Filling_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									{
										if( *cache_width != control->_width )
										{
											*cache_width = control->_width;
											val = *cache_width;
											if( cache_width_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_width;
											if( cache_width_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										}
									}
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									{
										float width = control->_width * group->get_Width();
										if( *cache_width != width )
										{
											*cache_width = width;
											val = *cache_width;
											if( cache_width_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_width;
											if( cache_width_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										}
									}
									else
									{
										*cache_width = 0;
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_FromParent )
								{
									float width = group->get_Width();
									if( *cache_width != width )
									{
										*cache_width = width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_Auto )
								{
									float width = 0.0;
									if( control->_horizontal_alignment != XEnum_Alignment_Center
										&& control->_horizontal_alignment != XEnum_Alignment_RightOrBottom
										&& control->_horizontal_alignment != XEnum_Alignment_Stick 
										&& control->_horizontal_alignment != XEnum_Alignment_LeftOrTop )
										width = group->get_Width() - group->get_RightStackOffset( control ) - group->get_LeftStackOffset( control ) - ( control->get_Left() - group->get_Left() );
									else
										width = group->get_Width() - group->get_RightStackOffset( control ) - group->get_LeftStackOffset( control );
									if( *cache_width != width )
									{
										*cache_width = width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_horizontal_filling == XEnum_Filling_FromContent )
								{
									if( *cache_width != control->get_ContentWidth() )
									{
										*cache_width = control->get_ContentWidth();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else
								{
									*cache_width = 0;
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
						}
						else if( parent_copy->get_IsWindow() )
						{
							rux::gui::Window* window = (rux::gui::Window*)parent_copy;
							if( control->_horizontal_filling == XEnum_Filling_Manual )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
								{
									if( *cache_width != control->_width )
									{
										*cache_width = control->_width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
											{
												control->AddRef();
												window->Dispatch( T::dispatch_on_resized , control , 0 );
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
								{
									float width = control->_width * window->get_Width();
									if( *cache_width != width )
									{
										*cache_width = width;
										add_width_margin();
										val = *cache_width;
										if( cache_width_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
											if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
											{
												control->AddRef();
												window->Dispatch( T::dispatch_on_resized , control , 0 );
											}
										}
									}
									else
									{
										val = *cache_width;
										if( cache_width_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
									}
								}
								else
								{
									*cache_width = 0;
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
							else if( control->_horizontal_filling == XEnum_Filling_FromParent )
							{
								float width = (float)window->get_Width();
								if( *cache_width != width )
								{
									*cache_width = width;
									add_width_margin();
									val = *cache_width;
									if( cache_width_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
							else if( control->_horizontal_filling == XEnum_Filling_Auto )
							{
								float width = 0.0;
								if( control->_horizontal_alignment != XEnum_Alignment_Center
									&& control->_horizontal_alignment != XEnum_Alignment_RightOrBottom
									&& control->_horizontal_alignment != XEnum_Alignment_Stick 
									&& control->_horizontal_alignment != XEnum_Alignment_LeftOrTop )
									width = window->get_Width() - ( control->get_Left() );
								else
									width = (float)window->get_Width();
								if( *cache_width != width )
								{
									*cache_width = width;
									add_width_margin();
									val = *cache_width;
									if( cache_width_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
							else if( control->_horizontal_filling == XEnum_Filling_FromContent )
							{
								if( *cache_width != control->get_ContentWidth() )
								{
									*cache_width = control->get_ContentWidth();
									add_width_margin();
									val = *cache_width;
									if( cache_width_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_width;
									if( cache_width_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
								}
							}
							else
							{
								*cache_width = 0;
								val = *cache_width;
								if( cache_width_ptr == NULL && control->get_IsVisible() )
									::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
							}
						}
						else
						{
							*cache_width = control->_width;
							val = *cache_width;
							if( cache_width_ptr == NULL && control->get_IsVisible() )
								::XInterlocked::Exchange( &control->_cache_width_inited , 1 );
						}
					}
					else
					{
						if( control->_horizontal_filling == XEnum_Filling_Manual )
						{
							if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
								|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								if( *cache_width != control->_width )
								{
									*cache_width = control->_width;
									add_width_margin();
									val = *cache_width;
								}
								else
									val = *cache_width;
							}
						}
					}
					*cache_width = ::rux::math::round( *cache_width , 0 );
					::XInterlocked::CompareExchange( &control->_cache_width_inited , 0 , 2 );
				}
				else
				{
					if( inited_res == 2 && cache_width_ptr == 0 )
					{
						while( ::XInterlocked::CompareExchange( &control->_cache_width_inited , 0 , 0 ) == 2 )
						{
							if( control->_cache_width_thread == ::rux::threading::XThread::get_CurrentThreadId() )
								break;
							::rux::threading::XThread::Sleep( 1 );
						}
					}
					val = control->_cache_width;
				}
				return val;
			};
			#define add_height_margin() if( *cache_height > control->_margin._bottom_offset + control->_margin._top_offset )\
					*cache_height -= control->_margin._bottom_offset + control->_margin._top_offset;\
				else\
					*cache_height = 0
			template< class T >
			rux_inline float rux_try_get_height( T* control , float* cache_height_ptr = NULL )
			{	
				rux_interlocked_return inited_res = 0;
				float val = 0.f;
				if( cache_height_ptr || ( inited_res = ::XInterlocked::CompareExchange( &control->_cache_height_inited , 2 , 0 ) ) == 0 )
				{
					if( cache_height_ptr == 0 )
						control->_cache_height_thread = ::rux::threading::XThread::get_CurrentThreadId();
					float* cache_height = cache_height_ptr;
					if( cache_height == NULL )
						cache_height = &control->_cache_height;
					if( control->get_IsSupportContentSize() == 0 
						&& control->_vertical_filling == XEnum_Filling_FromContent )
						control->_vertical_filling = XEnum_Filling_Manual;
					READ_LOCK( control->_cs_parent_control );
					::rux::gui::ParentBase* parent_copy = control->_parent_control;
					control->_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() )
						{
							::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)parent_copy;
							if( group->_child_location_type == XEnum_Child_Location_Type_Manual
								|| group->_child_location_type == XEnum_Child_Location_Type_HorizontalStack
								|| group->_child_location_type == XEnum_Child_Location_Type_Wrap )
							{
								if( group->_child_location_type == XEnum_Child_Location_Type_Wrap
									|| control->_vertical_filling == XEnum_Filling_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									{
										if( *cache_height != control->_height )
										{
											*cache_height = control->_height;
											if( group->_child_location_type != XEnum_Child_Location_Type_Wrap )
												add_height_margin();
											val = *cache_height;
											if( cache_height_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_height;
											if( cache_height_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										}
									}
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									{
										float height = control->_height * group->get_Height();
										if( *cache_height != height )
										{
											*cache_height = height;
											add_height_margin();
											val = *cache_height;
											if( cache_height_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_height;
											if( cache_height_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										}
									}
									else
									{
										*cache_height = 0;
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_FromParent )
								{
									float height = group->get_Height();
									if( *cache_height != height )
									{
										*cache_height = height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_Auto )
								{
									float height = 0.0;
									if( control->_vertical_alignment != XEnum_Alignment_Center
										&& control->_vertical_alignment != XEnum_Alignment_RightOrBottom
										&& control->_vertical_alignment != XEnum_Alignment_Stick 
										&& control->_vertical_alignment != XEnum_Alignment_LeftOrTop )
										height = group->get_Height() - ( control->get_Top() - group->get_Top() );
									else
										height = group->get_Height();
									if( *cache_height != height )
									{
										*cache_height = height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_FromContent )
								{
									if( *cache_height != control->get_ContentHeight() )
									{
										*cache_height = control->get_ContentHeight();
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else
								{
									*cache_height = 0;
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
							else if( group->_child_location_type == XEnum_Child_Location_Type_VerticalStack )
							{
								if( control->_vertical_filling == XEnum_Filling_Manual )
								{
									if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
									{
										if( *cache_height != control->_height )
										{
											*cache_height = control->_height;
											val = *cache_height;
											if( cache_height_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_height;
											if( cache_height_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										}
									}
									else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
										|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
									{
										float height = control->_height * group->get_Height();
										if( *cache_height != height )
										{
											*cache_height = height;
											val = *cache_height;
											if( cache_height_ptr == NULL )
											{
												if( control->get_IsVisible() )
													::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
												rux::gui::Window* window = control->get_ParentWindow();
												if( window )
												{													
													if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
													{
														control->AddRef();
														window->Dispatch( T::dispatch_on_resized , control , 0 );
													}
												}
											}
										}
										else
										{
											val = *cache_height;
											if( cache_height_ptr == NULL && control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										}
									}
									else
									{
										*cache_height = 0;
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_FromParent )
								{
									float height = group->get_Height();
									if( *cache_height != height )
									{
										*cache_height = height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_Auto )
								{
									float height = 0.0;
									if( control->_vertical_alignment != XEnum_Alignment_Center
										&& control->_vertical_alignment != XEnum_Alignment_RightOrBottom
										&& control->_vertical_alignment != XEnum_Alignment_Stick 
										&& control->_vertical_alignment != XEnum_Alignment_LeftOrTop )
										height = group->get_Height() - group->get_BottomStackOffset( control ) - group->get_TopStackOffset( control ) - ( control->get_Top() - group->get_Top() );
									else
										height = group->get_Height() - group->get_BottomStackOffset( control ) - group->get_TopStackOffset( control );
									if( *cache_height != height )
									{
										*cache_height = height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_vertical_filling == XEnum_Filling_FromContent )
								{
									if( *cache_height != control->get_ContentHeight() )
									{
										*cache_height = control->get_ContentHeight();
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											rux::gui::Window* window = control->get_ParentWindow();
											if( window )
											{												
												if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
												{
													control->AddRef();
													window->Dispatch( T::dispatch_on_resized , control , 0 );
												}
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else
								{
									*cache_height = 0;
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
						}
						else if( parent_copy->get_IsWindow() )
						{
							rux::gui::Window* window = (rux::gui::Window*)parent_copy;
							if( control->_vertical_filling == XEnum_Filling_Manual )
							{
								if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel 
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
								{
									if( *cache_height != control->_height )
									{
										*cache_height = control->_height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
											{
												control->AddRef();
												window->Dispatch( T::dispatch_on_resized , control , 0 );
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Percent
									|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlySize )
								{
									float height = control->_height * window->get_Height();
									if( *cache_height != height )
									{
										*cache_height = height;
										add_height_margin();
										val = *cache_height;
										if( cache_height_ptr == NULL )
										{
											if( control->get_IsVisible() )
												::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
											if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
											{
												control->AddRef();
												window->Dispatch( T::dispatch_on_resized , control , 0 );
											}
										}
									}
									else
									{
										val = *cache_height;
										if( cache_height_ptr == NULL && control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
									}
								}
								else
								{
									*cache_height = 0;
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
							else if( control->_vertical_filling == XEnum_Filling_FromParent )
							{
								float height =(float)window->get_Height();
								if( *cache_height != height )
								{
									*cache_height = height;
									add_height_margin();
									val = *cache_height;
									if( cache_height_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
							else if( control->_vertical_filling == XEnum_Filling_Auto )
							{
								float height = 0.0;
								if( control->_vertical_alignment != XEnum_Alignment_Center
									&& control->_vertical_alignment != XEnum_Alignment_RightOrBottom
									&& control->_vertical_alignment != XEnum_Alignment_Stick 
									&& control->_vertical_alignment != XEnum_Alignment_LeftOrTop )
									height = window->get_Height() - ( control->get_Top() );
								else
									height = (float)window->get_Height();
								if( *cache_height != height )
								{
									*cache_height = height;
									add_height_margin();
									val = *cache_height;
									if( cache_height_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
							else if( control->_vertical_filling == XEnum_Filling_FromContent )
							{
								if( *cache_height != control->get_ContentHeight() )
								{
									*cache_height = control->get_ContentHeight();
									add_height_margin();
									val = *cache_height;
									if( cache_height_ptr == NULL )
									{
										if( control->get_IsVisible() )
											::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
										if( XInterlocked::CompareExchange( &control->_already_dispatch_on_resized , 1 , 0 ) == 0 )
										{
											control->AddRef();
											window->Dispatch( T::dispatch_on_resized , control , 0 );
										}
									}
								}
								else
								{
									val = *cache_height;
									if( cache_height_ptr == NULL && control->get_IsVisible() )
										::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
								}
							}
							else
							{
								*cache_height = 0;
								val = *cache_height;
								if( cache_height_ptr == NULL && control->get_IsVisible() )
									::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
							}
						}
						else
						{
							*cache_height = control->_height;
							val = *cache_height;
							if( cache_height_ptr == NULL && control->get_IsVisible() )
								::XInterlocked::Exchange( &control->_cache_height_inited , 1 );
						}
					}
					else
					{
						if( control->_vertical_filling == XEnum_Filling_Manual )
						{
							if( control->_point_unit == ::rux::gui::XEnum_PointUnit_Pixel
								|| control->_point_unit == ::rux::gui::XEnum_PointUnit_PercentOnlyLocation )
							{
								if( *cache_height != control->_height )
								{
									*cache_height = control->_height;
									add_height_margin();
									val = *cache_height;
								}
								else
									val = *cache_height;
							}
						}
					}
					*cache_height = ::rux::math::round( *cache_height , 0 );
					::XInterlocked::CompareExchange( &control->_cache_height_inited , 0 , 2 );
				}
				else
				{
					if( inited_res == 2 && cache_height_ptr == 0 )
					{
						while( ::XInterlocked::CompareExchange( &control->_cache_height_inited , 0 , 0 ) == 2 )
						{
							if( control->_cache_height_thread == ::rux::threading::XThread::get_CurrentThreadId() )
								break;
							::rux::threading::XThread::Sleep( 1 );
						}
					}
					val = control->_cache_height;
				}
				return val;
			};
		};
	};
};
#endif
