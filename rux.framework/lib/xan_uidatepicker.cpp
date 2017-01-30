#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_uidatepicker.h>
#include <xan_uiinlines.h>
#include <xan_uiprimitives.h>
#include <xan_uirectangle.h>
#include <xan_float.h>
#include <xan_uitextblock.h>
#include <xan_uiprimitives.h>
#include <xan_keyvaluepair.h>
#ifndef BOOLDOG_NETWORK
#define BOOLDOG_NETWORK
#endif
#ifndef BOOLDOG_HEADER
#define BOOLDOG_HEADER( header ) <booldog/header>
#endif
#include <booldog/boo_time_utils.h>
#include <booldog/boo_heap_allocator.h>
#include <booldog/boo_mixed_allocator.h>

begin_implement_rux_class_with_properties_ns_base_class( DatePicker , rux::gui::controls , rux::gui::ControlBase )
	_tag.set_Info( "tag" , __file__ , __line__ );
	_private_tag.set_Info( "private_tag" , __file__ , __line__ );
	IMPLEMENT_BASE_UI_MEMBERS();	
	copy_color( _datepicker_background , ::rux::gui::Colors::ButtonNormalColor() );
	_datepicker_thickness = ::rux::gui::Thickness( .0f , .0f , .0f , .0f );
	copy_color( _date_foreground , ::rux::gui::Colors::White() );
	_date_container_thickness = ::rux::gui::Thickness( 0.f , 0.f , 0.f , 0.f ); 	
	copy_color( _date_container_background , ::rux::gui::Colors::Black( 0.8f , __file__ , __line__ ) );//create_color( 204 , 204 , 204 , 255 ) );
	copy_color( _date_container_border , ::rux::gui::Colors::White( 0.9f ) );
	_date_container_border_width = 1.f;
	_date_container.set_Height( 0.f );
	_is_editable = 0;	
	_posix_time = ::booldog::utils::time::posix::now_as_utc();
	_date_font = ::rux::engine::_globals->_gui_globals->_rux_gui_create_font( "tahoma.ttf" , 0 , 12 );
	_vertical_text_alignment = XEnum_Alignment_Center;
	_horizontal_text_alignment = XEnum_Alignment_Center;
	_on_datetime_changed_event_callback = NULL;
	_on_interval_changed_event_callback = 0;
	_date_container.set_IsVisible( 0 );
	_date_container_width = 154;
	_opened = 0;

	_mon = "Пн";
	_tue = "Вт";
	_wed = "Ср";
	_thu = "Чт";
	_fri = "Пт";
	_sat = "Сб";
	_sun = "Вс";
	_january = "Январь";
	_february = "Февраль";
	_march = "Март";
	_april = "Апрель";
	_may = "Май";
	_june = "Июнь";
	_july = "Июль";
	_august = "Август";
	_september = "Сентябрь";
	_october = "Октябрь";
	_november = "Ноябрь";
	_december = "Декабрь";
	_Jan = "Янв";
	_Feb = "Фев";
	_Mar = "Мар";
	_Apr = "Апр";
	_May = "Май";
	_Jun = "Июн";
	_Jul = "Июл";
	_Aug = "Авг";
	_Sep = "Сен";
	_Oct = "Окт";
	_Nov = "Ноя";
	_Dec = "Дек";
