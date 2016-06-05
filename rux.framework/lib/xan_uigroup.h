#ifndef XAN_UIGROUP_H
#define XAN_UIGROUP_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_hash.h>
#include <xan_float.h>
enum XEnum_Child_Location_Type
{
	XEnum_Child_Location_Type_Manual , 
	XEnum_Child_Location_Type_HorizontalStack ,
	XEnum_Child_Location_Type_VerticalStack ,
	XEnum_Child_Location_Type_Wrap
};
namespace rux
{
	namespace gui
	{
		class Window;
		namespace controls
		{
			class Group : public ::rux::gui::ParentBase
			{
				declare_rux_base_internal_methods();
				friend class XGCRef;
				friend class ::rux::gui::Window;
				friend class XGroup;
				friend class Button;
				friend class XButton;
				friend class DatePicker;
				template< class T >
				friend rux_inline void rux_try_render_controls( T* parent , XMallocArray< ::rux::gui::ControlBase* >& render_objects , float opacity , ::rux::gui::RenderContextBase* render_context , float& _selected_z_index , size_t ___rux__thread_index1986 );
				template< class T >
				friend rux_inline ::rux::uint8 rux_is_point_in_control( T* control , ::rux::int32 x , ::rux::int32 y );
				template< class T >
				friend rux_inline void rux_try_reset_cache( T* parent );
				friend class UserControl;
				friend rux_inline void rux_change_over_control( ::rux::gui::WindowMouseEvent* window_event , ::rux::gui::CommonBase* old_control , ::rux::gui::CommonBase* new_control , ::rux::byte lock = 1 );
			private:
				DECLARE_BASE_UI_MEMBERS();
				void AddOrInsertResetLocationAndSizeCache( size_t controls_start_index );
			public:
				::rux::uint8 _is_user_control;

