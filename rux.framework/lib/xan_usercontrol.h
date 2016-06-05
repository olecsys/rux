#ifndef XAN_USERCONTROL_H
#define XAN_USERCONTROL_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_ui_enums.h>
#include <xan_uigroup.h>
namespace rux
{
	namespace gui
	{
		namespace controls
		{
			class UserControl
			{
				friend class ::rux::gui::controls::Group;
				friend class ::rux::gui::Window;
			protected:
				::rux::gui::controls::XGroup _group_main;				
			public:
				DECLARE_RUX_BASE_UI_FUNCTIONS( UserControl );
				UserControl( void );
				declare_content_size();				
				virtual void InitializeControl( void ) = 0;
				void set_OnDeactivatedAll( ::rux::gui::events::on_deactivated_event_t on_activated );
				void AddControl( const XObject& control , ::rux::XString& error );
				void AddControl( const XGCRef& control , ::rux::XString& error );
				void InsertControl( ::rux::uint32 index , XObject& control , ::rux::XString& error );
				void RemoveControl( XGCRef& control );
				void RemoveControl( XObject& control );
				void ClearControls( void );
				template< class T >
				rux::Array< T >& Find( void )
				{
					return _group_main.Find< T >();
				};
				template< class T >
				rux::Array< T >& FindAll( void )
				{
					return _group_main.FindAll< T >();
				};
				template< class T >
				rux::Array< T >& Find( const ::rux::XString& name )
				{
					return _group_main.Find< T >( name );
				};
				template< class T >
				rux::Array< T >& FindAll( const ::rux::XString& name )
				{
					return _group_main.FindAll< T >( name );
				};
				template< class T >
				typename T::InternalClass& FindOne( const ::rux::XString& name , ::rux::uint8& is_found )
				{
					return _group_main.FindOne< T >( name , is_found );
				};
				template< class T >
				typename T::InternalClass& FindOneAll( const ::rux::XString& name , ::rux::uint8& is_found )
				{
					return _group_main.FindOneAll< T >( name , is_found );
				};
			};
#define begin_declare_rux_gui_user_control( rux_class ) 	begin_declare_rux_class_2( rux_class , ::rux::gui::controls::UserControl );\
				virtual void InitializeControl( void );\
				virtual void* DynamicCast( ::rux::int32 class_type ) const
#define begin_declare_rux_gui_user_control_members( rux_class ) begin_declare_rux_class_members( rux_class )				
#define end_declare_rux_gui_user_control_members( rux_class ) end_declare_rux_class_members( rux_class );\
				DECLARE_BASE_UI_FUNCTIONS()
#define end_declare_rux_gui_user_control() end_declare_rux_class()
#define begin_rux_gui_user_control_constructor_ns( rux_class , ns ) begin_implement_rux_class_ns( rux_class , ns )
#define end_rux_gui_user_control_constructor() end_implement_rux_class()
#define begin_rux_gui_user_control_initialize_ns( rux_class , ns ) IMPLEMENT_BASE_UI_FUNCTIONS( ns::X##rux_class );\
				void* ns::rux_class::DynamicCast( ::rux::int32 class_type ) const\
				{\
					if( class_type == ::rux::gui::XEnum_ClassType_UserControl )\
					{\
						::rux::gui::controls::UserControl* ptr = const_cast< ns::rux_class* >( this );\
						return ptr;\
					}\
					return 0;\
				};\
				void ns::rux_class::InitializeControl( void )\
				{\
					_group_main()->_is_user_control = 1;
#define end_rux_gui_user_control_initialize() }
		};
	};
};
#endif