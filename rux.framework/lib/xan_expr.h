#ifndef XAN_EXPR_H
#define XAN_EXPR_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_object.h>
#include <xan_uint32.h>
namespace rux
{
	enum XEnum_Expr
	{
		XEnum_Expr_Mul ,
		XEnum_Expr_Div ,
		XEnum_Expr_Sub ,
		XEnum_Expr_Add
	};
	begin_declare_rux_class( Expr );
		Object& Calculate( void );
		void set_FirstValue( const XObject& first );
		void set_SecondValue( const XObject& second );
		void set_FirstValue( XGCRef* first );
		void set_SecondValue( XGCRef* second );
		void set_Operation( XEnum_Expr operation );
		~Expr( void );
		static Object& Calculate( XGCRef* first , XGCRef* second , XEnum_Expr operation );
	begin_declare_rux_class_members( Expr );
		XGCRef* _first;
		XGCRef* _second;
		XEnum_Expr _operation;

	end_declare_rux_class_members( Expr );
		Object& Calculate( void );
		void set_FirstValue( const XObject& first );
		void set_SecondValue( const XObject& second );
		void set_FirstValue( XGCRef* first );
		void set_SecondValue( XGCRef* second );
		void set_Operation( XEnum_Expr operation );
	end_declare_rux_class();
};
#endif
