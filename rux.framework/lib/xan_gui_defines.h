#ifndef XAN_RENDER_CONTEXT_BASE_H
#define XAN_RENDER_CONTEXT_BASE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
#include <xan_includes.h>
namespace rux
{
	namespace gui
	{
		typedef void (* rux_dispatcher_t)(void*);	
	};
	namespace media
	{
		enum XEnum_Fourcc
		{
			XEnum_Fourcc_BI_RGB = 0x00000000 ,
			XEnum_Fourcc_I420 = 0x30323449 ,
			XEnum_Fourcc_RGB = 0x32424752 ,
			XEnum_Fourcc_YV12 = 0x32315659 ,
			XEnum_Fourcc_YUYV = 0x56595559 ,
			XEnum_Fourcc_BGR3 = 0x33524742 ,
			XEnum_Fourcc_GREY = 0x59455247 ,
			XEnum_Fourcc_Y800 = 0x30303859 ,
			XEnum_Fourcc_Y8 = 0x20203859 ,
			XEnum_Fourcc_Unknown = 0xffffffff
		};
#ifdef __WINDOWS__
#pragma pack( push )
#pragma pack( 1 )
		__declspec( align( 1 ) )
#endif
		struct XBitmapFileHeader
		{
			::rux::uint16 _type;  //specifies the file type
			::rux::uint32 _size;  //specifies the size in bytes of the bitmap file
			::rux::uint16 _reserved1;  //reserved; must be 0
			::rux::uint16 _reserved2;  //reserved; must be 0
			::rux::uint32 _off_bits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
		}
#ifdef __UNIX__
		__attribute__( ( packed , aligned( 1 ) ) )
#endif
			;
#ifdef __WINDOWS__
		__declspec( align( 1 ) )
#endif
		struct XBitmapInfoHeader
		{
			::rux::uint32 _size;  //specifies the number of bytes required by the struct
			::rux::uint32 _width;  //specifies width in pixels
			::rux::uint32 _height;  //species height in pixels
			::rux::uint16 _planes; //specifies the number of color planes, must be 1
			::rux::uint16 _bit_count; //specifies the number of bit per pixel
			::rux::int32 _compression;//spcifies the type of compression
			::rux::uint32 _size_image;  //size of image in bytes
			::rux::uint32 _x_pels_per_meter;  //number of pixels per meter in x axis
			::rux::uint32 _y_pels_per_meter;  //number of pixels per meter in y axis
			::rux::uint32 _clr_used;  //number of colors used by th ebitmap
			::rux::uint32 _clr_important;  //number of colors that are important
			::rux::uint32 _red_mask;
			::rux::uint32 _green_mask;
			::rux::uint32 _blue_mask;
			::rux::uint32 _alpha_mask;
			::rux::uint32 _cs_type;
			::rux::uint32 _endpoints[ 9 ];
			::rux::uint32 _gamma_red;
			::rux::uint32 _gamma_green;
			::rux::uint32 _gamma_blue;
			XBitmapInfoHeader()
			{
				_size = sizeof( XBitmapInfoHeader );
				_width = 0;
				_height = 0;
				_planes = 1;
				_bit_count = 0;
				_compression = XEnum_Fourcc_Unknown;
				_size_image = 0;
				_x_pels_per_meter = 0;
				_y_pels_per_meter = 0;
				_clr_used = 0;
				_clr_important = 0;
				_red_mask = 0;
				_green_mask = 0;
				_blue_mask = 0;
				_alpha_mask = 0;
				_cs_type = 0;
				memset( _endpoints , 0 , 9 * sizeof( ::rux::uint32 ) );
				_gamma_red = 0;
				_gamma_green = 0;
				_gamma_blue = 0;
			};
			XBitmapInfoHeader& operator =( const XBitmapInfoHeader& bitmap_info_header )
			{	
				_width = bitmap_info_header._width;
				_height = bitmap_info_header._height;	
				_bit_count = bitmap_info_header._bit_count;
				_compression = bitmap_info_header._compression;
				_size_image = bitmap_info_header._size_image;
				_x_pels_per_meter = bitmap_info_header._x_pels_per_meter;
				_y_pels_per_meter = bitmap_info_header._y_pels_per_meter;
				_clr_used = bitmap_info_header._clr_used;
				_clr_important = bitmap_info_header._clr_important;
				return *this;
			};
		}
#ifdef __UNIX__
		__attribute__( ( packed , aligned( 1 ) ) )
#endif
			;
#ifdef __WINDOWS__
#pragma pack( pop )
#endif
		dll_internal void rux_BGR24_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size  );
		dll_internal void rux_RGB32_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_BGR32_to_RGB32( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size );
		dll_internal void rux_BGR32_to_RGB24( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size );
		dll_internal void rux_YV12_to_BGR32( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha = 255 );
		dll_internal void rux_YUYV_to_BGR32( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size  );
		dll_internal void rux_RGB24_to_BGR32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_I420_to_BGR32( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size  );
		dll_internal void rux_BGR24_to_RGB24( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size );
		dll_internal void rux_RGB24_to_BGR24( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_BGR24_to_RGB32( const ::rux::uint8* BGR24_data_ptr , ::rux::uint32 BGR24_data_width , ::rux::uint32 BGR24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha = 255 );
		dll_internal void rux_I420_to_BGR24( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_YUYV_to_BGR24( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_YV12_to_RotatedBGR24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_YV12_to_BGR24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& BGR24_data_ptr , ::rux::int32& BGR24_data_size );
		dll_internal void rux_I420_to_RGB24( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size );
		dll_internal void rux_YUYV_to_RGB24( const ::rux::uint8* YUYV_data_ptr , ::rux::uint32 YUYV_data_width , ::rux::uint32 YUYV_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size );
		dll_internal void rux_YV12_to_RGB24( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB24_data_ptr , ::rux::int32& RGB24_data_size  );
		dll_internal void rux_YV12_to_RGB32( const ::rux::uint8* YV12_data_ptr , ::rux::uint32 YV12_data_width , ::rux::uint32 YV12_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha = 255 );
		dll_internal void rux_I420_to_RGB32( const ::rux::uint8* I420_data_ptr , ::rux::uint32 I420_data_width , ::rux::uint32 I420_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha = 255 );
		dll_internal void rux_RGB24_to_RGB32( const ::rux::uint8* RGB24_data_ptr , ::rux::uint32 RGB24_data_width , ::rux::uint32 RGB24_data_height , ::rux::uint8*& RGB32_data_ptr , ::rux::int32& RGB32_data_size , ::rux::uint8 alpha = 255 );
		dll_internal bool convert_to_rgb24( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size );
		dll_internal bool convert_to_rgb32( ::rux::uint8* image_data , ::rux::uint32 image_width , ::rux::uint32 image_height , ::rux::uint32 image_data_size , ::rux::int16 bit_count , ::rux::int32 compression , ::rux::uint8*& destination , ::rux::int32& destination_size , float alpha );
		dll_internal ::rux::uint32 image_size( ::rux::int32 fourcc , ::rux::uint32 width , ::rux::uint32 height , ::rux::int16 bit_count );
		// Y = 0.30R + 0.59G + 0.11B rgb -> grey       grey -> rgb  R = G = B = Y
	};
	namespace gui
	{
		class WindowBase;
		enum XEnum_Cursors
		{
			XEnum_Cursors_SizeAll , 
			XEnum_Cursors_Arrow , 
			XEnum_Cursors_SizeWE ,
			XEnum_Cursors_SizeNS ,
			XEnum_Cursors_SizeNWSE , 
			XEnum_Cursors_Hand
		};
		enum XEnum_PointUnit
		{
			XEnum_PointUnit_Pixel ,
			XEnum_PointUnit_Percent , 
			XEnum_PointUnit_PercentOnlyLocation , 
			XEnum_PointUnit_PercentOnlySize
		};		
		enum XEnum_EventType
		{
			XEnum_EventType_GuiInitialize = 0 , 
			XEnum_EventType_WindowShown = 1 ,
			XEnum_EventType_WindowHided = 2 , 
			XEnum_EventType_WindowLoaded = 3 ,
			XEnum_EventType_WindowClosed = 4 ,
			XEnum_EventType_WindowMinimizedState = 5 ,
			XEnum_EventType_WindowMaximizedState = 6 ,
			XEnum_EventType_WindowNormalState = 7 , 
			XEnum_EventType_WindowKeyUp = 8 ,
			XEnum_EventType_WindowKeyDown = 9 , 
			XEnum_EventType_WindowMove = 10 , 
			XEnum_EventType_WindowSize = 11 ,
			XEnum_EventType_WindowKeyChar = 12 ,
			XEnum_EventType_WindowMouseWheel = 13 ,
			XEnum_EventType_WindowMouseMove = 14 ,
			XEnum_EventType_WindowLeftMouseButtonDown = 15 ,
			XEnum_EventType_WindowLeftMouseButtonUp = 16 ,
			XEnum_EventType_WindowLeftMouseButtonDoubleClick = 17 ,
			XEnum_EventType_GuiDeInitialize = 18 , 
			XEnum_EventType_WindowRender = 19 , 
			XEnum_EventType_RenderContextCreated = 20 , 
			XEnum_EventType_WindowLeave = 21 ,
			XEnum_EventType_WindowMouseWheelUp = 22 , 
			XEnum_EventType_WindowRightMouseButtonDown = 23 ,
			XEnum_EventType_WindowRightMouseButtonUp = 24 ,
			XEnum_EventType_WindowRightMouseButtonDoubleClick = 25,
			XEnum_EventType_WindowMouseWheelDown = 26,
			XEnum_EventType_WindowMouseWheelDoubleClick = 27
		};
		enum XEnum_WindowState
		{
			XEnum_WindowState_Normal ,
			XEnum_WindowState_Maximized ,
			XEnum_WindowState_Minimized
		};
		enum XEnum_Keys
		{
			XEnum_Keys_Delete , 
			XEnum_Keys_Left , 
			XEnum_Keys_Right , 
			XEnum_Keys_Down ,
			XEnum_Keys_Up ,
			XEnum_Keys_Home , 
			XEnum_Keys_End ,
			XEnum_Keys_Back ,
			XEnum_Keys_Enter , 
			XEnum_Keys_Control_V ,
			XEnum_Keys_Control_C ,
			XEnum_Keys_Control_X ,
			XEnum_Keys_Control_A ,
			XEnum_Keys_Control_D ,
			XEnum_Keys_Tab , 
			XEnum_Keys_Unknown
		};
		struct Event
		{
			::rux::int32 _type;
			Event( ::rux::int32 type )
			{
				_type = type;
			};
		};		
		struct WindowEvent : ::rux::gui::Event
		{
			WindowBase* _window;
			WindowEvent( WindowBase* window , ::rux::int32 type )
				: Event( type )
			{
				_window = window;
			};
		};
		struct GuiInitializeEvent : ::rux::gui::Event
		{
			GuiInitializeEvent()
				: Event( XEnum_EventType_GuiInitialize )
			{
			};
		};	
		struct GuiDeInitializeEvent : ::rux::gui::Event
		{
			GuiDeInitializeEvent()
				: Event( XEnum_EventType_GuiDeInitialize )
			{
			};
		};
		struct WindowShownEvent : ::rux::gui::WindowEvent
		{			
			WindowShownEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowShown )
			{
			};
		};
		struct RenderContextCreatedEvent : ::rux::gui::WindowEvent
		{			
			RenderContextCreatedEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_RenderContextCreated )
			{
			};
		};
		struct WindowLeave : ::rux::gui::WindowEvent
		{			
			WindowLeave( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowLeave )
			{
			};
		};
		struct WindowHidedEvent : ::rux::gui::WindowEvent
		{
			WindowHidedEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowHided )
			{
			};
		};
		struct WindowLoadedEvent : ::rux::gui::WindowEvent
		{
			WindowLoadedEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowLoaded )
			{
			};
		};
		struct WindowClosedEvent : ::rux::gui::WindowEvent
		{
			WindowClosedEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowClosed )
			{
			};
		};	
		struct WindowKeyDownEvent : ::rux::gui::WindowEvent
		{
			XEnum_Keys _key;
			::rux::uint8 _alt;
			::rux::uint8 _control;
			::rux::uint8 _shift;
			::rux::uint32 _windows_vk;
			WindowKeyDownEvent( WindowBase* window , XEnum_Keys key ,
				::rux::uint8 alt , ::rux::uint8 control , ::rux::uint8 shift , ::rux::uint32 windows_vk )
				: WindowEvent( window , XEnum_EventType_WindowKeyDown )
			{
				_key = key;
				_alt = alt;
				_control = control;
				_shift = shift;
				_windows_vk = windows_vk;
			};
		};		
		struct WindowKeyUpEvent : ::rux::gui::WindowEvent
		{
			XEnum_Keys _key;
			::rux::uint8 _alt;
			::rux::uint8 _control;
			::rux::uint8 _shift;
			::rux::uint32 _windows_vk;
			WindowKeyUpEvent( WindowBase* window , XEnum_Keys key ,
				::rux::uint8 alt , ::rux::uint8 control , ::rux::uint8 shift , ::rux::uint32 windows_vk )
				: WindowEvent( window , XEnum_EventType_WindowKeyUp )
			{
				_key = key;
				_alt = alt;
				_control = control;
				_shift = shift;
				_windows_vk = windows_vk;
			};
		};
		struct WindowMouseEvent : ::rux::gui::WindowEvent
		{		
			::rux::int32 _delta;
			::rux::int32 _x;
			::rux::int32 _y;
			::rux::uint8 _alt;
			::rux::uint8 _control;
			::rux::uint8 _shift;
			::rux::uint8 _left_mouse_button_pressed;
			::rux::uint8 _middle_mouse_button_pressed;
			::rux::uint8 _right_mouse_button_pressed;
			::rux::uint8 _x_button1_pressed;
			::rux::uint8 _x_button2_pressed;
			WindowMouseEvent( XEnum_EventType event_type , WindowBase* window , ::rux::int32 delta , ::rux::int32 x , ::rux::int32 y ,
				::rux::uint8 alt , ::rux::uint8 control , ::rux::uint8 shift , ::rux::uint8 left_mouse_button_pressed ,
				::rux::uint8 middle_mouse_button_pressed , ::rux::uint8 right_mouse_button_pressed ,
				::rux::uint8 x_button1_pressed , ::rux::uint8 x_button2_pressed )
				: WindowEvent( window , event_type )
			{		
				_delta = delta;
				_x = x;
				_y = y;
				_alt = alt;
				_control = control;
				_shift = shift;
				_left_mouse_button_pressed = left_mouse_button_pressed;
				_middle_mouse_button_pressed = middle_mouse_button_pressed;
				_right_mouse_button_pressed = right_mouse_button_pressed;
				_x_button1_pressed = x_button1_pressed;
				_x_button2_pressed = x_button2_pressed;
			};
		};	
		struct WindowKeyCharEvent : ::rux::gui::WindowEvent
		{
			char _utf8_character[ 20 ];
			size_t _length;
			::rux::uint8 _alt;
			::rux::uint8 _control;
			::rux::uint8 _shift;
			WindowKeyCharEvent( WindowBase* window , char* utf8_character , size_t length ,
				::rux::uint8 alt , ::rux::uint8 control , ::rux::uint8 shift )
				: WindowEvent( window , XEnum_EventType_WindowKeyChar )
			{
				memset( _utf8_character , 0 , 20 );
				::memcpy( _utf8_character , utf8_character , length );
				_length = length;				
				_alt = alt;
				_control = control;
				_shift = shift;
			};
		};		
		struct WindowMinimizedStateEvent : ::rux::gui::WindowEvent
		{
			WindowMinimizedStateEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowMinimizedState )
			{
			};
		};		
		struct WindowMaximizedStateEvent : ::rux::gui::WindowEvent
		{
			WindowMaximizedStateEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowMaximizedState )
			{
			};
		};
		struct WindowNormalStateEvent : ::rux::gui::WindowEvent
		{
			WindowNormalStateEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowNormalState )
			{
			};
		};
		struct WindowSizeEvent : ::rux::gui::WindowEvent
		{
			::rux::int32 _width;
			::rux::int32 _height;
			WindowSizeEvent( WindowBase* window , ::rux::int32 width , ::rux::int32 height )
				: WindowEvent( window , XEnum_EventType_WindowSize )
			{
				_width = width;
				_height = height;
			};
		};
		struct WindowMoveEvent : ::rux::gui::WindowEvent
		{
			WindowMoveEvent( WindowBase* window )
				: WindowEvent( window , XEnum_EventType_WindowMove )
			{
			};
		};
		typedef void (* on_event_t)( ::rux::gui::Event& event_object , void* owner );
		struct Color;
		class RenderCacheBase
		{
		public	:
			::rux::int32 _cache_type;
			RenderCacheBase( ::rux::int32 cache_type );
			virtual ::rux::uint8 CopyToTexture( ::rux::uint8* , ::rux::uint32 , ::rux::uint32 , ::rux::uint32 , ::rux::int16 , ::rux::int32 , float , size_t ) = 0;
		};		
		class RenderContextBase;
		struct WindowRenderEvent : ::rux::gui::WindowEvent
		{			
			RenderContextBase* _render_context;
			WindowRenderEvent( WindowBase* window , RenderContextBase* render_context )
				: WindowEvent( window , XEnum_EventType_WindowRender )
			{
				_render_context = render_context;
			};
		};
		struct Point
		{
			float _x;
			float _y;
			::rux::gui::XEnum_PointUnit _point_unit;
			Point();
			Point( float left , float top , ::rux::gui::XEnum_PointUnit point_unit );
			Point( const Point& point );			
			Point& operator =( const Point& point );
		};
		class WindowBase
		{
		public:
			virtual void Show( void ) = 0;
			virtual void Hide( void ) = 0;
			virtual void Close( void ) = 0;
			virtual void Minimize( void ) = 0;
			virtual void Maximize( void ) = 0;
			virtual void Restore( void ) = 0;
			virtual void set_Owner( void* owner ) = 0;
			virtual void* get_Owner( void ) = 0;
			virtual void set_TopMost( ::rux::uint8 top_most ) = 0;
			virtual ::rux::uint8 get_TopMost( void ) = 0;
			virtual ::rux::uint8 get_IsVisible( void ) = 0;
			virtual void set_Width( ::rux::int32 width ) = 0;
			virtual void set_Height( ::rux::int32 width ) = 0;
			virtual void set_Left( ::rux::int32 width ) = 0;
			virtual void set_Top( ::rux::int32 width ) = 0;
			virtual void set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height ) = 0;
			virtual ::rux::int32 get_Left( void ) = 0;
			virtual ::rux::int32 get_Top( void ) = 0;
			virtual ::rux::int32 get_Width( void ) = 0;
			virtual ::rux::int32 get_Height( void ) = 0;
			virtual void set_MinWidth( ::rux::int32 width ) = 0;
			virtual void set_MinHeight( ::rux::int32 height ) = 0;
			virtual ::rux::gui::XEnum_WindowState get_WindowState( void ) = 0;
			virtual void CaptureMouse( void ) = 0;
			virtual void ReleaseMouse( void ) = 0;
			virtual void set_Cursor( ::rux::gui::XEnum_Cursors cursor ) = 0;
			virtual void get_CursorPosition( ::rux::gui::Point& position ) = 0;
			virtual void set_IsAllowResize( ::rux::byte is_allow_resize ) = 0;
			virtual void set_IsAllowRelocate( ::rux::byte is_allow_relocate ) = 0;
			virtual void set_IsAllowClose( ::rux::byte is_allow_relocate ) = 0;
		};
		class FontBase
		{
		public:
			::rux::uint32 _font_file_name_hash;
			char _font_file_name[ 1024 ];	
			::rux::uint32 _font_short_name_hash;
			::rux::uint32 _font_size_width;
			::rux::uint32 _font_size_height;	
			void* _free_type_font;
			virtual float get_Height( const char* utf8text , size_t ___rux__thread_index1986 ) = 0;
			virtual float get_Height( void ) = 0;
			virtual float get_CharacterWidth( ::rux::uint32 utf8_character , size_t ___rux__thread_index1986 ) = 0;
			virtual float get_TextWidth( const char* utf8text , size_t ___rux__thread_index1986 ) = 0;
		};
		struct ColorBase
		{
			::rux::int32 _color_type;
			rux_volatile _ref;
			ColorBase( ::rux::int32 color_type );
			void Release( void );
			void AddRef( void );
			virtual ::rux::uint8 Equals( ColorBase* color ) = 0;
			virtual ::rux::uint8 get_IsAlpha( void ) = 0;
			virtual ::rux::uint8 get_IsVisible( void ) = 0;
		};
		struct Color : public ColorBase
		{			
			::rux::uint8 _red;
			::rux::uint8 _green;
			::rux::uint8 _blue;
			::rux::uint8 _alpha;
			::rux::uint8* _shader_color_map;	
			Color();
			~Color();
			Color( ::rux::uint8 red , ::rux::uint8 green , ::rux::uint8 blue , ::rux::uint8 alpha );
			Color( const Color& color );
			Color& operator =( const Color& color );
			void set_Red( ::rux::uint8 red );
			void set_Green( ::rux::uint8 green );
			void set_Blue( ::rux::uint8 blue );
			void set_Alpha( ::rux::uint8 alpha );
			::rux::uint8 get_Alpha( void );
			::rux::uint8 get_IsAlpha( void );
			::rux::uint8 get_Red( void );
			::rux::uint8 get_Blue( void );
			::rux::uint8 get_Green( void );
			bool operator ==( const Color& color );
			::rux::uint8 get_IsVisible( void );			
			::rux::uint32 get_ShaderColorMapWidth( void );
			::rux::uint8* get_ShaderColorMap( void );
			static Color* FromString( const char* color_string );
			virtual ::rux::uint8 Equals( ColorBase* color );
		};
		struct GradientStop
		{					
			::rux::gui::Color _color;
			float _offset;
			GradientStop();			
			GradientStop( const ::rux::gui::Color& color , float offset );
			GradientStop( const GradientStop& gradientstop );
			GradientStop& operator =( const GradientStop& gradientstop );	
			bool operator ==( const GradientStop& gradient_stop );
		};
		enum XEnum_LinearGradientType
		{
			XEnum_LinearGradientType_Horizontal ,
			XEnum_LinearGradientType_Vertical
		};
		struct LinearGradientColor : public ColorBase
		{					
			XEnum_LinearGradientType _linear_gradient_type;
			XCrtSect _cs_gradient_stops;
			XMallocArray< GradientStop* > _gradient_stops;
			::rux::uint8* _shader_color_map;
			::rux::uint8* _shader_color_offset_map;		
			LinearGradientColor();
			~LinearGradientColor();
			LinearGradientColor( const LinearGradientColor& lineargradientcolor );
			LinearGradientColor& operator =( const LinearGradientColor& lineargradientcolor );
			void AddGradientStop( const ::rux::gui::GradientStop& gradient_stop );
			void set_LinearGradientType( XEnum_LinearGradientType linear_gradient_type );
			::rux::uint32 get_ShaderColorMapWidth( void );
			::rux::uint8* get_ShaderColorMap( void );
			::rux::uint8* get_ShaderColorOffsetMap( void );
			::rux::uint32 get_ShaderColorOffsetMapWidth( void );
			::rux::uint8 get_IsAlpha( void );
			::rux::uint8 get_IsVisible( void );			
			virtual ::rux::uint8 Equals( ColorBase* color );
			bool operator ==( const LinearGradientColor& linear_gradient_color );
		};		
		class Colors
		{
		public:
			static LinearGradientColor* VerticalSliderPanelColor( void );
			static LinearGradientColor* VerticalSliderThumbColor( void );
			static LinearGradientColor* VerticalSliderOverThumbColor( void );
			static LinearGradientColor* VerticalSliderDraggingThumbColor( void );
			static LinearGradientColor* WindowHeaderColor( void );
			static Color* Red( float opacity = 1.f );
			static Color* Blue( float opacity = 1.f );
			static Color* Green( float opacity = 1.f );
			static Color* LightGrey( float opacity = 1.f );
			static Color* Grey( float opacity = 1.f );
			static Color* DarkGrey( float opacity = 1.f );
			static Color* White( float opacity = 1.f );
			static Color* Black( float opacity = 1.f , const char* __file__ = __FILE__ , ::rux::int32 __line__ = __LINE__ );
			static Color* DarkSlateGray( float opacity = 1.f );
			static Color* BattleshipGrey( float opacity = 1.f );
			static Color* Arsenic( float opacity = 1.f );
			static Color* Transparent( void );
			static Color* SliderTrackNormalBackground( void );
			static Color* HighlightColor( void );
			static Color* ControlDarkDarkColor( void );
			static ::rux::gui::ColorBase* ButtonOverColor( void );
			static ::rux::gui::ColorBase* ButtonNormalColor( void );
			static ::rux::gui::ColorBase* HorizontalButtonNormalColor( void );
			static ::rux::gui::ColorBase* ButtonDownColor( float opacity = 1.f );
			static ::rux::gui::ColorBase* ButtonDisabledColor( void );
			static ::rux::gui::ColorBase* WindowBackgroundColor( const char* __file__ = __FILE__ , ::rux::int32 __line__ = __LINE__ );
			static ::rux::gui::ColorBase* WindowHeaderBackgroundColor( void );
			static ::rux::gui::ColorBase* GlyphColor( void );
			static ::rux::gui::Color* CellBorderColor( void );
			static ::rux::gui::ColorBase* CellBackgroundColor( void );
			static ::rux::gui::ColorBase* ButtonAlertDownColor( void );
			static ::rux::gui::ColorBase* ButtonAlertOverColor( void );
		};
