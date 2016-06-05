#include <xan_window.h>
#include <xan_uiimage.h>
#include <xan_uirectangle.h>
#include <xan_image.h>
#include <xan_file.h>
#include <xan_uitextblock.h>
#include <xan_uitextbox.h>
#include <xan_uigroupanimation.h>
#include <xan_uiline.h>
#include <xan_int32.h>
#include <xan_math.h>
#include <xan_uipath.h>
#include <xan_uigroup.h>
#include <xan_uiprimitives.h>
#include <xan_uiprogressbar.h>
#include <xan_uibutton.h>
#include <xan_uiselect.h>
#include <xan_uint32.h>
#include <xan_webcamera.h>
#include <xan_time.h>
#include <xan_float.h>
#include <xan_uislider.h>
#include <xan_uiapplication.h>
#include <xan_uidatepicker.h>
#include <xan_uitimepicker.h>
#include <xan_directory.h>
#include <xan_usercontrol.h>
#include <xan_gui_listbox.h>
#include <xan_property_info.h>
#include <xan_items_virtualization.h>
#include <xan_path.h>

#include "gtest/gtest.h"
const char* g_current_module_name = "rux.ui.test";
dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
namespace rux
{
	dll_internal rux_simple_t _first_function = NULL;
	dll_internal rux_simple_t _last_function = NULL;
	dll_internal rux_simple_t _pre_initialization = NULL;
	dll_internal rux_simple_t _post_deinitialization = NULL;
};
::rux::uint32 test_index0 = 0;
void test_thread0( void* param )
{	
	::rux::gui::XWindow* window = (::rux::gui::XWindow*)param;		
	test_index0++;
	char line[ 256 ] = {0};
	sprintf( line , "try #%u" , test_index0 );
	::rux::byte found = 0;
	::rux::gui::controls::XTextBlock test_textblock0;
	test_textblock0.set_ByRef( window->FindOne< ::rux::gui::controls::XTextBlock >( "test_textblock0" , found ) );
	if( found )
		test_textblock0.set_Text( line );
};

class CodeGeneratorTest : public ::testing::Test 
{
};
TEST_F( CodeGeneratorTest , generate ) 
{
	rux::engine::initialize();
	{
		rux_add_search_directory( "../../addons" );
		rux_add_search_directory( "../../addons/png_test" );

		::rux::XString error;
		::rux::gui::XWindow window;		
		window.set_BorderWidth( 5 );
		window.set_Width( 500 );
		window.set_Height( 500 );
		window.set_StartPosition( ::rux::gui::XEnum_StartPosition_Center );
		window.set_Background( ::rux::gui::Colors::Black() );
		window.set_BorderColor( ::rux::gui::Colors::White() );

		::rux::gui::controls::XSelect test_select0;
		test_select0.set_Left( 30 );
		test_select0.set_Top( 30 );
		test_select0.set_Height( 20 );
		test_select0.set_Width( 100 );
		test_select0.set_MaxVisibleItems( 3 );
		test_select0.ItemsAdd( ::rux::XString( "0" ) );
		test_select0.ItemsAdd( ::rux::XString( "1" ) );
		test_select0.ItemsAdd( ::rux::XString( "2" ) );
		test_select0.ItemsAdd( ::rux::XString( "3" ) );
		test_select0.ItemsAdd( ::rux::XString( "4" ) );
		test_select0.ItemsAdd( ::rux::XString( "5" ) );
		test_select0.ItemsAdd( ::rux::XString( "6" ) );
		//window.AddControl( test_select0 , error );

		::rux::gui::controls::XButton exit;
		exit.set_Width( 40 );
		exit.set_Height( 20 );
		exit.set_HorizontalAlignment( ::rux::gui::XEnum_Alignment_RightOrBottom );
		exit.set_VerticalAlignment( ::rux::gui::XEnum_Alignment_LeftOrTop );
		exit.set_Content( ::rux::XString( "EXIT" ) );
		//window.AddControl( exit , error );

		::rux::gui::controls::XImage test_image0;
		test_image0.set_Width( 400 );
		test_image0.set_Height( 300 );
		test_image0.set_Left( 0 );
		test_image0.set_Top( 0 );
		rux::media::XFrame frame;		
		//frame.set_ImageData( "1.yv12" , error );
		frame.set_ImageData( "usb.png" , error );
		::rux::byte new_image = 0;
		test_image0.set_Frame( frame );									
		window.AddControl( test_image0 , error );

		::rux::gui::controls::XTextBlock test_textblock2;
		test_textblock2.set_Font( "tahoma.ttf" , 0 , 10 );
		test_textblock2.set_ControlName( "test_textblock2" );
		test_textblock2.set_Background( ::rux::gui::Colors::Black() );
		test_textblock2.set_Left( 20 );
		test_textblock2.set_Top( 20 );
		test_textblock2.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock2.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock2.set_Text( "WOWWOWOWOW" );
		//window.AddControl( test_textblock2 , error );

		::rux::gui::controls::XTextBlock test_textblock1;
		test_textblock1.set_Font( "tahoma.ttf" , 0 , 11 );
		test_textblock1.set_ControlName( "test_textblock1" );
		test_textblock1.set_Background( ::rux::gui::Colors::Black() );
		test_textblock1.set_Left( 20 );
		test_textblock1.set_Top( 45 );
		test_textblock1.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock1.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock1.set_Text( "WOWWOWOWOW" );
		//window.AddControl( test_textblock1 , error );

		::rux::gui::controls::XTextBlock test_textblock0;
		test_textblock0.set_Font( "tahoma.ttf" , 0 , 30 );
		test_textblock0.set_ControlName( "test_textblock0" );
		test_textblock0.set_Background( ::rux::gui::Colors::Black() );
		test_textblock0.set_Left( 20 );
		test_textblock0.set_Top( 70 );
		test_textblock0.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock0.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
		test_textblock0.set_Text( "WOWWOWOWOW" );
		//window.AddControl( test_textblock0 , error );		

		::rux::threading::XThread thread;
		thread.set_Timeout( 1000 );
		thread.set_ThreadMode( XThreadMode_Repeat );
		thread.Start();
		thread.Push( test_thread0 , &window );

		window.Show();
	}
	rux::engine::deinitialize();
};
int main( int argc , char **argv )
{
    ::testing::InitGoogleTest( &argc , argv );
    return RUN_ALL_TESTS();
};