			private:
				size_t _removed_controls_schedule_index;
				::rux::uint32 _removed_controls_schedule_idle_time;
				::rux::threading::RdWrLock _cs_removed_controls;
				XMallocArray< ::rux::gui::ControlBase* > _removed_controls;
				::rux::byte _begin_multiple_add_or_remove_controls;
				::rux::byte _is_forward_events_if_children_do_not_raised_event;
				size_t _current_render_index;
				::rux::gui::Rectangle _clip;
				rux::threading::RdWrLock _cs_render_common_bases;
				XMallocArray< ::rux::gui::ControlBase* > _render_common_bases;
				XEnum_Child_Location_Type _child_location_type;
				float _border_width;	
				rux::gui::ColorBase* _background;
				rux::gui::Color* _border_color;
				rux::gui::Thickness _corner;
				rux::gui::RenderCacheBase* _ui_border_cache;
				rux::gui::RenderCacheBase* _ui_cache;
				DECLARE_RUX_BASE_UI_FUNCTIONS( Group );	
				virtual void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 );
				void private_ResetChildLocationAndSizeCache( ::rux::uint8 reset , ::rux::gui::ControlBase* control , ::rux::uint8 left , ::rux::uint8 top , ::rux::uint8 width , ::rux::uint8 height , size_t controls_start_index = 0 );
				virtual ::rux::byte Exists( ::rux::gui::CommonBase* control );
				declare_rux_property_without_member( ::rux::XFloat , ScrollXOffset );
				declare_rux_property_without_member( ::rux::XFloat , ScrollYOffset );
			public:
				float _scrollbar_x_offset;
				float _scrollbar_y_offset;
				explicit Group( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
				~Group();	
				declare_content_size();
				void BringToFront( const XObject& control );
				void BringToFront( const XGCRef& control );
				void set_ZIndex( const XObject& control , size_t index );
				void set_ZIndex( const XGCRef& control , size_t index );
				virtual void set_ZIndex( XGCRef* control , size_t index );
				void set_ChildLocationType( XEnum_Child_Location_Type child_location_type );
				void RemoveControl( XGCRef* control );
				void RemoveControl( XGCRef& control );
				void BeginMultipleAddOrRemoveControls( void );
				void EndMultipleAddOrRemoveControls( void );
				void RemoveTabControls( void );
				void AddTabControls( void );
				::rux::int32 IndexOf( const XObject& control );
				Object& get_ControlAt( ::rux::uint32 index );
				float get_LeftStackOffset( ::rux::gui::ControlBase* control );
				float get_TopStackOffset( ::rux::gui::ControlBase* control );
				float get_RightStackOffset( ::rux::gui::ControlBase* control );
				float get_BottomStackOffset( ::rux::gui::ControlBase* control );
				float get_WrapLeftOffset( ::rux::gui::ControlBase* control );
				float get_WrapTopOffset( ::rux::gui::ControlBase* control );
				size_t CalculateWrapVisibleControlsCount( float width , float height , const ::rux::gui::Margin& margin );
				virtual void set_ScrollXOffset( float scroll_x_offset );
				virtual void set_ScrollYOffset( float scroll_y_offset );
				virtual float get_ScrollXOffset( void );
				virtual float get_ScrollYOffset( void );
				virtual ::rux::int32 IndexOf( XGCRef* control );
				void ClearControls( void );
				void AddControl( const XObject& control , ::rux::XString& error );
				void AddControl( const XGCRef& control , ::rux::XString& error );
				void InsertControl( size_t index , XObject& control , ::rux::XString& error );
				::rux::uint8 ExistsControl( const XObject& control );
				::rux::uint8 ExistsControl( const XGCRef& control );
				size_t get_ControlsCount( void );
				void set_OnDeactivatedAll( ::rux::gui::events::on_deactivated_event_t on_activated );
				void set_Background( ::rux::gui::ColorBase* background );
				rux::gui::ColorBase* get_Background( void );
				rux::gui::ColorBase* get_BorderColor( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Corner( const ::rux::gui::Thickness& corner );
				void set_IsForwardEventsIfChildrenDoNotRaisedEvent( ::rux::byte is_forward_events_if_children_do_not_raised_event );
				::rux::byte get_IsForwardEventsIfChildrenDoNotRaisedEvent( void );
				void removed_controls_schedule( size_t controls_count , ::rux::byte force_remove_task );
				static void removed_controls_schedule( void* param , size_t ___rux__thread_index1986 );
				template< class T >
				rux::Array< T >& Find( void )
				{
					declare_variable( XArray< T > , found );
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();					
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
									found.Add( T( *tag.get_GCRef( 1 ) ) );
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
								found.Add( T( _render_common_bases[ index0 ] )  );
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
				template< class T >
				rux::Array< T >& FindAll( void )
				{
					declare_variable( XArray< T > , found );
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();					
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
									found.Add( T( *tag.get_GCRef( 1 ) ) );
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
							{
								found.Add( T( _render_common_bases[ index0 ] ) );
								if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
								{
									Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
									found.AddRange( child->FindAll< T >() );
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
							{
								Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
								found.AddRange( child->FindAll< T >() );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
				template< class T >
				rux::Array< T >& Find( const ::rux::XString& name )
				{
					declare_variable( XArray< T > , found );
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();
						declare_variable( ::rux::XString , utf8_name );
						utf8_name.set_ByRef( name.ConvertToUTF8() );
						::rux::uint32 name_hash = ::rux::cryptography::hash::times33_hash( utf8_name.str() , utf8_name.Size() - 1 );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									{
										XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
										found.Add( T( *tag.get_GCRef( 1 ) ) );
									}
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
							{
								if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									found.Add( T( _render_common_bases[ index0 ] ) );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
				template< class T >
				rux::Array< T >& FindAll( const ::rux::XString& name )
				{
					declare_variable( XArray< T > , found );
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();
						declare_variable( ::rux::XString , utf8_name );
						utf8_name.set_ByRef( name.ConvertToUTF8() );
						::rux::uint32 name_hash = ::rux::cryptography::hash::times33_hash( utf8_name.str() , utf8_name.Size() - 1 );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									{
										XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
										found.Add( T( *tag.get_GCRef( 1 ) ) );
									}
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
							{
								if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									found.Add( T( _render_common_bases[ index0 ] ) );
								if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
								{
									Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
									found.AddRange( child->FindAll< T >( name ) );
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
							{
								Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
								found.AddRange( child->FindAll< T >( name ) );
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
				template< class T >
				typename T::InternalClass& FindOne( const char* utf8name , size_t utf8namelen , ::rux::uint8& is_found )
				{
					declare_variable( T , found );
					is_found = 0;
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();
						::rux::uint32 name_hash = ::rux::cryptography::hash::times33_hash( utf8name 
							, utf8namelen == SIZE_MAX ? strlen( utf8name ) : utf8namelen );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									{
										XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
										found = *tag.get_GCRef( 1 );
										is_found = 1;
										break;
									}
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
							{
								if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
								{
									found = _render_common_bases[ index0 ];
									is_found = 1;
									break;
								}
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
				template< class T >
				typename T::InternalClass& FindOne( const ::rux::XString& name , ::rux::uint8& is_found )
				{
					declare_variable( ::rux::XString , utf8_name );
					utf8_name.set_ByRef( name.ConvertToUTF8() );
					return this->FindOne< T >( utf8_name.str() , utf8_name.Size() - 1 , is_found );
				};
				template< class T >
				typename T::InternalClass& FindOneAll( const ::rux::XString& name , ::rux::uint8& is_found )
				{
					is_found = 0;
					declare_variable( T , found );
					if( IsFreeSystemResourcesThread() == 0 )
					{
						WaitForFreeSystemResources();
						declare_variable( ::rux::XString , utf8_name );
						utf8_name.set_ByRef( name.ConvertToUTF8() );
						::rux::uint32 name_hash = ::rux::cryptography::hash::times33_hash( utf8_name.str() , utf8_name.Size() - 1 );
						_cs_render_common_bases.ReadLock( __FILE__ , __FUNCTION__ , __LINE__ , 0 );
						for( size_t index0 = 0 ; index0 < _render_common_bases.Count() ; index0++ )
						{
							if( _render_common_bases[ index0 ]->get_IsUserControl() == 1 )
							{						
								if( _render_common_bases[ index0 ]->private_get_Tag().get_RuxTypeIndex() == T::get_static_RuxTypeIndex() )
								{
									if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
									{
										XObject tag( _render_common_bases[ index0 ]->private_get_Tag() , "" , __FILE__ , __LINE__ );
										found = *tag.get_GCRef( 1 );
										is_found = 1;
										break;
									}
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == T::get_static_RuxTypeIndex() )
							{
								if( _render_common_bases[ index0 ]->get_ControlNameHash() == name_hash )
								{
									found = _render_common_bases[ index0 ];
									is_found = 1;
									break;
								}
								if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
								{
									Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
									found.set_ByRef( child->FindOneAll< T >( name , is_found ) );	
									if( is_found == 1 )
										break;
								}
							}
							else if( _render_common_bases[ index0 ]->get_TypeIndex() == ::rux::get_group_rux_type_index() )
							{
								Group* child = (Group*)_render_common_bases[ index0 ]->get_ObjectAddress();
								found.set_ByRef( child->FindOneAll< T >( name , is_found ) );
								if( is_found == 1 )
									break;
							}
						}
						_cs_render_common_bases.ReadUnlock();
					}
					return found++;
				};
			};
			class XGroup : public XGCHandle<Group>
			{
				declare_rux_base_methods(Group);
				friend class Group;	
				friend class XButton;				
				template< class T >
				friend rux_inline void rux_try_render_controls( T* parent , XMallocArray< ::rux::gui::ControlBase* >& render_objects , float opacity , ::rux::gui::RenderContextBase* render_context , float& _selected_z_index , size_t ___rux__thread_index1986 );
				template< class T >
				friend rux_inline float rux_try_get_height( T* control , float* cache_height_ptr );				
			private:
			public:
				declare_rux_set_operators( Group );
				XGroup();
				XGroup( const ::rux::gui::ParentBase& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XGroup( const Group& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XGroup( const XGroup& uigroup , const char* variable_name , const char* __file__ , ::rux::int32 __line__ );
				XGroup( const ::rux::gui::ParentBase& uigroup );
				XGroup( const Group& uigroup );
				XGroup( const XGroup& uigroup );	
				explicit XGroup( const XObject& object );
				explicit XGroup( XGCRef* object );
				void AddControl( const XObject& control , ::rux::XString& error );
				void AddControl( const XGCRef& control , ::rux::XString& error );
				void InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error );
				void RemoveControl( XGCRef& control );
				void RemoveControl( XObject& control );
				void BeginMultipleAddOrRemoveControls( void );
				void EndMultipleAddOrRemoveControls( void );
				void ClearControls( void );
				DECLARE_BASE_UI_FUNCTIONS();
			public:		
				void get_Clip( ::rux::gui::Rectangle*& clip , ::rux::uint8 scrollbar = 0 );
				float get_StackLeft( float width );
				float get_StackRight( float width , ::rux::uint8 is_scrollbar );
				float get_StackWidth( void );
				float get_StackTop( float width , ::rux::uint8 is_scrollbar );
				float get_StackBottom( float width , ::rux::uint8 is_scrollbar );
				float get_StackHeight( ::rux::uint8 is_scrollbar );
				void set_ChildLocationType( XEnum_Child_Location_Type child_location_type );
				::rux::int32 IndexOf( const XObject& control );
				Object& get_ControlAt( ::rux::uint32 index );
				::rux::uint32 get_ControlsCount( void );
				void BringToFront( const XObject& control );
				void set_ZIndex( const XObject& control , ::rux::uint32 index );
				void BringToFront( const XGCRef& control );
				void set_ZIndex( const XGCRef& control , ::rux::uint32 index );
				::rux::uint8 ExistsControl( const XGCRef& control );
				::rux::uint8 ExistsControl( const XObject& control );
				void set_Background( ::rux::gui::ColorBase* background );
				rux::gui::ColorBase* get_Background( void );
				rux::gui::ColorBase* get_BorderColor( void );
				void set_BorderColor( ::rux::gui::ColorBase* border_color );
				void set_BorderWidth( float border_width );
				void set_Corner( const ::rux::gui::Thickness& corner );
				void set_IsForwardEventsIfChildrenDoNotRaisedEvent( ::rux::byte is_forward_events_if_children_do_not_raised_event );
				::rux::byte get_IsForwardEventsIfChildrenDoNotRaisedEvent( void );
				void set_ScrollXOffset( float scroll_x_offset );
				void set_ScrollYOffset( float scroll_y_offset );
				float get_ScrollXOffset( void );
				float get_ScrollYOffset( void );
				size_t CalculateWrapVisibleControlsCount( float width , float height , const ::rux::gui::Margin& margin );
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
				typename T::InternalClass& FindOne( const char* utf8name , size_t utf8namelen , ::rux::uint8& is_found )
				{
					return (*this)()->FindOne< T >( utf8name , utf8namelen , is_found );
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
			};
		};
	};
};
#endif
