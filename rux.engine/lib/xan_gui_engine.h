#ifndef XAN_GUI_ENGINE_H
#define XAN_GUI_ENGINE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_gui_defines.h>
#include <xan_opengl_functions.h>
#include <xan_render_font.h>
#include <xan_malloc_array.h>
#include <xan_user_data.h>
namespace rux
{
	namespace gui
	{
#if !defined( __WINDOWS__ )
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    
#define VK_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VK_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14
#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19
#define VK_ESCAPE         0x1B
#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F
#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D
#define VK_SLEEP          0x5F
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#define VK_OEM_NEC_EQUAL  0x92
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5
#define VK_BROWSER_BACK        0xA6
#define VK_BROWSER_FORWARD     0xA7
#define VK_BROWSER_REFRESH     0xA8
#define VK_BROWSER_STOP        0xA9
#define VK_BROWSER_SEARCH      0xAA
#define VK_BROWSER_FAVORITES   0xAB
#define VK_BROWSER_HOME        0xAC
#define VK_VOLUME_MUTE         0xAD
#define VK_VOLUME_DOWN         0xAE
#define VK_VOLUME_UP           0xAF
#define VK_MEDIA_NEXT_TRACK    0xB0
#define VK_MEDIA_PREV_TRACK    0xB1
#define VK_MEDIA_STOP          0xB2
#define VK_MEDIA_PLAY_PAUSE    0xB3
#define VK_LAUNCH_MAIL         0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1         0xB6
#define VK_LAUNCH_APP2         0xB7
#define VK_OEM_1          0xBA   // ';:' for US
#define VK_OEM_PLUS       0xBB   // '+' any country
#define VK_OEM_COMMA      0xBC   // ',' any country
#define VK_OEM_MINUS      0xBD   // '-' any country
#define VK_OEM_PERIOD     0xBE   // '.' any country
#define VK_OEM_2          0xBF   // '/?' for US
#define VK_OEM_3          0xC0   // '`~' for US
#define VK_OEM_4          0xDB  //  '[{' for US
#define VK_OEM_5          0xDC  //  '\|' for US
#define VK_OEM_6          0xDD  //  ']}' for US
#define VK_OEM_7          0xDE  //  ''"' for US
#define VK_OEM_8          0xDF
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO
#define VK_PROCESSKEY     0xE5
#define VK_ICO_CLEAR      0xE6
#define VK_PACKET         0xE7
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5
#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE
#else
#define XK_BackSpace            0xFF08  /* back space, back char */
#define XK_Tab                  0xFF09
#define XK_Linefeed             0xFF0A  /* Linefeed, LF */
#define XK_Clear                0xFF0B
#define XK_Return               0xFF0D  /* Return, enter */
#define XK_Pause                0xFF13  /* Pause, hold */
#define XK_Scroll_Lock          0xFF14
#define XK_Sys_Req              0xFF15
#define XK_Escape               0xFF1B
#define XK_Delete               0xFFFF  /* Delete, rubout */
#define XK_Multi_key            0xFF20  /* Multi-key character compose */
#define XK_Kanji                0xFF21  /* Kanji, Kanji convert */
#define XK_Muhenkan             0xFF22  /* Cancel Conversion */
#define XK_Henkan_Mode          0xFF23  /* Start/Stop Conversion */
#define XK_Henkan               0xFF23  /* Alias for Henkan_Mode */
#define XK_Romaji               0xFF24  /* to Romaji */
#define XK_Hiragana             0xFF25  /* to Hiragana */
#define XK_Katakana             0xFF26  /* to Katakana */
#define XK_Hiragana_Katakana    0xFF27  /* Hiragana/Katakana toggle */
#define XK_Zenkaku              0xFF28  /* to Zenkaku */
#define XK_Hankaku              0xFF29  /* to Hankaku */
#define XK_Zenkaku_Hankaku      0xFF2A  /* Zenkaku/Hankaku toggle */
#define XK_Touroku              0xFF2B  /* Add to Dictionary */
#define XK_Massyo               0xFF2C  /* Delete from Dictionary */
#define XK_Kana_Lock            0xFF2D  /* Kana Lock */
#define XK_Kana_Shift           0xFF2E  /* Kana Shift */
#define XK_Eisu_Shift           0xFF2F  /* Alphanumeric Shift */
#define XK_Eisu_toggle          0xFF30  /* Alphanumeric toggle */
#define XK_Home                 0xFF50
#define XK_Left                 0xFF51  /* Move left, left arrow */
#define XK_Up                   0xFF52  /* Move up, up arrow */
#define XK_Right                0xFF53  /* Move right, right arrow */
#define XK_Down                 0xFF54  /* Move down, down arrow */
#define XK_Prior                0xFF55  /* Prior, previous */
#define XK_Page_Up              0xFF55
#define XK_Next                 0xFF56  /* Next */
#define XK_Page_Down            0xFF56
#define XK_End                  0xFF57  /* EOL */
#define XK_Begin                0xFF58  /* BOL */
#define XK_Select               0xFF60  /* Select, mark */
#define XK_Print                0xFF61
#define XK_Execute              0xFF62  /* Execute, run, do */
#define XK_Insert               0xFF63  /* Insert, insert here */
#define XK_Undo                 0xFF65  /* Undo, oops */
#define XK_Redo                 0xFF66  /* redo, again */
#define XK_Menu                 0xFF67
#define XK_Find                 0xFF68  /* Find, search */
#define XK_Cancel               0xFF69  /* Cancel, stop, abort, exit */
#define XK_Help                 0xFF6A  /* Help, ? */
#define XK_Break                0xFF6B
#define XK_Mode_switch          0xFF7E  /* Character set switch */
#define XK_script_switch        0xFF7E  /* Alias for mode_switch */
#define XK_Num_Lock             0xFF7F
#define XK_KP_Space             0xFF80  /* space */
#define XK_KP_Tab               0xFF89
#define XK_KP_Enter             0xFF8D  /* enter */
#define XK_KP_F1                0xFF91  /* PF1, KP_A, ... */
#define XK_KP_F2                0xFF92
#define XK_KP_F3                0xFF93
#define XK_KP_F4                0xFF94
#define XK_KP_Home              0xFF95
#define XK_KP_Left              0xFF96
#define XK_KP_Up                0xFF97
#define XK_KP_Right             0xFF98
#define XK_KP_Down              0xFF99
#define XK_KP_Prior             0xFF9A
#define XK_KP_Page_Up           0xFF9A
#define XK_KP_Next              0xFF9B
#define XK_KP_Page_Down         0xFF9B
#define XK_KP_End               0xFF9C
#define XK_KP_Begin             0xFF9D
#define XK_KP_Insert            0xFF9E
#define XK_KP_Delete            0xFF9F
#define XK_KP_Equal             0xFFBD  /* equals */
#define XK_KP_Multiply          0xFFAA
#define XK_KP_Add               0xFFAB
#define XK_KP_Separator         0xFFAC  /* separator, often comma */
#define XK_KP_Subtract          0xFFAD
#define XK_KP_Decimal           0xFFAE
#define XK_KP_Divide            0xFFAF
#define XK_KP_0                 0xFFB0
#define XK_KP_1                 0xFFB1
#define XK_KP_2                 0xFFB2
#define XK_KP_3                 0xFFB3
#define XK_KP_4                 0xFFB4
#define XK_KP_5                 0xFFB5
#define XK_KP_6                 0xFFB6
#define XK_KP_7                 0xFFB7
#define XK_KP_8                 0xFFB8
#define XK_KP_9                 0xFFB9
#define XK_F1                   0xFFBE
#define XK_F2                   0xFFBF
#define XK_F3                   0xFFC0
#define XK_F4                   0xFFC1
#define XK_F5                   0xFFC2
#define XK_F6                   0xFFC3
#define XK_F7                   0xFFC4
#define XK_F8                   0xFFC5
#define XK_F9                   0xFFC6
#define XK_F10                  0xFFC7
#define XK_F11                  0xFFC8
#define XK_L1                   0xFFC8
#define XK_F12                  0xFFC9
#define XK_L2                   0xFFC9
#define XK_F13                  0xFFCA
#define XK_L3                   0xFFCA
#define XK_F14                  0xFFCB
#define XK_L4                   0xFFCB
#define XK_F15                  0xFFCC
#define XK_L5                   0xFFCC
#define XK_F16                  0xFFCD
#define XK_L6                   0xFFCD
#define XK_F17                  0xFFCE
#define XK_L7                   0xFFCE
#define XK_F18                  0xFFCF
#define XK_L8                   0xFFCF
#define XK_F19                  0xFFD0
#define XK_L9                   0xFFD0
#define XK_F20                  0xFFD1
#define XK_L10                  0xFFD1
#define XK_F21                  0xFFD2
#define XK_R1                   0xFFD2
#define XK_F22                  0xFFD3
#define XK_R2                   0xFFD3
#define XK_F23                  0xFFD4
#define XK_R3                   0xFFD4
#define XK_F24                  0xFFD5
#define XK_R4                   0xFFD5
#define XK_F25                  0xFFD6
#define XK_R5                   0xFFD6
#define XK_F26                  0xFFD7
#define XK_R6                   0xFFD7
#define XK_F27                  0xFFD8
#define XK_R7                   0xFFD8
#define XK_F28                  0xFFD9
#define XK_R8                   0xFFD9
#define XK_F29                  0xFFDA
#define XK_R9                   0xFFDA
#define XK_F30                  0xFFDB
#define XK_R10                  0xFFDB
#define XK_F31                  0xFFDC
#define XK_R11                  0xFFDC
#define XK_F32                  0xFFDD
#define XK_R12                  0xFFDD
#define XK_F33                  0xFFDE
#define XK_R13                  0xFFDE
#define XK_F34                  0xFFDF
#define XK_R14                  0xFFDF
#define XK_F35                  0xFFE0
#define XK_R15                  0xFFE0
#define XK_Shift_L              0xFFE1  /* Left shift */
#define XK_Shift_R              0xFFE2  /* Right shift */
#define XK_Control_L            0xFFE3  /* Left control */
#define XK_Control_R            0xFFE4  /* Right control */
#define XK_Caps_Lock            0xFFE5  /* Caps lock */
#define XK_Shift_Lock           0xFFE6  /* Shift lock */
#define XK_Meta_L               0xFFE7  /* Left meta */
#define XK_Meta_R               0xFFE8  /* Right meta */
#define XK_Alt_L                0xFFE9  /* Left alt */
#define XK_Alt_R                0xFFEA  /* Right alt */
#define XK_Super_L              0xFFEB  /* Left super */
#define XK_Super_R              0xFFEC  /* Right super */
#define XK_Hyper_L              0xFFED  /* Left hyper */
#define XK_Hyper_R              0xFFEE  /* Right hyper */
#endif
#ifndef XK_Win_L
#define XK_Win_L                0xFF5B  /* Left-hand Windows */
#endif
#ifndef XK_Win_R
#define XK_Win_R                0xFF5C  /* Right-hand Windows */
#endif
#ifndef XK_App
#define XK_App                  0xFF5D  /* Menu key */
#endif
		class Keyboard
		{
		public:
			static ::rux::byte XlibToWinVirtualKey( ::rux::uint16 xlib_key_sym );
		};
		namespace engine
		{
			class Window;
#ifdef __UNIX__
			struct MWMHints 
			{
			    unsigned long flags;
			    unsigned long functions;
			    unsigned long decorations;
			    long input_mode;
			    unsigned long status;
			};
			#define MWM_HINTS_DECORATIONS ( 1L << 1 )
			#define PROP_MWM_HINTS_ELEMENTS 5
			enum
			{
				_NET_WM_STATE_REMOVE = 0 ,
				_NET_WM_STATE_ADD = 1 ,
				_NET_WM_STATE_TOGGLE = 2
			};
#endif			
			extern dll_internal ft_structs_h::FT_Library _rux_ft_library;
			extern dll_internal XCrtSect* _cs_rux_fonts;
			extern dll_internal XMallocArray< freetype_font* , 5 > _rux_fonts;
			extern dll_internal XCrtSect* _cs_font_bases;
			extern dll_internal XMallocArray< rux::gui::engine::RenderFont* , 5 > _font_bases;
			extern dll_internal rux::pid_t _message_processing_thread_id;
			extern dll_internal XCrtSect* _cs_engine;
			extern dll_internal rux::uint8 _quit;
			extern dll_internal XMallocArray< rux::gui::on_event_t > _on_events;
			extern dll_internal XMallocArray< void* > _on_event_owners;			
			extern dll_internal rux_volatile _is_initialized;
			extern dll_internal rux_volatile _dispatchers_count;
			extern dll_internal XCrtSect* _cs_dispatchers;
			extern dll_internal XMallocArray< XUserData3* > _dispatchers;
			extern dll_internal size_t _is_pump_message;
			extern dll_internal ::rux::uint32 _display_frequencies[ 32 ];
			extern dll_internal ::rux::uint32 _display_bits[ 32 ];
			extern dll_internal size_t _displays_count;
#ifdef __WINDOWS__
			LRESULT CALLBACK WndProc( HWND hwnd , rux::uint32 msg , WPARAM wparam , LPARAM lparam );
#elif defined( __UNIX__ )
#ifdef __ANDROID__
#else
			extern dll_internal Display* _x_display_main;
			extern dll_internal XCrtSect* _cs_x_display_main;			
			extern dll_internal XVisualInfo* _x_visual_info;	
			extern dll_internal XSetWindowAttributes _x_set_window_attributes;
			extern dll_internal XVisualInfo* _x_infos;
			extern dll_internal rux::uint8 _is_x_if_event;
			extern dll_internal ::rux::uint8 _is_x_key_pressed;
			dll_internal void WndProc( XEvent& event );
			extern dll_internal Atom _wm_delete_window;
			extern dll_internal Atom _net_wm_state;
			extern dll_internal Atom _wm_change_state;
			extern dll_internal Atom _rux_delete_window;
			extern dll_internal Atom _rux_gui_initialize;
			extern dll_internal Atom _net_wm_state_max_vert;
			extern dll_internal Atom _net_wm_state_max_horz;
			extern dll_internal Atom _net_wm_state_hidden;
			extern dll_internal Atom _net_wm_action_fullscreen;
			extern dll_internal Atom _net_wm_allowed_actions;
			extern dll_internal Atom _net_close_window;
			extern dll_internal Atom _wm_protocols;
			extern dll_internal rux::uint8 _is_mouse_down;
			extern dll_internal ::rux::byte _is_right_mouse_down;
			extern dll_internal ::rux::byte _is_middle_mouse_down;
			extern dll_internal rux::uint64 _mouse_down_time;
			extern dll_internal ::rux::uint64 _right_mouse_down_time;
			extern dll_internal ::rux::uint64 _middle_mouse_down_time;
			dll_internal bool lock_display( rux::gui::WindowBase* window_base = NULL );
			dll_internal void unlock_display( void );
			dll_internal rux::uint8 is_allowed_atom( Display* display , ::Window xwindow , Atom atom );
			dll_internal rux::int32 display_io_error_handler( Display* display );
			dll_internal rux::int32 display_error_handler( Display* display , XErrorEvent* error_event );
			dll_internal ::rux::gui::engine::Window* find_window( ::Window& xwindow );
#endif
#endif
			dll_internal void start( rux::gui::on_event_t on_event , void* owner );
			dll_internal void stop( rux::gui::on_event_t on_event , void* owner );
			dll_internal void raise_event( rux::gui::Event& xevent );
			dll_internal rux::gui::WindowBase* create_window( rux::gui::on_event_t on_event );
			dll_internal void destroy_window( rux::gui::WindowBase* window );
			dll_internal rux::gui::FontBase* create_font( const char* font_file_name ,
				rux::uint32 font_size_height ,
				rux::uint32 font_size_width );
			dll_internal void add_cache( rux::gui::RenderCacheBase* cache );
			dll_internal rux::uint8 dispatch( rux::gui::rux_dispatcher_t rux_dispatcher , void* param , rux::gui::WindowBase* window = NULL , rux::uint8 wait = 1 );
			dll_internal size_t add_schedule( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint64 timeout_in_microseconds );
			dll_internal void remove_schedule( size_t schedule_index );
			dll_internal size_t windows_count( void );
			dll_internal void pump_message( rux::uint8 wait_for_message = 1 );
			dll_internal void close_all_windows( void );
			dll_internal XMallocArray< rux::gui::ScreenBase* >* get_screens( void );
			dll_internal void free_screens( XMallocArray< rux::gui::ScreenBase* >* screens );
			dll_internal void dispatch_create_window( void* param );
		};
	};
};
#endif
