#ifndef XAN_TYPE_H
#define XAN_TYPE_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_gcref.h"
#include "xan_crtsect.h"
enum XEnumMethodParameterType
{
	XEnumMethodParameterType_Int32,
	XEnumMethodParameterType_UInt32,
	XEnumMethodParameterType_Int64,
	XEnumMethodParameterType_Double,
	XEnumMethodParameterType_Ptr	,
	XEnumMethodParameterType_String ,
	XEnumMethodParameterType_Void ,
	XEnumMethodParameterType_Boolean	
};
namespace rux
{	
	class PropertyInfo;	
	class ConstructorInfo;
	class FunctionInfo;
};
class Type : public XGCRef
{
	friend class XType;
	friend class XTypeManager;
	friend class ::rux::PropertyInfo;
	friend class ::rux::FunctionInfo;
	declare_rux_base_internal_methods();
private:
	::rux::uint32 _type_index;
	::rux::uint32 _type_hash;
	::rux::int32 _type_module_index;
	XMallocArray< ::rux::PropertyInfo* > _property_infos;
	XMallocArray< ::rux::FunctionInfo* > _function_infos;
	::rux::ConstructorInfo* _constructor_info;
	::rux::uint8 _is_array;
	Type* _array_item_type;
public:
	Type( void );
	virtual ~Type();
	const char* get_TypeName( void );	
	friend bool operator ==(Type& type1,XType& type2);
	friend bool operator ==(XType& type1,Type& type2);
	friend bool operator ==(XType& type1,XType& type2);
	friend bool operator ==(Type& type1,Type& type2);		
	friend bool operator !=(Type& type1,XType& type2);
	friend bool operator !=(XType& type1,Type& type2);
	friend bool operator !=(XType& type1,XType& type2);
	friend bool operator !=(Type& type1,Type& type2);	
	void RegisterProperty( const char* name ,
		Type* param_type ,
		void* set_property_address ,
		void* get_property_address );
	void RegisterFunction( ::rux::uint8 with_return , const char* name ,
		void* function_address ,
		Type* param_type0 );
	void RegisterConstructor( void* ctor_address );
	::rux::PropertyInfo* get_PropertyInfo( ::rux::uint32 times33_hash );
	::rux::PropertyInfo* get_PropertyInfo( const char* property_name );
	::rux::ConstructorInfo* get_ConstructorInfo( void );
	::rux::FunctionInfo* get_FunctionInfo( const char* function_name );
	::rux::FunctionInfo* get_FunctionInfo( ::rux::uint32 function_name_hash );
	::rux::uint32 get_TypeIndex( void );
	::rux::uint8 get_IsArray( void );
	XMallocArray< ::rux::PropertyInfo* >* get_Properties( void );
	Type& get_ArrayItemType( void );
	void set_ModuleIndex( ::rux::int32 module_index );
	::rux::int32 get_ModuleIndex( void );
};
class XType : public GCContainer
{
	declare_rux_base_methods_for_object();
	friend class XObject;
	friend class XTypeManager;
public:	
	typedef Type InternalClass;
	XType( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	XType( XType& object );
	XType( Type& type , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );	
	XType( const XGCRef& gc_ref , const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	virtual ~XType();	
	XType& operator=( const XType& object );
	XType& operator=( const Type& gc_ref );
	XType& operator=( const XGCRef& gc_ref );
	XType& operator=(::rux::int32 null_ptr);	
	Type* operator ()() const;	
	virtual XGCRef* get_GCRef( ::rux::uint8 create = 1 , ::rux::uint8 transform = 1 ) const;
	Type& operator++(::rux::int32) const;
	void set_ModuleIndex( ::rux::int32 module_index );
	::rux::uint32 get_TypeIndex( void );
	const char* get_TypeName( void );
	::rux::int32 get_ModuleIndex( void );
	bool operator ==( const XType& type );
	friend bool operator ==(Type& type1,XType& type2);
	friend bool operator ==(XType& type1,Type& type2);
	friend bool operator ==(XType& type1,XType& type2);
	friend bool operator ==(Type& type1,Type& type2);	
	friend bool operator !=(Type& type1,XType& type2);
	friend bool operator !=(XType& type1,Type& type2);
	friend bool operator !=(XType& type1,XType& type2);
	friend bool operator !=(Type& type1,Type& type2);	
	void set_ByRef( const XType& type ) const;
	void set_ByRef( const Type& type ) const;
	void set_ByRef( const XGCRef& gcref ) const;
	::rux::PropertyInfo* get_PropertyInfo( ::rux::uint32 times33_hash );
	::rux::PropertyInfo* get_PropertyInfo( const char* property_name );
	::rux::FunctionInfo* get_FunctionInfo( const char* function_name );
	::rux::FunctionInfo* get_FunctionInfo( ::rux::uint32 function_name_hash );
	::rux::ConstructorInfo* get_ConstructorInfo( void );
	::rux::uint8 get_IsArray( void );
	Type& get_ArrayItemType( void );
	Type& get_Type( void );
	XMallocArray< ::rux::PropertyInfo* >* get_Properties( void );
};
#endif
