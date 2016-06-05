#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_tabcontrol.h>
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
begin_rux_gui_user_control_constructor_ns( TabControl , rux::gui::controls )
	_selected_index = SIZE_MAX;
	_group_main.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );

	_group_pages.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
	_group_pages.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_pages.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_pages.set_Height( 24 );

	_group_page.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
	_group_page.set_Background(  create_color( 0x88 , 0x88 , 0x88 , (uint8)( (double)0xff * 0.4 ) ) );
	_group_page.set_HorizontalFilling( ::rux::gui::XEnum_Filling_Auto );
	_group_page.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
end_rux_gui_user_control_constructor();

begin_rux_gui_user_control_initialize_ns( TabControl , rux::gui::controls )
	declare_variable( ::rux::XString , error );
	AddControl( _group_pages , error );
	AddControl( _group_page , error );
end_rux_gui_user_control_initialize();
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			TabControl::~TabControl( void )
			{
				FreeResources( 0 );
			};
			implement_duplicate_internal_function_2( TabControl , set_IsPageVisible , const ::rux::XString& , ::rux::byte );
			void TabControl::set_IsPageVisible( const ::rux::XString& name , ::rux::byte visible )
			{
				declare_variable( ::rux::XArray< ::rux::gui::controls::XGroup > , pages );
				pages.set_ByRef( _group_pages.FindAll< ::rux::gui::controls::XGroup >( name ) );
				for( size_t index0 = 0 ; index0 < pages.Count() ; index0++ )
				{
					pages[ index0 ].set_IsVisible( visible );
					if( visible == 0 )
					{
						::rux::int32 index_of = _group_pages.IndexOf( XObject( pages[ index0 ] ) );
						if( _selected_index == index_of )
							set_SelectedPage( 0 );
					}
				}
			};
			implement_duplicate_internal_function_2( TabControl , AddPage , const ::rux::XString& , ::rux::gui::controls::XGroup& );
			void TabControl::AddPage( const ::rux::XString& name , ::rux::gui::controls::XGroup& page )
			{
				size_t count = _group_pages.get_ControlsCount();
				if( count == 0 )
					_selected_index = SIZE_MAX;

				declare_variable( ::rux::gui::controls::XGroup , group_button );
				group_button.set_ControlName( name );
				group_button.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
				group_button.set_OnMouseEnter( on_group_button_enter );
				group_button.set_OnMouseLeave( on_group_button_leave );
				group_button.set_OnLeftMouseButtonDown( on_group_button_left_mouse_button_down );
				group_button.set_TagPtr( this );
				group_button.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				group_button.set_Opacity( 0.3f );
				group_button.set_Corner( ::rux::gui::Thickness( 0.f , 0.5f , 0.f , 0.5f ) );
				group_button.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
				group_button.set_Background(  create_color( 0x88 , 0x88 , 0x88 , (uint8)( (double)0xff * 0.4 ) ) );
				group_button.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );

				declare_variable( ::rux::XString , error );

				declare_variable( ::rux::gui::controls::XTextBlock , textblock );
				textblock.set_IsForwardEvents( 1 );
				textblock.set_VerticalFilling( ::rux::gui::XEnum_Filling_Auto );
				textblock.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
				textblock.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
				textblock.set_Margin( ::rux::gui::Margin( 10 , 10 , 0 , 0 ) );
				textblock.set_Text( name );

				group_button.AddControl( textblock , error );

				_group_pages.AddControl( group_button , error );

				page.set_IsVisible( 0 );
				_group_page.AddControl( page , error );
				if( count == 0 )
					set_SelectedPage( 0 );
			};
			implement_duplicate_internal_function_1( TabControl , set_SelectedPage , ::rux::gui::controls::XGroup& );
			void TabControl::set_SelectedPage( ::rux::gui::controls::XGroup& page )
			{
				::rux::int32 index = _group_page.IndexOf( page );
				if( index == -1 )
					index = _group_pages.IndexOf( page );
				if( index != -1 )
					set_SelectedPage( index );
			};
			void TabControl::set_SelectedPage( size_t page_index )
			{
				if( _selected_index != page_index )
				{
					size_t count = _group_pages.get_ControlsCount();
					if( page_index != -1 && page_index < count )
					{
						if( _selected_index < count )
						{
							declare_variable( ::rux::gui::controls::XGroup , group_button );
							group_button.set_ByRef( _group_pages.get_ControlAt( _selected_index ) );
							group_button.set_Opacity( .3f );

							declare_variable( ::rux::gui::controls::XGroup , page );
							page.set_ByRef( _group_page.get_ControlAt( _selected_index ) );
							page.set_IsVisible( 0 );
						}
						_selected_index = page_index;
						if( _selected_index < count )
						{
							declare_variable( ::rux::gui::controls::XGroup , group_button );
							group_button.set_ByRef( _group_pages.get_ControlAt( _selected_index ) );
							group_button.set_Opacity( 1.f );

							declare_variable( ::rux::gui::controls::XGroup , page );
							page.set_ByRef( _group_page.get_ControlAt( _selected_index ) );
							page.set_IsVisible( 1 );
						}
					}
				}
			};
			void TabControl::on_group_button_enter( const ::rux::gui::events::Event& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				if( group_main.get_Opacity() != 1.f )
					group_main.set_Opacity( .5f );
			};
			void TabControl::on_group_button_leave( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				if( group_main.get_Opacity() != 1.f )
					group_main.set_Opacity( .3f );
			};
			void TabControl::on_group_button_left_mouse_button_down( const ::rux::gui::events::MouseEvent& event )
			{
				::rux::gui::controls::XGroup group_main( event._sender );
				TabControl* tabcontrol = (TabControl*)group_main.get_TagPtr();
				tabcontrol->set_SelectedPage( group_main );
			};
		};
	};
};
