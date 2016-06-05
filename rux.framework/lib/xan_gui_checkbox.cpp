#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_checkbox.h>
#include <xan_keyvaluepair.h>
#include <xan_uirectangle.h>
#include <xan_uibutton.h>
#include <xan_uitextblock.h>
#include <xan_uitextbox.h>
#include <xan_int64.h>
#include <xan_uiprimitives.h>
#include <xan_uint64.h>
#include <xan_uiimage.h>
#include <xan_boolean.h>
begin_rux_gui_user_control_constructor_ns( CheckBox , rux::gui::controls )
	_group_main.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
	_group_main.set_OnMouseEnter( on_group_main_enter );
	_group_main.set_OnMouseLeave( on_group_main_leave );
	_group_main.set_OnLeftMouseButtonDown( on_group_main_left_mouse_button_down );
	_group_main.set_TagPtr( this );
	_group_main.set_Opacity( .7f );
	_group_main.set_IsForwardEventsIfChildrenDoNotRaisedEvent( 1 );
	_checked = 0;
	_tag_ptr = 0;
end_rux_gui_user_control_constructor();

begin_rux_gui_user_control_initialize_ns( CheckBox , rux::gui::controls )
	_group_symbol.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_symbol.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_Center );
	_group_symbol.set_Width( 20 );
	_group_symbol.set_Height( 20 );

	_rectangle_symbol.set_Background( ::rux::gui::Colors::Transparent() );
	_rectangle_symbol.set_BorderColor( ::rux::gui::Colors::White() );
	_rectangle_symbol.set_BorderWidth( 3 );
	_rectangle_symbol.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_rectangle_symbol.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );

	_line_checked0.set_Color( ::rux::gui::Colors::White() );
	_line_checked0.set_Thickness( 3 );
	_line_checked0.set_StartLeft( 0.25f );
	_line_checked0.set_StartTop( 0.49f );
	_line_checked0.set_EndLeft( 0.51f );
	_line_checked0.set_EndTop( 0.75f );
	_line_checked0.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );

	_line_checked1.set_Color( ::rux::gui::Colors::White() );
	_line_checked1.set_Thickness( 3 );
	_line_checked1.set_StartLeft( 0.75f );
	_line_checked1.set_StartTop( 0.25f );
	_line_checked1.set_EndLeft( 0.49f );
	_line_checked1.set_EndTop( 0.75f );
	_line_checked1.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );

	_line_checked0.set_IsVisible( _checked );
	_line_checked1.set_IsVisible( _checked );

	_group_text.set_Margin( rux::gui::Margin( 5 , 0 , 0 , 0 ) );
	_group_text.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_text.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
	_group_text.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
		
	_textblock_text.set_TextHorizontalAlignment( rux::gui::XEnum_Alignment_LeftOrTop );
	_textblock_text.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
	_textblock_text.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );

	declare_variable( ::rux::XString , error );
	_group_symbol.AddControl( _rectangle_symbol , error );

	_group_symbol.AddControl( _line_checked0 , error );
	_group_symbol.AddControl( _line_checked1 , error );

	_group_text.AddControl( _textblock_text , error );

	AddControl( _group_symbol , error );
	AddControl( _group_text , error );
end_rux_gui_user_control_initialize();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			CheckBox::~CheckBox( void )
			{
				FreeResources( 0 );
			};
			implement_duplicate_internal_result_function( ::rux::byte , CheckBox , get_Checked );
			::rux::byte CheckBox::get_Checked( void )
			{
				return _checked;
			};
			implement_duplicate_internal_function_1( CheckBox , set_OnCheckedChanged , ::rux::gui::events::on_event_t );
			void CheckBox::set_OnCheckedChanged( ::rux::gui::events::on_event_t on_checked_changed )
			{
				_on_checked_changed = on_checked_changed;
			};
			implement_duplicate_internal_function_2( CheckBox , set_Checked , ::rux::byte , ::rux::byte );
			void CheckBox::set_Checked( ::rux::byte checked , rux::byte raise_event )
			{
				if( _checked != checked )
				{
					_checked = checked;
					_line_checked0.set_IsVisible( _checked );
					_line_checked1.set_IsVisible( _checked );
					if( raise_event )
						_on_checked_changed.raise< const ::rux::gui::events::Event& >( ::rux::gui::events::Event( *this , 1 ) );
				}
			};
			void CheckBox::on_group_main_enter( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				CheckBox* check_box = (CheckBox*)group_main.get_TagPtr();
				check_box->_line_checked0.set_Color( create_color( 0x5c , 0xbf , 0xe6 , 0xff ) );
				check_box->_line_checked1.set_Color( create_color( 0x5c , 0xbf , 0xe6 , 0xff ) );
				check_box->_group_main.set_Opacity( 1.f );
			};
			void CheckBox::on_group_main_leave( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				CheckBox* check_box = (CheckBox*)group_main.get_TagPtr();
				check_box->_line_checked0.set_Color( ::rux::gui::Colors::White() );
				check_box->_line_checked1.set_Color( ::rux::gui::Colors::White() );
				check_box->_group_main.set_Opacity( .7f );
			};
			void CheckBox::on_group_main_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				CheckBox* check_box = (CheckBox*)group_main.get_TagPtr();
				check_box->set_Checked( check_box->_checked ? 0 : 1 );
			};
			void CheckBox::set_TagPtr( void* tag_ptr )
			{
				_tag_ptr = tag_ptr;
			}
			void* CheckBox::get_TagPtr( void ) const
			{
				return _tag_ptr;
			};
			implement_duplicate_internal_function_1( CheckBox , set_Text , const ::rux::XString& );
			void CheckBox::set_Text( const ::rux::XString& text )
			{
				_textblock_text.set_Text( text );
			};
			implement_duplicate_internal_function_3( CheckBox , set_Font , const char* , ::rux::uint32 , ::rux::uint32 );
			void CheckBox::set_Font( const char* font_file_name , ::rux::uint32 font_size_height , ::rux::uint32 font_size_width )
			{
				_textblock_text.set_Font( font_file_name , font_size_height , font_size_width );
			};
		};
	};
};
