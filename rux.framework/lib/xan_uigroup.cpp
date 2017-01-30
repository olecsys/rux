#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uigroup.h>
#include <xan_window.h>
#include <xan_uiinlines.h>
#include <xan_uibutton.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uiapplication.h>
#include <xan_uitimepicker.h>
#include <xan_usercontrol.h>
#include <xan_task.h>
begin_implement_rux_class_with_properties_ns_base_class( Group , rux::gui::controls , rux::gui::ParentBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();
	_begin_multiple_add_or_remove_controls = 0;
	_removed_controls_schedule_index = SIZE_MAX;
	_removed_controls_schedule_idle_time = 0;
	strcpy( _utf8_control_name , "Group" );
	_utf8_control_name_hash = ::rux::cryptography::hash::times33_hash( _utf8_control_name , SIZE_MAX );
	_child_location_type = XEnum_Child_Location_Type_Manual;
	_scrollbar_x_offset = 0.f;
	_scrollbar_y_offset = 0.f;
	_is_user_control = 0;
	_current_render_index = SIZE_MAX;
	copy_color( _background , ::rux::gui::Colors::Transparent() );
	copy_color( _border_color , ::rux::gui::Colors::Transparent() );
	_border_width = 0;
	_is_forward_events_if_children_do_not_raised_event = 0;
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{			
			Group::~Group()
			{	
				FreeResources( 0 );
				if( _background )
					_background->Release();
				if( _border_color )
					_border_color->Release();
			};
			begin_implement_rux_base_ui_functions( Group )
				register_rux_property( ::rux::XFloat , ScrollXOffset);
				register_rux_property( ::rux::XFloat , ScrollYOffset );
			end_implement_rux_base_ui_functions();
			implement_rux_property_with_property_class( Group , ::rux::XFloat , ScrollXOffset );
			implement_rux_property_with_property_class( Group , ::rux::XFloat , ScrollYOffset );

			implement_duplicate_internal_function_1( Group , RemoveControl , XGCRef& );
			implement_duplicate_internal_function_1( Group , BringToFront , const XObject& );
			implement_duplicate_internal_function_2( Group , set_ZIndex , const XObject& , ::rux::uint32 );
			implement_duplicate_internal_function_1( Group , BringToFront , const XGCRef& );
			implement_duplicate_internal_function_2( Group , set_ZIndex , const XGCRef& , ::rux::uint32 );
			XGroup::XGroup( const ::rux::gui::ParentBase& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< Group >( variable_name , __file__ , __line__ )
			{
				XGCRef* gcref = const_cast< ::rux::gui::ParentBase* >( &uigroup );
				if( gcref )
				{
					gcref->AddRef();
					(*this) = *gcref;
					gcref->Release();
				}
			};
			XGroup::XGroup( const ::rux::gui::ParentBase& uigroup )
			{
				XGCRef* gcref = const_cast< ::rux::gui::ParentBase* >( &uigroup );
				if( gcref )
				{
					gcref->AddRef();
					(*this) = *gcref;
					gcref->Release();
				}
			};
			XGroup::XGroup( const Group& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< Group >( variable_name , __file__ , __line__ )
			{
				uigroup.AddRef();
				(*this) = uigroup;
				uigroup.Release();
			};
			XGroup::XGroup( const XGroup& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ )
				: XGCHandle< Group >( variable_name , __file__ , __line__ )
			{
				(*(static_cast<XGCHandle<Group>*>( this ))) = uigroup;
			};
			void XGroup::RemoveControl( XObject& control )
			{
				(*this)()->RemoveControl( control.get_GCRef() );
			};
			implement_duplicate_internal_result_function( ::rux::gui::ColorBase* , Group , get_Background );
			::rux::gui::ColorBase* Group::get_Background( void )
			{
				return _background;
			};
			implement_duplicate_internal_result_function( ::rux::gui::ColorBase* , Group , get_BorderColor );
			::rux::gui::ColorBase* Group::get_BorderColor( void )
			{
				return _border_color;
			};
			implement_duplicate_internal_function_1( Group , set_Background , ::rux::gui::ColorBase* );
			void Group::set_Background( ::rux::gui::ColorBase* background )
			{				
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _background , background );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_1( Group , set_BorderColor , ::rux::gui::ColorBase* );
			void Group::set_BorderColor( ::rux::gui::ColorBase* border_color )
			{
				_cs_drawing_elements.wlock( debuginfo_macros );
				rux::gui::copy_color( _border_color , border_color );
				_cs_drawing_elements.wunlock( debuginfo_macros );
			};
			implement_duplicate_internal_function_1( Group , set_BorderWidth , float );
			void Group::set_BorderWidth( float border_width )
			{
				if( _border_width != border_width )
					_border_width = ::rux::math::round( border_width , 0 );
			};
			implement_duplicate_internal_function_1( Group , set_Corner , const ::rux::gui::Thickness& );
			void Group::set_Corner( const ::rux::gui::Thickness& corner )
			{
				if( _corner != corner )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					_corner = corner;
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
			};
			float Group::get_ContentWidth( void )
			{
				float childs_width = _width;
				if( _horizontal_filling == XEnum_Filling_FromContent )
					childs_width = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					if( _render_common_bases[ index0 ]->get_IsVisible() == 1
						&& _render_common_bases[ index0 ]->get_HorizontalFilling() != XEnum_Filling_Auto
						&& _render_common_bases[ index0 ]->get_HorizontalFilling() != XEnum_Filling_FromParent )
					{
						float left = 0.f;
						_render_common_bases[ index0 ]->get_Left( 1 , &left );
						float width = 0.f;
						_render_common_bases[ index0 ]->get_Width( &width );
						rux::gui::Margin margin;
						margin = _render_common_bases[ index0 ]->get_Margin();
						if( left + width + margin._right_offset > childs_width )
							childs_width = left + width + margin._right_offset;
					}
				}
				_cs_render_common_bases.ReadUnlock();
				return childs_width;
			};
			float XGroup::get_ContentWidth( void )
			{
				return (*this)()->get_ContentWidth();
			};
			float XGroup::get_ContentHeight( void )
			{
				return (*this)()->get_ContentHeight();
			};
			float Group::get_LeftStackOffset( ::rux::gui::ControlBase* control )
			{
				rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float left_stack_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];
					if( base == control )
						break;
					if( base->get_IsVisible() == 1
						&& base->get_HorizontalAlignment() == XEnum_Alignment_LeftOrTop )
					{
						margin = base->get_Margin();
						left_stack_offset += margin._left_offset + margin._right_offset + base->get_Width();
					}
				}
				_cs_render_common_bases.ReadUnlock();	
				return left_stack_offset;
			};
			float Group::get_TopStackOffset( ::rux::gui::ControlBase* control )
			{
				rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float top_stack_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];
					if( base == control )
						break;
					if( base->get_IsVisible() == 1
						&& base->get_VerticalAlignment() == XEnum_Alignment_LeftOrTop )
					{
						margin = base->get_Margin();
						top_stack_offset += margin._top_offset + margin._bottom_offset + base->get_Height();
					}
				}
				_cs_render_common_bases.ReadUnlock();	
				return top_stack_offset;
			};
			implement_duplicate_internal_result_function( float , Group , get_ScrollXOffset );
			float Group::get_ScrollXOffset( void )
			{
				return _scrollbar_x_offset;
			};
			implement_duplicate_internal_result_function( float , Group , get_ScrollYOffset );
			float Group::get_ScrollYOffset( void )
			{
				return _scrollbar_y_offset;
			};
			implement_duplicate_internal_function_1( Group , set_ScrollXOffset , float );
			void Group::set_ScrollXOffset( float scroll_x_offset )
			{
				if( _scrollbar_x_offset != scroll_x_offset )
				{
					_scrollbar_x_offset = scroll_x_offset;
					_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
					{
						::rux::gui::ControlBase* base = _render_common_bases[ index0 ];
						base->GCRefAddRef( __FILE__ , __LINE__ );
						_cs_render_common_bases.ReadUnlock();
						if( base->get_IsVisible() == 1 )
							base->private_ResetLocationAndSizeCache( 1 , 0 , 0 , 0 , 0 );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						base->GCRefRelease( __FILE__ , __LINE__ );
					}
					_cs_render_common_bases.ReadUnlock();
				}
			};
			implement_duplicate_internal_function_1( Group , set_ScrollYOffset , float );
			void Group::set_ScrollYOffset( float scroll_y_offset )
			{
				if( _scrollbar_y_offset != scroll_y_offset )
				{
					_scrollbar_y_offset = scroll_y_offset;
					_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
					{
						::rux::gui::ControlBase* base = _render_common_bases[ index0 ];
						base->GCRefAddRef( __FILE__ , __LINE__ );
						_cs_render_common_bases.ReadUnlock();
						if( base->get_IsVisible() == 1 )
							base->private_ResetLocationAndSizeCache( 0 , 1 , 0 , 0 , 0 );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						base->GCRefRelease( __FILE__ , __LINE__ );
					}
					_cs_render_common_bases.ReadUnlock();
				}
			};
			float Group::get_WrapTopOffset( ::rux::gui::ControlBase* control )
			{
				float group_width = get_Width();
				::rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float wrap_left_offset = 0.f , max_top_offset = 0.f , wrap_top_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];					
					if( base->get_IsVisible() == 1 )
					{
						margin = base->get_Margin();
						float offset = margin._top_offset + margin._bottom_offset + base->get_Height();
						if( offset > max_top_offset )
							max_top_offset = offset;

						wrap_left_offset += margin._left_offset + margin._right_offset + base->get_Width();
						if( wrap_left_offset > group_width )
						{
							wrap_top_offset += max_top_offset;
							if( base == control )
								break;
							else
								wrap_left_offset = margin._left_offset + margin._right_offset + base->get_Width();
						}
						else if( base == control )
							break;
					}
					else if( base == control )
						break;
				}
				_cs_render_common_bases.ReadUnlock();	
				return wrap_top_offset;
			};
			implement_duplicate_internal_result_function_3( size_t , Group , CalculateWrapVisibleControlsCount , float , float , const ::rux::gui::Margin& );
			size_t Group::CalculateWrapVisibleControlsCount( float width , float height , const ::rux::gui::Margin& margin )
			{
				float group_width = get_Width();
				size_t row_count = group_width / ( width + margin._left_offset + margin._right_offset );
				float group_height = get_Height();
				size_t col_count = group_height / ( height + margin._top_offset + margin._bottom_offset );
				return row_count * col_count;
			};
			float Group::get_WrapLeftOffset( ::rux::gui::ControlBase* control )
			{
				float group_width = get_Width();
				::rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float wrap_left_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];
					if( base->get_IsVisible() == 1 )
					{
						margin = base->get_Margin();
						wrap_left_offset += margin._left_offset + margin._right_offset + base->get_Width();
						if( wrap_left_offset > group_width )
						{
							if( base == control )
							{
								wrap_left_offset = 0;
								break;
							}
							else
								wrap_left_offset = margin._left_offset + margin._right_offset + base->get_Width();
						}
						else if( base == control )
						{
							wrap_left_offset -= margin._left_offset + margin._right_offset + base->get_Width();
							break;
						}
					}
					else if( base == control )
						break;
				}
				_cs_render_common_bases.ReadUnlock();	
				return wrap_left_offset;
			};
			float Group::get_RightStackOffset( ::rux::gui::ControlBase* control )
			{
				rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float right_stack_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];
					if( base == control )
						break;
					if( base->get_IsVisible() == 1
						&& base->get_HorizontalAlignment() == XEnum_Alignment_RightOrBottom )
					{
						margin = base->get_Margin();
						right_stack_offset += margin._left_offset + margin._right_offset + base->get_Width();
					}
				}
				_cs_render_common_bases.ReadUnlock();	
				return right_stack_offset;
			};
			float Group::get_BottomStackOffset( ::rux::gui::ControlBase* control )
			{
				rux::gui::Margin margin;
				rux::gui::ControlBase* base = NULL;
				float bottom_stack_offset = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					base = _render_common_bases[ index0 ];
					if( base == control )
						break;
					if( base->get_IsVisible() == 1
						&& base->get_VerticalAlignment() == XEnum_Alignment_RightOrBottom )
					{
						margin = base->get_Margin();
						bottom_stack_offset += margin._top_offset + margin._bottom_offset + base->get_Height();
					}
				}
				_cs_render_common_bases.ReadUnlock();	
				return bottom_stack_offset;
			};
			float Group::get_ContentHeight( void )
			{
				float childs_height = _height;
				if( _vertical_filling == XEnum_Filling_FromContent )
					childs_height = 0.f;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					if( _render_common_bases[ index0 ]->get_IsVisible() == 1
						&& _render_common_bases[ index0 ]->get_VerticalFilling() != XEnum_Filling_Auto
						&& _render_common_bases[ index0 ]->get_VerticalFilling() != XEnum_Filling_FromParent )
					{
						float top = 0.f;
						_render_common_bases[ index0 ]->get_Top( 1 , &top );
						float height = 0.f;
						_render_common_bases[ index0 ]->get_Height( &height );
						rux::gui::Margin margin; 
						margin = _render_common_bases[ index0 ]->get_Margin();
						if( top + height + margin._bottom_offset > childs_height )
							childs_height = top + height + margin._bottom_offset;
					}
				}
				_cs_render_common_bases.ReadUnlock();	
				return childs_height;
			};
			::rux::uint8 Group::get_IsSupportContentSize( void )
			{
				return 1;
			};
			void Group::BringToFront( const XGCRef& control )
			{
				control.AddRef();
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				set_ZIndex( control , _render_common_bases.Count() );	
				_cs_render_common_bases.WriteUnlock();
				control.Release();
			};			
			void Group::BringToFront( const XObject& control )
			{
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				set_ZIndex( *control.get_GCRef( 1 ) , _render_common_bases.Count() );	
				_cs_render_common_bases.WriteUnlock();
			};
			void Group::set_ZIndex( const XObject& control , size_t index )
			{
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				set_ZIndex( *control.get_GCRef( 1 ) , index );	
				_cs_render_common_bases.WriteUnlock();
			};
			void Group::set_ZIndex( XGCRef* control , size_t index )
			{
				control->AddRef();
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( index > _render_common_bases.Count() )
					index = _render_common_bases.Count();
				::rux::uint8 is_exists = 0;
				size_t index0 = 0;
				::rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)control->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				for( index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					if( _render_common_bases[ index0 ] == common_base )
					{
						if( index0 != index )
							is_exists = 1;
						break;
					}
				}
				if( is_exists == 1 )
				{
					common_base->GCRefAddRef( __FILE__ , __LINE__ );
					_render_common_bases.Insert( index , common_base );		
					for( size_t index1 = 0 ; index1 < _render_common_bases.Count() ; index1++ )
					{
						if( index1 != index )
						{
							if( _render_common_bases[ index1 ] == common_base )
							{
								common_base->GCRefRelease( __FILE__ , __LINE__ );
								_render_common_bases.RemoveAt( index1 );
								break;
							}
						}
					}
					Invalidate();
				}
				_cs_render_common_bases.WriteUnlock();
				control->Release();
			};
			void Group::set_ZIndex( const XGCRef& control , size_t index )
			{
				set_ZIndex( const_cast< XGCRef* >( &control ) , index );
			};
			void Group::RemoveControl( XGCRef& control )
			{
				RemoveControl( &control );
			};
			void Group::RemoveControl( XGCRef* control )
			{
				size_t controls_start_index = 0;
				::rux::byte found = 0;
				rux::gui::Window* window = get_ParentWindow();
				::rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)control->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ; controls_start_index < _render_common_bases.Count() ; controls_start_index++ )
				{
					if( _render_common_bases[ controls_start_index ] == common_base )
					{
						_render_common_bases.RemoveAt( controls_start_index );
						found = 1;			
						break;
					}
				}
				_cs_render_common_bases.WriteUnlock();
				if( found )
				{
					if( window )
					{
						CRITICAL_SECTION_LOCK( window->_cs_tab_common_bases );	
						for( size_t index1 = 0 ; index1 < window->_tab_common_bases.Count() ; index1++ )
						{
							if( window->_tab_common_bases[ index1 ] == common_base )
							{
								if( common_base->get_IsGroup() == 1 )
									((Group*)common_base->get_ObjectAddress())->RemoveTabControls();
								window->_tab_common_bases.RemoveAt( index1 );
								break;
							}
						}
						CRITICAL_SECTION_UNLOCK( window->_cs_tab_common_bases );
						::rux::gui::CommonBase* temp = common_base;
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
							::rux::gui::controls::rux_change_over_control( 0 , temp , NULL );
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
					WRITE_LOCK( _cs_removed_controls );
					common_base->BeforeFreeSystemResources();
					_removed_controls.FillEmptyOrAdd( common_base );
					if( _removed_controls_schedule_index == SIZE_MAX )
						::rux::task::add( _removed_controls_schedule_index , removed_controls_schedule , this , 100U , XThreadMode_Repeat );
					_cs_removed_controls.WriteUnlock();
					if( _begin_multiple_add_or_remove_controls == 0 )
						AddOrInsertResetLocationAndSizeCache( controls_start_index );
				}
			};
			void Group::removed_controls_schedule( size_t controls_count , ::rux::byte force_remove_task )
			{
				size_t removed_controls_schedule_index = SIZE_MAX;
				if( force_remove_task )
				{
					WRITE_LOCK( _cs_removed_controls );
					removed_controls_schedule_index = _removed_controls_schedule_index;
					_removed_controls_schedule_index = SIZE_MAX;
					_removed_controls_schedule_idle_time = 0;
					_cs_removed_controls.WriteUnlock();
					if( removed_controls_schedule_index != SIZE_MAX )
						::rux::task::remove( removed_controls_schedule_index );
				}
				size_t controls_count_copy = controls_count;
				WRITE_LOCK( _cs_removed_controls );
				for( size_t index0 = 0 ; index0 < _removed_controls.Count() ; index0++ )
				{
					::rux::gui::ControlBase* common_base = _removed_controls[ index0 ];
					if( common_base )
					{
						common_base->set_FreeSystemResourcesThreadId();
						controls_count_copy--;
					}
					if( controls_count_copy == 0 )
						break;
				}
				size_t removed_controls_index = 0;
				while( _removed_controls.ItemsCount() > 0 )
				{
					if( removed_controls_index >= _removed_controls.Count() )
						removed_controls_index = 0;
					::rux::gui::ControlBase* common_base = _removed_controls[ removed_controls_index ];
					if( common_base )
					{
						_removed_controls.set_EmptyAt( removed_controls_index );
						_cs_removed_controls.WriteUnlock();
						common_base->FreeResources( 0 );
						common_base->FreeSystemResources();
						common_base->GCRefRelease( __FILE__ , __LINE__ );
						controls_count--;
						WRITE_LOCK( _cs_removed_controls );
					}
					if( controls_count == 0 )
						break;
					removed_controls_index++;
				}
				if( _removed_controls.ItemsCount() == 0 )
				{
					::rux::uint32 now = ::rux::XTime::GetTickCount();
					if( force_remove_task == 0 )
					{
						if( _removed_controls_schedule_idle_time > now )
							_removed_controls_schedule_idle_time = now;
					}
					if( force_remove_task || ( _removed_controls_schedule_idle_time != 0 && now - _removed_controls_schedule_idle_time >= 30000 ) )
					{
						removed_controls_schedule_index = _removed_controls_schedule_index;
						_removed_controls_schedule_index = SIZE_MAX;
						_removed_controls_schedule_idle_time = 0;
					}
					else if( _removed_controls_schedule_idle_time == 0 )
						_removed_controls_schedule_idle_time = now;
				}
				_cs_removed_controls.WriteUnlock();
				if( removed_controls_schedule_index != SIZE_MAX )
					::rux::task::remove( removed_controls_schedule_index );
			};
			void Group::removed_controls_schedule( void* param , size_t ___rux__thread_index1986 )
			{
				UNREFERENCED_PARAMETER( ___rux__thread_index1986 );
				::rux::gui::controls::Group* group = (::rux::gui::controls::Group*)param;
				group->removed_controls_schedule( 15 , 0 );
			};
			::rux::uint32 XGroup::get_ControlsCount( void )
			{	
				return (*this)()->get_ControlsCount();
			};
			size_t Group::get_ControlsCount( void )
			{		
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				size_t count = _render_common_bases.Count();
				_cs_render_common_bases.ReadUnlock();
				return count;
			};
			::rux::int32 XGroup::IndexOf( const XObject& control )
			{
				return (*this)()->IndexOf( control );
			};
			::rux::int32 Group::IndexOf( const XObject& control )
			{
				return IndexOf( control.get_GCRef( 1 ) );
			};
			void Group::set_OnDeactivatedAll( ::rux::gui::events::on_deactivated_event_t on_activated )
			{
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					::rux::gui::CommonBase* common_base = _render_common_bases[ index0 ];
					if( common_base->get_IsGroup() == 1 )
						((Group*)common_base->get_ObjectAddress())->set_OnDeactivatedAll( on_activated );
					else
						common_base->set_OnDeactivated( on_activated );
				}
				_cs_render_common_bases.ReadUnlock();
			};
			::rux::int32 Group::IndexOf( XGCRef* control )
			{
				::rux::int32 index = -1;
				::rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)control->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				if( common_base == 0 )
				{
					::rux::gui::controls::UserControl* user_control = (::rux::gui::controls::UserControl*)control->DynamicCast( XEnum_ClassType_UserControl );
					if( user_control )
						common_base = user_control->_group_main();
				}
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					if( _render_common_bases[ index0 ] == common_base )
					{
						index = (int32)index0;
						break;
					}
				}
				_cs_render_common_bases.ReadUnlock();
				return index;
			};
			Object& XGroup::get_ControlAt( ::rux::uint32 index )
			{
				return (*this)()->get_ControlAt( index );
			};
			Object& Group::get_ControlAt( ::rux::uint32 index )
			{
				declare_variable( XObject , control );
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( _render_common_bases.Count() > index )
					control.set_ByRef( _render_common_bases[ index ] );
				_cs_render_common_bases.ReadUnlock();
				return control++;
			};
			void XGroup::AddControl( const XObject& control , ::rux::XString& error )
			{
				(*this)()->AddControl( control , error );
			};
			::rux::uint8 Group::ExistsControl( const XObject& control )
			{
				return ExistsControl( *control.get_GCRef() );
			};
			::rux::uint8 XGroup::ExistsControl( const XGCRef& control )
			{
				return (*this)()->ExistsControl( control );
			};
			::rux::uint8 Group::ExistsControl( const XGCRef& control )
			{
				::rux::uint8 exists = 0;
				::rux::gui::ControlBase* common_base = 0;
				::rux::gui::controls::UserControl* user_control = (::rux::gui::controls::UserControl*)control.DynamicCast( XEnum_ClassType_UserControl );
				if( user_control )
					common_base = user_control->_group_main();
				else
					common_base = (::rux::gui::ControlBase*)control.DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					if( common_base == _render_common_bases[ index0 ] )
					{
						exists = 1;
						break;
					}
				}
				_cs_render_common_bases.ReadUnlock();
				return exists;
			};
			::rux::uint8 XGroup::ExistsControl( const XObject& control )
			{	
				return (*this)()->ExistsControl( control );
			};
			void Group::AddControl( const XObject& control , ::rux::XString& error )
			{
				AddControl( *control.get_GCRef() , error );
			};
			void Group::AddTabControls( void )
			{
				rux::gui::Window* window = get_ParentWindow();
				::rux::int32 tab_index = -1;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
				{
					tab_index = _render_common_bases[ index0 ]->get_TabIndex();
					if( _render_common_bases[ index0 ]->get_IsGroup() == 1 )
						((Group*)_render_common_bases[ index0 ]->get_ObjectAddress())->AddTabControls();
					if( window && tab_index >= 0 )
						window->private_InsertTabControl( tab_index , _render_common_bases[ index0 ] );
				}
				_cs_render_common_bases.ReadUnlock();
			};
			void XGroup::AddControl( const XGCRef& control , ::rux::XString& error )
			{
				(*this)()->AddControl( control , error );
			};
			void XGroup::InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error )
			{
				(*this)()->InsertControl( index , control , error );
			};
			implement_duplicate_internal_function( Group , BeginMultipleAddOrRemoveControls );
			void Group::BeginMultipleAddOrRemoveControls( void )
			{
				_begin_multiple_add_or_remove_controls = 1;
			};
			implement_duplicate_internal_function( Group , EndMultipleAddOrRemoveControls );
			void Group::EndMultipleAddOrRemoveControls( void )
			{
				_begin_multiple_add_or_remove_controls = 0;
				AddOrInsertResetLocationAndSizeCache( 0 );
			};
			void Group::AddOrInsertResetLocationAndSizeCache( size_t controls_start_index )
			{
				if( get_ParentControl() )
				{
					if( _horizontal_filling == XEnum_Filling_FromContent || _vertical_filling == XEnum_Filling_FromContent )
						private_ResetLocationAndSizeCache( 0 , 0 , _horizontal_filling == XEnum_Filling_FromContent ? 1 : 0 , _vertical_filling == XEnum_Filling_FromContent ? 1 : 0 );
					else
						private_ResetChildLocationAndSizeCache( 1 , 0 , 0 , 0 , 0 , 0 , controls_start_index );
				}
			};
			void Group::InsertControl( size_t index , XObject& control , ::rux::XString& error )
			{
				WaitForFreeSystemResources();
				error.Clear();
				::rux::gui::ControlBase* common_base = NULL;
				::rux::gui::controls::UserControl* user_control = (::rux::gui::controls::UserControl*)control.get_GCRef()->DynamicCast( XEnum_ClassType_UserControl );
				if( user_control )
				{
					user_control->InitializeControl();
					common_base = user_control->_group_main(); 
					common_base->private_set_Tag( control );
				}
				if( common_base || ( common_base = (::rux::gui::ControlBase*)control.get_GCRef()->DynamicCast( XEnum_ClassType_ControlBase ) ) != 0 )
				{
					common_base->WaitForFreeSystemResources();
					common_base->set_ParentControl( this );
					common_base->GCRefAddRef( __FILE__ , __LINE__ );
					_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					if( index > _render_common_bases.Count() )
						index = _render_common_bases.Count();
					_render_common_bases.Insert( index , common_base );
					common_base->UpdateParentWindow();
					_cs_render_common_bases.WriteUnlock();
					if( _begin_multiple_add_or_remove_controls == 0 )
						AddOrInsertResetLocationAndSizeCache( index );
				}
				else
				{
					rux_write_error( "control is not ::rux::gui::ControlBase class" , error );
				}
			};
			void Group::AddControl( const XGCRef& control , ::rux::XString& error )
			{
				control.AddRef();
				WaitForFreeSystemResources();
				error.Clear();
				::rux::gui::ControlBase* common_base = NULL;
				::rux::gui::controls::UserControl* user_control = (::rux::gui::controls::UserControl*)control.DynamicCast( XEnum_ClassType_UserControl );
				if( user_control )
				{
					user_control->InitializeControl();
					common_base = user_control->_group_main(); 
					common_base->private_set_Tag( XObject( control ) );
				}
				if( common_base || ( common_base = (::rux::gui::ControlBase*)control.DynamicCast( XEnum_ClassType_ControlBase ) ) != 0 )
				{
					common_base->WaitForFreeSystemResources();
					common_base->set_ParentControl( this );
					common_base->GCRefAddRef( __FILE__ , __LINE__ );
					_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					size_t controls_start_index = _render_common_bases.Add( common_base );
					common_base->UpdateParentWindow();
					_cs_render_common_bases.WriteUnlock();
					if( _begin_multiple_add_or_remove_controls == 0 )
						AddOrInsertResetLocationAndSizeCache( controls_start_index );
				}
				else
				{
					rux_write_error( "control is not ::rux::gui::ControlBase class" , error );
				}
				control.Release();
			};
			void Group::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{
				::rux::gui::Rectangle* clip = NULL;
				get_Clip( clip );
				render_context->set_Clip( *clip );

				float __left = get_Left();
				float __top = get_Top();
				float __width = get_Width();
				float __height = get_Height();

				_cs_drawing_elements.wlock( debuginfo_macros );
				if( ( _background && _background->get_IsVisible() ) || ( _border_width > 0 && _border_color && _border_color->get_IsVisible() ) )
				{
					render_context->DrawRectangle( __left , __top , __width , __height , _border_width , _background , _border_color , _corner , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 0 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
					_selected_z_index += 1.f;
				}
				_cs_drawing_elements.wunlock( debuginfo_macros );
				rux_try_render_controls< Group >( this , _render_common_bases , opacity * _opacity , render_context , _selected_z_index , ___rux__thread_index1986 );
			};
			void* Group::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< Group* >( this );
					return ptr;
				}
				else if( class_type == ::rux::gui::XEnum_ClassType_ParentBase )
				{
					::rux::gui::ParentBase* ptr = const_cast< Group* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 Group::get_IsGroup( void )
			{
				return 1;
			};
			::rux::uint8 Group::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 Group::get_IsButton( void )
			{
				return 0;
			};
			void Group::Invalidate( void )
			{
				private_ResetCache();
			};
			void Group::private_ResetCache( void )
			{
				rux_try_reset_cache< Group >( this );
			};
			void XGroup::get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar )
			{
				(*this)()->get_Clip( clip , scrollbar );
			};
			void Group::get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar )
			{
				::rux::gui::Window* window = get_ParentWindow();
				if( window && _current_render_index != window->_render_index )
				{					
					float left = get_Left();
					float top = get_Top();
					float width = get_Width();
					float height = get_Height();
					READ_LOCK( _cs_parent_control );
					::rux::gui::ParentBase* parent_copy = _parent_control;
					_cs_parent_control.ReadUnlock();
					if( parent_copy )
					{
						if( parent_copy->get_IsGroup() || parent_copy->get_IsButton() || parent_copy->get_IsTimePicker() )
						{
							::rux::gui::Rectangle* clip_ptr = &_clip;
							parent_copy->get_Clip( clip_ptr );
						}
						else
							_clip = ::rux::gui::Rectangle( left , top , width , height );
					}
					if( left < _clip._left )
					{
						width -= _clip._left - left;
						left = _clip._left;
					}
					if( top < _clip._top )
					{
						height -= _clip._top - top;
						top = _clip._top;
					}	
					if( _clip._left + _clip._width < left + width )
						width = _clip._left + _clip._width - left;	
					if( _clip._top + _clip._height < top + height )
						height = _clip._top + _clip._height - top;
					_clip._left = left , _clip._top = top , _clip._width = width , _clip._height = height;
					_current_render_index = window->_render_index;
				}
				if( clip )
					*clip = _clip;
				else
					clip = &_clip;
			};
			void Group::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_wheel.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			::rux::byte Group::Exists( ::rux::gui::CommonBase* control )
			{
				::rux::byte exists = 0;
				if( control == ((::rux::gui::CommonBase*)this) )
					exists = 1;
				else
				{
					_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					if( _render_common_bases.Count() > 0 )
					{
						for( size_t index0 = _render_common_bases.Count() - 1 ; ; index0-- )
						{
							::rux::gui::ControlBase* temp_control = _render_common_bases[ index0 ];
							if( temp_control == control )
							{
								exists = 1;
								break;
							}
							::rux::gui::ParentBase* parent = (::rux::gui::ParentBase*)temp_control->DynamicCast( ::rux::gui::XEnum_ClassType_ParentBase );
							if( parent )
							{
								exists = parent->Exists( control );
								if( exists )
									break;
							}
							if( index0 == 0 )
								break;
						}
					}
					_cs_render_common_bases.ReadUnlock();
				}
				return exists;
			};
			void Group::raise_OnActivate( void )
			{		
			};
			void Group::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{		
				XObject activated;
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1 ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			implement_duplicate_internal_result_function( ::rux::byte , Group , get_IsForwardEventsIfChildrenDoNotRaisedEvent );
			::rux::byte Group::get_IsForwardEventsIfChildrenDoNotRaisedEvent( void )
			{
				return _is_forward_events_if_children_do_not_raised_event;
			};
			implement_duplicate_internal_function_1( Group , set_IsForwardEventsIfChildrenDoNotRaisedEvent , ::rux::byte );
			void Group::set_IsForwardEventsIfChildrenDoNotRaisedEvent( ::rux::byte is_forward_events_if_children_do_not_raised_event )
			{
				_is_forward_events_if_children_do_not_raised_event = is_forward_events_if_children_do_not_raised_event;
			};
			::rux::byte Group::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{	
				::rux::byte enter_raised = 0;
				_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( _render_common_bases.Count() > 0 )
				{		
					::rux::gui::Window* window = get_ParentWindow();
					if( window )
					{
						::rux::gui::CommonBase* common_base = this;
						for(size_t index0 = _render_common_bases.Count() - 1;;--index0)
						{
							::rux::gui::ControlBase* temp_control = _render_common_bases[index0];
							::rux::uint8 temp_res = 0;
							if(temp_control->get_IsVisible())
							{
								temp_control->GCRefAddRef( __FILE__ , __LINE__ );
								_cs_render_common_bases.ReadUnlock();							
								temp_res = rux_try_mouse_move(window, temp_control, window_event, enter_raised);
								temp_control->GCRefRelease(__FILE__, __LINE__);
								_cs_render_common_bases.ReadLock(__FILE__, __FUNCTION__, __LINE__, 0);
							}
							if(index0 == 0 || temp_res == 1)
							{	
								if( _is_forward_events_if_children_do_not_raised_event == 0 )
								{
									WRITE_LOCK( window->_cs_over_control );
									if( temp_res == 0 && window->_over_control != ((::rux::gui::CommonBase*)common_base) )
									{
										if( window->_over_control != NULL )
										{
											::rux::gui::CommonBase* over_control = window->_over_control;
											common_base->GCRefAddRef( __FILE__ , __LINE__ );
											window->_over_control = common_base;
											window->_cs_over_control.WriteUnlock();
											_cs_render_common_bases.ReadUnlock();
											::rux::gui::controls::rux_change_over_control( window_event , over_control , common_base );
											over_control->GCRefRelease( __FILE__ , __LINE__ );
											_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
										}
										else
										{
											common_base->GCRefAddRef( __FILE__ , __LINE__ );
											window->_over_control = common_base;
											window->_cs_over_control.WriteUnlock();
											::rux::gui::controls::rux_change_over_control( window_event , NULL , common_base );
										}
										window->set_Cursor( common_base->get_Cursor() );
									}
									else
										window->_cs_over_control.WriteUnlock();		
								}
								else if( temp_res == 0 )
									raised_event = 0;
								if( _is_forward_events_if_children_do_not_raised_event == 0 || temp_res )
								{
									if( enter_raised == 0 )
									{
										_cs_render_common_bases.ReadUnlock();
										raise_OnMouseMove( window_event , temp_res == 1 ? 0 : 1 );
										_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
									}
								}
								break;
							}
						}
					}
				}
				else
				{
					::rux::gui::Window* window = get_ParentWindow();
					if( window )
					{
						if( _is_forward_events_if_children_do_not_raised_event == 0 )
						{
							::rux::gui::CommonBase* common_base = this;
							WRITE_LOCK( window->_cs_over_control );
							if( window->_over_control != ((::rux::gui::CommonBase*)common_base) )
							{
								if( window->_over_control != NULL )
								{
									::rux::gui::CommonBase* over_control = window->_over_control;
									common_base->GCRefAddRef( __FILE__ , __LINE__ );
									window->_over_control = common_base;
									window->_cs_over_control.WriteUnlock();
									_cs_render_common_bases.ReadUnlock();
									::rux::gui::controls::rux_change_over_control( window_event , over_control , common_base );
									over_control->GCRefRelease( __FILE__ , __LINE__ );
									_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
								}
								else
								{
									common_base->GCRefAddRef( __FILE__ , __LINE__ );
									window->_over_control = common_base;
									window->_cs_over_control.WriteUnlock();
									::rux::gui::controls::rux_change_over_control( window_event , NULL , common_base );
								}
								window->set_Cursor( common_base->get_Cursor() );
							}
							else
								window->_cs_over_control.WriteUnlock();		
						}
						else
							raised_event = 0;
						if( _is_forward_events_if_children_do_not_raised_event == 0 )
						{
							if( enter_raised == 0 )
							{
								_cs_render_common_bases.ReadUnlock();
								raise_OnMouseMove( window_event , 1 );
								_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
							}
						}
					}
				}
				_cs_render_common_bases.ReadUnlock();
				return enter_raised;
			};
			void Group::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{
			};
			void Group::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void Group::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Group::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Group::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void Group::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::UpdateParentWindow( void )
			{
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				_parent_window = get_ParentWindow();
				if( _render_common_bases.Count() )
				{
					for( size_t resource_index = 0 ; resource_index < _render_common_bases.Count() ; resource_index++ )
						_render_common_bases[ resource_index ]->UpdateParentWindow();
				}
				_cs_render_common_bases.WriteUnlock();
			};
			void Group::FreeResources( ::rux::byte from_removed_controls_schedule )
			{
				if( from_removed_controls_schedule == 0 )
					removed_controls_schedule( SIZE_MAX , 1 );				
				XMallocArray< ::rux::gui::ControlBase* > to_delete;
				rux::gui::Window* window = _parent_window;
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				if( _render_common_bases.Count() )
				{
					for( size_t resource_index = 0 ; resource_index < _render_common_bases.Count() ; resource_index++ )
						to_delete.Add( _render_common_bases[ resource_index ] );
				}
				_render_common_bases.Clear();
				_cs_render_common_bases.WriteUnlock();
				for( size_t resource_index = 0 ; resource_index < to_delete.Count() ; resource_index++ )
				{
					::rux::gui::ControlBase* common_base = to_delete[ resource_index ];
					if( window )
					{
						::rux::gui::CommonBase* temp = common_base;
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
					common_base->BeforeFreeSystemResources();
					common_base->FreeResources( from_removed_controls_schedule );
					common_base->FreeSystemResources();
					common_base->GCRefRelease( __FILE__ , __LINE__ );
				}
				_current_render_index = SIZE_MAX;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
			};
			::rux::uint8 Group::get_IsTransparent( void )
			{
				::rux::uint8 is_transparent = _opacity > 0 ? 0 : 1;
				if( is_transparent == 0 )
				{
					_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
					{
						is_transparent = _render_common_bases[ index0 ]->get_IsVisible() == 1 && _render_common_bases[ index0 ]->get_IsTransparent() == 0 ? 0 : 1;
						if( is_transparent == 0 )
							break;
					}
					_cs_render_common_bases.ReadUnlock();
				}	
				if( is_transparent )
				{
					_cs_drawing_elements.wlock( debuginfo_macros );
					is_transparent = ::rux::gui::rux_color_is_visible( _background ) == 1 ? 0 : 1;
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
				return is_transparent;
			};
			void Group::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_mouse_move.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void Group::private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index )
			{		
				if( reset == 0 )
				{
					if( _horizontal_filling == XEnum_Filling_FromContent )
						private_ResetLocationAndSizeCache( _horizontal_alignment == XEnum_Alignment_Stick || _horizontal_alignment == XEnum_Alignment_RightOrBottom || _horizontal_alignment == XEnum_Alignment_Center ? 1 : 0 , 0 , 1 , 0 );
					if( _vertical_filling == XEnum_Filling_FromContent )
						private_ResetLocationAndSizeCache( 0 , _vertical_alignment == XEnum_Alignment_Stick || _vertical_alignment == XEnum_Alignment_RightOrBottom || _vertical_alignment == XEnum_Alignment_Center ? 1 : 0 , 0 , 1 );
					
					if( _child_location_type == XEnum_Child_Location_Type_HorizontalStack
						|| _child_location_type == XEnum_Child_Location_Type_VerticalStack )
					{
						::rux::gui::XEnum_PointUnit point_unit = XEnum_PointUnit_Pixel;
						::rux::gui::XEnum_Filling filling = XEnum_Filling_Manual;
						::rux::gui::XEnum_Alignment alignment = XEnum_Alignment_Manual;
						rux::gui::ControlBase* base = NULL;
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = controls_start_index ; index0 < _render_common_bases.Count() ; index0++ )
						{
							base = _render_common_bases[ index0 ];
							if( base != control && base->get_IsVisible() == 1 )
							{
								base->GCRefAddRef( __FILE__ , __LINE__ );
								_cs_render_common_bases.ReadUnlock();
								alignment = base->get_HorizontalAlignment();
								::rux::uint8 reseted_cache = alignment != XEnum_Alignment_Manual ? 1 : 0;
								if( reseted_cache == 0 )
								{
									alignment = base->get_VerticalAlignment();
									reseted_cache = alignment != XEnum_Alignment_Manual ? 1 : 0;
								}
								if( reseted_cache == 0 )
								{
									filling = base->get_HorizontalFilling();
									reseted_cache = filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent ? 1 : 0;
								}
								if( reseted_cache == 0 )
								{
									filling = base->get_VerticalFilling();
									reseted_cache = filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent ? 1 : 0;
								}
								if( reseted_cache == 0 )
								{
									point_unit = base->get_PointUnit();
									reseted_cache = point_unit != XEnum_PointUnit_Pixel ? 1 : 0;
								}
								if( reseted_cache == 1 )
									base->private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 , 0 );
								_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
								base->GCRefRelease( __FILE__ , __LINE__ );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					else if( _child_location_type == XEnum_Child_Location_Type_Wrap )
					{
						rux::gui::ControlBase* base = NULL;
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = controls_start_index ; index0 < _render_common_bases.Count() ; index0++ )
						{
							base = _render_common_bases[ index0 ];
							if( base != control && base->get_IsVisible() == 1 )
							{
								base->GCRefAddRef( __FILE__ , __LINE__ );
								_cs_render_common_bases.ReadUnlock();							
								base->private_ResetLocationAndSizeCache( 1 , 1 , 0 , 0 , 0 );
								_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
								base->GCRefRelease( __FILE__ , __LINE__ );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
				}
				else
				{
					if( left || top || width || height || _child_location_type == XEnum_Child_Location_Type_HorizontalStack
						|| _child_location_type == XEnum_Child_Location_Type_VerticalStack || _child_location_type == XEnum_Child_Location_Type_Wrap )
					{
						::rux::uint8 reset_left = 0 , reset_top = 0 , reset_width = 0 , reset_height = 0;
						::rux::gui::XEnum_PointUnit point_unit = XEnum_PointUnit_Pixel;
						::rux::gui::XEnum_Filling filling = XEnum_Filling_Manual;
						::rux::gui::XEnum_Alignment alignment = XEnum_Alignment_Manual;
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = controls_start_index ; index0 < _render_common_bases.Count() ; index0++ )
						{
							::rux::gui::ControlBase* base = _render_common_bases[ index0 ];
							if( base != control && base->get_IsVisible() == 1 )
							{
								base->GCRefAddRef( __FILE__ , __LINE__ );
								_cs_render_common_bases.ReadUnlock();
								reset_left = left , reset_top = top , reset_width = 0 , reset_height = 0;
								if( _child_location_type == XEnum_Child_Location_Type_Wrap )
									reset_left = reset_top = 1;
								if( width || height )
								{
									point_unit = base->get_PointUnit();
									if( point_unit != XEnum_PointUnit_Pixel )
										reset_left = reset_top = reset_width = reset_height = 1;
								}
								if( width )
								{
									if( reset_left == 0 )
									{
										alignment = base->get_HorizontalAlignment();
										if( alignment != XEnum_Alignment_Manual )
											reset_left = 1;
									}
									if( reset_width == 0 )
									{
										filling = base->get_HorizontalFilling();
										if( filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent )
											reset_width = 1;
									}
								}
								if( height )
								{
									if( reset_top == 0 )
									{
										alignment = base->get_VerticalAlignment();
										if( alignment != XEnum_Alignment_Manual )
											reset_top = 1;
									}
									if( reset_height == 0 )
									{
										filling = base->get_VerticalFilling();
										if( filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent )
											reset_height = 1;
									}
								}
								if( _child_location_type == XEnum_Child_Location_Type_HorizontalStack )
								{
									if( reset_left == 0 )
									{
										alignment = base->get_HorizontalAlignment();
										if( alignment != XEnum_Alignment_Manual )
											reset_left = 1;
									}
									if( reset_width == 0 )
									{
										filling = base->get_HorizontalFilling();
										if( filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent )
											reset_width = 1;
									}
								}
								else if( _child_location_type == XEnum_Child_Location_Type_VerticalStack )
								{
									if( reset_top == 0 )
									{
										alignment = base->get_VerticalAlignment();
										if( alignment != XEnum_Alignment_Manual )
											reset_top = 1;
									}
									if( reset_height == 0 )
									{
										filling = base->get_VerticalFilling();
										if( filling == XEnum_Filling_Auto || filling == XEnum_Filling_FromParent )
											reset_height = 1;
									}
								}
								if( reset_left || reset_top || reset_width || reset_height )
									base->private_ResetLocationAndSizeCache( reset_left , reset_top , reset_width , reset_height , 0 );
								_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
								base->GCRefRelease( __FILE__ , __LINE__ );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
				}
			};
			implement_duplicate_internal_function_1( Group , set_ChildLocationType , XEnum_Child_Location_Type );
			void Group::set_ChildLocationType( XEnum_Child_Location_Type child_location_type )
			{
				if( _child_location_type != child_location_type )
				{
					_child_location_type = child_location_type;
					private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );
				}
			};
			void Group::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};			
			void Group::RemoveTabControls( void )
			{
				rux::gui::Window* window = get_ParentWindow();
				if( window )
				{
					CRITICAL_SECTION_LOCK( window->_cs_tab_common_bases );		
					_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
					for( ::rux::uint32 index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
					{
						for( ::rux::uint32 index1 = 0 ; index1 < window->_tab_common_bases.Count() ; index1++ )
						{
							if( window->_tab_common_bases[ index1 ] == _render_common_bases[ index0 ] )
							{
								if( _render_common_bases[ index0 ]->get_IsGroup() == 1 )
									((Group*)_render_common_bases[ index0 ]->get_ObjectAddress())->RemoveTabControls();
								window->_tab_common_bases.RemoveAt( index1 );
								break;
							}
						}
					}
					_cs_render_common_bases.ReadUnlock();
					CRITICAL_SECTION_UNLOCK( window->_cs_tab_common_bases );	
				}
			};
			void XGroup::ClearControls( void )
			{
				(*this)()->ClearControls();
			};			
			void Group::ClearControls( void )
			{	
				XMallocArray< ::rux::gui::ControlBase* > to_delete;
				rux::gui::Window* window = get_ParentWindow();
				_cs_render_common_bases.WriteLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
				for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
					to_delete.Add( _render_common_bases[ index0 ] );
				_render_common_bases.Clear();
				_cs_render_common_bases.WriteUnlock();
				for( size_t index0 = 0 ; index0 < to_delete.Count() ; index0++ )
				{
					::rux::gui::ControlBase* control_base = to_delete[ index0 ];
					if( window )
					{		
						CRITICAL_SECTION_LOCK( window->_cs_tab_common_bases );		
						for( size_t index1 = 0 ; index1 < window->_tab_common_bases.Count() ; index1++ )
						{
							if( window->_tab_common_bases[ index1 ] == control_base )
							{
								if( control_base->get_IsGroup() == 1 )
									((Group*)control_base->get_ObjectAddress())->RemoveTabControls();
								window->_tab_common_bases.RemoveAt( index1 );
								break;
							}
						}
						CRITICAL_SECTION_UNLOCK( window->_cs_tab_common_bases );
						::rux::gui::CommonBase* temp = control_base;
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
							::rux::gui::controls::rux_change_over_control( 0 , temp , NULL );
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
					control_base->BeforeFreeSystemResources();
					control_base->FreeResources( 0 );
					control_base->FreeSystemResources();
					control_base->GCRefRelease( __FILE__ , __LINE__ );
				}
				private_ResetLocationAndSizeCache( 1 , 1 , 1 , 1 );
			};
			IMPLEMENT_BASE_UI_FUNCTIONS( XGroup );
		};
	};
};