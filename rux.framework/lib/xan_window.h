#ifndef XAN_XWINDOW_H
#define XAN_XWINDOW_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_keyvaluepair.h>
#include <xan_uiline.h>
#include <xan_uigroupanimation.h>
#include <xan_uigroup.h>
#include <xan_event_handler.h>
namespace rux
{
	namespace gui
	{		
		class XUIApplication;
		class XAnimation;
		class XGroupAnimation;
		namespace controls
		{
			class XGroup;
			class Group;			
			template< class T >
			rux_inline float rux_try_get_left( T* control , ::rux::uint8 relative_to_parent , float* cache_left_ptr );
			rux_inline void rux_try_mouse_deactivate( ::rux::gui::Window* window , ::rux::gui::CommonBase* control );
			template< class T >
			rux_inline float rux_try_get_top( T* control , ::rux::uint8 relative_to_parent , float* cache_top_ptr );	
			rux_inline ::rux::uint8 rux_try_mouse_move( ::rux::gui::Window* window , ::rux::gui::ControlBase* control , ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& enter_raised );
			template< class T >
			rux_inline void rux_try_reset_cache( T* parent );
			template< class T >
			rux_inline rux::gui::Window* get_parent_window( T* _this );
			template< class T >
			rux_inline void set_parent_control( T* _this , ::rux::gui::ParentBase* parent_control );
			template< class T >
			rux_inline void wait_for_free_system_resources( T* _this );
			template< class T >
			rux_inline void before_free_system_resources( T* _this );
			template< class T >
			rux_inline void free_system_resources( T* _this );
			template< class T >
			rux_inline void rux_try_render_controls( T* parent , XMallocArray< ::rux::gui::ControlBase* >& render_objects , float opacity , ::rux::gui::RenderContextBase* render_context , float& _selected_z_index , size_t ___rux__thread_index1986 );
			template< class T >
			rux_inline float rux_try_get_width( T* control , float* cache_width_ptr );
			template< class T >
			rux_inline float rux_try_get_height( T* control , float* cache_height_ptr );
		};
		class Window : public ::rux::gui::ParentBase
		{
			declare_rux_base_internal_methods();
			friend class XWindow;
			friend class XUIApplication;
			friend class XAnimation;
			friend class XGroupAnimation;
			friend class controls::XGroup;	
			friend class controls::Group;
			template< class T >
			friend rux_inline float controls::rux_try_get_left( T* control , ::rux::uint8 relative_to_parent , float* cache_left_ptr );
			friend rux_inline void controls::rux_try_mouse_deactivate( ::rux::gui::Window* window , ::rux::gui::CommonBase* control );
			template< class T >
			friend rux_inline float controls::rux_try_get_top( T* control , ::rux::uint8 relative_to_parent , float* cache_top_ptr );	
			friend rux_inline ::rux::uint8 controls::rux_try_mouse_move( ::rux::gui::Window* window , ::rux::gui::ControlBase* control , ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& enter_raised );
			template< class T >
			friend rux_inline void controls::rux_try_reset_cache( T* parent );
			template< class T >
			friend rux_inline rux::gui::Window* get_parent_window( T* _this );
			template< class T >
			friend rux_inline void set_parent_control( T* _this , ::rux::gui::ParentBase* parent_control );
			template< class T >
			friend rux_inline void wait_for_free_system_resources( T* _this );
			template< class T >
			friend rux_inline void controls::before_free_system_resources( T* _this );
			template< class T >
			friend rux_inline void free_system_resources( T* _this );
			template< class T >
			friend rux_inline void controls::rux_try_render_controls( T* parent , XMallocArray< ::rux::gui::ControlBase* >& render_objects , float opacity , ::rux::gui::RenderContextBase* render_context , float& _selected_z_index , size_t ___rux__thread_index1986 );
			template< class T >
			friend rux_inline float controls::rux_try_get_width( T* control , float* cache_width_ptr );
			template< class T >
			friend rux_inline float controls::rux_try_get_height( T* control , float* cache_height_ptr );
		public:
			size_t _render_index;
		private:
			::rux::gui::Margin _margin;
			::rux::gui::controls::XGroup _group;
			//::rux::byte _closed;
			rux::gui::XEnum_Cursors _cursor;
			char _utf8_control_name[ 128 ];
			::rux::uint32 _utf8_control_name_hash;
			float _selected_z_index;
			rux_volatile _window_base_state;
			rux::gui::WindowBase* _window_base;
			XCrtSect _cs_tab_common_bases;
			XMallocArray< ::rux::gui::ControlBase* > _tab_common_bases;
			::rux::uint8 _is_start_drag;
			rux::gui::controls::XLine _caret;
			XGroupAnimation _caret_animation;

			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_move;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel;
			::rux::EventHandler< ::rux::gui::events::on_keyboard_event_t > _on_key_down;
			::rux::EventHandler< ::rux::gui::events::on_keyboard_event_t > _on_key_up;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_down;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_up;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_left_mouse_button_double_click;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_down;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_up;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_right_mouse_button_double_click;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_down;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_up;
			::rux::EventHandler< ::rux::gui::events::on_mouse_event_t > _on_mouse_wheel_double_click;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_location;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_resize;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_shown;	
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_hided;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_closed;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_restored;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_minimized;
			::rux::EventHandler< ::rux::gui::events::on_event_t > _on_maximized;

