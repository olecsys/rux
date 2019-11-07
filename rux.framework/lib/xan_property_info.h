#ifndef XAN_PROPERTY_INFO_H
#define XAN_PROPERTY_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_gchandle.h"
namespace rux
{	
	typedef void (*set_property_t)( const XGCRef* object , const XGCRef* param );
	typedef XGCRef& (*get_property_t)( const XGCRef* object );
	begin_declare_rux_class( PropertyInfo );
		friend class ::Type;
		void Set( const XObject& object , const XObject& param );
		void Set( const XGCRef* object , const XGCRef* param );
		XGCRef& Get( const XObject& object );
		XGCRef& Get( const XGCRef& object );
		XGCRef& Get( const XGCRef* object );
		const char* get_Name( void );
	begin_declare_rux_class_members( PropertyInfo );
		::rux::uint32 _name_hash;
		Type* _object_type;
		Type* _param_type;
		set_property_t _set_property_address;
		get_property_t _get_property_address;
	end_declare_rux_class_members( PropertyInfo );
		void Set( const XObject& object , const XObject& param );
		void Set( const XGCRef* object , const XGCRef* param );
		XGCRef& Get( const XObject& object );
		XGCRef& Get( const XGCRef& object );
		XGCRef& Get( const XGCRef* object );
		const char* get_Name( void );
	end_declare_rux_class();
};
#endif