end_implement_rux_class();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			DatePicker::~DatePicker()
			{
				FreeResources( 0 );
				_date_container_group_animation.Stop();		
				if( _date_foreground )
					_date_foreground->Release();
				if( _datepicker_background )
					_datepicker_background->Release();
				if( _datepicker_border )
					_datepicker_border->Release();
				if( _date_container_background )
					_date_container_background->Release();
				if( _date_container_border )
					_date_container_border->Release();
			};
			void DatePicker::on_deactivated( const XObject& sender , const XObject& activated_control )
			{
				::rux::gui::ControlBase* sender_common_base = (::rux::gui::ControlBase*)sender.get_GCRef()->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
				if( sender_common_base )
				{
					::rux::byte its_mine = 0;
					DatePicker* date_picker = (DatePicker*)sender_common_base->get_TagPtr();
					::rux::gui::ParentBase* parent_base = date_picker->_date_container();
					::rux::gui::ControlBase* common_base = (::rux::gui::ControlBase*)activated_control.get_GCRef()->DynamicCast( ::rux::gui::XEnum_ClassType_ControlBase );
					if( common_base )
					{
						::rux::gui::ParentBase* find_parent_base = common_base->get_ParentControl();
						for( ; ; )
						{
							if( find_parent_base )
							{
								if( find_parent_base->get_IsWindow() )
									break;
								if( find_parent_base == parent_base )
								{
									its_mine = 1;
									break;
								}
							}
							else
								break;
							find_parent_base = find_parent_base->get_ParentControl();
						}
					}
					if( its_mine == 0 )
					{
						date_picker->_date_container.RemoveControl();
						date_picker->_date_container.set_IsVisible( 0 );
						::rux::gui::ControlBase* date_picker_common_base = date_picker;
						if( common_base == date_picker_common_base )
							date_picker->_opened = 2;
						else
							date_picker->_opened = 0;
					}
				}
			};
			void DatePicker::on_mouse_wheel( const ::rux::gui::events::MouseEvent& event )
			{
				declare_variable_param( ::rux::gui::controls::XGroup , group , event._sender );
				DatePicker* date_picker = (DatePicker*)group.get_TagPtr();
				if( group.get_ControlName() == "group_header" )
				{
					if( date_picker->_group_calendar.get_IsVisible() )
					{
						if( event._window_event->_delta > 0 )
						{
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							loctime = ::booldog::utils::time::posix::add_months( loctime , 1 );

							date_picker->refresh_date_container( loctime );
							date_picker->accept_date();
						}
						else
						{
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							loctime = ::booldog::utils::time::posix::sub_months( loctime , 1 );

							date_picker->refresh_date_container( loctime );
							date_picker->accept_date();
						}
					}
					else if( date_picker->_group_calendar_months.get_IsVisible() )
					{
						if( event._window_event->_delta > 0 )
						{
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							date_picker->refresh_date_container( ::booldog::utils::time::posix::add_years( loctime , 1 ) );
							date_picker->accept_date();
						}
						else
						{
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							date_picker->refresh_date_container( ::booldog::utils::time::posix::sub_years( loctime , 1 ) );
							date_picker->accept_date();
						}
					}
				}
				else if( group.get_ControlName() == "group_calendar" )
				{
					if( event._window_event->_delta > 0 )
					{
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						date_picker->refresh_date_container( ::booldog::utils::time::posix::add_days( loctime , 1 ) );
						date_picker->accept_date();
					}
					else
					{
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						date_picker->refresh_date_container( ::booldog::utils::time::posix::sub_days( loctime , 1 ) );
						date_picker->accept_date();
					}
				}
				else if( group.get_ControlName() == "group_calendar_months" )
				{
					if( event._window_event->_delta > 0 )
					{
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						date_picker->refresh_date_container( ::booldog::utils::time::posix::add_months( loctime , 1 ) );
						date_picker->accept_date();
					}
					else
					{
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						date_picker->refresh_date_container( ::booldog::utils::time::posix::sub_months( loctime , 1 ) );
						date_picker->accept_date();
					}
				}
			}
			void DatePicker::set_DayColorAndHeight(int index, ::rux::gui::ColorBase* color, float height_in_percent)
			{
				XArray< XRectangle > rects;
				rects.set_ByRef(_group_calendar.FindAll< XRectangle >());
				if(rects.Count() > index)
				{
					rects[index].set_Background(color);
					rects[index].set_Height(height_in_percent);
				}
			}
			void XDatePicker::set_DayColorAndHeight(int index, ::rux::gui::ColorBase* color, float height_in_percent)
			{
				(*this)()->set_DayColorAndHeight(index, color, height_in_percent);
			}
			void DatePicker::add_gui( void )
			{
				_date_container()->WaitForFreeSystemResources();
				if( _date_container.get_ControlsCount() == 0 )
				{
					_date_container.set_Width( _date_container_width );
					_date_container.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );
					_date_container.set_Background( _date_container_background );
					_date_container.set_BorderColor( _date_container_border );
					_date_container.set_BorderWidth( _date_container_border_width );
					_date_container.set_Corner( _date_container_thickness );
					_date_container.set_OnDeactivated( on_deactivated );
					declare_variable( ::rux::XString , error );
					float item_size = ( _date_container_width - 14.f ) / 7.f;
					
					_group_header.set_ControlName( "group_header" );
					_group_header.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
					_group_header.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
					_group_header.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromParent );
					_group_header.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					_group_header.set_OnMouseWheel( on_mouse_wheel );
					::rux::gui::Margin margin( 1 , 1 , 1 , 1 );
										
					_button_left.set_OnClick( on_button_click );
					_button_left.set_Corner( ::rux::gui::Thickness( 0.f , 0.f , 0.f , 0.f ) );
					_button_left.set_ControlName( "left" );
					_button_left.set_Content( XPrimitives::LeftArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
					_button_left.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					_button_left.set_Margin( margin );
					_button_left.set_Height( item_size );
					_button_left.set_Width( item_size / 2.f );
					_button_left.set_OnDeactivated( on_deactivated );
					_button_left.set_TagPtr( this );
					_group_header.AddControl( _button_left , error );
					
					_button_right.set_OnClick( on_button_click );
					_button_right.set_Corner( ::rux::gui::Thickness( 0.f , 0.f , 0.f , 0.f ) );
					_button_right.set_ControlName( "right" );
					_button_right.set_Content( XPrimitives::RightArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
					_button_right.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_RightOrBottom );
					_button_right.set_Margin( margin );
					_button_right.set_Height( item_size );
					_button_right.set_Width( item_size / 2.f );
					_button_right.set_OnDeactivated( on_deactivated );
					_button_right.set_TagPtr( this );
					_group_header.AddControl( _button_right , error );

					_textblock_month.set_ControlName( "month" );
					_textblock_month.set_Foreground( ::rux::gui::Colors::White( 0.6f ) );
					_textblock_month.set_IsVisible( 1 );
					_textblock_month.set_Background( ::rux::gui::Colors::Arsenic() );
					_textblock_month.set_Font( "tahoma.ttf" , 0 , 11 );
					_textblock_month.set_OnMouseEnter( on_date_enter );
					_textblock_month.set_OnMouseLeave( on_date_leave );
					_textblock_month.set_OnLeftMouseButtonDown( on_date_left_mouse_down );
					_textblock_month.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					_textblock_month.set_Margin( margin );
					_textblock_month.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
					_textblock_month.set_Height( item_size );
					_textblock_month.set_OnDeactivated( on_deactivated );
					_textblock_month.set_TagPtr( this );
					_group_header.AddControl( _textblock_month , error );	

					_textblock_year.set_ControlName( "year" );
					_textblock_year.set_Foreground( ::rux::gui::Colors::White( 0.6f ) );
					_textblock_year.set_IsVisible( 0 );
					_textblock_year.set_Background( ::rux::gui::Colors::Arsenic() );
					_textblock_year.set_Font( "tahoma.ttf" , 0 , 11 );	
					_textblock_year.set_OnMouseEnter( on_date_enter );
					_textblock_year.set_OnMouseLeave( on_date_leave );
					_textblock_year.set_OnLeftMouseButtonDown( on_date_left_mouse_down );
					_textblock_year.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					_textblock_year.set_Margin( margin );
					_textblock_year.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
					_textblock_year.set_Height( item_size );
					_textblock_year.set_OnDeactivated( on_deactivated );
					_textblock_year.set_TagPtr( this );
					_group_header.AddControl( _textblock_year , error );

					_group_header.set_OnDeactivated( on_deactivated );
					_group_header.set_TagPtr( this );
					_date_container.AddControl( _group_header , error );
					
					_group_week_days.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
					_group_week_days.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
					_group_week_days.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromParent );
					_group_week_days.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					
					declare_variable( XTextBlock , textblock );
					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_mon.str(), _mon.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_tue.str(), _tue.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );		

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_wed.str(), _wed.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );		

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_thu.str(), _thu.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );		

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_fri.str(), _fri.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );		

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_sat.str(), _sat.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );		

					textblock = XTextBlock();
					textblock.set_Background( create_color( 153 , 153 , 153 , 255 ) );
					textblock.set_Text(_sun.str(), _sun.Size() - 1);	
					textblock.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					textblock.set_Margin( margin );
					textblock.set_Width( item_size );
					textblock.set_Height( item_size );	
					textblock.set_OnDeactivated( on_deactivated );
					textblock.set_TagPtr( this );
					_group_week_days.AddControl( textblock , error );
					
					_group_week_days.set_OnDeactivated( on_deactivated );
					_group_week_days.set_TagPtr( this );
					_date_container.AddControl( _group_week_days , error );

					_group_calendar.set_ControlName( "group_calendar" );
					_group_calendar.set_ChildLocationType( XEnum_Child_Location_Type_Wrap );
					_group_calendar.set_OnMouseWheel( on_mouse_wheel );
					_group_calendar.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
					_group_calendar.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromParent );
					_group_calendar.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );

					::rux::gui::controls::XGroup group;
					::rux::gui::controls::XRectangle rect;

					for( size_t index0 = 0 ; index0 < 6 ; index0++ )
					{
						for( size_t index1 = 0 ; index1 < 7 ; index1++ )
						{
							group = XGroup();
							group.set_Margin(margin);
							group.set_Width(item_size);
							group.set_Height(item_size);

							rect = XRectangle();
							rect.set_PointUnit(::rux::gui::XEnum_PointUnit_Percent);
							rect.set_VerticalAlignment(::rux::gui::XEnum_Alignment_RightOrBottom);
							rect.set_Width(1.f);
							rect.set_Height(.7f);
							rect.set_Background(::rux::gui::Colors::Transparent());

							group.AddControl(rect, error);

							textblock = XTextBlock();	
							//textblock.set_Margin( margin );
							textblock.set_OnLeftMouseButtonDown(on_date_left_mouse_down);
							textblock.set_OnMouseDoubleClick(on_date_left_mouse_double_click);
							textblock.set_OnMouseEnter(on_date_enter);
							textblock.set_OnMouseLeave(on_date_leave);
							textblock.set_ControlName("month_day");
							textblock.set_HorizontalFilling(::rux::gui::XEnum_Filling_FromParent);
							textblock.set_VerticalFilling(::rux::gui::XEnum_Filling_FromParent);
							textblock.set_OnDeactivated(on_deactivated);
							textblock.set_TagPtr( this );

							group.AddControl(textblock, error);

							_group_calendar.AddControl(group, error);
						}
					}		
					_group_calendar.set_OnDeactivated( on_deactivated );
					_group_calendar.set_TagPtr( this );
					_date_container.AddControl( _group_calendar , error );

					_group_calendar_months.set_ControlName( "group_calendar_months" );
					_group_calendar_months.set_IsVisible( 0 );
					_group_calendar_months.set_OnMouseWheel( on_mouse_wheel );
					_group_calendar_months.set_Margin( margin );
					_group_calendar_months.set_ChildLocationType( XEnum_Child_Location_Type_Wrap );
					_group_calendar_months.set_Height( _group_calendar.get_ContentHeight() + _group_week_days.get_ContentHeight() );
					_group_calendar_months.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromParent );
					_group_calendar_months.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
					for( size_t index1 = 0 ; index1 < 12 ; index1++ )
					{
						textblock = XTextBlock();
						if( index1 == 0 )
							textblock.set_Text(_Jan.str(), _Jan.Size() - 1);
						else if( index1 == 1 )
							textblock.set_Text(_Feb.str(), _Feb.Size() - 1);
						else if( index1 == 2 )
							textblock.set_Text(_Mar.str(), _Mar.Size() - 1);
						else if( index1 == 3 )
							textblock.set_Text(_Apr.str(), _Apr.Size() - 1);
						else if( index1 == 4 )
							textblock.set_Text(_May.str(), _May.Size() - 1);
						else if( index1 == 5 )
							textblock.set_Text(_Jun.str(), _Jun.Size() - 1);
						else if( index1 == 6 )
							textblock.set_Text(_Jul.str(), _Jul.Size() - 1);
						else if( index1 == 7 )
							textblock.set_Text(_Aug.str(), _Aug.Size() - 1);
						else if( index1 == 8 )
							textblock.set_Text(_Sep.str(), _Sep.Size() - 1);
						else if( index1 == 9 )
							textblock.set_Text(_Oct.str(), _Oct.Size() - 1);
						else if( index1 == 10 )
							textblock.set_Text(_Nov.str(), _Nov.Size() - 1);
						else if( index1 == 11 )
							textblock.set_Text(_Dec.str(), _Dec.Size() - 1);
						textblock.set_OnLeftMouseButtonDown( on_date_left_mouse_down );
						textblock.set_OnMouseDoubleClick( on_date_left_mouse_double_click );
						textblock.set_OnMouseEnter( on_date_enter );
						textblock.set_OnMouseLeave( on_date_leave );
						textblock.set_ControlName( "months" );	
						textblock.set_PointUnit( ::rux::gui::XEnum_PointUnit_PercentOnlySize );
						textblock.set_Width( 0.25f );
						textblock.set_Height( 0.33f );
						textblock.set_OnDeactivated( on_deactivated );
						textblock.set_TagPtr( this );
						_group_calendar_months.AddControl( textblock , error );
					}
					_group_calendar_months.set_OnDeactivated( on_deactivated );
					_group_calendar_months.set_TagPtr( this );
					_date_container.AddControl( _group_calendar_months , error );

					_date_container.set_IsVisible( 0 );			
					_date_container.set_TagPtr( this );	
				}
			};
			void XDatePicker::set_OnIntervalChanged(::rux::gui::events::on_interval_changed on_interval_changed_event_callback)
			{
				(*this)()->set_OnIntervalChanged(on_interval_changed_event_callback);
			}
			void DatePicker::set_OnIntervalChanged(::rux::gui::events::on_interval_changed on_interval_changed_event_callback)
			{
				_on_interval_changed_event_callback = on_interval_changed_event_callback;
			}
			void XDatePicker::set_OnDatetimeChanged( ::rux::gui::events::on_event_t on_datetime_changed_event_callback )
			{
				(*this)()->_on_datetime_changed_event_callback = on_datetime_changed_event_callback;
			};
			void DatePicker::UpdateParentWindow( void )
			{
				_parent_window = get_ParentWindow();
			};
			void DatePicker::FreeResources( ::rux::byte )
			{
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_cache );
				_ui_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_border_cache );
				_ui_border_cache = NULL;
				::rux::engine::_globals->_gui_globals->_rux_gui_add_cache( _ui_date_cache );
				_ui_date_cache = NULL;
			};
			void DatePicker::raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void DatePicker::raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_double_click.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void DatePicker::raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_right_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void DatePicker::raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_down.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void DatePicker::raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event
				, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_double_click.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void DatePicker::raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event)
			{
				_on_mouse_wheel_up.raise< const ::rux::gui::events::MouseEvent& >(
					::rux::gui::events::MouseEvent(*this, window_event, explicit_event));
			};
			void DatePicker::raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				raise_OnLeftMouseButtonDown( window_event , 1 );
			};
			::rux::byte DatePicker::raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* , ::rux::byte& raised_event , ::rux::byte explicit_event )
			{
				_on_mouse_enter.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , explicit_event ) );
				return 1;
			};
			void DatePicker::raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_mouse_leave_callback.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			::rux::uint64 XDatePicker::get_DateTime( void )
			{
				return (*this)()->_posix_time;
			};
			void DatePicker::refresh_date_container( ::rux::uint64 loctime )
			{
				add_gui();
				XArray< XTextBlock > textblocks;
				textblocks.set_ByRef( _group_calendar.FindAll< XTextBlock >() );
				
				::booldog::uint64 left_interval = 0, right_interval = 0;

				::booldog::uint32 day_of_month = 0 , month = 0 , year = 0 , firstday_of_month = 0 
					, firstmonth = 0 , firstyear = 0;

				::booldog::utils::time::posix::date( loctime , year , month , day_of_month );

				::booldog::uint64 first_day = ::booldog::utils::time::posix::generate
					( ::booldog::utils::time::posix::millisecond( loctime )
					, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
					, ::booldog::utils::time::posix::hour( loctime ) 
					, 1 , month , year );
				
				::rux::uint32 first_day_uint32 = ::booldog::utils::time::posix::day_of_week( first_day );
				if( first_day_uint32 == 0 )
					first_day_uint32 = 7;
				first_day = ::booldog::utils::time::posix::sub_days( first_day , first_day_uint32 - 1 );
				left_interval = first_day;
				for( size_t index0 = 0 ; index0 < textblocks.Count() ; index0++ )
				{
					::booldog::utils::time::posix::date( first_day , firstyear , firstmonth , firstday_of_month );

					XUInt32 day( firstday_of_month );
					textblocks[ index0 ].set_Text( day.ToString() );
					if( year == firstyear
						&& month == firstmonth )
					{
						textblocks[ index0 ].set_Foreground( ::rux::gui::Colors::White() );
						textblocks[ index0 ].set_ControlName( "month_day" );
						if( firstday_of_month == day_of_month )
						{
							set_Tag( day );
							textblocks[ index0 ].set_Tag( day );
							textblocks[ index0 ].set_Background( ::rux::gui::Colors::ButtonDownColor() );	
						}
						else
						{
							textblocks[ index0 ].set_Tag( XObject( "obj" , __FILE__ , __LINE__ ) );
							textblocks[ index0 ].set_Background( ::rux::gui::Colors::Transparent() );
						}
					}
					else
					{
						textblocks[ index0 ].set_Tag( XObject( "obj" , __FILE__ , __LINE__ ) );
						textblocks[ index0 ].set_Foreground( ::rux::gui::Colors::White( 0.6f ) );
						textblocks[ index0 ].set_Background( ::rux::gui::Colors::Transparent() );
						if( year > firstyear
							|| month > firstmonth )
							textblocks[ index0 ].set_ControlName( "prev_month_day" );
						else
							textblocks[ index0 ].set_ControlName( "next_month_day" );
					}
					first_day = ::booldog::utils::time::posix::add_days( first_day , 1 );
				}
				right_interval = first_day;
				size_t month_index = month - 1;
				textblocks.set_ByRef( _group_calendar_months.FindAll< XTextBlock >() );
				for( size_t index0 = 0 ; index0 < textblocks.Count() ; index0++ )
				{
					if( index0 == month_index )
					{
						textblocks[ index0 ].set_Tag( ::rux::XBoolean( true ) );
						textblocks[ index0 ].set_Background( ::rux::gui::Colors::ButtonDownColor() );
					}
					else
					{
						textblocks[ index0 ].set_Tag( XObject( "obj" , __FILE__ , __LINE__ ) );
						textblocks[ index0 ].set_Background( ::rux::gui::Colors::Transparent() );
					}
				}
				switch(month)
				{
				case 1:
					_textblock_month.set_Text(_january.str(), _january.Size() - 1);
					break;
				case 2:
					_textblock_month.set_Text(_february.str(), _february.Size() - 1);
					break;
				case 3:
					_textblock_month.set_Text(_march.str(), _march.Size() - 1);
					break;
				case 4:
					_textblock_month.set_Text(_april.str(), _april.Size() - 1);
					break;
				case 5:
					_textblock_month.set_Text(_may.str(), _may.Size() - 1);
					break;
				case 6:
					_textblock_month.set_Text(_june.str(), _june.Size() - 1);
					break;
				case 7:
					_textblock_month.set_Text(_july.str(), _july.Size() - 1);
					break;
				case 8:
					_textblock_month.set_Text(_august.str(), _august.Size() - 1);
					break;
				case 9:
					_textblock_month.set_Text(_september.str(), _september.Size() - 1);
					break;
				case 10:
					_textblock_month.set_Text(_october.str(), _october.Size() - 1);
					break;
				case 11:
					_textblock_month.set_Text(_november.str(), _november.Size() - 1);
					break;
				case 12:
					_textblock_month.set_Text(_december.str(), _december.Size() - 1);
					break;
				}				
				_textblock_month.set_Tag(XUInt32(month));
				_textblock_year.set_Text( XUInt32( year ).ToString() );

				_on_interval_changed_event_callback.raise< const XObject&, ::rux::uint64, ::rux::uint64 >(XObject(*this), left_interval
					, right_interval);
			};
			void DatePicker::accept_date( void )
			{
				declare_variable( ::rux::XString , year_string );
				year_string.set_ByRef( _textblock_year.get_Text() );
				::rux::uint32 year = year_string.ToUInt32();
				XUInt32 month( _textblock_month.get_Tag() );
				XUInt32 day( get_Tag() );
				::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( _posix_time );
				loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
					, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
					, ::booldog::utils::time::posix::hour( loctime ) 
					, day() , month() , year );
				set_DateTime( ::booldog::utils::time::posix::toutc( loctime ) );
			};
			void DatePicker::on_button_click( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XButton button( event._sender );
				DatePicker* date_picker = (DatePicker*)button.get_TagPtr();
				if( button.get_ControlName() == "left" )
				{	
					if( date_picker->_group_calendar.get_IsVisible() == 1 )
					{
						::rux::XString year_string;
						year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
						::rux::uint32 year = year_string.ToUInt32();
						XUInt32 month( date_picker->_textblock_month.get_Tag() );
						XUInt32 day( date_picker->get_Tag() );
						if( month() == 1 )
						{
							month = XUInt32( 12 );
							year--;
						}
						else
							month -= 1;		
						if( year >= 1970 )
						{
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
								, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
								, ::booldog::utils::time::posix::hour( loctime ) 
								, day() , month() , year );
							date_picker->refresh_date_container( loctime );
							date_picker->accept_date();
						}
					}
					else if( date_picker->_group_calendar_months.get_IsVisible() == 1 )
					{
						::rux::XString year_string;
						year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
						::rux::uint32 year = year_string.ToUInt32();
						XUInt32 month( date_picker->_textblock_month.get_Tag() );
						XUInt32 day( date_picker->get_Tag() );
						if( year > 1971 )
						{
							year--;			
							::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
							loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
								, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
								, ::booldog::utils::time::posix::hour( loctime ) 
								, day() , month() , year );
							date_picker->refresh_date_container( loctime );			
							date_picker->accept_date();
						}
					}
				}
				else if( button.get_ControlName() == "right" )
				{	
					if( date_picker->_group_calendar.get_IsVisible() == 1 )
					{
						::rux::XString year_string;
						year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
						::rux::uint32 year = year_string.ToUInt32();
						XUInt32 month( date_picker->_textblock_month.get_Tag() );
						XUInt32 day( date_picker->get_Tag() );
						if( month() == 12 )
						{
							month = XUInt32( 1 );
							year++;
						}
						else
							month += 1;
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
							, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
							, ::booldog::utils::time::posix::hour( loctime ) 
							, day() , month() , year );
						date_picker->refresh_date_container( loctime );
						date_picker->accept_date();
					}
					else if( date_picker->_group_calendar_months.get_IsVisible() == 1 )
					{
						::rux::XString year_string;
						year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
						::rux::uint32 year = year_string.ToUInt32();
						XUInt32 month( date_picker->_textblock_month.get_Tag() );
						XUInt32 day( date_picker->get_Tag() );
						year++;		
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
							, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
							, ::booldog::utils::time::posix::hour( loctime ) 
							, day() , month() , year );
						date_picker->refresh_date_container( loctime );
						date_picker->accept_date();
					}
				}
			};
			void DatePicker::raise_OnKeyChar( const ::rux::XString& , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 )
			{		
			};
			void DatePicker::raise_OnKeyDown( ::rux::gui::XEnum_Keys , ::rux::uint8 , ::rux::uint8 , ::rux::uint8 , ::rux::uint32 , ::rux::byte explicit_event )
			{
			};
			void DatePicker::raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{
				_on_left_mouse_button_down.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , 1 ) );
				if( _opened )
				{
					_date_container_group_animation.Clear();
					_date_container.RemoveControl();
					_date_container.set_IsVisible( 0 );
					_opened = 0;
				}
				else
				{
					refresh_date_container( _posix_time );
					float date_container_height = _date_container.get_ContentHeight();
					::rux::gui::XWindow window( get_ParentWindow() );
					
					if( get_Top() + get_Height() + date_container_height > window.get_Height() )
					{				
						_date_container_group_animation.Clear();
						_date_container.set_Height( 0 );
						_date_container.set_BottomSticker( get_Top() );
						_date_container.set_Left( get_Left() );
						XAnimation animation;	
						animation.set_Control( _date_container );
						animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
						animation.set_StartValue( XFloat( 0.f ) );
						animation.set_PropertyName( "Height" );
						animation.AddTimeline( 170000ULL , XFloat( date_container_height ) );				
						_date_container_group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
						_date_container_group_animation.AddAnimation( animation );						
						::rux::XString error;
						window.AddControl( _date_container , error );
						_date_container.set_IsVisible( 1 );
						_date_container.Activate();
						_date_container_group_animation.Start();
					}
					else
					{
						_date_container_group_animation.Clear();
						_date_container.set_Top( get_Top() + get_Height() );
						_date_container.set_Left( get_Left() );
						_date_container.set_Height( 0 );			
						_date_container.ResetBottomSticker();
						XAnimation animation;	
						animation.set_Control( _date_container );
						animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
						animation.set_StartValue( XFloat( 0.f ) );
						animation.set_PropertyName( "Height" );
						animation.AddTimeline( 170000ULL , XFloat( date_container_height ) );				
						_date_container_group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
						_date_container_group_animation.AddAnimation( animation );						
						::rux::XString error;
						window.AddControl( _date_container , error );
						_date_container.set_IsVisible( 1 );
						_date_container.Activate();
						_date_container_group_animation.Start();
					}		
					_opened = 1;
				}
			};
			void DatePicker::raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				_on_left_mouse_button_up.raise< const ::rux::gui::events::MouseEvent& >( ::rux::gui::events::MouseEvent( *this , window_event , explicit_event ) );
			};
			void DatePicker::Invalidate( void )
			{
			};
			void DatePicker::raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
			};
			void XDatePicker::set_DateTime( uint64 unix_time )
			{
				(*this)()->set_DateTime( unix_time );
			};
			void DatePicker::set_DateTime( uint64 unix_time )
			{
				if( _posix_time != unix_time )
				{
					_posix_time = unix_time;
					_on_datetime_changed_event_callback.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
				}
			};
			void XDatePicker::set_DateTimeWithoutEvent( uint64 unix_time )
			{
				(*this)()->set_DateTimeWithoutEvent( unix_time );
			};
			void DatePicker::set_DateTimeWithoutEvent( uint64 unix_time )
			{
				if( _posix_time != unix_time )
					_posix_time = unix_time;
			};
			void DatePicker::raise_OnActivate( void )
			{		
			};
			void DatePicker::raise_OnDeactivate( ::rux::gui::CommonBase* activated_control )
			{	
				XObject activated( "obj" , __FILE__ , __LINE__ );
				if( activated_control )
					activated.set_ByRef( XObject( activated_control , 1, "obj" , __FILE__ , __LINE__ ) );
				_on_deactivated.raise< const XObject& , const XObject& >( XObject( *this ) , activated );
			};
			void DatePicker::on_date_left_mouse_down( const ::rux::gui::events::MouseEvent& event )
			{
				XTextBlock textblock( event._sender );
				if( textblock.get_ControlName() == "month" )
				{					
					DatePicker* date_picker = (DatePicker*)textblock.get_TagPtr();
					date_picker->_textblock_month.set_IsVisible( 0 );
					date_picker->_group_calendar.set_IsVisible( 0 );
					date_picker->_group_week_days.set_IsVisible( 0 );
					date_picker->_textblock_year.set_IsVisible( 1 );					
					date_picker->_group_calendar_months.set_IsVisible( 1 );
					date_picker->_textblock_year.Activate();
				}
				else if( textblock.get_ControlName() == "year" )
				{
					DatePicker* date_picker = (DatePicker*)textblock.get_TagPtr();
					date_picker->_textblock_year.set_IsVisible( 0 );					
					date_picker->_group_calendar_months.set_IsVisible( 0 );
					date_picker->_textblock_month.set_IsVisible( 1 );
					date_picker->_group_calendar.set_IsVisible( 1 );	
					date_picker->_group_week_days.set_IsVisible( 1 );
					date_picker->_textblock_month.Activate();
				}
				else if( textblock.get_ControlName() == "months" )
				{
					DatePicker* date_picker = (DatePicker*)textblock.get_TagPtr();
					::rux::XString year_string;
					year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
					::rux::uint32 year = year_string.ToUInt32();
					::rux::int32 month = date_picker->_group_calendar_months.IndexOf( textblock ) + 1;


					::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
					loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
						, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
						, ::booldog::utils::time::posix::hour( loctime ) , ::booldog::utils::time::posix::day_of_month( loctime )
						, month , year );

					date_picker->refresh_date_container( loctime );
					date_picker->accept_date();

					date_picker->_textblock_year.set_IsVisible( 0 );					
					date_picker->_group_calendar_months.set_IsVisible( 0 );
					date_picker->_textblock_month.set_IsVisible( 1 );
					date_picker->_group_calendar.set_IsVisible( 1 );	
					date_picker->_group_week_days.set_IsVisible( 1 );
					date_picker->_textblock_month.Activate();
				}
				else
				{
					::rux::XString day_string;
					day_string.set_ByRef( textblock.get_Text() );
					if( day_string.Length() > 0 )
					{
						DatePicker* date_picker = (DatePicker*)textblock.get_TagPtr();
						::rux::XString year_string;
						year_string.set_ByRef( date_picker->_textblock_year.get_Text() );
						::rux::uint32 year = year_string.ToUInt32();
						XUInt32 month( date_picker->_textblock_month.get_Tag() );		
						::rux::uint32 day = day_string.ToUInt32();
						::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( date_picker->_posix_time );
						loctime = ::booldog::utils::time::posix::generate( ::booldog::utils::time::posix::millisecond( loctime )
							, ::booldog::utils::time::posix::second( loctime ) , ::booldog::utils::time::posix::minute( loctime )
							, ::booldog::utils::time::posix::hour( loctime ) 
							, day , month() , year );
						if( textblock.get_ControlName() == "prev_month_day" )
							loctime = ::booldog::utils::time::posix::sub_months( loctime , 1 );
						else if( textblock.get_ControlName() == "next_month_day" )
							loctime = ::booldog::utils::time::posix::add_months( loctime , 1 );
						date_picker->refresh_date_container( loctime );
						date_picker->accept_date();
					}
				}
			};
			void DatePicker::on_date_left_mouse_double_click( const ::rux::gui::events::MouseEvent& event )
			{
				on_date_left_mouse_down( event );
				XTextBlock textblock( event._sender );
				declare_variable_param( ::rux::gui::controls::XGroup , group , *textblock.get_ParentControl() );
				DatePicker* date_picker = (DatePicker*)group.get_TagPtr();
				date_picker->accept_date();
				date_picker->_date_container.RemoveControl();
				date_picker->_date_container.set_IsVisible( 0 );
				date_picker->_opened = 0;
			};
			implement_empty_content_size( DatePicker );
			void* DatePicker::DynamicCast( ::rux::int32 class_type ) const
			{
				if( class_type == ::rux::gui::XEnum_ClassType_ControlBase )
				{
					::rux::gui::ControlBase* ptr = const_cast< DatePicker* >( this );
					return ptr;
				}
				return 0;
			};
			::rux::uint8 DatePicker::get_IsGroup( void )
			{
				return 0;
			};
			::rux::uint8 DatePicker::get_IsTimePicker( void )
			{
				return 0;
			};
			::rux::uint8 DatePicker::get_IsButton( void )
			{
				return 0;
			};
			void DatePicker::on_date_enter( const ::rux::gui::events::Event& event )
			{
				XTextBlock textblock( event._sender );
				if( textblock.get_ControlName() == "month" 
					|| textblock.get_ControlName() == "year"  )
					textblock.set_Foreground( ::rux::gui::Colors::White() );
				else
				{
					declare_variable( XObject , tag );
					tag.set_ByRef( textblock.get_Tag() );
					if( rux_is_object( tag , XObject ) == true )
						textblock.set_Background( ::rux::gui::Colors::ButtonDownColor( 0.4f ) );
				}
			};
			void DatePicker::on_date_leave( const ::rux::gui::events::MouseEvent& event )
			{
				XTextBlock textblock( event._sender );
				if( textblock.get_ControlName() == "month" 
					|| textblock.get_ControlName() == "year"  )
					textblock.set_Foreground( ::rux::gui::Colors::White( 0.6f ) );
				else
				{
					declare_variable( XObject , tag );
					tag.set_ByRef( textblock.get_Tag() );
					if( rux_is_object( tag , XObject ) == false )
						textblock.set_Background( ::rux::gui::Colors::ButtonDownColor() );
					else
						textblock.set_Background( ::rux::gui::Colors::Transparent() );
				}
			};
			void DatePicker::raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event )
			{	
				if( window_event->_delta > 0 )
				{
					::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( _posix_time );
					refresh_date_container( ::booldog::utils::time::posix::add_days( loctime , 1 ) );
					accept_date();
				}
				else
				{
					::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( _posix_time );
					refresh_date_container( ::booldog::utils::time::posix::sub_days( loctime , 1 ) );
					accept_date();
				}
			};
			void DatePicker::render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 )
			{	
				::booldog::allocators::easy::heap heap;
				::booldog::allocators::single_threaded::mixed< 512 > mixed( &heap );
				{
					float __left = get_Left();
					float __top = get_Top();
					float __width = get_Width();
					float __height = get_Height();
					_cs_drawing_elements.wlock( debuginfo_macros );
					render_context->DrawRectangle( __left , __top , __width , __height , 0.f , _datepicker_background , _datepicker_border , _datepicker_thickness , _opacity * opacity , &_ui_border_cache , &_ui_cache , _selected_z_index , 1 , __FILE__ , __LINE__ , ___rux__thread_index1986 );
				
					::booldog::uint64 loctime = ::booldog::utils::time::posix::tolocal( _posix_time );
					::booldog::result_mbchar mbchar( &mixed );

					::booldog::utils::time::posix::mbs::tostring< 16 >( &mbchar , mbchar.mballocator , "%d.%m.%Y" , loctime 
						, debuginfo_macros );

					float text_width = _date_font->get_TextWidth( mbchar.mbchar , ___rux__thread_index1986 );
					float text_height = _date_font->get_Height( mbchar.mbchar , ___rux__thread_index1986 );
					float offset = 0.0;
					if( _horizontal_text_alignment == XEnum_Alignment_Center )
					{
						if( text_width < __width )
						{
							offset = ( __width - text_width ) / 2;
							__left += offset;
							__width -= offset;
						}
					}
					else if( _horizontal_text_alignment == XEnum_Alignment_RightOrBottom )
					{
						if( text_width < __width )
						{
							offset = __width - text_width;
							__left += offset;
							__width -= offset;
						}		
					}
					if( _vertical_text_alignment == XEnum_Alignment_Center )
					{		
						if( text_height < __height )
						{
							offset = ( __height - text_height ) / 2;
							__top += offset;
							__height -= offset;
						}
					}
					else if( _vertical_text_alignment == XEnum_Alignment_RightOrBottom )
					{		
						if( text_height < __height )
						{
							offset = ( __height - text_height ) / 2;
							__top += offset;
							__height -= offset;
						}
					}
					render_context->DrawText( __left , __top , __width , __height , mbchar.mbchar , _date_font , _date_foreground 
						, &_ui_date_cache , _opacity * opacity , _selected_z_index , __FILE__ , __LINE__ 
						, ___rux__thread_index1986 );
					_cs_drawing_elements.wunlock( debuginfo_macros );
				}
			};
			void DatePicker::private_ResetCache( void )
			{
			};
			::rux::uint8 DatePicker::get_IsTransparent( void )
			{
				return 0;
			};
			begin_implement_rux_base_ui_functions( DatePicker )
			end_implement_rux_base_ui_functions();
			IMPLEMENT_BASE_UI_FUNCTIONS( XDatePicker );
		};
	};
};