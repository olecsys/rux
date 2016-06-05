#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_expr.h>
#include <xan_uint64.h>
#include <xan_double.h>
#include <xan_float.h>
#include <xan_int32.h>
#include <xan_uint32.h>
#include <xan_int64.h>
#include <xan_numeric.h>
#include <xan_binding.h>
#include <xan_keyvaluepair.h>
#include <xan_boolean.h>
begin_implement_rux_class_ns( Expr , rux );
	_first = 0;
	_second = 0;
	_operation = XEnum_Expr_Add;
end_implement_rux_class();
namespace rux
{
	implement_duplicate_internal_result_function( Object& , Expr , Calculate );
	implement_duplicate_internal_function_1( Expr , set_FirstValue , const XObject& );
	implement_duplicate_internal_function_1( Expr , set_SecondValue , const XObject& );
	implement_duplicate_internal_function_1( Expr , set_FirstValue , XGCRef* );
	implement_duplicate_internal_function_1( Expr , set_SecondValue , XGCRef* );
	implement_duplicate_internal_function_1( Expr , set_Operation , XEnum_Expr );
	Expr::~Expr( void )
	{
		if( _first )
			_first->Release();
		if( _second )
			_second->Release();
	};
	void Expr::set_FirstValue( const XObject& first )
	{
		set_FirstValue( first.get_GCRef() );
	};
	void Expr::set_SecondValue( const XObject& second )
	{
		set_SecondValue( second.get_GCRef() );
	};
	void Expr::set_FirstValue( XGCRef* first )
	{
		if( _first )
			_first->Release();
		_first = first;
		_first->AddRef();
	};
	void Expr::set_SecondValue( XGCRef* second )
	{
		if( _second )
			_second->Release();
		_second = second;
		_second->AddRef();
	};
	void Expr::set_Operation( XEnum_Expr operation )
	{
		_operation = operation;
	};
	Object& Expr::Calculate( void )
	{
		return ::rux::Expr::Calculate( _first , _second , _operation );
	};
	Object& Expr::Calculate( XGCRef* first , XGCRef* second , XEnum_Expr operation )
	{
		if( first && second )
		{
			if( rux_is_object_ptr( first , ::rux::XDouble ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::Double* firval = (::rux::Double*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XFloat ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::Float* firval = (::rux::Float*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XInt64 ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::Int64* firval = (::rux::Int64*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XUInt64 ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					::rux::uint64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XUInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::UInt64* firval = (::rux::UInt64*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					::rux::uint64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XUInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XInt32 ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					::rux::int32 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt32( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::Int32* firval = (::rux::Int32*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					::rux::int32 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt32( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XUInt32 ) )
			{
				if( rux_is_object_ptr( second , ::rux::XDouble ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::Double* secval = (::rux::Double*)second->get_ObjectAddress();
					double res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XDouble( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XFloat ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::Float* secval = (::rux::Float*)second->get_ObjectAddress();
					float res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XFloat( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt64 ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::Int64* secval = (::rux::Int64*)second->get_ObjectAddress();
					::rux::int64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt64 ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::UInt64* secval = (::rux::UInt64*)second->get_ObjectAddress();
					::rux::uint64 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XUInt64( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XInt32 ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::Int32* secval = (::rux::Int32*)second->get_ObjectAddress();
					::rux::int32 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XInt32( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XUInt32 ) )
				{
					::rux::UInt32* firval = (::rux::UInt32*)first->get_ObjectAddress();
					::rux::UInt32* secval = (::rux::UInt32*)second->get_ObjectAddress();
					::rux::uint32 res = 0;
					switch( operation )
					{
					case XEnum_Expr_Mul:
						{
							res = firval->operator()() * secval->operator()();
							break;
						}
					case XEnum_Expr_Div:
						{
							if( secval->operator()() != 0 )
								res = firval->operator()() / secval->operator()();
							break;
						}
					case XEnum_Expr_Sub:
						{
							res = firval->operator()() - secval->operator()();
							break;
						}
					case XEnum_Expr_Add:
						{
							res = firval->operator()() + secval->operator()();
							break;
						}
					}
					return XObject( ::rux::XUInt32( res ) )++;
				}
				else if( rux_is_object_ptr( second , ::rux::XNumeric ) )
				{
					::rux::Numeric* first_val = (::rux::Numeric*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->_value_gcref , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XExpr ) )
				{
					::rux::Expr* first_val = (::rux::Expr*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->Calculate().get_GCRefObj() , operation );
				}
				else if( rux_is_object_ptr( second , ::rux::XBinding ) )
				{
					::rux::Binding* first_val = (::rux::Binding*)second->get_ObjectAddress();
					return ::rux::Expr::Calculate( first , first_val->GetValue().get_GCRefObj() , operation );
				}
			}
			else if( rux_is_object_ptr( first , ::rux::XNumeric ) )
			{
				::rux::Numeric* first_val = (::rux::Numeric*)first->get_ObjectAddress();
				return ::rux::Expr::Calculate( first_val->_value_gcref , second , operation );
			}
			else if( rux_is_object_ptr( first , ::rux::XExpr ) )
			{
				::rux::Expr* first_val = (::rux::Expr*)first->get_ObjectAddress();
				return ::rux::Expr::Calculate( first_val->Calculate().get_GCRefObj() , second , operation );
			}
			else if( rux_is_object_ptr( first , ::rux::XBinding ) )
			{
				::rux::Binding* first_val = (::rux::Binding*)first->get_ObjectAddress();
				return ::rux::Expr::Calculate( first_val->GetValue().get_GCRefObj() , second , operation );
			}
		}
		return XObject( "" , __FILE__ , __LINE__ )++;
	};
};