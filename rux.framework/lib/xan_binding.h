#ifndef XAN_BINDING_H
#define XAN_BINDING_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_object.h>
#include <xan_uint32.h>
namespace rux
{
	begin_declare_rux_class( Binding );
		~Binding( void );
		Object& GetValue( void );
		void set_Object( const XObject& object );
		void set_Object( XGCRef* object );
		void set_PropertyName( const char* utf8propertyname );
	begin_declare_rux_class_members( Binding );
		XGCRef* _object;
		char* _property_name;
	end_declare_rux_class_members( Binding );
		Object& GetValue( void );
		void set_Object( const XObject& object );
		void set_Object( XGCRef* object );
		void set_PropertyName( const char* utf8propertyname );
	end_declare_rux_class();
};
#endif