#define create_color( red , green , blue , alpha ) alloc_object_4_params_macros( ::rux::gui::Color , (::rux::uint8)red , (::rux::uint8)green , (::rux::uint8)blue , (::rux::uint8)alpha )		
		rux_inline void copy_color( ::rux::gui::ColorBase*& dest , ::rux::gui::ColorBase* src )
		{
			if( src )
			{
				src->AddRef();
				if( dest )
					dest->Release();
				dest = src;
			}
		};
		rux_inline void copy_color( ::rux::gui::Color*& dest , ::rux::gui::Color* src )
		{
			if( src )
			{
				src->AddRef();
				if( dest )
					dest->Release();
				dest = src;
			}
		};
		rux_inline void copy_color( ::rux::gui::Color*& dest , ::rux::gui::ColorBase* src )
		{
			if( src )
			{
				if( src->_color_type == 0 )
					copy_color( dest , (::rux::gui::Color*)src );
			}
		};
		struct Thickness
		{
			float _left_down_corner;
			float _left_up_corner;	
			float _right_down_corner;
			float _right_up_corner;
			Thickness();
			Thickness( float left_down_corner , float left_up_corner , 	float right_down_corner , float right_up_corner );
			Thickness( const Thickness& thickness );
			Thickness& operator =( const Thickness& thickness );
			bool operator !=( const Thickness& thickness );
		};
		struct Margin
		{
			float _left_offset;
			float _right_offset;	
			float _top_offset;
			float _bottom_offset;
			Margin();
			Margin( float left_offset , float right_offset , float top_offset , float bottom_offset );
			Margin( const Margin& margin );
			Margin& operator =( const Margin& margin );
			bool operator !=( const Margin& margin );
		};
		rux_inline ::rux::uint8 rux_color_is_visible( ::rux::gui::ColorBase* color_object )
		{
			::rux::uint8 is_visible = 1;
			if( color_object && color_object->_color_type == 0 )
				is_visible = ((::rux::gui::Color*)color_object)->get_IsVisible();
			else if( color_object && color_object->_color_type == 1 )
				is_visible = ((::rux::gui::LinearGradientColor*)color_object)->get_IsVisible();
			else
				is_visible = 0;
			return is_visible;
		};
		struct Rectangle
		{
			::rux::int32 _left;
			::rux::int32 _top;
			::rux::int32 _width;
			::rux::int32 _height;
			Rectangle( void );
			Rectangle( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height );
			Rectangle& operator =( const Rectangle& rectangle );
			::rux::uint8 IsPointInside( ::rux::int32 x , ::rux::int32 y );
			::rux::uint8 IsIntersects( ::rux::gui::Rectangle& rectangle );
		};
		class RenderContextBase
		{
		public:			
			virtual void DrawImage( ::rux::int32 fourcc , ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , float opacity , ::rux::gui::RenderCacheBase** cache , float brightness , float contrast , float& _selected_z_index , ::rux::uint8 is_alpha = 0 , rux::int32 image_left = 0 , rux::int32 image_top = 0 , rux::int32 image_width = INT32_MAX , rux::int32 image_height = INT32_MAX , const char* __filename__ = __FILE__ , ::rux::int32 __line__ = __LINE__ , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
			virtual void DrawRectangle( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , ::rux::int32 border_width , ::rux::gui::ColorBase* background_object_color , ::rux::gui::Color* border , const ::rux::gui::Thickness& corner , float opacity , ::rux::gui::RenderCacheBase** border_cache , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index , ::rux::uint8 smooth = 1 , const char* __filename__ = __FILE__ , ::rux::int32 __line__ = __LINE__ , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
			virtual void DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< ::rux::gui::Point* >& points , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index , ::rux::uint8 smooth = 1 , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
			virtual void DrawPath( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , XMallocArray< float >& point_x , XMallocArray< float >& point_y , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , float& _selected_z_index , ::rux::uint8 smooth = 1 , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
			virtual void DrawText( ::rux::int32 left , ::rux::int32 top , ::rux::int32 width , ::rux::int32 height , const char* utf8_text , ::rux::gui::FontBase* font , ::rux::gui::Color* color , ::rux::gui::RenderCacheBase** cache , float opacity , float& _selected_z_index , const char* __filename__ = __FILE__ , ::rux::int32 __line__ = __LINE__ , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
			virtual void set_Clip( ::rux::gui::Rectangle clip ) = 0;
			virtual ::rux::gui::Rectangle& get_Clip( void ) = 0;
			virtual void DrawLine( ::rux::int32 start_left , ::rux::int32 start_top , ::rux::int32 end_left , ::rux::int32 end_top , ::rux::int32 thickness , float opacity , ::rux::gui::ColorBase* background_object_color , ::rux::gui::RenderCacheBase** cache , ::rux::gui::RenderCacheBase** cap1_cache , ::rux::gui::RenderCacheBase** cap2_cache , float& _selected_z_index , size_t ___rux__thread_index1986 = SIZE_MAX ) = 0;
		};
		class ScreenBase
		{		
		public:		
			virtual ::rux::int32 get_Left( void ) = 0;
			virtual ::rux::int32 get_Top( void ) = 0;
			virtual ::rux::int32 get_Width( void ) = 0;
			virtual ::rux::int32 get_Height( void ) = 0;		
		};
		namespace engine
		{
			struct schedule
			{
				::rux::gui::rux_dispatcher_t _dispatcher;
				void* _param;
				::rux::uint32 _timeout;
				::rux::uint32 _execute_time;
			};
			typedef XMallocArray< ::rux::gui::ScreenBase* >* (*rux_gui_get_screens_t)( void );
			typedef void (*rux_gui_free_screens_t)( XMallocArray< ::rux::gui::ScreenBase* >* screens );
			typedef void (*rux_gui_start_t)( ::rux::gui::on_event_t on_event , void* owner );
			typedef ::rux::gui::WindowBase* (*rux_gui_create_window_t)( ::rux::gui::on_event_t on_event );
			typedef ::rux::uint8 (*rux_gui_dispatch_t)( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::gui::WindowBase* window , ::rux::uint8 wait );
			typedef size_t (*gui_add_schedule_t)( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint64 timeout_in_microseconds );
			typedef void (*gui_remove_schedule_t)( size_t schedule_index );
			typedef void (*rux_gui_destroy_window_t)( ::rux::gui::WindowBase* );
			typedef ::rux::gui::FontBase* (*rux_gui_create_font_t)( const char* font_file_name , ::rux::uint32 font_size_height ,	::rux::uint32 font_size_width );
			typedef void (*rux_gui_add_cache_t)( ::rux::gui::RenderCacheBase* cache );
			typedef size_t (*rux_gui_windows_count_t)( void );
			typedef void (*rux_gui_close_all_windows_t)( void );
			class globals
			{
			public:
				globals( void );
				~globals( void );
				::rux::gui::engine::rux_gui_start_t _rux_gui_start;
				::rux::gui::engine::rux_gui_start_t _rux_gui_stop;
				::rux::gui::engine::rux_gui_create_window_t _rux_gui_create_window;
				::rux::gui::engine::rux_gui_windows_count_t _rux_gui_windows_count;
				::rux::gui::engine::rux_gui_destroy_window_t _rux_gui_destroy_window;
				::rux::gui::engine::rux_gui_create_font_t _rux_gui_create_font;
				::rux::gui::engine::rux_gui_add_cache_t _rux_gui_add_cache;
				::rux::gui::engine::rux_gui_dispatch_t _rux_gui_dispatch;
				::rux::gui::engine::gui_add_schedule_t _gui_add_schedule;
				::rux::gui::engine::gui_remove_schedule_t _gui_remove_schedule;
				::rux::gui::engine::rux_gui_close_all_windows_t _rux_gui_close_all_windows;
				::rux::gui::engine::rux_gui_get_screens_t _rux_gui_get_screens;
				::rux::gui::engine::rux_gui_free_screens_t _rux_gui_free_screens;
				::rux::rux_gui_get_message_processing_thread_id_t _rux_gui_get_message_processing_thread_id;
				::rux::rux_gui_pump_message_t _rux_gui_pump_message;
				::rux::threading::RdWrLock* _cs_gui_schedule;
				XMallocArray< ::rux::gui::engine::schedule* >* _gui_schedule;
				XMallocArray< ::rux::uint32 >* _debug_utf8_control_names;
			};
		};
	};
};
#endif