			::rux::threading::RdWrLock _cs_active_control;
			::rux::gui::CommonBase* _active_control;	
			rux::gui::ControlBase* _drag_control;
			::rux::int32 _old_left;
			::rux::int32 _old_top;
			::rux::uint32 _old_width;
			::rux::int32 _old_cursor_left;
			::rux::uint32 _old_height;
			::rux::int32 _old_cursor_top;
			rux_volatile _already_dispatch_on_resized;
		public:
			::rux::threading::RdWrLock _cs_over_control;
			::rux::gui::CommonBase* _over_control;
		private:
			static void dispatch_on_resized( void* param );
			::rux::threading::RdWrLock _cs_pressed_control;
			::rux::gui::CommonBase* _pressed_control;
			XObject _drag_and_drop_object;
			XObject _drag_and_drop_control;
			XEnum_StartPosition _start_position;
			XObject _tag;			
			::rux::int32 _tab_index;
			::rux::gui::Rectangle _clip;
			size_t _current_render_index;
			virtual void set_IsForwardEvents( ::rux::byte value );
			virtual ::rux::byte get_IsForwardEvents( void );
		public:
			void set_BorderColor( ::rux::gui::Color* border_color );
			void set_BorderWidth( float border_width );
			void set_Background( ::rux::gui::ColorBase* background );	
			void set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_move_callback );
			void set_OnKeyDown( ::rux::gui::events::on_keyboard_event_t on_key_down_callback );
			void set_OnKeyUp( ::rux::gui::events::on_keyboard_event_t on_key_up_callback );
			void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnMouseWheelUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnResize( ::rux::gui::events::on_event_t on_resize_callback );
			void set_OnLocationChanged( ::rux::gui::events::on_event_t on_location_callback );
			void set_OnShown( ::rux::gui::events::on_event_t on_shown_callback );
			void set_OnHided( ::rux::gui::events::on_event_t on_hided_callback );
			void set_OnClosed( ::rux::gui::events::on_event_t on_closed_callback );
			void set_OnRestored( ::rux::gui::events::on_event_t on_restored_callback );
			void set_OnMinimized( ::rux::gui::events::on_event_t on_minimized_callback );	
			void set_OnMaximized( ::rux::gui::events::on_event_t on_maximized_callback );
			void set_IsAllowResize( ::rux::byte is_allow_resize );
			void set_IsAllowRelocate( ::rux::byte is_allow_relocate );
			void set_IsAllowClose( ::rux::byte is_allow_close );
			virtual void GCRefRelease( const char* file , ::rux::int32 line );
			virtual void GCRefAddRef( const char* file , ::rux::int32 line );
			virtual ::rux::uint8 IsPointInControl( ::rux::int32  x , ::rux::int32 y );
			virtual void raise_OnMouseLeave( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			virtual ::rux::gui::XEnum_Cursors get_Cursor( void );
			virtual ::rux::uint8 get_IsButton( void );
			virtual ::rux::uint8 get_IsWindow( void );
			virtual ::rux::uint8 get_IsGroup( void );
			virtual void* DynamicCast( ::rux::int32 class_type ) const;
			virtual ::rux::uint8 get_IsTimePicker( void );
			virtual ::rux::byte get_IsMouseEntered( void );
			virtual void set_IsMouseEntered( ::rux::byte );
			virtual ::rux::byte Exists( ::rux::gui::CommonBase* control );
			virtual ::rux::byte raise_OnMouseEnter( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte& raised_event , ::rux::byte explicit_event );
			virtual void set_ScrollXOffset( float scroll_x_offset );
			virtual void set_ScrollYOffset( float scroll_y_offset );
			virtual float get_ScrollXOffset( void );
			virtual float get_ScrollYOffset( void );
			virtual ::rux::int32 IndexOf( XGCRef* control );
			virtual void set_ZIndex( XGCRef* control , size_t index );
			rux::String& get_ControlName( void );
			::rux::uint32 get_ControlNameHash( void );
			void private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index = 0 );
			::rux::gui::CommonBase* get_ActiveControl( void );	
			void set_ActiveControl( ::rux::gui::ControlBase* control );
			rux::gui::WindowBase* get_WindowBase( void );
			float _scrollbar_left_offset;
			float _scrollbar_top_offset;
			explicit Window( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
			~Window();
			virtual void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 );
			void set_TopMost( ::rux::uint8 is_top_most );
			::rux::int32 get_IsVisible( void );
			::rux::uint8 get_TopMost( void );
			void set_MinHeight( ::rux::int32 height );
			void set_MinWidth( ::rux::int32 width );
			void Restore( void );
			void Minimize( void );
			void Maximize( void );
			void set_ZIndex( const XGCRef& control , ::rux::uint32 index );
			void set_ZIndex( const XObject& control , ::rux::uint32 index );
			void Show( void );
			void Hide( void );
			void Close( void );
			rux::gui::XEnum_WindowState get_WindowState( void );
			virtual float get_Width( float* param = 0 );
			virtual float get_Height( float* param = 0 );
			void Invalidate( void );
			void render( ::rux::gui::RenderContextBase* render_context );
			void FreeResources( ::rux::byte from_removed_controls_schedule );
			void UpdateParentWindow( void );
			Object& private_ActivateControl( void );				
			void raise_OnKeyChar( const ::rux::XString& symbol , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt );
			void raise_OnKeyDown( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event );
			void private_KeyUp( ::rux::gui::XEnum_Keys key , ::rux::uint8 shift , ::rux::uint8 control , ::rux::uint8 alt , ::rux::uint32 value , ::rux::byte explicit_event );
			void raise_OnRightMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void raise_OnMouseWheelDown(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);
			void raise_OnRightMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void raise_OnMouseWheelUp(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);
			void raise_OnMouseWheelDoubleClick(::rux::gui::WindowMouseEvent* window_event, ::rux::byte explicit_event);
			void raise_OnRightMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void raise_OnLeftMouseButtonDown( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void raise_OnLeftMouseButtonUp( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void raise_OnLeftMouseButtonDoubleClick( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			void private_InsertTabControl( ::rux::int32 tab_index , ::rux::gui::ControlBase* control );
			void raise_OnWindowResize( ::rux::int32 width , ::rux::int32 height );
			void private_OnClosed( void );
			void private_Relocation( void );
			void private_Shown( void );
			void private_Hided( void );
			void raise_OnWindowRestored( void );
			void raise_OnWindowMinimized( void );
			void raise_OnWindowMaximized( void );
			void raise_OnMouseMove( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );

			virtual void raise_OnMouseWheel( ::rux::gui::WindowMouseEvent* window_event , ::rux::byte explicit_event );
			virtual ::rux::gui::ParentBase* get_ParentControl( ::rux::byte lock = 1 , ::rux::byte real_parent = 0 );
			virtual void raise_OnActivate( void );
			virtual void raise_OnDeactivate( ::rux::gui::CommonBase* control );
			virtual Window* get_ParentWindow( void ) const;
			virtual void private_set_Tag( const XObject& ) const;
			virtual Object& private_get_Tag( void );

			void set_Width( ::rux::int32 width );
			void set_Height( ::rux::int32 height );
			void set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height );
			void StopCaret( void );
			virtual void set_Left( float left );
			virtual void set_Top( float top );			
			void RemoveControl( XGCRef* control );
			void AddControl( const XObject& control , ::rux::XString& error );
			void ClearControls( void );
			void set_StartPosition( XEnum_StartPosition start_position );
			void InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error );
			void StartCaret( float left , float top , float height , ::rux::gui::Color* color );	
			void BringToFront( const XObject& control );
			void BringToFront( const XGCRef& control );
			void RemoveControl( XObject& control );
			void RemoveTabControls( void );
			virtual float get_Top( ::rux::uint8 p0 = 0 , float* p1 = 0 );
			virtual float get_Left( ::rux::uint8 p0 = 0 , float* p1 = 0 );
			virtual Object& get_Tag( void );
			void StartDrag( void );
			void StartDrag( const XObject& control , ::rux::XString& error );
			void StopDrag( void );
			void set_Cursor( ::rux::gui::XEnum_Cursors cursor );
			void AddTabControls( void );
			::rux::uint32 get_ControlsCount( void );
			::rux::uint8 ExistsControl( const XObject& control );
			::rux::uint8 ExistsControl( const XGCRef& control );
			void dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , const XObject& param , ::rux::uint8 wait = 1 );
			void dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait = 1 );
			void Dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param , ::rux::uint8 wait = 1 );			
			template< class T >
			rux::Array< T >& Find( void )
			{
				return _group.Find< T >();
			};
			template< class T >
			rux::Array< T >& FindAll( void )
			{
				return _group.FindAll< T >();
			};
			template< class T >
			rux::Array< T >& Find( const ::rux::XString& name )
			{
				return _group.Find< T >( name );
			};
			template< class T >
			rux::Array< T >& FindAll( const ::rux::XString& name )
			{
				return _group.FindAll< T >( name );
			};
			template< class T >
			typename T::InternalClass& FindOne( const ::rux::XString& name , ::rux::uint8& is_found )
			{
				return _group.FindOne< T >( name , is_found );
			};
			template< class T >
			typename T::InternalClass& FindOneAll( const ::rux::XString& name , ::rux::uint8& is_found )
			{
				return _group.FindOneAll< T >( name , is_found );
			};
			private:
			virtual void render( ::rux::gui::RenderContextBase* render_context , float opacity , float& _selected_z_index , size_t ___rux__thread_index1986 );
			virtual void private_ResetLocationAndSizeCache( ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , ::rux::uint8 reset_parent = 1 );
			virtual ::rux::uint8 get_IsSupportContentSize( void );
			virtual ::rux::uint8 get_IsTransparent( void );
			virtual void FreeSystemResources( void );
			virtual void BeforeFreeSystemResources( void );
			virtual void set_FreeSystemResourcesThreadId( void );
			virtual ::rux::byte IsFreeSystemResourcesThread( void );\
			virtual void WaitForFreeSystemResources( void );
			virtual ::rux::int32 get_TabIndex( void );
			virtual void set_ParentControl( ::rux::gui::ParentBase* parent_control );
			virtual void private_ResetCache( void );
			virtual void set_OnDeactivated( ::rux::gui::events::on_deactivated_event_t on_activated );
			virtual ::rux::uint32 get_TypeIndex( void );
			virtual ::rux::uint8 get_IsUserControl( void );
			virtual void RemoveControl( void );
			virtual void Activate( void );
			virtual ::rux::gui::XEnum_Filling get_VerticalFilling( void );
			virtual ::rux::gui::XEnum_Filling get_HorizontalFilling( void );
			virtual ::rux::gui::XEnum_Alignment get_VerticalAlignment( void );
			virtual ::rux::gui::XEnum_Alignment get_HorizontalAlignment( void );
			virtual ::rux::gui::Margin& get_Margin( void );
			virtual ::rux::gui::XEnum_PointUnit get_PointUnit( void );
			virtual void set_Left( float left ) const;
			virtual void set_Top( float top ) const;
			virtual void* get_TagPtr( void ) const;
			void cleanup(void);
			public:
			void RestartRender();
		};
		class XWindow : public XGCHandle<Window>
		{
			declare_rux_base_methods(Window);
			friend class XUIApplication;
			friend class controls::XGroup;	
			friend class controls::Group;			
		private:
		public:
			declare_rux_set_operators( Window );
			XWindow();
			XWindow( const Window& window );
			XWindow( Window* window );
			XWindow( const XWindow& window );
			explicit XWindow( const XObject& object );
			void set_IsAllowResize( ::rux::byte is_allow_resize );
			void set_IsAllowRelocate( ::rux::byte is_allow_relocate );
			void set_IsAllowClose( ::rux::byte is_allow_close );
			void Minimize( void );
			void Maximize( void );
			void Restore( void );
			void set_MinHeight( ::rux::int32 height );
			void set_MinWidth( ::rux::int32 width );
			void Show( void );	
			void Close( void );
			void InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error );
			::rux::uint32 get_ControlsCount( void );
			void AddControl( const XObject& control , ::rux::XString& error );
			void RemoveControl( XObject& control );
			void ClearControls( void );
			rux::gui::XEnum_WindowState get_WindowState( void );
			void set_Width( ::rux::int32 width );
			void set_Height( ::rux::int32 height );
			void set_Left( ::rux::int32 left );
			void set_Top( ::rux::int32 top );
			void set_LocationAndSize( ::rux::int32 x , ::rux::int32 y , ::rux::int32 width , ::rux::int32 height );
			void set_TopMost( ::rux::uint8 is_top_most );
			::rux::uint8 ExistsControl( const XObject& control );
			::rux::uint8 ExistsControl( const XGCRef& control );
			void set_Scale( float scale );
			void Hide( void );
			::rux::uint8 get_IsVisible( void );
			::rux::uint8 get_TopMost( void );
			void set_Background( ::rux::gui::ColorBase* background );	
			void set_BorderWidth( float border_width );
			void set_BorderColor( ::rux::gui::Color* border_color );
			void set_OnMouseMove( ::rux::gui::events::on_mouse_event_t on_mouse_move_callback );
			void set_OnKeyDown( ::rux::gui::events::on_keyboard_event_t on_key_down_callback );
			void set_OnKeyUp( ::rux::gui::events::on_keyboard_event_t on_key_up_callback );
			void set_OnLeftMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnLeftMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnRightMouseButtonDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnRightMouseButtonUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnMouseWheelDown( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_down_callback );
			void set_OnMouseWheelUp( ::rux::gui::events::on_mouse_event_t on_left_mouse_button_up_callback );
			void set_OnResize( ::rux::gui::events::on_event_t on_resize_callback );
			void set_OnLocationChanged( ::rux::gui::events::on_event_t on_location_callback );
			void set_OnShown( ::rux::gui::events::on_event_t on_shown_callback );
			void set_OnHided( ::rux::gui::events::on_event_t on_hided_callback );
			void set_OnClosed( ::rux::gui::events::on_event_t on_closed_callback );
			void set_OnRestored( ::rux::gui::events::on_event_t on_restored_callback );
			void set_OnMinimized( ::rux::gui::events::on_event_t on_minimized_callback );	
			void set_OnMaximized( ::rux::gui::events::on_event_t on_maximized_callback );
			void StartCaret( float left , float top , float height , ::rux::gui::Color* color );
			void StopCaret( void );	
			::rux::int32 get_Left( void );
			::rux::int32 get_Top( void );
			::rux::int32 get_Width( void );
			::rux::int32 get_Height( void );
			void StartDrag( const XObject& control , const XObject& object );
			void set_StartPosition( XEnum_StartPosition start_position );	
			static void on_show_timeout_message_completed( const ::rux::gui::events::Event& sender );
			static void on_hide_timeout_message_completed( const ::rux::gui::events::Event& sender );
			void ShowTimeoutMessage( const ::rux::XString& message , uint64 show_timeout , uint64 hide_timeout );
			void Dispatch( ::rux::gui::rux_dispatcher_t rux_dispatcher , void* param );
			rux::String& get_ControlName( void );
			::rux::uint32 get_ControlNameHash( void );
			template< class T >
			rux::Array< T >& Find( void )
			{
				return (*this)()->Find< T >();
			};
			template< class T >
			rux::Array< T >& FindAll( void )
			{
				return (*this)()->FindAll< T >();
			};
			template< class T >
			rux::Array< T >& Find( const ::rux::XString& name )
			{
				return (*this)()->Find< T >( name );
			};
			template< class T >
			rux::Array< T >& FindAll( const ::rux::XString& name )
			{
				return (*this)()->FindAll< T >( name );
			};
			template< class T >
			typename T::InternalClass& FindOne( const ::rux::XString& name , ::rux::uint8& is_found )
			{
				return (*this)()->FindOne< T >( name , is_found );
			};
			template< class T >
			typename T::InternalClass& FindOneAll( const ::rux::XString& name , ::rux::uint8& is_found )
			{
				return (*this)()->FindOneAll< T >( name , is_found );
			};
			void Invalidate( void );
			void set_Tag( const XObject& tag );
			Object& get_Tag( void );
			void set_ZIndex( const XObject& control , ::rux::uint32 index );
			void BringToFront( const XObject& control );
			void BringToFront( const XGCRef& control );
			void StartDrag( void );
			void StartDrag( const XObject& control , ::rux::XString& error );
			void StopDrag( void );
			void set_Cursor( ::rux::gui::XEnum_Cursors cursor );
			void RestartRender();
		};
	};
};
#endif