//class TestItemsVirtualization : public ::rux::gui::ItemsVirtualization
//{
//public:
//	virtual size_t Count( void )
//	{
//		::rux::threading::XThread::Sleep( 2000 );
//		return 30000;
//	};
//	virtual ::rux::Array< ::rux::gui::XRow >& Get( size_t offset , size_t limit )
//	{
//		::rux::threading::XThread::Sleep( 5000 );
//		::rux::XArray< ::rux::gui::XRow > events;
//		size_t count = Count();
//		for( size_t index0 = offset ; index0 < offset + limit ; index0++ )
//		{
//			if( index0 >= count )
//				break;
//			rux::gui::XRowCell row_cell0;
//			row_cell0.set_ColumnName( "date" );
//			::rux::XString val;
//			val.set_ByRef( ::rux::XString( "2014-07-" ) + ::rux::XUInt32( index0 + 1 ).ToString() );
//			row_cell0.set_ColumnValue( val );
//
//			rux::gui::XRowCell row_cell1;
//			row_cell1.set_ColumnName( "time" );
//			val.set_ByRef( ::rux::XString( "23:00:54" ) + ::rux::XUInt32( index0 + 1 ).ToString() );
//			row_cell1.set_ColumnValue( val );
//
//			rux::gui::XRow row0;
//			row0.AddCell( row_cell0 );
//			row0.AddCell( row_cell1 );
//
//			events.Add( row0 );
//		}
//		return events++;
//	};
//};
//
//using namespace rux;
//using namespace ::rux::gui;
//using namespace ::rux::gui::controls;
//#define MAKEFCC(ch0, ch1, ch2, ch3)                              \
//                ((uint32)(uint8)(ch0) | ((uint32)(uint8)(ch1) << 8) |   \
//                ((uint32)(uint8)(ch2) << 16) | ((uint32)(uint8)(ch3) << 24 ))
//#define FCC_BGR3 MAKEFCC('B','G','R','3')
//
//rux::Array< ::rux::XGridPosition >& generate_set_positions( int8 grid , ::rux::uint32 rows , ::rux::uint32 cols )
//{
//	XArray< ::rux::XGridPosition > positions;
//	if( rows > 8 )
//	{
//		rows = 8;
//	}
//	if( cols > 8 )
//	{
//		cols = 8;
//	}
//	::rux::uint32 cameracount = 0;	
//	::rux::uint32 index = 0 , index2 = 0;
//	::rux::uint32 position_rows = 0;
//	::rux::uint32 position_cols = 0;
//	::rux::uint32 position_x = 0;
//	::rux::uint32 position_y = 0;
//	::rux::uint32 position_dx = 0;
//	::rux::uint32 position_dy = 0;
//	if( grid == 0 )
//	{
//		if( rows < 3 )
//		{
//			rows = 3;
//			cols = rows;
//		}
//		if( cols < 3 )
//		{
//			cols = 3;
//			rows = cols;
//		}
//		if( rows != cols )
//		{
//			cols = rows;
//		}
//		cameracount = 0;		
//		position_rows = rows;
//		position_cols = cols;
//		position_x = 1;
//		position_y = 1;
//		position_dx = rows - 1;
//		position_dy = cols - 1;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;
//		for( ; cameracount - 1 < rows ; )
//		{			
//			position_x = cameracount;
//			position_y = cols;
//			position_dx = 1;
//			position_dy = 1;		
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}		
//		for( ; cameracount - 1 - rows < cols - 1 ; )
//		{			
//			position_x = rows;
//			position_y = cameracount - rows;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}		
//	}
//	else if( grid == 1 )
//	{
//		rows = 4;
//		cols = 4;
//		cameracount = 0;
//		position_rows = 2;
//		position_cols = 2;
//		position_x = 1;
//		position_y = 1;
//		position_dx = 1;
//		position_dy = 1;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;			
//		position_x = 2;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;
//		for( ; cameracount - 2 < cols ; )
//		{
//			position_rows = 4;
//			position_cols = 4;
//			position_x = cameracount - 1;
//			position_y = 3;				
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//		for( ; cameracount - 2 - cols < cols ; )
//		{			
//			position_x = cameracount - 1 - cols;
//			position_y = 4;			
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//	}
//	else if( grid == 2 )
//	{		
//		cameracount = 0;
//		position_rows = 2;
//		position_cols = 2;
//		position_x = 1;
//		position_y = 1;
//		position_dx = 1;
//		position_dy = 1;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;			
//		position_x = 2;		
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;		
//		position_x = 1;
//		position_y = 2;		
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;		
//		for( ; cameracount - 3 < 2 ; )
//		{
//			position_rows = 4;
//			position_cols = 4;
//			position_x = cameracount;
//			position_y = 3;			
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//		for( ; cameracount - 5 < 2 ; )
//		{			
//			position_x = cameracount - 2;
//			position_y = 4;			
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}		
//	}
//	else if( grid == 3 )
//	{
//		if( rows < 4 )
//		{
//			rows = 4;
//		}
//		if( cols < 4 )
//		{
//			cols = 4;
//		}
//		if( rows != cols )
//		{
//			rows = cols;
//		}
//		position_rows = rows;
//		position_cols = cols;
//		position_x = 2;
//		position_y = 2;
//		position_dx = rows - 2;
//		position_dy = cols - 2;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;						
//		for( ; cameracount - 1 < rows ; )
//		{			
//			position_x = cameracount ;
//			position_y = 1;	
//			position_dx = 1;
//			position_dy = 1;		
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//		for( ; cameracount - 1 - rows < rows ; )
//		{			
//			position_x = cameracount - rows;
//			position_y = rows;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//		for( ; cameracount - 1 - 2*rows < cols - 2 ; )
//		{			
//			position_x = 1;
//			position_y = 1 + cameracount - 2*rows;			
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}
//		for( ; cameracount + 1 - 2*rows - cols < cols - 2 ; )
//		{					
//			position_x = cols;
//			position_y = 3 + cameracount - 2*rows - cols;						
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;	
//		}		
//	}
//	else if( grid == 4 )
//	{		
//		position_rows = 6;
//		position_cols = 6;
//		position_x = 2;
//		position_y = 2;
//		position_dx = 2;
//		position_dy = 2;
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;								
//		position_x = 4;
//		position_y = 2;		
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;								
//		position_x = 2;
//		position_y = 4;		
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;								
//		position_x = 4;
//		position_y = 4;		
//		positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//		cameracount++;						
//		for( ; cameracount - 4 < 6 ; )
//		{
//			position_x = cameracount - 3;
//			position_y = 1;		
//			position_dx = 1;
//			position_dy = 1;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}		
//		for( ; cameracount - 10 < 6 ; )
//		{
//			position_x = cameracount - 9;
//			position_y = 6;		
//			position_dx = 1;
//			position_dy = 1;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}	
//		for( ; cameracount - 16 < 4 ; )
//		{
//			position_x = 1;
//			position_y = cameracount - 14;		
//			position_dx = 1;
//			position_dy = 1;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}	
//		for( ; cameracount - 20 < 4 ; )
//		{
//			position_x = 6;
//			position_y = cameracount - 18;		
//			position_dx = 1;
//			position_dy = 1;
//			positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//			cameracount++;
//		}	
//	}
//	else if( grid == 5 )
//	{
//		cameracount = 0;		
//		if( cols == rows )
//		{
//			position_rows = cols;
//			position_cols = rows;
//			index2 = 0;
//			for( ; index2 < cols ; )
//			{
//				position_x = index2 + 1;
//				position_dx = 1;
//				position_dy = 1;
//				index = 0;
//				for( ; index < rows ; )
//				{
//					position_y = index + 1;
//					index++;
//					positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//					cameracount++;
//				}
//				index2++;
//			}
//		}
//		else if( cols > rows )
//		{
//			position_rows = cols*2;
//			position_cols = cols;
//			index2 = 0;
//			for( ; index2 < cols ; )
//			{
//				position_x = index2 + 1;
//				position_dx = 1;
//				position_dy = 2;
//				index = 0;
//				for( ; index < rows ; )
//				{
//					position_y = 1 + index*2 + cols - rows;
//					index++;
//					positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//					cameracount++;
//				}
//				index2++;
//			}
//		}	
//		else
//		{
//			position_rows = rows;
//			position_cols = rows*2;
//			index2 = 0;
//			for( ; index2 < rows ; )
//			{
//				position_y = index2 + 1;
//				position_dx = 2;
//				position_dy = 1;
//				index = 0;
//				for( ; index < cols ; )
//				{
//					position_x = 1 + index*2 + rows - cols;
//					index++;
//					positions.Add( ::rux::XGridPosition( position_cols , position_rows , position_x , position_y , position_dx , position_dy ) );
//					cameracount++;
//				}
//				index2++;
//			}
//		}	
//	}
//	return positions++;
//};
//const char* g_current_module_name = "rux.ui.test.exe";
//dll_internal ::rux::uint8 _rux_is_memory_manager = 0;
//namespace rux
//{
//	dll_internal rux_simple_t _pre_initialization = NULL;
//	dll_internal rux_simple_t _post_deinitialization = NULL;
//};
//void on_mouse_down( const XObject& sender , ::rux::int16 , ::rux::int16 )
//{
//	rux::gui::XWindow window( sender );
//	//XUIApplication::Quit();
//	//window.ShowTimeoutMessage( "HELLO WORLD!!!" , 500000ULL , 500000ULL );
//};
//void on_mouse_move( const XObject& , ::rux::int16 , ::rux::int16 , ::rux::uint8 )
//{
//	//XInt32 x_int32( x ) , y_int32( y );
//	//ui_text_block.set_Text( "x=" + x_int32.ToString() + " , y=" + y_int32.ToString() );
//};
//void on_show_time_message_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	window.ShowTimeoutMessage( "HELLO WORLD!!!" , 500000ULL , 500000ULL );
//}
//void on_listbox_increase_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	::rux::XArray< ::rux::gui::controls::XListBox > lisboxes;
//	lisboxes.set_ByRef( window.Find< ::rux::gui::controls::XListBox >() );
//	if( lisboxes.Count() > 0 )
//	{
//		if( lisboxes[ 0 ].get_Height() > 400 )
//			lisboxes[ 0 ].set_Height( 50 );
//		else
//			lisboxes[ 0 ].set_Height( 500 );
//	}
//};
//void on_quit_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	window.Close();
//};
//uint64 _fps_start_time;
//void web_on_frame( ::rux::media::WebCamera* , const ::rux::media::XFrame& )
//{
//	const char* filename = "/tmp/frame.yuyv";
//	rux::XString error;
//	if( ::rux::io::XFile::Exists( filename ) == 1 )
//		rux::io::XFile::Delete( filename , error );
//	//frame.SaveToFile( "/tmp/frame" , error );
//	//_web_ui_image.set_FrameData( (char*)frame.get_ImageData() , frame.get_ImageWidth() , frame.get_ImageHeight() , (XEnum_Fourcc)frame.get_Fourcc() );
//	/*_frames_per_sec += 1;
//	::rux::uint64 now = ::rux::XTime::get_NowAsWindowsTime();	
//	if( _fps_start_time == 0 || ( now - _fps_start_time ) >= 1000ULL * 10000ULL )
//	{
//		_fps_textblock.set_Text( _frames_per_sec.ToString() );
//		_frames_per_sec = 0;
//		_fps_start_time = now;
//	}*/
//};
//void on_speed_value_changed( const XObject& sender , float , float new_value )
//{
//	XSlider slider( sender );
//	declare_variable_param( ::rux::gui::controls::XGroup , parent , *slider.get_ParentControl() );
//	XArray< XTextBlock > textblocks;
//	textblocks.set_ByRef( parent.Find< XTextBlock >() );
//	if( textblocks.Count() > 0 )
//		textblocks[ 0 ].set_Text( XInt32( new_value ).ToString() + "x" );
//};
//void on_button_mouse_enter( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	XArray< XGroup > groups;
//	groups.set_ByRef( window.FindAll< XGroup >( "speed" ) );
//	if( groups.Count() > 0 && groups[ 0 ].get_IsVisible() == 0 )
//	{		
//		XGroupAnimation group_animation( groups[ 0 ].get_Tag() );
//		groups[ 0 ].set_Height( 0 );
//		groups[ 0 ].set_BottomSticker( button.get_Top() );
//		groups[ 0 ].set_Left( button.get_Left() );
//		groups[ 0 ].set_Width( button.get_Width() );
//		window.BringToFront( groups[ 0 ] );
//		groups[ 0 ].set_IsVisible( 1 );
//		group_animation.Start();		
//	}
//
//	if( button.get_ControlName() == "mute" )
//		button.set_ContentBackground( "bullhorn" , create_color( 255 , 255 , 255 , 255 ) );
//	else
//		button.set_ContentBackground( create_color( 255 , 255 , 255 , 255 ) );
//};
//void on_slider_value_changed( const XObject& sender , float , float new_value )
//{
//	XSlider slider( sender );
//	rux::gui::XWindow parent( slider.get_ParentWindow() );
//	XArray< XImage > images;
//	images.set_ByRef( parent.FindAll< XImage >() );
//	if( slider.get_ControlName() == "brightness" )
//	{
//		for( ::rux::uint32 index0 = 0 ; index0 < images.Count() ; index0++ )
//			images[ index0 ].set_Brightness( new_value );
//	}
//	else if( slider.get_ControlName() == "contrast" )
//	{
//		for( ::rux::uint32 index0 = 0 ; index0 < images.Count() ; index0++ )
//			images[ index0 ].set_Contrast( new_value );
//	}
//};
//void on_button_mouse_leave( const XObject& sender , ::rux::int16 x , ::rux::int16 y )
//{
//	rux::gui::controls::XButton button( sender );
//
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	XArray< XGroup > groups;
//	groups.set_ByRef( window.FindAll< XGroup >( "speed" ) );
//	if( groups.Count() > 0 )
//	{
//		if( groups[ 0 ].IsPointInControl( x , y ) == 0 )
//		{			
//			XGroupAnimation group_animation( groups[ 0 ].get_Tag() );		
//			group_animation.Stop();
//			groups[ 0 ].set_IsVisible( 0 );
//		}
//	}
//	if( button.get_ControlName() == "mute" )
//		button.set_ContentBackground( "bullhorn" , create_color( 255 , 255 , 255 , (uint8)( 255.f * 0.7f ) ) );
//	else
//		button.set_ContentBackground( create_color( 255 , 255 , 255 , (uint8)( 255.f * 0.7f ) ) );
//};
//uint32 _first_shown = 1;
//XMallocArray< Window* > _few_windows;
//void on_few_windows_shown( const XObject& )
//{
//	if( _first_shown == 1 )
//	{
//		_first_shown = 0;
//		for( ::rux::uint32 index0 = 0 ; index0 < 10 ; index0++ )
//		{
//			//XThread::Sleep( 3000 );
//			rux::gui::XWindow _window;
//			_window.set_Background( create_color( 0x45 , 0x41 , 0x3c , 255 ) );
//			rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
//			linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Horizontal );
//			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0 , 128 , 0 , 255 ) , 0.0 ) );
//			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 255 , 0 , 0 , 255 ) , 0.5 ) );
//			linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0 , 0 , 255 , 255 ) , 1.0 ) );		
//			_window.set_Background( linear_gradient );
//			rux::gui::ColorBase* color0;
//			color0 = ::rux::gui::Colors::WindowBackgroundColor();
//			_window.set_Background( color0 );
//			_window.set_Background( ::rux::gui::Colors::White() );
//			_window.set_BorderWidth( 2.0 );
//			_window.set_BorderColor( ::rux::gui::Colors::CellBorderColor() );
//			_window.set_Left( 20 );
//			_window.set_Top( 20 );
//			_window.set_Width( 400 );
//			_window.set_Height( 400 );
//			_window()->AddRef();
//			_few_windows.Add( _window() );
//			_window.Show();
//		}
//	}
//};
//
//
//uint8 _is_snap_to_grid = 0;
//uint32 _cols_rows_count = 10;
//uint8 _save_cpp = 1;
//uint8 _path_designer_mode = 0;
//rux::gui::controls::Rectangle* _path_designer_active_point = NULL;
//void update_preview_window( XWindow& preview_window , XMallocArray< float >& relative_x , XMallocArray< float >& relative_y )
//{
//	::rux::uint8 found = 0;
//	declare_variable( ::rux::gui::controls::XGroup , preview_group );
//	preview_group = preview_window.FindOne< XGroup >( "preview_group" , found );
//	if( found == 1 )
//	{
//		XArray< ::rux::gui::controls::XPath > paths;
//		paths.set_ByRef( preview_group.FindAll< ::rux::gui::controls::XPath >() );
//		for( ::rux::uint32 index0 = 0 ; index0 < paths.Count() ; index0++ )
//			preview_group.RemoveControl( paths[ index0 ] );
//		rux::gui::controls::XPath path;
//		for( ::rux::uint32 index0 = 0 ; index0 < relative_x.Count() ; index0++ )
//			path.AddPoint( ::rux::gui::Point( relative_x[ index0 ] , relative_y[ index0 ] , ::rux::gui::XEnum_PointUnit_Percent ) );
//		path.set_Left( 0 );
//		path.set_Top( 0 );
//		path.set_Width( 1 );
//		path.set_Height( 1 );
//		path.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//		path.set_Background( ::rux::gui::Colors::Red() );
//		rux::XString error;
//		preview_group.AddControl( path , error );
//	}
//};
//void on_path_designer_point_conture_mouse_enter( const XObject& sender )
//{
//	rux::gui::controls::XLine line( sender );
//	line.set_Thickness( 3.f );
//};
//void on_path_designer_point_conture_mouse_leave( const XObject& sender , ::rux::int16 , ::rux::int16 )
//{
//	rux::gui::controls::XLine line( sender );
//	line.set_Thickness( 1.f );
//};
//void on_path_designer_point_conture_left_mouse_button_down( const XObject& sender , ::rux::int16 x , ::rux::int16 y );
//void update_preview_window_from_painted( ::rux::gui::controls::XGroup& paint_group )
//{
//	XArray< ::rux::gui::controls::XRectangle > points;
//	points.set_ByRef( paint_group.FindAll< ::rux::gui::controls::XRectangle >( "point" ) );
//	if( points.Count() > 2 )
//	{
//		rux::gui::XWindow window( paint_group.get_ParentWindow() );
//		rux::XString error;
//		XArray< XLine > lines;
//		lines.set_ByRef( paint_group.FindAll< XLine >() );
//		for( ::rux::uint32 index0 = 0 ; index0 < lines.Count() ; index0++ )
//			paint_group.RemoveControl( lines[ index0 ] );
//		XMallocArray< float > relative_x , relative_y;
//		rux::gui::controls::XRectangle start_point , end_point;
//		for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//		{
//			relative_x.Add( ( points[ index0 ].get_Left() - paint_group.get_Left() + points[ index0 ].get_Width() / 2.f ) / paint_group.get_Width() );
//			relative_y.Add( ( points[ index0 ].get_Top() - paint_group.get_Top() + points[ index0 ].get_Height() / 2.f ) / paint_group.get_Height() );
//			if( index0 + 1 < points.Count() )
//			{
//				start_point = points[ index0 ];
//				end_point = points[ index0 + 1 ];
//			}
//			else
//			{
//				start_point = points[ index0 ];
//				end_point = points[ 0 ];				
//			}	
//			rux::gui::controls::XLine line;
//			line.set_Tag( XUInt32( index0 ) );
//			line()->set_IsForwardLeftMouseButtonDownUpEvent( 0 );
//			line()->set_IsForwardMouseMoveEnterLeaveEvent( 0 );
//			line.set_OnLeftMouseButtonDown( on_path_designer_point_conture_left_mouse_button_down );
//			line.set_OnMouseEnter( on_path_designer_point_conture_mouse_enter );
//			line.set_OnMouseLeave( on_path_designer_point_conture_mouse_leave );
//			line.set_StartLeft( start_point.get_Left() - paint_group.get_Left() + start_point.get_Width() / 2.f );
//			line.set_StartTop( start_point.get_Top() - paint_group.get_Top() + start_point.get_Height() / 2.f );
//			line.set_EndLeft( end_point.get_Left() - paint_group.get_Left() + end_point.get_Width() / 2.f );
//			line.set_EndTop( end_point.get_Top() - paint_group.get_Top() + end_point.get_Height() / 2.f );
//			line.set_Thickness( 1.f );
//			line.set_Color( ::rux::gui::Colors::Red() );
//			paint_group.AddControl( line , error );			
//		}
//		for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//			paint_group.BringToFront( points[ index0 ] );
//		rux::gui::XWindow preview_window( ( *(rux::gui::Window*)paint_group.get_TagPtr() ) );
//		preview_window.Show();
//		update_preview_window( preview_window , relative_x , relative_y );
//	}
//};
//void path_designer_correct_coords( XGroup&paint_group , ::rux::int16& x , ::rux::int16& y )
//{
//	if( _is_snap_to_grid == 1 )
//	{
//		float cell_width = paint_group.get_Width() / (float)_cols_rows_count;
//		float cell_height = paint_group.get_Height() / (float)_cols_rows_count;
//		float cell_x = 0.f;
//		float cell_y = paint_group.get_Top();		
//		::rux::uint32 col = 0;
//		for( ::rux::uint32 row = 0 ; row < _cols_rows_count ; row++ )
//		{	
//			cell_x = paint_group.get_Left();
//			for( col = 0 ; col < _cols_rows_count ; col++ )
//			{
//				if( cell_x <= x && cell_x + cell_width >= x
//					&& cell_y <= y && cell_y + cell_height >= y )
//				{
//					x = ( cell_x + cell_width / 2.f );
//					y = ( cell_y + cell_height / 2.f );
//					break;
//				}
//				cell_x += cell_width;
//			}
//			if( col < _cols_rows_count )
//				break;
//			cell_y += cell_height;
//		}
//	}	
//};
//void path_designer_correct_coords( XGroup&paint_group , float& x , float& y )
//{
//	if( _is_snap_to_grid == 1 )
//	{
//		float cell_width = paint_group.get_Width() / (float)_cols_rows_count;
//		float cell_height = paint_group.get_Height() / (float)_cols_rows_count;
//		float cell_x = 0.f;
//		float cell_y = paint_group.get_Top();		
//		::rux::uint32 col = 0;
//		for( ::rux::uint32 row = 0 ; row < _cols_rows_count ; row++ )
//		{	
//			cell_x = paint_group.get_Left();
//			for( col = 0 ; col < _cols_rows_count ; col++ )
//			{
//				if( cell_x <= x && cell_x + cell_width >= x
//					&& cell_y <= y && cell_y + cell_height >= y )
//				{
//					x = ( cell_x + cell_width / 2.f );
//					y = ( cell_y + cell_height / 2.f );
//					break;
//				}
//				cell_x += cell_width;
//			}
//			if( col < _cols_rows_count )
//				break;
//			cell_y += cell_height;
//		}
//	}	
//};
//void on_path_designer_point_mouse_left_mouse_button_down( const XObject& sender , ::rux::int16 , ::rux::int16 )
//{
//	rux::gui::controls::XRectangle point( sender );
//	rux::gui::XWindow window( point.get_ParentWindow() );
//
//	_path_designer_active_point = point();
//
//	rux::XString error;
//	window.StartDrag( point , error );
//};
//void on_path_designer_point_mouse_left_mouse_button_up( const XObject& sender , ::rux::int16 , ::rux::int16 )
//{
//	rux::gui::controls::XRectangle point( sender );
//	rux::gui::XWindow window( point.get_ParentWindow() );
//	window.StopDrag();
//	declare_variable_param( ::rux::gui::controls::XGroup , paint_group , *point.get_ParentControl() );
//	float left = point.get_Left();
//	float top = point.get_Top();
//	path_designer_correct_coords( paint_group , left , top );	
//	if( point.get_Left() != left || point.get_Top() != top )
//	{
//		point.set_Left( left - paint_group.get_Left() - point.get_Width() / 2.f );	
//		point.set_Top( top - paint_group.get_Top() - point.get_Height() / 2.f );
//	}
//	update_preview_window_from_painted( paint_group );
//};
//Window& create_preview_window( void )
//{
//	float offset = 3.f;
//	rux::gui::XWindow window;	
//	window.set_Background( ::rux::gui::Colors::White() );
//	window.set_BorderWidth( 2.0 );
//	window.set_BorderColor( ::rux::gui::Colors::CellBorderColor() );
//	window.set_Left( 430 );
//	window.set_Top( 20 );
//	window.set_Width( 150 );	
//	rux::XString error;
//	declare_variable( ::rux::gui::controls::XGroup , menu_group );
//	menu_group.set_HorizontalFilling( XEnum_Filling_Auto );
//	menu_group.set_Height( 30 );
//	rux::gui::controls::XRectangle menu_group_back;
//	menu_group_back.set_HorizontalFilling( XEnum_Filling_Auto );
//	menu_group_back.set_VerticalFilling( XEnum_Filling_Auto );
//	menu_group_back.set_Background( ::rux::gui::Colors::WindowHeaderColor() );
//	menu_group.AddControl( menu_group_back , error );
//	window.set_Height( window.get_Width() + menu_group.get_Height() );	
//	window.AddControl( menu_group , error );	
//	declare_variable( ::rux::gui::controls::XGroup , paint_group );	
//	paint_group.set_ControlName( "preview_group" );
//	paint_group.set_Top( menu_group.get_Height() + offset );
//	paint_group.set_Left( offset );
//	paint_group.set_Width( window.get_Width() - 2 * offset );
//	paint_group.set_Height( window.get_Height() - menu_group.get_Height() - 2 * offset );
//	rux::gui::controls::XRectangle paint_group_back;
//	paint_group_back()->set_IsForwardLeftMouseButtonDownUpEvent( 1 );
//	paint_group_back()->set_IsForwardLeftMouseDoubleClickEvent( 1 );
//	paint_group_back()->set_IsForwardMouseMoveEnterLeaveEvent( 1 );
//	paint_group_back()->set_IsForwardMouseWheelEvent( 1 );
//	paint_group_back.set_HorizontalFilling( XEnum_Filling_Auto );
//	paint_group_back.set_VerticalFilling( XEnum_Filling_Auto );
//	paint_group_back.set_Background( ::rux::gui::Colors::WindowBackgroundColor() );
//	paint_group.AddControl( paint_group_back , error );
//	window.AddControl( paint_group , error );
//	return window++;
//};
//void path_designer_update_grid( XWindow& window )
//{
//	::rux::uint8 found = 1;
//	declare_variable_param( ::rux::gui::controls::XGroup , paint_group , window.FindOne< XGroup >( "paint_group" , found ) );
//	if( found == 1 )
//	{
//		rux::XString error;
//		declare_variable_param( ::rux::gui::controls::XGroup , grid_group , paint_group.FindOne< XGroup >( "grid_group" , found ) );
//		if( found == 0 )
//		{
//			grid_group.set_ControlName( "grid_group" );
//			grid_group()->set_IsForwardLeftMouseButtonDownUpEvent( 1 );
//			grid_group()->set_IsForwardLeftMouseDoubleClickEvent( 1 );
//			grid_group()->set_IsForwardMouseMoveEnterLeaveEvent( 1 );
//			grid_group()->set_IsForwardMouseWheelEvent( 1 );
//			grid_group.set_HorizontalFilling( XEnum_Filling_Auto );
//			grid_group.set_VerticalFilling( XEnum_Filling_Auto );
//			paint_group.AddControl( grid_group , error );
//		}			
//		grid_group.ClearControls();
//		float width_and_height = 1.f / (float)_cols_rows_count;
//		float xy_offset = width_and_height;
//		for( ::rux::uint32 index0 = 0 ; index0 < _cols_rows_count - 1 ; index0++ )
//		{				
//			rux::gui::controls::XLine horizontal_line;
//			horizontal_line.set_Color( ::rux::gui::Colors::Grey() );
//			horizontal_line.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			horizontal_line.set_Thickness( 1.f );
//			horizontal_line.set_StartLeft( 0.f );
//			horizontal_line.set_StartTop( xy_offset );
//			horizontal_line.set_EndLeft( 1.f );
//			horizontal_line.set_EndTop( xy_offset );
//			grid_group.AddControl( horizontal_line , error );
//			rux::gui::controls::XLine vertical_line;
//			vertical_line.set_Color( ::rux::gui::Colors::Grey() );
//			vertical_line.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			vertical_line.set_Thickness( 1.f );
//			vertical_line.set_StartLeft( xy_offset );
//			vertical_line.set_StartTop( 0.f );
//			vertical_line.set_EndLeft( xy_offset );
//			vertical_line.set_EndTop( 1.f );
//			grid_group.AddControl( vertical_line , error );
//			xy_offset += width_and_height;
//		}		
//		rux::gui::controls::XLine horizontal_line;
//		horizontal_line.set_Color( ::rux::gui::Colors::Green() );
//		horizontal_line.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//		horizontal_line.set_Thickness( 1.f );
//		horizontal_line.set_StartLeft( 0.f );
//		horizontal_line.set_StartTop( 0.5f );
//		horizontal_line.set_EndLeft( 1.f );
//		horizontal_line.set_EndTop( 0.5f );
//		grid_group.AddControl( horizontal_line , error );
//		rux::gui::controls::XLine vertical_line;
//		vertical_line.set_Color( ::rux::gui::Colors::Green() );
//		vertical_line.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//		vertical_line.set_Thickness( 1.f );
//		vertical_line.set_StartLeft( 0.5f );
//		vertical_line.set_StartTop( 0.f );
//		vertical_line.set_EndLeft( 0.5f );
//		vertical_line.set_EndTop( 1.f );
//		grid_group.AddControl( vertical_line , error );
//	}
//};
//void on_path_designer_grid_row_add_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	_cols_rows_count++;
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	path_designer_update_grid( window );
//};
//void on_path_designer_grid_row_remove_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	_cols_rows_count--;
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	path_designer_update_grid( window );
//};
//void on_path_designer_remove_point_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	if( _path_designer_active_point )
//	{
//		rux::gui::XWindow window( button.get_ParentWindow() );
//		::rux::uint8 found = 1;
//		declare_variable_param( ::rux::gui::controls::XGroup , paint_group , window.FindOne< XGroup >( "paint_group" , found ) );
//		if( found == 1 )
//		{
//			XObject control( *_path_designer_active_point );
//			paint_group.RemoveControl( control );
//			update_preview_window_from_painted( paint_group );
//		}
//		_path_designer_active_point = NULL;
//	}
//};
//void on_path_mode_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	_path_designer_mode++;
//	if( _path_designer_mode > 1 )
//		_path_designer_mode = 0;
//	if( _path_designer_mode == 0 )
//		button.set_Content( ::rux::XString( "Point" ) );
//	else if( _path_designer_mode == 1 )
//		button.set_Content( ::rux::XString( "Circle" ) );
//};
//void on_path_designer_bind_unbind_to_grid_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	_is_snap_to_grid = _is_snap_to_grid == 1 ? 0 : 1;
//	if( _is_snap_to_grid == 1 )
//		button.set_Content( ::rux::XString( "Unbind to grid" ) );
//	else
//		button.set_Content( ::rux::XString( "Bind to grid" ) );
//};
//void on_path_designer_save_click( const XObject& sender )
//{
//	rux::gui::controls::XButton button( sender );
//	rux::gui::XWindow window( button.get_ParentWindow() );
//	::rux::uint8 found = 0;
//	declare_variable( ::rux::gui::controls::XGroup , paint_group );
//	paint_group = window.FindOne< XGroup >( "paint_group" , found );
//	if( found == 1 )
//	{
//		XArray< ::rux::gui::controls::XRectangle > points;
//		points.set_ByRef( paint_group.FindAll< ::rux::gui::controls::XRectangle >( "point" ) );
//		if( points.Count() > 2 )
//		{
//			rux::XString save_cpp;
//			if( _save_cpp == 1 )
//			{
//				save_cpp += "XPath path;\n";
//				for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//				{
//					save_cpp += "path.AddPoint( ::rux::gui::Point( " + XFloat( ( points[ index0 ].get_Left() - paint_group.get_Left() + points[ index0 ].get_Width() / 2.f ) / paint_group.get_Width() ).ToString() 
//						+ " , " + XFloat( ( points[ index0 ].get_Top() - paint_group.get_Top() + points[ index0 ].get_Height() / 2.f ) / paint_group.get_Height() ).ToString() + " , ::rux::gui::XEnum_PointUnit_Percent ) );\n";
//				}
//				save_cpp += "path.set_Left( 0 );\n";
//				save_cpp += "path.set_Top( 0 );\n";
//				save_cpp += "path.set_Width( 1 );\n";
//				save_cpp += "path.set_Height( 1 );\n";
//				save_cpp += "path.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );\n";
//				save_cpp += "path.set_Background( ::rux::gui::Color::Red() );\n";
//			}
//			else
//			{
//				rux::data::XDataObject object;
//				object.SetValue( "control" , ::rux::XString( "path" ) );
//				object.SetValue( "left" , XDouble( 0 ) );
//				object.SetValue( "top" , XDouble( 0 ) );
//				object.SetValue( "width" , XDouble( 1 ) );
//				object.SetValue( "height" , XDouble( 1 ) );
//				object.SetValue( "point_unit" , ::rux::XString( "percent" ) );
//				object.SetValue( "background" , ::rux::XString( "#ffff0000" ) );
//				for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//				{
//					rux::data::XDataObject point;
//					object.SetValue( "x" , XDouble( ( points[ index0 ].get_Left() - paint_group.get_Left() + points[ index0 ].get_Width() / 2.f ) / paint_group.get_Width() ) );
//					object.SetValue( "y" , XDouble( ( points[ index0 ].get_Top() - paint_group.get_Top() + points[ index0 ].get_Height() / 2.f ) / paint_group.get_Height() ) );
//					object.SetValue( "point_unit" , ::rux::XString( "percent" ) );
//					object.AddValueToArray( "points" , point );
//				}
//				rux::XString error;
//				save_cpp.set_ByRef( ::rux::data::XJsonSerializer::Serialize( object , 1 , error ) );
//			}
//			declare_stack_variable( char , filename , 1024 );
//			rux_get_executable_directory( filename );
//			rux_concatenation( filename , "/path_designer.txt" );
//			rux_clear_and_write_to_file( filename , save_cpp()->_array_ptr );		
//		}
//	}
//};
//void on_path_designer_point_conture_left_mouse_button_down( const XObject& sender , ::rux::int16 x , ::rux::int16 y )
//{
//	rux::gui::controls::XLine line( sender );
//	XUInt32 index( line.get_Tag() );
//	declare_variable_param( ::rux::gui::controls::XGroup , paint_group , *line.get_ParentControl() );
//	XArray< ::rux::gui::controls::XRectangle > points;
//	points.set_ByRef( paint_group.FindAll< ::rux::gui::controls::XRectangle >( "point" ) );
//	for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//		paint_group.RemoveControl( points[ index0 ] );
//	rux::gui::controls::XRectangle point;
//	point.set_ControlName( "point" );
//	point.set_Width( 5 );
//	point.set_Height( 5 );
//	point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//	point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//	point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//	point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//	point.set_Left( x - paint_group.get_Left() - point.get_Width() / 2.f );
//	point.set_Top( y - paint_group.get_Top() - point.get_Height() / 2.f );
//	if( points.Count() - 1 == index() )
//		points.Insert( 0 , point );
//	else
//		points.Insert( index() + 1 , point );	
//	rux::XString error;
//	for( ::rux::uint32 index0 = 0 ; index0 < points.Count() ; index0++ )
//		paint_group.AddControl( points[ index0 ] , error );
//	update_preview_window_from_painted( paint_group );	
//};
//void on_paint_group_left_mouse_button_down( const XObject& sender , ::rux::int16 x , ::rux::int16 y )
//{
//	declare_variable_param( ::rux::gui::controls::XGroup , paint_group , sender );
//	path_designer_correct_coords( paint_group , x , y );	
//	if( _path_designer_mode == 0 )//Point
//	{
//		rux::gui::controls::XRectangle point;
//		point.set_ControlName( "point" );
//		point.set_Width( 5 );
//		point.set_Height( 5 );
//		point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//		point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//		point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//		point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//		point.set_Left( x - paint_group.get_Left() - point.get_Width() / 2.f );
//		point.set_Top( y - paint_group.get_Top() - point.get_Height() / 2.f );
//		rux::XString error;
//		paint_group.AddControl( point , error );
//		update_preview_window_from_painted( paint_group );
//	}
//	else if( _path_designer_mode == 1 )//Circle
//	{
//		::rux::uint8 found = 0;
//		rux::gui::controls::XRectangle circle_begin( paint_group.FindOne< ::rux::gui::controls::XRectangle >( "circle_begin" , found ) );
//		if( found == 1 )
//		{
//			rux::XString error;
//			float circle_begin_x = circle_begin.get_Left() + circle_begin.get_Width() / 2.f;
//			float circle_begin_y = circle_begin.get_Top() + circle_begin.get_Height() / 2.f;
//			paint_group.RemoveControl( circle_begin );
//			float circle_center_x = 0.f , circle_center_y = 0.f;
//			rux::XMath::segment_midpoint( circle_begin_x , circle_begin_y , x , y , circle_center_x , circle_center_y );
//			float radius = ::rux::XMath::segment_length( circle_begin_x , circle_begin_y , x , y ) / 2.f;
//
//			float radian = ( 3.14f / 2.0f ) / 12.f;
//			float corner_in_rad = radian , x_next = 0.f , y_next = 0.f;
//			for( ::rux::uint32 index0 = 0 ; index0 < 12 ; index0++ )
//			{											
//				x_next = circle_center_x - radius * cos( corner_in_rad );
//				y_next = circle_center_y - radius * sin( corner_in_rad );				
//				corner_in_rad += radian;
//				rux::gui::controls::XRectangle point;
//				point.set_ControlName( "point" );
//				point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//				point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//				point.set_Width( 5 );
//				point.set_Height( 5 );
//				point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//				point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//				point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//				point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//				paint_group.AddControl( point , error );
//				if( index0 == 11 )
//				{
//					x_next = circle_center_x;
//					y_next = circle_center_y - radius;
//					corner_in_rad += radian;
//					rux::gui::controls::XRectangle point;
//					point.set_ControlName( "point" );
//					point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//					point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//					point.set_Width( 5 );
//					point.set_Height( 5 );
//					point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//					point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//					point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//					point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//					paint_group.AddControl( point , error );
//				}
//			}
//			corner_in_rad = radian;
//			for( ::rux::uint32 index0 = 0 ; index0 < 12 ; index0++ )
//			{											
//				x_next = circle_center_x + radius * sin( corner_in_rad );
//				y_next = circle_center_y - radius * cos( corner_in_rad );				
//				corner_in_rad += radian;
//				rux::gui::controls::XRectangle point;
//				point.set_ControlName( "point" );
//				point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//				point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//				point.set_Width( 5 );
//				point.set_Height( 5 );
//				point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//				point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//				point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//				point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//				paint_group.AddControl( point , error );
//				if( index0 == 11 )
//				{
//					x_next = circle_center_x + radius;
//					y_next = circle_center_y;
//					corner_in_rad += radian;
//					rux::gui::controls::XRectangle point;
//					point.set_ControlName( "point" );
//					point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//					point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//					point.set_Width( 5 );
//					point.set_Height( 5 );
//					point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//					point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//					point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//					point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//					paint_group.AddControl( point , error );
//				}
//			}
//			corner_in_rad = radian;
//			for( ::rux::uint32 index0 = 0 ; index0 < 12 ; index0++ )
//			{											
//				x_next = circle_center_x + radius * cos( corner_in_rad );
//				y_next = circle_center_y + radius * sin( corner_in_rad );				
//				corner_in_rad += radian;
//				rux::gui::controls::XRectangle point;
//				point.set_ControlName( "point" );
//				point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//				point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//				point.set_Width( 5 );
//				point.set_Height( 5 );
//				point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//				point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//				point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//				point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//				paint_group.AddControl( point , error );
//				if( index0 == 11 )
//				{
//					x_next = circle_center_x;
//					y_next = circle_center_y + radius;
//					corner_in_rad += radian;
//					rux::gui::controls::XRectangle point;
//					point.set_ControlName( "point" );
//					point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//					point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//					point.set_Width( 5 );
//					point.set_Height( 5 );
//					point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//					point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//					point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//					point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//					paint_group.AddControl( point , error );
//				}
//			}
//			corner_in_rad = radian;
//			for( ::rux::uint32 index0 = 0 ; index0 < 12 ; index0++ )
//			{											
//				x_next = circle_center_x - radius * sin( corner_in_rad );
//				y_next = circle_center_y + radius * cos( corner_in_rad );				
//				corner_in_rad += radian;
//				rux::gui::controls::XRectangle point;
//				point.set_ControlName( "point" );
//				point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//				point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//				point.set_Width( 5 );
//				point.set_Height( 5 );
//				point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//				point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//				point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//				point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//				paint_group.AddControl( point , error );
//				if( index0 == 11 )
//				{
//					x_next = circle_center_x - radius;
//					y_next = circle_center_y;
//					corner_in_rad += radian;
//					rux::gui::controls::XRectangle point;
//					point.set_ControlName( "point" );
//					point.set_OnLeftMouseButtonDown( on_path_designer_point_mouse_left_mouse_button_down );
//					point.set_OnLeftMouseButtonUp( on_path_designer_point_mouse_left_mouse_button_up );
//					point.set_Width( 5 );
//					point.set_Height( 5 );
//					point.set_Background( ::rux::gui::Colors::ButtonDownColor() );
//					point.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//					point.set_Left( x_next - paint_group.get_Left() - point.get_Width() / 2.f );
//					point.set_Top( y_next - paint_group.get_Top() - point.get_Height() / 2.f );				
//					paint_group.AddControl( point , error );
//				}
//			}			
//			update_preview_window_from_painted( paint_group );
//		}
//		else
//		{
//			circle_begin.set_ControlName( "circle_begin" );
//			circle_begin.set_Width( 5 );
//			circle_begin.set_Height( 5 );
//			circle_begin.set_Background( ::rux::gui::Colors::ButtonAlertDownColor() );
//			circle_begin.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//			circle_begin.set_Left( x - paint_group.get_Left() - circle_begin.get_Width() / 2.f );
//			circle_begin.set_Top( y - paint_group.get_Top() - circle_begin.get_Height() / 2.f );
//			rux::XString error;
//			paint_group.AddControl( circle_begin , error );
//		}
//	}
//};
//XTextBlock ttt;
//::rux::uint32 __index = 0;
//void add_control_thread( void* param )
//{	
//	XWindow* window = (XWindow*)param;
//		
//	__index++;
//	char line[ 256 ] = {0};
//	sprintf( line , "try #%u" , __index );
//	ttt.set_Text( line );
//	rux::XString error;
//	if( window->ExistsControl( ttt ) == 0 )
//	{
//		ttt.set_Left( 10 );
//		ttt.set_Top( 40 );
//		ttt.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
//		ttt.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
//		ttt.set_Background( ::rux::gui::Colors::Black() );
//		window->AddControl( ttt , error );
//	}
//};
//void on_resize_left_mouse_button_down( const XObject& sender , ::rux::int16 x , ::rux::int16 y )
//{
//	XGroup video_window( sender );
//	video_window.StartDrag();
//};
//void on_resize_left_mouse_button_up( const XObject& sender , ::rux::int16 x , ::rux::int16 y )
//{
//	XGroup video_window( sender );
//	video_window.StopDrag();
//};
//rux::media::Frame& get_frame( const ::rux::XString& filename )
//{
//	rux::media::XFrame frame;
//	rux::XString error;
//	frame.set_ImageData( filename , error );	
//	return frame++;
//};
//int main(int argc, char **argv)
//{	
//	::rux::engine::initialize();
//	::rux::int32 is_window = 1;
//	if( is_window == 0 )
//	{
//		//rux::XString text( "0\n" );
//		//printf( text()->_array_ptr );
//	}
//	if( is_window == 1 )
//	{
//		THREAD_REGISTER( "main" );
//		rux_add_search_directory( "../../addons" );
//		rux_add_search_directory( "../../addons/png_test" );
//		
//		rux::XString error;
//		char* module_directory = rux_get_module_directory();			
//
//		::rux::uint32 fcc = FCC_BGR3;
//		rux::threading::XThread thread;
//		rux::gui::XWindow _window;
//		_window.set_Background( create_color( 0x45 , 0x41 , 0x3c , 255 ) );
//		rux::gui::LinearGradientColor* linear_gradient = alloc_object_macros( ::rux::gui::LinearGradientColor );
//		linear_gradient->set_LinearGradientType( ::rux::gui::XEnum_LinearGradientType_Horizontal );
//		linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0 , 128 , 0 , 255 ) , 0.0 ) );
//		linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 255 , 0 , 0 , 255 ) , 0.5 ) );
//		linear_gradient->AddGradientStop( ::rux::gui::GradientStop( ::rux::gui::Color( 0 , 0 , 255 , 255 ) , 1.0 ) );		
//		_window.set_Background( linear_gradient );
//		rux::gui::ColorBase* color0;
//		color0 = ::rux::gui::Colors::WindowBackgroundColor();
//		_window.set_Background( color0 );
//		_window.set_Background( ::rux::gui::Colors::White() );
//		_window.set_BorderWidth( 2.0 );
//		_window.set_BorderColor( ::rux::gui::Colors::CellBorderColor() );
//		_window.set_Left( 20 );
//		_window.set_Top( 20 );
//		_window.set_Width( 400 );
//		_window.set_Height( 400 );
//		_window.set_OnLeftMouseButtonDown( on_mouse_down );
//		_window.set_OnMouseMove( on_mouse_move );
//		if( argc == 2 && strcmp( argv[ 1 ] , "--empty-test" ) == 0 )
//		{
//
//			rux::gui::controls::XRectangle shader_rectangle;
//			shader_rectangle.set_Background( linear_gradient );
//			shader_rectangle.set_Corner( ::rux::gui::Thickness( 1.f , 1.f , 1.f , 1.f ) );
//			shader_rectangle.set_Left( 40 );
//			shader_rectangle.set_Top( 40 );
//			shader_rectangle.set_Width( 50 );
//			shader_rectangle.set_Height( 50 );
//			//shader_rectangle.set_Opacity( .5f );
//			_window.AddControl( shader_rectangle , error );
//
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--text-test" ) == 0 )
//		{
//			XTextBlock textblock;
//			textblock.set_Left( 10 );
//			textblock.set_Top( 10 );
//
//			textblock.set_HorizontalFilling( ::rux::gui::XEnum_Filling_FromContent );
//			textblock.set_VerticalFilling( ::rux::gui::XEnum_Filling_FromContent );
//			textblock.set_Foreground( ::rux::gui::Colors::Green() );
//			textblock.set_Font( "tahoma.ttf" , 0 , 40 );
//			textblock.set_Text( "ПРИМЕР!!!" );
//			_window.AddControl( textblock , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--control-resize-test" ) == 0 )
//		{
//			XGroup main_group;
//			main_group.set_HorizontalFilling( XEnum_Filling_Auto );
//			main_group.set_VerticalFilling( XEnum_Filling_Auto );
//
//			XGroup group;
//
//			group.set_Left( 10 );
//			group.set_Top( 10 );
//			group.set_Width( 100 );
//			group.set_Height( 100 );
//			group.set_OnLeftMouseButtonDown( on_resize_left_mouse_button_down );
//			group.set_OnLeftMouseButtonUp( on_resize_left_mouse_button_up );
//
//			rux::gui::controls::XRectangle back;
//			back()->set_IsForwardLeftMouseButtonDownUpEvent( 1 );
//			back.set_HorizontalFilling( XEnum_Filling_Auto );
//			back.set_VerticalFilling( XEnum_Filling_Auto );
//			back.set_Background( ::rux::gui::Colors::DarkGrey() );
//
//			group.AddControl( back , error );
//			main_group.AddControl( group , error );
//			_window.AddControl( main_group , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--combobox-test" ) == 0 )
//		{
//			XObject json_object( ::rux::data::XJsonSerializer::Deserialize( "{\"sets\":{\"internal\":[{\"name5\":\"item0\"},{\"name5\":\"item1\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"},{\"name5\":\"item2\"}]}}" , error ) );
//
//			XSelect combobox;
//			combobox.set_BindingSource( json_object );
//			combobox.set_BindingSourcePath( "sets.internal" );
//			combobox.set_BindingSourceDisplayPropertyName( "name5" );
//			combobox.set_Left( 90.f );
//			combobox.set_Top( 10 );
//			combobox.set_Width( 100 );
//			combobox.set_Height( 20 );		
//			_window.AddControl( combobox , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--datepicker-test" ) == 0 )
//		{
//			XDatePicker datepicker;
//			datepicker.set_DateTime( ::rux::XTime::get_NowAsUnixTime() );
//			datepicker.set_Left( 55.f );
//			datepicker.set_Top( 55.f );
//			datepicker.set_Width( 80.f );
//			datepicker.set_Height( 15.f );
//			_window.AddControl( datepicker , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--textbox-test" ) == 0 )
//		{
//			rux::gui::controls::XTextBox textbox;
//			textbox.set_Text( "Hello,halloween kingdom or castle, please take me to the moon\nMy dear friend\nGAMOSKIN\nJOKER\nSOLDIER\nKERNEL\nSHELL\npoker" );
//			textbox.set_IsMultiLines( 1 );
//			textbox.set_Left( 55.f );
//			textbox.set_Top( 55.f );
//			textbox.set_Width( 80.f );
//			textbox.set_Height( 80.f );
//			_window.AddControl( textbox , error );
//
//			rux::gui::controls::XTextBox textbox1;
//			textbox1.set_Text( "17:U9:89 pop" );
//			textbox1.set_LeftMouseDownSelectionRegexpMask( "(0[0-9]|1[0-9]|2[0-3])|[0-5][0-9]|[0-5][0-9]" , error );
//			textbox1.set_RegexpMask( XRegexp::TimeMask() , error );			
//			textbox1.set_Left( 55.f );
//			textbox1.set_IsMultiLines( 0 );
//			textbox1.set_Top( 200.f );
//			textbox1.set_Width( 80.f );
//			textbox1.set_Height( 20.f );
//			_window.AddControl( textbox1 , error );
//
//			rux::gui::controls::XTextBox textbox2;
//			textbox2.set_Text( "17:U9:89 pop" );
//			textbox2.set_RegexpMask( XRegexp::Ipv4Mask() , error );			
//			textbox2.set_Left( 55.f );
//			textbox2.set_IsMultiLines( 0 );
//			textbox2.set_Top( 230.f );
//			textbox2.set_Width( 80.f );
//			textbox2.set_Height( 20.f );
//			_window.AddControl( textbox2 , error );
//
//			rux::gui::controls::XTextBox textbox3;
//			textbox3.set_Text( "17:U9:89 pop" );
//			textbox3.set_IsMasked( 1 );
//			textbox3.set_Left( 55.f );
//			textbox3.set_IsMultiLines( 0 );
//			textbox3.set_Top( 260.f );
//			textbox3.set_Width( 80.f );
//			textbox3.set_Height( 20.f );
//			_window.AddControl( textbox3 , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--timepicker-test" ) == 0 )
//		{
//			XTimePicker timepicker;
//			timepicker.set_Left( 55.f );
//			timepicker.set_Top( 200.f );
//			timepicker.set_Width( 80.f );
//			timepicker.set_Height( 15.f );
//			_window.AddControl( timepicker , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--antialiasing-test" ) == 0 )
//		{
//			rux::gui::controls::XRectangle shader_rectangle;
//			shader_rectangle.set_Background( linear_gradient );
//			shader_rectangle.set_Corner( ::rux::gui::Thickness( 1.f , 1.f , 1.f , 1.f ) );
//			shader_rectangle.set_Left( 40 );
//			shader_rectangle.set_Top( 40 );
//			shader_rectangle.set_Width( 50 );
//			shader_rectangle.set_Height( 50 );
//			//shader_rectangle.set_Opacity( .5f );
//			_window.AddControl( shader_rectangle , error );
//
//			shader_rectangle = ::rux::gui::controls::XRectangle();
//			shader_rectangle.set_BorderColor( ::rux::gui::Colors::Black() );
//			shader_rectangle.set_BorderWidth( 5.f );
//			shader_rectangle.set_Background( ::rux::gui::Colors::Green() );
//			shader_rectangle.set_Corner( ::rux::gui::Thickness( 1.f , 1.f , 1.f , 1.f ) );
//			shader_rectangle.set_Left( 40 );
//			shader_rectangle.set_Top( 100 );
//			shader_rectangle.set_Width( 40 );
//			shader_rectangle.set_Height( 40 );
//			//shader_rectangle.set_Opacity( .5f );
//			_window.AddControl( shader_rectangle , error );
//
//			declare_variable( ::rux::gui::controls::XGroup , group );
//			group = XPrimitives::DownArrow( ::rux::gui::Colors::Red() );
//			group.set_Left( 100 );
//			group.set_Top( 20 );
//			group.set_Width( 100 );
//			group.set_Height( 100 );
//			_window.AddControl( group , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--video-test" ) == 0 )
//		{
//			XArray< ::rux::media::XWebCamera > webs;
//			webs.set_ByRef( ::rux::media::XWebCamera::get_MediaDevices( error ) );
//			if( webs.Count() > 0 )
//			{
//				/*XArray< ::rux::media::XFrameFormat > frame_formats;
//				frame_formats.set_ByRef( webs[ 0 ].get_FrameFormats( error ) );
//				if( frame_formats.Count() > 0 )
//				{
//					rux::XRectangleF area4x3( ::rux::XMath::CalculateRect( ::rux::XRectangleF( ::rux::XPointF( 2.0 , 2.0f ) , _window.get_Width() - 4.0f , _window.get_Height() - 4.0f ) , 4 , 3 ) );
//					rux::XPointF point0;
//					point0 = area4x3.get_Point();
//					XImage _web_ui_image;
//					_web_ui_image.set_VerticalFilling( XEnum_Filling_Auto );
//					_web_ui_image.set_HorizontalFilling( XEnum_Filling_Auto );
//					_window.AddControl( _web_ui_image , error );					
//					webs[ 0 ].set_OnFrame( web_on_frame );
//					webs[ 0 ].StartCapture( ::rux::media::XFrameFormat( frame_formats[ 0 ] ) , error );
//				}*/
//			}
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--rectangle-test" ) == 0 ) 
//		{
//			rux::gui::controls::XRectangle test_rectangle0;
//			test_rectangle0.set_Background( ::rux::gui::Colors::Green() );
//			test_rectangle0.set_Left( 10 );
//			test_rectangle0.set_Top( 10 );
//			test_rectangle0.set_Width( 40 );
//			test_rectangle0.set_Height( 40 );
//			_window.AddControl( test_rectangle0 , error );
//
//			::rux::gui::XAnimation alarm_animation0;	
//			alarm_animation0.set_Control( test_rectangle0 );
//			alarm_animation0.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation0.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation0.set_PropertyName( "Opacity" );
//			alarm_animation0.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation0.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//
//			rux::gui::controls::XRectangle test_rectangle1;
//			test_rectangle1.set_Background( ::rux::gui::Colors::ButtonNormalColor() );
//			test_rectangle1.set_Left( 60 );
//			test_rectangle1.set_Top( 10 );
//			test_rectangle1.set_Width( 40 );
//			test_rectangle1.set_Height( 40 );
//			_window.AddControl( test_rectangle1 , error );
//
//			::rux::gui::XAnimation alarm_animation1;	
//			alarm_animation1.set_Control( test_rectangle1 );
//			alarm_animation1.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation1.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation1.set_PropertyName( "Opacity" );
//			alarm_animation1.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation1.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle2;
//			test_rectangle2.set_Background( ::rux::gui::Colors::VerticalSliderThumbColor() );
//			test_rectangle2.set_Left( 110 );
//			test_rectangle2.set_Top( 10 );
//			test_rectangle2.set_Width( 40 );
//			test_rectangle2.set_Height( 40 );
//			_window.AddControl( test_rectangle2 , error );
//
//			::rux::gui::XAnimation alarm_animation2;	
//			alarm_animation2.set_Control( test_rectangle2 );
//			alarm_animation2.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation2.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation2.set_PropertyName( "Opacity" );
//			alarm_animation2.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation2.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle3;
//			test_rectangle3.set_BorderColor( ::rux::gui::Colors::Blue( 0.5f ) );
//			test_rectangle3.set_Background( ::rux::gui::Colors::Green() );
//			test_rectangle3.set_BorderWidth( 5.f );
//			test_rectangle3.set_Left( 160 );
//			test_rectangle3.set_Top( 10 );
//			test_rectangle3.set_Width( 40 );
//			test_rectangle3.set_Height( 40 );
//			_window.AddControl( test_rectangle3 , error );
//
//			::rux::gui::XAnimation alarm_animation3;	
//			alarm_animation3.set_Control( test_rectangle3 );
//			alarm_animation3.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation3.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation3.set_PropertyName( "Opacity" );
//			alarm_animation3.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation3.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle4;
//			test_rectangle4.set_BorderColor( ::rux::gui::Colors::Blue( 0.5f ) );
//			test_rectangle4.set_Background( ::rux::gui::Colors::ButtonNormalColor() );
//			test_rectangle4.set_BorderWidth( 5.f );
//			test_rectangle4.set_Left( 210 );
//			test_rectangle4.set_Top( 10 );
//			test_rectangle4.set_Width( 40 );
//			test_rectangle4.set_Height( 40 );
//
//			::rux::gui::XAnimation alarm_animation4;	
//			alarm_animation4.set_Control( test_rectangle4 );
//			alarm_animation4.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation4.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation4.set_PropertyName( "Opacity" );
//			alarm_animation4.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation4.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle5;
//			test_rectangle5.set_BorderColor( ::rux::gui::Colors::Blue( 0.5f ) );
//			test_rectangle5.set_Background( ::rux::gui::Colors::VerticalSliderThumbColor() );
//			test_rectangle5.set_BorderWidth( 5.f );
//			test_rectangle5.set_Left( 260 );
//			test_rectangle5.set_Top( 10 );
//			test_rectangle5.set_Width( 40 );
//			test_rectangle5.set_Height( 40 );
//			_window.AddControl( test_rectangle5 , error );
//
//			::rux::gui::XAnimation alarm_animation5;	
//			alarm_animation5.set_Control( test_rectangle5 );
//			alarm_animation5.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation5.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation5.set_PropertyName( "Opacity" );
//			alarm_animation5.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation5.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle6;
//			test_rectangle6.set_BorderColor( ::rux::gui::Colors::Blue( 0.5f ) );
//			test_rectangle6.set_Background( ::rux::gui::Colors::Transparent() );
//			test_rectangle6.set_BorderWidth( 5.f );
//			test_rectangle6.set_Left( 310 );
//			test_rectangle6.set_Top( 10 );
//			test_rectangle6.set_Width( 40 );
//			test_rectangle6.set_Height( 40 );
//			_window.AddControl( test_rectangle6 , error );
//
//			::rux::gui::XAnimation alarm_animation6;	
//			alarm_animation6.set_Control( test_rectangle6 );
//			alarm_animation6.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation6.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation6.set_PropertyName( "Opacity" );
//			alarm_animation6.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation6.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle7;
//			test_rectangle7.set_Corner( ::rux::gui::Thickness( 0.5f , 0.5f , 0.7f , 0 ) );
//			test_rectangle7.set_Background( ::rux::gui::Colors::Red( 0.5f ) );
//			test_rectangle7.set_Left( 10 );
//			test_rectangle7.set_Top( 60 );
//			test_rectangle7.set_Width( 40 );
//			test_rectangle7.set_Height( 40 );
//			_window.AddControl( test_rectangle7 , error );
//
//			::rux::gui::XAnimation alarm_animation7;	
//			alarm_animation7.set_Control( test_rectangle7 );
//			alarm_animation7.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation7.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation7.set_PropertyName( "Opacity" );
//			alarm_animation7.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation7.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//
//			rux::gui::controls::XRectangle test_rectangle8;
//			test_rectangle8.set_Corner( ::rux::gui::Thickness( 0.5f , 0.5f , 0.7f , 0 ) );
//			test_rectangle8.set_BorderWidth( 5.f );
//			test_rectangle8.set_BorderColor( ::rux::gui::Colors::Red( 0.5f ) );
//			test_rectangle8.set_Background( ::rux::gui::Colors::ButtonNormalColor() );
//			test_rectangle8.set_Left( 60 );
//			test_rectangle8.set_Top( 60 );
//			test_rectangle8.set_Width( 40 );
//			test_rectangle8.set_Height( 40 );
//			_window.AddControl( test_rectangle8 , error );
//
//			::rux::gui::XAnimation alarm_animation8;	
//			alarm_animation8.set_Control( test_rectangle8 );
//			alarm_animation8.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation8.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation8.set_PropertyName( "Opacity" );
//			alarm_animation8.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation8.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//						
//			::rux::gui::XGroupAnimation alarm_group_animation;
//			alarm_group_animation.set_AnimationType( ::rux::gui::XEnum_AnimationType_Repeat );
//			alarm_group_animation.AddAnimation( alarm_animation0 );
//			alarm_group_animation.AddAnimation( alarm_animation1 );
//			alarm_group_animation.AddAnimation( alarm_animation2 );
//			alarm_group_animation.AddAnimation( alarm_animation3 );
//			alarm_group_animation.AddAnimation( alarm_animation4 );
//			alarm_group_animation.AddAnimation( alarm_animation5 );
//			alarm_group_animation.AddAnimation( alarm_animation6 );
//			alarm_group_animation.AddAnimation( alarm_animation7 );
//			alarm_group_animation.AddAnimation( alarm_animation8 );
//
//			test_rectangle0.set_Tag( alarm_group_animation );
//			alarm_group_animation.Start();
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--image-test" ) == 0 ) 
//		{
//			XImage ui_image;
//			//ui_image.set_Width( 400 );
//			//ui_image.set_Height( 300 );
//			ui_image.set_Width( 400 );
//			ui_image.set_Height( 300 );
//			ui_image.set_Left( 0 );
//			ui_image.set_Top( 0 );
//			
//			rux::media::XFrame frame;		
//			frame.set_ImageData( "1.yv12" , error );	
//			//frame = frame.ConvertToRGB32( 128 , error );
//			//frame.SaveAsBmp( ::rux::XString( module_directory ) + "\\1yv12.bmp" , error );
//			::rux::byte new_image = 0;
//			ui_image.set_FrameData( (char*)frame.get_ImageData( new_image ) , frame.get_ImageWidth() , frame.get_ImageHeight() , (rux::media::XEnum_Fourcc)frame.get_Fourcc() );
//
//			rux::gui::controls::XGroup ui_group;
//			ui_group.set_Left( 10 );
//			ui_group.set_Top( 10 );
//			ui_group.set_Width( 300 );
//			ui_group.set_Height( 300 );
//
//			ui_group.AddControl( ui_image , error );
//
//
//			_window.AddControl( ui_group , error );
//
//			rux::gui::controls::XButton grid_row_remove_button;
//			grid_row_remove_button.set_Top( 5 );
//			grid_row_remove_button.set_Left( 135 );
//			grid_row_remove_button.set_Height( 20 );
//			grid_row_remove_button.set_Width( 60 );
//			grid_row_remove_button.set_Content( ::rux::XString( "Minus" ) );
//			//grid_row_remove_button.set_OnClick( on_path_designer_grid_row_remove_click );
//			//_window.AddControl( grid_row_remove_button , error );
//
//			XSlider brightness;
//			brightness.set_ControlName( "brightness" );
//			brightness.set_ValueStep( 0.1f );
//			brightness.set_MinValue( -1.f );
//			brightness.set_MaxValue( 1.f );
//			brightness.set_Width( 30 );
//			brightness.set_Height( 50 );
//			brightness.set_Top( 10 );
//			brightness.set_Left( 420 );
//			brightness.set_OnValueChanged( on_slider_value_changed );
//			_window.AddControl( brightness , error );
//			XSlider contrast;
//			contrast.set_ControlName( "contrast" );
//			contrast.set_ValueStep( 0.1f );
//			contrast.set_MinValue( 1.f );
//			contrast.set_MaxValue( 10.f );
//			contrast.set_Width( 30 );
//			contrast.set_Height( 50 );
//			contrast.set_Top( 70 );
//			contrast.set_Left( 420 );
//			contrast.set_OnValueChanged( on_slider_value_changed );
//			//_window.AddControl( contrast , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--line-test" ) == 0 ) 
//		{
//			rux::gui::controls::XLine line;
//			line.set_Color( ::rux::gui::Colors::Red( 0.5f) );
//			line.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			line.set_StartLeft( 0.1f );
//			line.set_StartTop( 0.1f );
//			line.set_EndLeft( 0.5f );
//			line.set_EndTop( 0.7f );
//			line.set_Thickness( 10 );
//			_window.AddControl( line , error );
//
//			::rux::gui::XAnimation alarm_animation0;	
//			alarm_animation0.set_Control( line );
//			alarm_animation0.set_AnimationSetValueType( ::rux::gui::XEnum_AnimationSetValueType_Gradual );
//			alarm_animation0.set_StartValue( ::rux::XFloat( 0.f ) );
//			alarm_animation0.set_PropertyName( "Opacity" );
//			alarm_animation0.AddTimeline( 300000ULL , ::rux::XFloat( 0.8f ) );
//			alarm_animation0.AddTimeline( 300000ULL , ::rux::XFloat( 0.f ) );
//			::rux::gui::XGroupAnimation alarm_group_animation;
//			alarm_group_animation.set_AnimationType( ::rux::gui::XEnum_AnimationType_Repeat );
//			alarm_group_animation.AddAnimation( alarm_animation0 );
//
//			line.set_Tag( alarm_group_animation );
//			alarm_group_animation.Start();
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--path-test" ) == 0 ) 
//		{
//			{
//				rux::gui::controls::XPath path;
//				declare_stack_variable( char , executable_directory , 1024 );
//				rux_get_executable_directory( executable_directory );
//				rux_concatenation( executable_directory , "/path_designer.txt" );
//				rux::io::XFile file = ::rux::io::XFile::Open( executable_directory , error );
//				if( file.Exists() == 1 )
//				{
//					rux::XString json;
//					json.set_ByRef( file.ReadTextAll( error ) );
//					if( error.Length() == 0 )
//					{
//						//XObject control( XUIApplication::JSONToControl( json , error ) );
//						if( error.Length() == 0 )
//						{
//							//if( rux_is_object( control , XPath ) )
//							//	path = control;
//						}
//					}
//					file.Close();
//				}				
//				declare_variable( ::rux::gui::controls::XGroup , large_group );
//				large_group.set_Left( 0 );
//				large_group.set_Top( 0 );
//				large_group.set_Width( 150 );
//				large_group.set_Height( 150 );
//				rux::XString error;
//				large_group.AddControl( path , error );
//				//XGroup cogwheel( XPrimitives::Star( 0 , 0 , 50 , 50 , ::rux::gui::XEnum_PointUnit_Pixel , XObject( ::rux::gui::LinearGradientColor::VerticalSliderOverThumbColor() ) ) );
//				_window.AddControl( large_group , error );
//			}
//			{				
//				rux::gui::controls::XPath path;
//				declare_stack_variable( char , executable_directory , 1024 );
//				rux_get_executable_directory( executable_directory );
//				rux_concatenation( executable_directory , "/path_designer.txt" );
//				rux::io::XFile file = ::rux::io::XFile::Open( executable_directory , error );
//				if( file.Exists() == 1 )
//				{
//					rux::XString json;
//					json.set_ByRef( file.ReadTextAll( error ) );
//					if( error.Length() == 0 )
//					{
//						//XObject control( XUIApplication::JSONToControl( json , error ) );
//						if( error.Length() == 0 )
//						{
//							//if( rux_is_object( control , XPath ) )
//							//	path = control;
//						}
//					}
//					file.Close();
//				}
//				declare_variable( ::rux::gui::controls::XGroup , group );
//				group.set_Left( 160 );
//				group.set_Top( 0 );
//				group.set_Width( 50 );
//				group.set_Height( 50 );
//				group.AddControl( path , error );				
//				_window.AddControl( group , error );
//
//				//XGroup cogwheel( XPrimitives::Star( 30 , 0 , 150 , 150 , ::rux::gui::XEnum_PointUnit_Pixel , XObject( ::rux::gui::LinearGradientColor::VerticalSliderOverThumbColor() ) ) );
//				//_window.AddControl( cogwheel , error );
//			}
//			{				
//				rux::gui::controls::XPath path;
//				declare_stack_variable( char , executable_directory , 1024 );
//				rux_get_executable_directory( executable_directory );
//				rux_concatenation( executable_directory , "/path_designer.txt" );
//				rux::io::XFile file = ::rux::io::XFile::Open( executable_directory , error );
//				if( file.Exists() == 1 )
//				{
//					rux::XString json;
//					json.set_ByRef( file.ReadTextAll( error ) );
//					if( error.Length() == 0 )
//					{
//						//XObject control( XUIApplication::JSONToControl( json , error ) );
//						if( error.Length() == 0 )
//						{
//							//if( rux_is_object( control , XPath ) )
//							//	path = control;
//						}
//					}
//					file.Close();
//				}
//				declare_variable( ::rux::gui::controls::XGroup , group );
//				group.set_Left( 220 );
//				group.set_Top( 0 );
//				group.set_Width( 15 );
//				group.set_Height( 15 );
//				group.AddControl( path , error );				
//				_window.AddControl( group , error );
//
//				//XGroup cogwheel( XPrimitives::Star( 30 , 0 , 150 , 150 , ::rux::gui::XEnum_PointUnit_Pixel , XObject( ::rux::gui::LinearGradientColor::VerticalSliderOverThumbColor() ) ) );
//				//_window.AddControl( cogwheel , error );
//			}
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--path-designer" ) == 0 ) 
//		{
//			float offset = 3.f;
//			declare_variable( ::rux::gui::controls::XGroup , menu_group );
//			menu_group.set_HorizontalFilling( XEnum_Filling_Auto );
//			menu_group.set_Height( 30 );
//			rux::gui::controls::XRectangle menu_group_back;
//			menu_group_back.set_HorizontalFilling( XEnum_Filling_Auto );
//			menu_group_back.set_VerticalFilling( XEnum_Filling_Auto );
//			menu_group_back.set_Background( ::rux::gui::Colors::WindowHeaderColor() );
//			menu_group.AddControl( menu_group_back , error );
//
//			rux::gui::controls::XButton save_button;
//			save_button.set_Top( 5 );
//			save_button.set_Left( 5 );
//			save_button.set_Height( 20 );
//			save_button.set_Width( 60 );
//			save_button.set_Content( ::rux::XString( "Save" ) );
//			save_button.set_OnClick( on_path_designer_save_click );
//			menu_group.AddControl( save_button , error );
//
//			rux::gui::controls::XButton grid_row_add_button;
//			grid_row_add_button.set_Top( 5 );
//			grid_row_add_button.set_Left( 70 );
//			grid_row_add_button.set_Height( 20 );
//			grid_row_add_button.set_Width( 60 );
//			grid_row_add_button.set_Content( ::rux::XString( "Plus" ) );
//			grid_row_add_button.set_OnClick( on_path_designer_grid_row_add_click );
//			menu_group.AddControl( grid_row_add_button , error );
//
//			_window.set_Height( _window.get_Width() + menu_group.get_Height() );
//
//			rux::gui::controls::XButton grid_row_remove_button;
//			grid_row_remove_button.set_Top( 5 );
//			grid_row_remove_button.set_Left( 135 );
//			grid_row_remove_button.set_Height( 20 );
//			grid_row_remove_button.set_Width( 60 );
//			grid_row_remove_button.set_Content( ::rux::XString( "Minus" ) );
//			grid_row_remove_button.set_OnClick( on_path_designer_grid_row_remove_click );
//			menu_group.AddControl( grid_row_remove_button , error );
//
//			rux::gui::controls::XButton bind_unbind_to_grid_button;
//			bind_unbind_to_grid_button.set_Top( 5 );
//			bind_unbind_to_grid_button.set_Left( 200 );
//			bind_unbind_to_grid_button.set_Height( 20 );
//			bind_unbind_to_grid_button.set_Width( 70 );
//			bind_unbind_to_grid_button.set_Content( ::rux::XString( "Bind to grid" ) );
//			bind_unbind_to_grid_button.set_OnClick( on_path_designer_bind_unbind_to_grid_click );
//			menu_group.AddControl( bind_unbind_to_grid_button , error );
//
//			rux::gui::controls::XButton mode_button;
//			mode_button.set_Top( 5 );
//			mode_button.set_Left( 275 );
//			mode_button.set_Height( 20 );
//			mode_button.set_Width( 60 );
//			mode_button.set_Content( ::rux::XString( "Point" ) );
//			mode_button.set_OnClick( on_path_mode_click );
//			menu_group.AddControl( mode_button , error );
//
//			rux::gui::controls::XButton remove_point_button;
//			remove_point_button.set_Top( 5 );
//			remove_point_button.set_Left( 340 );
//			remove_point_button.set_Height( 20 );
//			remove_point_button.set_Width( 60 );
//			remove_point_button.set_Content( ::rux::XString( "Remove" ) );
//			remove_point_button.set_OnClick( on_path_designer_remove_point_click );
//			menu_group.AddControl( remove_point_button , error );
//
//			_window.AddControl( menu_group , error );			
//
//			declare_variable( ::rux::gui::controls::XGroup , paint_group );
//			paint_group.set_ControlName( "paint_group" );
//			paint_group.set_OnLeftMouseButtonDown( on_paint_group_left_mouse_button_down );
//			paint_group.set_Top( menu_group.get_Height() + offset );
//			paint_group.set_Left( offset );
//			paint_group.set_Width( _window.get_Width() - 2 * offset );
//			paint_group.set_Height( _window.get_Height() - menu_group.get_Height() - 2 * offset );
//			rux::gui::controls::XRectangle paint_group_back;
//			paint_group_back()->set_IsForwardLeftMouseButtonDownUpEvent( 1 );
//			paint_group_back()->set_IsForwardLeftMouseDoubleClickEvent( 1 );
//			paint_group_back()->set_IsForwardMouseMoveEnterLeaveEvent( 1 );
//			paint_group_back()->set_IsForwardMouseWheelEvent( 1 );
//			paint_group_back.set_HorizontalFilling( XEnum_Filling_Auto );
//			paint_group_back.set_VerticalFilling( XEnum_Filling_Auto );
//			paint_group_back.set_Background( ::rux::gui::Colors::WindowBackgroundColor() );
//			paint_group.AddControl( paint_group_back , error );
//			_window.AddControl( paint_group , error );
//			path_designer_update_grid( _window );
//						
//			rux::gui::XWindow preview_window( create_preview_window() );
//			preview_window()->AddRef();
//			paint_group.set_TagPtr( preview_window() );			
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--few-windows-test" ) == 0 ) 
//		{
//			_window.set_OnShown( on_few_windows_shown );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--image-test2" ) == 0 ) 
//		{
//			declare_stack_variable( char , executable_directory , 1024 );
//			rux_get_executable_directory( executable_directory );
//			rux_concatenation( executable_directory , "/../../addons" );
//			XArray< ::rux::io::XFile > files;
//			files.set_ByRef( ::rux::io::XDirectory::GetFiles( executable_directory ) );
//			float left = 10.f , top = 10.f;
//			size_t image_count = 0;			
//			XImage image0;
//			for( size_t index0 = 0 ; index0 < files.Count() ; index0++ )
//			{
//				rux::XString ext( files[ index0 ].get_FileExtension() );
//				ext.set_ByRef( ext.ToLower() );
//				if( ext == "bmp" )
//				{
//					//rux::media::XFrame image;
//					//image.set_ImageData( files[ index0 ].get_FileName() , error );
//					//image.ReplaceAllColors( ::rux::gui::Colors::Transparent() , ::rux::gui::Colors::Red() , error );
//					//image.SaveAsBmp( files[ index0 ].get_FileName() + "1" , error );
//					//if( image_count == 5 )
//					{
//						//_window.RemoveControl( image0 );
//						//XThread::Sleep( 3000 );
//						image0 = XImage();
//						image0.set_Width( 30 );
//						image0.set_Height( 30 );
//						image0.set_Left( left );
//						image0.set_Top( top );
//						image0.set_IsAlpha( 1 );
//						image0.set_IsSmooth( 1 );
//						//image0.set_AlphaColor( ::rux::gui::Colors::Black() );
//						//image0.ReplaceAllColors( ::rux::gui::Colors::Green() , ::rux::gui::Colors::Black() );
//						image0.set_URL( files[ index0 ].get_FileName() );
//						_window.AddControl( image0 , error );
//						top += 40.f;
//						if( top >= 250.f )
//						{
//							top= 10.f;
//							left += 40.f;
//						}
//					}
//					image_count++;
//				}
//			}
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--stack-with-margin-test" ) == 0 ) 
//		{
//			declare_variable( ::rux::XString , error );
//			/* control group begin */
//			rux::gui::controls::XRectangle _control_rectangle;
//			_control_rectangle.set_Background( create_color( 0 , 0 , 0 , 255 * 0.7f ) );
//			_control_rectangle.set_HorizontalFilling( XEnum_Filling_Auto );
//			_control_rectangle.set_VerticalFilling( XEnum_Filling_Auto );
//			_control_rectangle.set_Corner( ::rux::gui::Thickness( 0 , 0.5f , 0 , 0.5f ) );
//			declare_variable( ::rux::gui::controls::XGroup , _control_group );
//			_control_group.AddControl( _control_rectangle , error );
//
//			_control_group.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
//			_control_group.set_HorizontalAlignment( XEnum_Alignment_Center );
//			_control_group.set_Width( 150.f );
//			_control_group.set_Height( 40.f );
//		
//			rux::gui::controls::XButton _playback_button;
//			_playback_button = XPrimitives::TransparentButton();
//			_playback_button.set_ControlName( "playback" );
//			_playback_button.set_VerticalAlignment( XEnum_Alignment_Center );
//			_playback_button.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );
//			//archive_button.set_OnMouseEnter( XVideoWindow::on_archive_button_mouse_enter );
//			//archive_button.set_OnMouseLeave( XVideoWindow::on_archive_button_mouse_leave );
//			_playback_button.set_Width( 32 );
//			_playback_button.set_Height( 32 );
//			_playback_button.set_Content( get_frame( "bplay.png" ) );
//			_playback_button.set_ContentOpacity( 0.7f );
//			_playback_button.set_Margin( ::rux::gui::Margin( 2.5f , 0.f , 0.f , 0.f ) );	
//			//archive_button.set_OnClick( XVideoWindow::on_archive_click );
//			_control_group.AddControl( _playback_button , error );
//
//			rux::gui::controls::XButton _pause_button;
//			_pause_button = XPrimitives::TransparentButton();
//			_pause_button.set_ControlName( "pause" );
//			_pause_button.set_VerticalAlignment( XEnum_Alignment_Center );
//			_pause_button.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );
//			//archive_button.set_OnMouseEnter( XVideoWindow::on_archive_button_mouse_enter );
//			//archive_button.set_OnMouseLeave( XVideoWindow::on_archive_button_mouse_leave );
//			_pause_button.set_Width( 32 );
//			_pause_button.set_Height( 32 );
//			_pause_button.set_Content( get_frame( "pause.png" ) );
//			_pause_button.set_ContentOpacity( 0.7f );
//			_pause_button.set_Margin( ::rux::gui::Margin( 2.5f , 0.f , 0.f , 0.f ) );	
//			//archive_button.set_OnClick( XVideoWindow::on_archive_click );
//			_control_group.AddControl( _pause_button , error );
//
//			rux::gui::controls::XButton _playforward_button;
//			_playforward_button = XPrimitives::TransparentButton();
//			_playforward_button.set_ControlName( "playforward" );
//			_playforward_button.set_VerticalAlignment( XEnum_Alignment_Center );
//			_playforward_button.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );
//			//archive_button.set_OnMouseEnter( XVideoWindow::on_archive_button_mouse_enter );
//			//archive_button.set_OnMouseLeave( XVideoWindow::on_archive_button_mouse_leave );
//			_playforward_button.set_Width( 32 );
//			_playforward_button.set_Height( 32 );
//			_playforward_button.set_Content( get_frame( "play.png" ) );
//			_playforward_button.set_ContentOpacity( 0.7f );
//			_playforward_button.set_Margin( ::rux::gui::Margin( 2.5f , 0.f , 0.f , 0.f ) );	
//			//archive_button.set_OnClick( XVideoWindow::on_archive_click );
//			_control_group.AddControl( _playforward_button , error );
//
//			declare_variable( ::rux::gui::controls::XGroup , _main_group );
//			_main_group.AddControl( _control_group , error );
//
//			/* control group end */
//		
//		
//			_main_group.set_VerticalAlignment( XEnum_Alignment_RightOrBottom );
//			_main_group.set_HorizontalFilling( XEnum_Filling_Auto );
//			_main_group.set_Height( 40.f );
//
//
//
//
//
//
//		
//			_window.AddControl( _main_group , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--reflection-test" ) == 0 ) 
//		{
//			rux::gui::controls::XButton button;
//
//			XType type( button.get_Type() );
//
//			rux::PropertyInfo* property_info = type.get_PropertyInfo( "Left" );
//			if( property_info )
//			{
//				property_info->Set( button , ::rux::XFloat( 10 ) );
//
//				rux::XFloat left_float( property_info->Get( button ) );
//			}
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--user-control-test" ) == 0 ) 
//		{
//			::rux::gui::controls::XListBox test_user_control;
//
//			test_user_control.set_Left( 130 );
//			test_user_control.set_Top( 20 );
//			test_user_control.set_Width( 500 );
//			test_user_control.set_Height( 50 );
//
//			test_user_control.AddColumn( "date" );
//			test_user_control.AddColumn( "time" );
//
//			TestItemsVirtualization* test_items_virtualization = ::rux::engine::alloc_object< TestItemsVirtualization >();
//
//			//test_user_control.set_ItemsVirtualization( test_items_virtualization );
//			
//			_window.AddControl( test_user_control , error );
//
//			rux::gui::controls::XButton quit;
//			quit.set_OnClick( on_quit_click );
//			quit.set_Left( 10 );
//			quit.set_Top( 10 );
//			quit.set_Width( 100 );
//			quit.set_Height( 20 );
//			quit.set_Content( ::rux::XString( "Quit" ) );
//			_window.AddControl( quit , error );
//
//			rux::gui::controls::XButton increase;
//			increase.set_OnClick( on_listbox_increase_click );
//			increase.set_Left( 10 );
//			increase.set_Top( 35 );
//			increase.set_Width( 100 );
//			increase.set_Height( 20 );
//			increase.set_Content( ::rux::XString( "Increase" ) );
//			_window.AddControl( increase , error );
//		}
//		else if( argc == 2 && strcmp( argv[ 1 ] , "--png-test" ) == 0 ) 
//		{
//			rux::gui::controls::XRectangle test_rectangle3;
//			test_rectangle3.set_BorderColor( ::rux::gui::Colors::Blue( 0.5f ) );
//			test_rectangle3.set_Background( ::rux::gui::Colors::Green() );
//			test_rectangle3.set_BorderWidth( 5.f );
//			test_rectangle3.set_Left( 160 );
//			test_rectangle3.set_Top( 10 );
//			test_rectangle3.set_Width( 40 );
//			test_rectangle3.set_Height( 40 );
//			//_window.AddControl( test_rectangle3 , error );
//
//			_window.set_Background( ::rux::gui::Colors::Black() );
//
//			declare_stack_variable( char , executable_directory , 1024 );
//			rux_get_executable_directory( executable_directory );
//			rux_concatenation( executable_directory , "/../../addons/png_test" );
//			XArray< ::rux::io::XFile > files;
//			files.set_ByRef( ::rux::io::XDirectory::GetFiles( executable_directory ) );
//			float left = 10.f , top = 10.f;
//			size_t image_count = 0;			
//			XImage image0;
//			for( size_t index0 = 0 ; index0 < files.Count() ; index0++ )
//			{
//				rux::XString ext( files[ index0 ].get_FileExtension() ) , filename( files[ index0 ].get_FileName() );
//				filename.set_ByRef( ::rux::io::XPath::GetFileName( filename ) );
//				ext.set_ByRef( ext.ToLower() );
//				//if( filename == "loader24x26.png" )
//				{
//					if( ext == "png" )
//					{			
//						//for( size_t iii = 0 ; iii < 32 ; iii++ )
//						{
//							//if( image_count < 10 )
//							{	
//								rux::media::XFrame frame;
//								frame.set_ImageData( files[ index0 ].get_FileName() , error );
//							
//								image0 = XImage();
//								image0.set_Width( 24 );
//								image0.set_Height( 24 );
//								image0.set_Left( left );
//								image0.set_Top( top );
//								image0.set_IsAlpha( 1 );
//								//image0.set_Opacity( 0.7f );						
//							
//								//frame.set_Opacity( 0.7f , error );
//								//frame.ReplaceNotTransparentColors( ::rux::gui::Colors::DarkGrey() , error );
//
//								image0.set_Frame( frame );
//								_window.AddControl( image0 , error );
//								top += 40.f;
//								if( top >= 400.f )
//								{
//									top= 10.f;
//									left += 40.f;
//								}							
//							}
//							image_count++;
//						}
//					}
//				}
//			}
//		}
//		else
//		{
//			thread.set_Timeout( 1000 );
//			thread.set_ThreadMode( XThreadMode_Repeat );
//			thread.Start();
//			thread.Push( add_control_thread , &_window );
//			
//			XImage ui_image;
//			//ui_image.set_Width( 400 );
//			//ui_image.set_Height( 300 );
//			ui_image.set_Width( 800 );
//			ui_image.set_Height( 600 );
//			ui_image.set_Left( 0 );
//			ui_image.set_Top( 0 );
//			rux::media::XFrame frame;		
//			frame.set_ImageData( "1.yv12" , error );	
//			//frame = frame.ConvertToRGB32( 128 , error );
//			//frame.SaveAsBmp( ::rux::XString( module_directory ) + "\\1yv12.bmp" , error );
//			::rux::byte new_image = 0;
//			ui_image.set_FrameData( (char*)frame.get_ImageData( new_image ) , frame.get_ImageWidth() , frame.get_ImageHeight() , (rux::media::XEnum_Fourcc)frame.get_Fourcc() );
//									
//			_window.AddControl( ui_image , error );
//
//			frame = ::rux::media::XFrame();
//			frame.set_ImageData( ::rux::XString( module_directory ) + "\\2.yv12" , error );	
//			//frame.SaveAsBmp( ::rux::XString( module_directory ) + "\\i420.bmp" , error );
//			ui_image = XImage();
//			ui_image.set_Top( 150 );
//			ui_image.set_Width( 200 );	
//			ui_image.set_Height( 150 );
//		//	ui_image.set_Frame( frame );
//			//_window.AddControl( ui_image );	
//			
//			declare_variable( ::rux::gui::controls::XGroup , speed_group );
//			speed_group.set_IsVisible( 0 );
//			XAnimation speed_group_animation;	
//			speed_group_animation.set_Control( speed_group );
//			speed_group_animation.set_AnimationSetValueType( XEnum_AnimationSetValueType_Gradual );
//			speed_group_animation.set_StartValue( XFloat( 0 ) );
//			speed_group_animation.set_PropertyName( "Height" );
//			speed_group_animation.AddTimeline( 300000ULL , XFloat( 80 ) );
//			XGroupAnimation group_animation;
//			group_animation.set_AnimationType( XEnum_AnimationType_OneTime );
//			group_animation.AddAnimation( speed_group_animation );		
//			speed_group.set_Tag( group_animation );
//			speed_group.set_ControlName( "speed" );	
//			rux::gui::controls::XRectangle speed;
//			speed.set_HorizontalFilling( XEnum_Filling_Auto );
//			speed.set_VerticalFilling( XEnum_Filling_Auto );	
//			speed.set_Background( ::rux::gui::Colors::VerticalSliderPanelColor() );
//			speed_group.AddControl( speed , error );	
//			XSlider ui_slider;
//			ui_slider.set_MinValue( -16.f );
//			ui_slider.set_MaxValue( 16.f );
//			ui_slider.set_ValueStep( 2.f );
//			ui_slider.set_Top( 20 );
//			ui_slider.set_HorizontalFilling( XEnum_Filling_Auto );
//			ui_slider.set_VerticalFilling( XEnum_Filling_Auto );	
//			ui_slider.set_OnValueChanged( on_speed_value_changed );
//			//ui_slider.set_MouseClickValueChangeType( XEnum_SliderMouseClickValueChange_Instant );
//			speed_group.AddControl( ui_slider , error );
//			XTextBlock speed_textblock;
//			speed_textblock.set_Font( "tahoma.ttf" , 0 , 10 );
//			speed_textblock.set_HorizontalFilling( XEnum_Filling_Auto );
//			speed_textblock.set_Height( 20 );
//			speed_group.AddControl( speed_textblock , error );	
//			_window.AddControl( speed_group , error );
//
//			rux::gui::controls::XTextBox ui_text_box;
//			ui_text_box.set_Left( 120 );
//			ui_text_box.set_Top( 10 );
//			ui_text_box.set_Font( "verdana.ttf" , 0 , 12 );
//			ui_text_box.set_Text( "TextBox Санек  superable\nGamoskin\nRUX\nYeallow\nGreen\nKernel32" );
//			//_window.AddControl( ui_text_box );
//
//			rux::gui::controls::XTextBox ui_text_box_one_line;		
//			ui_text_box_one_line.set_Text( "LOW\nMKK" );
//			ui_text_box_one_line.set_Left( 40 );
//			ui_text_box_one_line.set_Top( 40 );
//			ui_text_box_one_line.set_Width( 200 );
//			ui_text_box_one_line.set_Height( 40 );
//			//_window.AddControl( ui_text_box_one_line , error );
//						
//			frame = ::rux::media::XFrame();
//			frame.set_ImageData( ::rux::XString( module_directory ) + "\\3.yv12" , error );
//			//frame.SaveAsBmp( ::rux::XString( module_directory ) + "\\test.bmp" , error );
//			ui_image = XImage();
//			ui_image.set_Left( 0 );
//			ui_image.set_Width( 0.5 );
//			ui_image.set_Height( 0.5 );
//			ui_image.set_FrameData( (char*)frame.get_ImageData( new_image ) , frame.get_ImageWidth() , frame.get_ImageHeight() , (rux::media::XEnum_Fourcc)frame.get_Fourcc() );
//			ui_image.set_Opacity( 0.3f );
//			ui_image.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			//_window.AddControl( ui_image );
//			declare_variable( ::rux::gui::controls::XGroup , ui_group0 );
//			ui_group0.set_Left( 0.5 );
//			ui_group0.set_Top( 0.5 );
//			ui_group0.set_Width( 0.5 );
//			ui_group0.set_Height( 0.5 );
//			ui_group0.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			//_window.AddControl( ui_group0 );
//			//ui_group0.AddControl( ui_image );
//
//
//			rux::gui::controls::XRectangle ui_rectangle;
//			ui_rectangle.set_Top( 0 );
//			ui_rectangle.set_Left( 0 );
//			ui_rectangle.set_Width( 200 );
//			ui_rectangle.set_Height( 150 );	
//			//ui_rectangle.set_Corner( ::rux::gui::Thickness( 0.5 , 0.5 , 0.2 , 0.2 ) );
//			//ui_rectangle.set_BorderWidth( 5.0 );
//			color0 = ::rux::gui::Colors::ButtonNormalColor();
//			//ui_rectangle.set_Background( color0 );
//			ui_rectangle.set_Background( create_color( 255 , 0 , 255 , 255 ) );
//			ui_rectangle.set_BorderColor( create_color( 0 , 0 , 255 , 255 ) );
//			//ui_rectangle.set_Opacity( 0.5 );
//			declare_variable( ::rux::gui::controls::XGroup , ui_recta_group );
//			ui_recta_group.set_Top( 60 );
//			ui_recta_group.set_Left( 200 );
//			ui_recta_group.set_Width( 200 );
//			ui_recta_group.set_Height( 150 );
//			//ui_recta_group.AddControl( ui_rectangle );
//			//_window.AddControl( ui_recta_group );	
//
//			//XAnimation ui_float_animation( &window );
//			//ui_float_animation.set_Control( ui_text_box );
//			//ui_float_animation.set_PropertyName( "Height" );
//			//ui_float_animation.set_StartValue( 20 );
//			//ui_float_animation.AddTimeline( 1 * 1000000ULL , 80 );
//			//ui_float_animation.AddTimeline( 2 * 1000000ULL , 20 );
//			//ui_float_animation.Start();
//			//XAnimation ui_float_animation1( &window );
//			//ui_float_animation1.set_Control( ui_text_box );
//			//ui_float_animation1.set_PropertyName( "Width" );
//			//ui_float_animation1.set_StartValue( 80 );
//			//ui_float_animation1.AddTimeline( 1 * 1000000ULL , 100 );
//			//ui_float_animation1.AddTimeline( 2 * 1000000ULL , 80 );
//			//ui_float_animation1.Start();
//
//			rux::gui::controls::XLine caret;
//			caret.set_Color( create_color( 255 , 0 , 0 , 128 ) );
//			caret.set_StartLeft( 50 );
//			caret.set_StartTop( 100 );
//			caret.set_EndLeft( 100 );
//			caret.set_EndTop( 50 );
//			caret.set_Thickness( 5 );
//			//_window.AddControl( caret );
//	
//			rux::gui::controls::XPath ui_path;
//			ui_path.AddPoint( ::rux::gui::Point( 0 , 30 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 20 , 20 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 30 , 40 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 40 , 20 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 60 , 30 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 40 , 0 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.AddPoint( ::rux::gui::Point( 20 , 0 , ::rux::gui::XEnum_PointUnit_Pixel ) );
//			ui_path.set_Left( 200 );
//			ui_path.set_Top( 20 );
//			ui_path.set_Width( 100 );
//			ui_path.set_Height( 100 );
//			//ui_path.set_Background( create_color( 128 , 128 , 0 , 128 , 1 ) );
//			ui_path.set_Background( linear_gradient );
//			//window.AddControl( ui_path );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , ui_group2 , XPrimitives::Plus( 40 , 40 , 15 , 15 , ::rux::gui::XEnum_PointUnit_Pixel , create_color( 255 , 0 , 0 , 255 ) ) );
//			//window.AddControl( ui_group2 );
//			declare_variable_param( ::rux::gui::controls::XGroup , ui_group3 , XPrimitives::Star( 70 , 70 , 25 , 25 , ::rux::gui::XEnum_PointUnit_Pixel , create_color( 255 , 255 , 128 , 255 ) ) );
//			//_window.AddControl( ui_group3 );
//
//			rux::gui::controls::XPath ui_path0;
//			ui_path0.AddPoint( ::rux::gui::Point( 0.5f , 0 , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0.35f , 0.33f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0 , 0.33f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0.28f , 0.58f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0.15f , 0.9f , ::rux::gui::XEnum_PointUnit_Percent ) );
//
//			ui_path0.AddPoint( ::rux::gui::Point( 0.5f , 0.69f , ::rux::gui::XEnum_PointUnit_Percent ) );
//	
//			ui_path0.AddPoint( ::rux::gui::Point( 0.85f , 0.9f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0.72f , 0.58f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 1 , 0.33f , ::rux::gui::XEnum_PointUnit_Percent ) );
//			ui_path0.AddPoint( ::rux::gui::Point( 0.65f , 0.33f , ::rux::gui::XEnum_PointUnit_Percent ) );
//
//			ui_path0.set_Left( 0 );
//			ui_path0.set_Top( 0 );
//			ui_path0.set_Width( 1 );
//			ui_path0.set_Height( 1 );
//			ui_path0.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );
//			ui_path0.set_Background( create_color( 255 , 255 , 128 , 255 ) );
//			declare_variable( ::rux::gui::controls::XGroup , ui_group1 );
//			ui_group1.set_Left( 70 );
//			ui_group1.set_Top( 70 );
//			ui_group1.set_Width( 25 );
//			ui_group1.set_Height( 25 );
//			//ui_group1.AddControl( ui_path0 );
//			//window.AddControl( ui_group1 );
//
//			rux::gui::controls::XRectangle ui_progress_bar_rectangle;
//			ui_progress_bar_rectangle.set_Left( 100 );
//			ui_progress_bar_rectangle.set_Top( 100 );
//			ui_progress_bar_rectangle.set_Width( 140 );
//			ui_progress_bar_rectangle.set_Height( 32 );
//			ui_progress_bar_rectangle.set_Background( linear_gradient );
//			//_window.AddControl( ui_progress_bar_rectangle );
//
//			XProgressBar ui_progress_bar;
//			ui_progress_bar.set_Left( 120 );
//			ui_progress_bar.set_Top( 108 );
//			ui_progress_bar.set_Width( 100 );
//			ui_progress_bar.set_Height( 16 );
//			//window.AddControl( ui_progress_bar );
//
//	
//			rux::gui::controls::XButton ui_button;	
//			ui_button.set_Top( 10 );
//			ui_button.set_Left( 10 );
//			ui_button.set_Width( 106 );
//			ui_button.set_Height( 80 );				
//			ui_button.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//			ui_button.set_Content( ::rux::XString( "Remove Пр" ) );
//			//_window.AddControl( ui_button );
//
//			ui_button = XButton();	
//			ui_button.set_Top( 46 );
//			ui_button.set_Left( 10 );
//			ui_button.set_Width( 106 );
//			ui_button.set_Height( 26 );				
//
//
//
//
//			ui_button = XButton();	
//			ui_button.set_Top( 82 );
//			ui_button.set_Left( 10 );
//			ui_button.set_Width( 106 );
//			ui_button.set_Height( 26 );				
//			//ui_button.set_Content( ::rux::XString( "Cols" ) );	
//			//_window.AddControl( ui_button );
//			//_window.AddControl( _cols_textblock );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , reel_of_film , XPrimitives::ReelOfFilm() );
//			reel_of_film.set_Left( 25 );
//			reel_of_film.set_Top( 25 );
//			reel_of_film.set_Width( 25 );
//			reel_of_film.set_Height( 25 );
//			rux::gui::controls::XButton archive_button;
//			archive_button.set_Left( 25 );
//			archive_button.set_Top( 25 );
//			archive_button.set_Width( 25 );
//			archive_button.set_Height( 25 );
//			archive_button.set_Opacity( 0.2f );
//			archive_button.set_Content( reel_of_film );
//			//_window.AddControl( archive_button );
//			declare_variable( ::rux::gui::controls::XGroup , button_panel );
//			button_panel.set_HorizontalFilling( XEnum_Filling_Auto );
//			//button_panel.set_IsVerticalScrollbarEnable( 1 );
//			button_panel.set_Top( 200);
//			button_panel.set_Left( 20 );
//			button_panel.set_Height( 100 );
//			//button_panel.set_Width( 200 );
//			button_panel.set_ChildLocationType( XEnum_Child_Location_Type_HorizontalStack );
//
//			rux::gui::controls::XRectangle button_panel_back;
//			button_panel_back.set_HorizontalFilling( XEnum_Filling_Auto );
//			button_panel_back.set_VerticalFilling( XEnum_Filling_Auto );
//			button_panel_back.set_Background( ::rux::gui::Colors::White() );
//			//button_panel.AddControl( button_panel_back , error );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , left_arrow , XPrimitives::LeftArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button0;
//			button0.set_Corner( ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) );
//			button0.set_OnMouseEnter( on_button_mouse_enter );
//			button0.set_OnMouseLeave( on_button_mouse_leave );		
//			button0.set_Content( left_arrow );
//			button0.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );	
//			button0.set_Width( 15 );
//			button0.set_Height( 15 );
//			button_panel.AddControl( button0 , error );	
//			//button_panel.AddControl( XPrimitives::Divider( XEnum_Alignment_LeftOrTop , 0 , 15 ) );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , right_arrow , XPrimitives::RightArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button1;
//			button1.set_OnMouseEnter( on_button_mouse_enter );
//			button1.set_OnMouseLeave( on_button_mouse_leave );		
//			button1.set_Corner( ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) );
//			button1.set_Content( right_arrow );
//			button1.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );	
//			button1.set_Width( 15 );
//			button1.set_Height( 15 );
//			//button_panel.AddControl( button1 , error );
//			declare_variable_param( ::rux::gui::controls::XGroup , down_arrow , XPrimitives::DownArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button2;
//			button2.set_OnMouseEnter( on_button_mouse_enter );
//			button2.set_OnMouseLeave( on_button_mouse_leave );		
//			button2.set_Content( down_arrow );
//			button2.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );	
//			button2.set_Width( 20 );
//			button2.set_Height( 20 );
//			//button_panel.AddControl( button2 );
//			declare_variable_param( ::rux::gui::controls::XGroup , up_arrow , XPrimitives::UpArrow( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button3;
//			button3.set_OnMouseEnter( on_button_mouse_enter );
//			button3.set_OnMouseLeave( on_button_mouse_leave );		
//			button3.set_Content( up_arrow );
//			button3.set_HorizontalAlignment( XEnum_Alignment_LeftOrTop );	
//			button3.set_Width( 20 );
//			button3.set_Height( 20 );
//			//button_panel.AddControl( button3 );
//			declare_variable_param( ::rux::gui::controls::XGroup , left_and_line_arrow , XPrimitives::LeftArrowAndLine( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button4;
//			button4.set_OnMouseEnter( on_button_mouse_enter );
//			button4.set_OnMouseLeave( on_button_mouse_leave );		
//			button4.set_Content( left_and_line_arrow );
//			button4.set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );	
//			button4.set_Width( 20 );
//			button4.set_Height( 20 );
//			//button_panel.AddControl( button4 , error );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , left_and_two_line_arrow , XPrimitives::MuteOn( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button5;
//			button5.set_ControlName( "mute" );
//			button5.set_OnMouseEnter( on_button_mouse_enter );
//			button5.set_OnMouseLeave( on_button_mouse_leave );		
//			button5.set_Content( left_and_two_line_arrow );
//			button5.set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );	
//			button5.set_Width( 20 );
//			button5.set_Height( 20 );
//			//button_panel.AddControl( button5 );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , right_and_line_arrow , XPrimitives::RightArrowAndLine( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button6;
//			button6.set_OnMouseEnter( on_button_mouse_enter );
//			button6.set_OnMouseLeave( on_button_mouse_leave );		
//			button6.set_Content( right_and_line_arrow );
//			button6.set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );	
//			button6.set_Width( 20 );
//			button6.set_Height( 20 );
//			//button_panel.AddControl( button6 );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , right_and_two_line_arrow , XPrimitives::RightArrowAndThreeLines( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button7;
//			button7.set_OnMouseEnter( on_button_mouse_enter );
//			button7.set_OnMouseLeave( on_button_mouse_leave );		
//			button7.set_Content( right_and_two_line_arrow );
//			button7.set_HorizontalAlignment( XEnum_Alignment_RightOrBottom );	
//			button7.set_Width( 15 );
//			button7.set_Height( 15 );
//			//button_panel.AddControl( button7 );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , stop , XPrimitives::Stop( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button8;
//			//button8.set_OnMouseEnter( on_button_mouse_enter );
//			//button8.set_OnMouseLeave( on_button_mouse_leave );		
//			button8.set_Corner( ::rux::gui::Thickness( 1.f , 1.f , 1.f , 1.f ) );
//			button8.set_Content( stop );
//			button8.set_Left( 200 );
//			button8.set_Width( 40 );
//			button8.set_Height( 40 );
//			button8.set_OnClick( on_show_time_message_click );
//			button_panel.AddControl( button8 , error );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , pause , XPrimitives::LeftFlag( create_color( 255 , 255 , 255 , (uint8)( 255.0 * 0.7 ) ) ) );
//			rux::gui::controls::XButton button9;
//			button9.set_OverBackground( ::rux::gui::Colors::ButtonAlertOverColor() );
//			button9.set_PressedBackground( ::rux::gui::Colors::ButtonAlertDownColor() );
//			button9.set_OnClick( on_quit_click );
//			button9.set_OnMouseEnter( on_button_mouse_enter );
//			button9.set_OnMouseLeave( on_button_mouse_leave );		
//			button9.set_Corner( ::rux::gui::Thickness( 0 , 0 , 0 , 0 ) );
//			button9.set_Content( pause );
//			button9.set_HorizontalAlignment( XEnum_Alignment_Center );	
//			button9.set_Top( 40 );
//			button9.set_Width( 40 );
//			button9.set_Height( 40 );
//			button_panel.AddControl( button9 , error );
//
//			XTextBlock scroll_text;
//			scroll_text.set_Foreground( ::rux::gui::Colors::Black() );
//			scroll_text.set_Top( 90 );
//			scroll_text.set_Height( 20 );
//			scroll_text.set_Width( 100 );
//			scroll_text.set_Left( 10 );
//			scroll_text.set_Text( "Пустяк" );
//			//button_panel.AddControl( scroll_text , error );
//			_window.AddControl( button_panel , error );
//
//			declare_variable( ::rux::gui::controls::XGroup , vertical_stack );
//			vertical_stack.set_ChildLocationType( XEnum_Child_Location_Type_VerticalStack );
//			vertical_stack.set_Top( 40 );
//			vertical_stack.set_Left( 400 );
//			vertical_stack.set_Width( 100 );
//			vertical_stack.set_Height( 90 );
//			//_window.AddControl( vertical_stack , error );
//
//			XTextBlock item0;
//			for( ::rux::uint32 item_index = 0 ; item_index < 32 ; item_index++ )
//			{
//				item0 = XTextBlock();
//				item0.set_VerticalAlignment( XEnum_Alignment_LeftOrTop );
//				item0.set_HorizontalFilling( XEnum_Filling_Auto );
//				item0.set_Height( 20 );
//				item0.set_Text( "Example0" );
//				vertical_stack.AddControl( item0 , error );
//			}
//		
//			XObject json_object( ::rux::data::XJsonSerializer::Deserialize( "{\"sets\":{\"internal\":[{\"name\":\"item0\"},{\"name5\":\"item1\"},{\"name\":\"item2\"}]}}" , error ) );
//
//			XSelect combobox;
//			combobox.set_BindingSource( json_object );
//			combobox.set_BindingSourcePath( "sets.internal" );
//			combobox.set_BindingSourceDisplayPropertyName( "name5" );
//			combobox.set_Left( 90.f );
//			combobox.set_Top( 500 );
//			combobox.set_Width( 100 );
//			combobox.set_Height( 20 );		
//			//_window.AddControl( combobox , error );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , camera , XPrimitives::Camera() );
//			camera.set_Left( 10.5 );
//			camera.set_Top( 0 );
//			camera.set_Width( 175 );
//			camera.set_Height( 175 );
//			//_window.AddControl( camera );
//
//			declare_variable_param( ::rux::gui::controls::XGroup , loader , XPrimitives::Loader() );
//			loader.set_Left( 10 );
//			loader.set_Top( 10 );
//			loader.set_Width( 25 );
//			loader.set_Height( 25 );
//
//			XGroupAnimation loader_animation;
//			XPrimitives::LoaderAnimation( loader , loader_animation , 200000ULL );
//			//loader_animation.Start();
//			//_window.AddControl( loader , error );
//
//			rux::gui::controls::XRectangle ui_rect;
//			//ui_rect.set_PointUnit( ::rux::gui::XEnum_PointUnit_Percent );	
//			ui_rect.set_Background( create_color( 0 , 0 , 0 , 0 ) );
//			ui_rect.set_Background( create_color( 0xd2 , 0xd4 , 0xd2 , 0xff ) );
//			ui_rect.set_BorderColor( create_color( 0xd2 , 0xd4 , 0xd2 , 0xff ) );
//			ui_rect.set_Corner( ::rux::gui::Thickness( 1 , 1 , 1 , 1 ) );
//			//ui_rect.set_BorderWidth( 2 );
//			ui_rect.set_Left( 64.5 );
//			ui_rect.set_Top( 37.5 );
//			ui_rect.set_Width( 12 );
//			ui_rect.set_Height( 12 );	
//			//_window.AddControl( ui_rect );
//			
//			XArray< ::rux::media::XWebCamera > webs;
//			webs.set_ByRef( ::rux::media::XWebCamera::get_MediaDevices( error ) );
//			if( webs.Count() > 0 )
//			{
//				//XArray< ::rux::media::XFrameFormat > frame_formats;
//				//frame_formats.set_ByRef( webs[ 0 ].get_FrameFormats( error ) );
//				//if( frame_formats.Count() > 0 )
//				//{
//				//	rux::XRectangleF area4x3( ::rux::XMath::CalculateRect( ::rux::XRectangleF( ::rux::XPointF( 2.0f , 2.0f ) , _window.get_Width() - 4.0f , _window.get_Height() - 4.0f ) , 4 , 3 ) );
//				//	rux::XPointF point0;
//				//	point0 = area4x3.get_Point();
//				//	XImage _web_ui_image;
//				//	_web_ui_image.set_Left( point0.get_X() );
//				//	_web_ui_image.set_Top( point0.get_Y() );
//				//	_web_ui_image.set_Width( area4x3.get_Width() );
//				//	_web_ui_image.set_Height( area4x3.get_Height() );
//				//	//_window.AddControl( _web_ui_image , error );
//				//	XTextBlock _fps_textblock;
//				//	_fps_textblock.set_Left( point0.get_X() );
//				//	_fps_textblock.set_Top( point0.get_Y() );
//				//	_fps_textblock.set_Width( 60 );
//				//	_fps_textblock.set_Height( 20 );
//				//	_fps_textblock.set_Background( create_color( 0 , 0 , 0 , 128 ) );
//				//	_window.AddControl( _fps_textblock , error );
//				//	webs[ 0 ].set_OnFrame( web_on_frame );
//				//	//webs[ 0 ].StartCapture( frame_formats[ 0 ] , error );
//				//}
//			}
//		}
//		_window.Show();
//		thread.Stop();
//		rux::engine::free_mem( module_directory );
//	}
//	return 0;
//}
