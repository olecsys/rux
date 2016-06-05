#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_math.h>
#include <xan_error.h>
#include <xan_keyvaluepair.h>
#include <xan_uint64.h>
#include <xan_boolean.h>
implement_rux_base_methods_ns( Point , rux );
implement_rux_set_operators_ns_( Point , rux );
implement_rux_base_methods_ns( PointF , rux );
implement_rux_set_operators_ns_( PointF , rux );
implement_rux_base_methods_ns( PointD , rux );
implement_rux_set_operators_ns_( PointD , rux );
implement_rux_base_methods_ns( Vector , rux );
implement_rux_set_operators_ns_( Vector , rux );
implement_rux_base_methods_ns( VectorF , rux );
implement_rux_set_operators_ns_( VectorF , rux );
implement_rux_base_methods_ns( VectorD , rux );
implement_rux_set_operators_ns_( VectorD , rux );
implement_rux_base_methods_ns( RectangleF , rux );
implement_rux_set_operators_ns_( RectangleF , rux );
implement_rux_base_methods_ns( RectangleD , rux );
implement_rux_set_operators_ns_( RectangleD , rux );
implement_rux_base_methods_ns( GridPosition , rux );
implement_rux_set_operators_ns_( GridPosition , rux );
implement_rux_base_methods_ns( TriangleF , rux );
implement_rux_set_operators_ns_( TriangleF , rux );
implement_rux_base_methods_ns( TriangleD , rux );
implement_rux_set_operators_ns_( TriangleD , rux );
implement_rux_base_methods_ns( Math , rux );
implement_rux_set_operators_ns_( Math , rux );
namespace rux
{	
	implement_ctor_and_register_type( Point );
	void XPoint::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( PointF );
	void XPointF::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( PointD );
	void XPointD::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( Vector );
	void XVector::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( VectorF );
	void XVectorF::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( VectorD );
	void XVectorD::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( RectangleF );
	void XRectangleF::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( RectangleD );
	void XRectangleD::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( GridPosition );
	void XGridPosition::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( TriangleF );
	void XTriangleF::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( TriangleD );
	void XTriangleD::x_initialize( void )
	{
	};
	implement_ctor_and_register_type( Math );
	void XMath::x_initialize( void )
	{
	};
	float math::round( float number , ::rux::uint8 precision )
	{	
		number *= pow( 10.f , precision );
		number = floor( number + 0.5f );
		number /= pow( 10.f , precision );
		return number;
	};
	double math::round( double number , ::rux::uint8 precision )
	{	
		number *= pow( 10.0 , precision );
		number = floor( number + 0.5 );
		number /= pow( 10.0 , precision );
		return number;
	};
	::rux::uint8 math::is_point_belongs_to_rectangle( float x0 , float y0 , float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x , float y , ::rux::uint8 precision )
	{
		x0 = ::rux::math::round( x0 , precision ); 
		x1 = ::rux::math::round( x1 , precision );
		x2 = ::rux::math::round( x2 , precision );
		x3 = ::rux::math::round( x3 , precision );
		x = ::rux::math::round( x , precision );
		y0 = ::rux::math::round( y0 , precision ); 
		y1 = ::rux::math::round( y1 , precision );
		y2 = ::rux::math::round( y2 , precision );
		y3 = ::rux::math::round( y3 , precision );
		y = ::rux::math::round( y , precision );
		float max_x = x0;
		float max_y = y0;
		float min_x = x0;
		float min_y = y0;
		if( x1 > max_x )
			max_x = x1;
		if( y1 > max_y )
			max_y = y1;
		if( x1 < min_x )
			min_x = x1;
		if( y1 < min_y )
			min_y = y1;
		if( x2 > max_x )
			max_x = x2;
		if( y2 > max_y )
			max_y = y2;
		if( x2 < min_x )
			min_x = x2;
		if( y2 < min_y )
			min_y = y2;
		if( x3 > max_x )
			max_x = x3;
		if( y3 > max_y )
			max_y = y3;
		if( x3 < min_x )
			min_x = x3;
		if( y3 < min_y )
			min_y = y3;
		if( x >= min_x && y >= min_y && x <= max_x && y <= max_y )
		{
			size_t intersection_count = 0;
			if( ::rux::XMath::is_segments_intersect( x , y , max_x + 64 , y , x0 , y0 , x1 , y1 , min_x , min_y , 0 , precision ) == 1 )
				intersection_count++;
			if( ::rux::XMath::is_segments_intersect( x , y , max_x + 64 , y , x1 , y1 , x2 , y2 , min_x , min_y , 0 , precision ) == 1 )
				intersection_count++;
			if( ::rux::XMath::is_segments_intersect( x , y , max_x + 64 , y , x2 , y2 , x3 , y3 , min_x , min_y , 0 , precision ) == 1 )
				intersection_count++;
			if( ::rux::XMath::is_segments_intersect( x , y , max_x + 64 , y , x3 , y3 , x0 , y0 , min_x , min_y , 0 , precision ) == 1 )
				intersection_count++;
			if( intersection_count > 0 )
			{
				::rux::log::WriteDebug( "intersection_count, %u" , (::rux::uint32)intersection_count );
				if( intersection_count % 2 == 0 )
					return 0;
				else
					return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	};
	::rux::int32 math::quadratic_equation( float a , float b , float c , float& x1 , float& x2 )
	{
		float D = b * b - 4 * a * c;
		if( D > 0 )
		{
			x1 = ( -b + sqrt( D ) ) / 2 * a;
			x2 = ( -b - sqrt( D ) ) / 2 * a;
			return 2;
		}
		else if( D == 0 )
		{
			x1 = ( -b ) / 2 * a;
			x2 = x1;
			return 1;
		}
		else
			return 0;
	};
	void math::calculate_third_point_of_right_triangle( float x1 , float y1 , float x2 , float y2 , float cathetus , float& x3 , float& y3 , float& x4 , float& y4 )
	{
		if( y1 == y2 )
		{
			x3 = x1;
			x4 = x1;
			y3 = y1 - cathetus;
			y4 = y1 + cathetus;
		}
		else if( x1 == x2 )
		{
			y3 = y1;
			y4 = y1;
			x3 = x1 - cathetus;
			x4 = x1 + cathetus;
		}
		else
		{
			::rux::math::quadratic_equation( 1 , -2 * x1 , x1 * x1 - ( cathetus * cathetus / ( 1 + ( ( x2 - x1 ) * (x2 - x1 ) / ( ( y2 - y1 ) * ( y2 - y1 ) ) ) ) ) , x3 , x4 );
			y3 = y1 - ( ( x2 - x1 ) * ( x3 - x1 ) / ( y2 - y1 ) );
			y4 = y1 - ( ( x2 - x1 ) * ( x4 - x1 ) / ( y2 - y1 ) );
		}
	};
	::rux::uint64 math::gcd( ::rux::uint64 number0 , ::rux::uint64 number1 )
	{
		while( number0 )
		{
			number1 %= number0;
			::std::swap( number1 , number0 );
		}
		return number1;		
	};
	::rux::int64 math::gcd( ::rux::int64 number0 , ::rux::int64 number1 )
	{
		while( number0 )
		{
			number1 %= number0;
			std::swap( number1 , number0 );
		}
		return number1;
	};
	::rux::uint32 math::gcd( ::rux::uint32 number0 , ::rux::uint32 number1 )
	{
		while( number0 )
		{
			number1 %= number0;
			std::swap( number1 , number0 );
		}
		return number1;
	};
	::rux::int32 math::gcd( ::rux::int32 number0 , ::rux::int32 number1 )
	{
		while( number0 )
		{
			number1 %= number0;
			std::swap( number1 , number0 );
		}
		return number1;
	};
	PointF::PointF( const char* , const char* , ::rux::int32 ) 
		: XGCRef( get_static_Type )
	{
		_x = 0;
		_y = 0;
	};
	XPointF::XPointF()
	{	
	};
	XPointF::XPointF( const PointF& point )
	{
		point.AddRef();
		(*this)()->_x = ::rux::math::round( point._x , 2 );
		(*this)()->_y = ::rux::math::round( point._y , 2 );
		point.Release();
	};
	XPointF::XPointF( const XObject& object )
	{
	  *static_cast<XGCHandle<PointF >*>(this) = object;
	};
	XPointF::XPointF( const XPointF& point )
	{
	  (*(static_cast<XGCHandle<PointF >*>( this ))) = point;
	};
	XPointF& XPointF::operator =( const PointF& point )
	{
		point.AddRef();
		(*this)()->_x = ::rux::math::round( point._x , 2 );
		(*this)()->_y = ::rux::math::round( point._y , 2 );
		point.Release();
		return *this;
	};
	XPointF& XPointF::operator =( const XPointF& point )
	{
	  (*(static_cast<XGCHandle<PointF >*>( this ))) = point;
		return *this;
	};
	XPointF::XPointF( float x , float y , ::rux::uint8 round_values )
	{
		if( round_values )
		{
			(*this)()->_x = ::rux::math::round( x , 2 );
			(*this)()->_y = ::rux::math::round( y , 2 );
		}
		else
		{
			(*this)()->_x = x;
			(*this)()->_y = y;
		}
	};
	PointF& operator *( float coef, const XPointF& point2 )
	{
		XPointF result_point;
		result_point()->_x = ::rux::math::round( coef * point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( coef * point2()->_y , 2 );
		return result_point++;
	};
	PointF& operator +( const XPointF& point1, const XPointF& point2 )
	{
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1()->_x + point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_x + point2()->_y , 2 );
		return result_point++;
	};
	PointF& operator +( const XPointF& point1, const PointF& point2 )
	{
		point2.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1()->_x + point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y + point2._y , 2 );
		point2.Release();
		return result_point++;
	};
	PointF& operator +( const PointF& point1, const XPointF& point2 )
	{
		point1.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1._x + point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1._y + point2()->_y , 2 );
		point1.Release();
		return result_point++;
	};
	PointF& operator +( const PointF& point1, const PointF& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1._x + point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1._y + point2._y , 2 );
		point1.Release();
		point2.Release();
		return result_point++;
	};
	PointF& operator -( const XPointF& point1, const XPointF& point2 )
	{
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1()->_x - point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y - point2()->_y , 2 );
		return result_point++;
	};
	PointF& operator -( const XPointF& point1, const PointF& point2 )
	{
		point2.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1()->_x - point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y - point2._y , 2 );
		point2.Release();
		return result_point++;
	};
	PointF& operator -( const PointF& point1, const XPointF& point2 )
	{
		point1.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1._x - point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1._y - point2()->_y , 2 );
		point1.Release();
		return result_point++;
	};
	PointF& operator -( const PointF& point1, const PointF& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		XPointF result_point;
		result_point()->_x = ::rux::math::round( point1._x - point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1._y - point2._y , 2 );
		point1.Release();
		point2.Release();
		return result_point++;
	};
	bool operator ==( const XPointF& point1, const XPointF& point2 )
	{
		return point1()->_x == point2()->_x && point1()->_y == point2()->_y;
	};
	bool operator !=( const PointF& point1, const PointF& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		bool result = point1._x != point2._x || point1._y != point2._y;
		point1.Release();
		point2.Release();
		return result;
	};
	implement_duplicate_internal_result_function( float , PointF , get_X );
	float PointF::get_X( void )
	{
		return _x;
	};
	implement_duplicate_internal_result_function( float , PointF , get_Y );
	float PointF::get_Y( void )
	{
		return _y;
	};
	implement_duplicate_internal_function_1( PointF , set_X , float );
	void PointF::set_X( float x )
	{
		_x = x;
	};
	implement_duplicate_internal_function_1( PointF , set_Y , float );
	void PointF::set_Y( float y )
	{
		_y = y;
	};
	PointD::PointD( const char* , const char* , ::rux::int32 ) 
		: XGCRef( get_static_Type )
	{
		_x = 0;
		_y = 0;
	};
	XPointD::XPointD()
	{	
	};
	XPointD::XPointD( const PointD& point )
	{
		point.AddRef();
		(*this)()->_x = ::rux::math::round( point._x , 2 );
		(*this)()->_y = ::rux::math::round( point._y , 2 );
		point.Release();
	};
	XPointD::XPointD( const XObject& object )
	{
	  *static_cast<XGCHandle<PointD >*>(this) = object;
	};
	XPointD::XPointD( const XPointD& point )
	{
	  (*(static_cast<XGCHandle<PointD >*>( this ))) = point;
	};
	XPointD& XPointD::operator =( const PointD& point )
	{
	  point.AddRef();
		(*this)()->_x = ::rux::math::round( point._x , 2 );
		(*this)()->_y = ::rux::math::round( point._y , 2 );
		point.Release();
		return *this;
	};
	XPointD& XPointD::operator =( const XPointD& point )
	{
	  (*(static_cast<XGCHandle<PointD >*>( this ))) = point;
		return *this;
	};
	XPointD::XPointD( double x , double y , ::rux::uint8 round_values )
	{
		if( round_values )
		{
			(*this)()->_x = ::rux::math::round( x , 2 );
			(*this)()->_y = ::rux::math::round( y , 2 );
		}
		else
		{
			(*this)()->_x = x;
			(*this)()->_y = y;
		}
	};
	PointD& operator *( double coef, const XPointD& point2 )
	{
		XPointD result_point;
		result_point()->_x = ::rux::math::round( coef * point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( coef * point2()->_y , 2 );
		return result_point++;
	};
	PointD& operator +( const XPointD& point1, const XPointD& point2 )
	{
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1()->_x + point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_x + point2()->_y , 2 );
		return result_point++;
	};
	PointD& operator +( const XPointD& point1, const PointD& point2 )
	{
		point2.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1()->_x + point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y + point2._y , 2 );
		point2.Release();
		return result_point++;
	};
	PointD& operator +( const PointD& point1, const XPointD& point2 )
	{
		point1.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1._x + point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1._y + point2()->_y , 2 );
		point1.Release();
		return result_point++;
	};
	PointD& operator +( const PointD& point1, const PointD& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1._x + point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1._y + point2._y , 2 );
		point1.Release();
		point2.Release();
		return result_point++;
	};
	PointD& operator -( const XPointD& point1, const XPointD& point2 )
	{
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1()->_x - point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y - point2()->_y , 2 );
		return result_point++;
	};
	PointD& operator -( const XPointD& point1, const PointD& point2 )
	{
		point2.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1()->_x - point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1()->_y - point2._y , 2 );
		point2.Release();
		return result_point++;
	};
	PointD& operator -( const PointD& point1, const XPointD& point2 )
	{
		point1.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1._x - point2()->_x , 2 );
		result_point()->_y = ::rux::math::round( point1._y - point2()->_y , 2 );
		point1.Release();
		return result_point++;
	};
	PointD& operator -( const PointD& point1, const PointD& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		XPointD result_point;
		result_point()->_x = ::rux::math::round( point1._x - point2._x , 2 );
		result_point()->_y = ::rux::math::round( point1._y - point2._y , 2 );
		point1.Release();
		point2.Release();
		return result_point++;
	};
	bool operator ==( const XPointD& point1, const XPointD& point2 )
	{
		return point1()->_x == point2()->_x && point1()->_y == point2()->_y;
	};
	bool operator !=( const PointD& point1, const PointD& point2 )
	{
		point1.AddRef();
		point2.AddRef();
		bool result = point1._x != point2._x || point1._y != point2._y;
		point1.Release();
		point2.Release();
		return result;
	};
	double XPointD::get_X( void )
	{
		return (*this)()->_x;
	};
	double XPointD::get_Y( void )
	{
		return (*this)()->_y;
	};
	void XPointD::set_X( double x )
	{
		(*this)()->_x = x;
	};
	void XPointD::set_Y( double y )
	{
		(*this)()->_y = y;
	};
	Point::Point( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{
		_x = 0;
		_y = 0;
	};
	XPoint::XPoint()
	{	
	};
	XPoint::XPoint( const Point& point )
	{
	  (*this) = point;
	};
	XPoint::XPoint( const XObject& object )
	{
	  *static_cast<XGCHandle<Point>*>(this) = object;
	};
	XPoint::XPoint( const XPoint& point )
	{
	  (*(static_cast<XGCHandle<Point>*>( this ))) = point;
	};
	XPoint& XPoint::operator =( const Point& point )
	{
	  (*(static_cast<XGCHandle<Point>*>( this ))) = point;
		return *this;
	};
	XPoint& XPoint::operator =( const XPoint& point )
	{
	  (*(static_cast<XGCHandle<Point>*>( this ))) = point;
		return *this;
	};
	XPoint::XPoint( ::rux::int64 x , ::rux::int64 y )
	{
		(*this)()->_x = x;
		(*this)()->_y = y;
	};	
	Point& operator +( const XPoint& point1, const XPoint& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1()->_x + point2()->_x;
		result_point()->_y = point1()->_x + point2()->_y;
		return result_point++;
	};
	Point& operator +( const XPoint& point1, const Point& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1()->_x + point2._x;
		result_point()->_y = point1()->_y + point2._y;
		return result_point++;
	};
	Point& operator +( const Point& point1, const XPoint& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1._x + point2()->_x;
		result_point()->_y = point1._y + point2()->_y;
		return result_point++;
	};
	Point& operator +( const Point& point1, const Point& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1._x + point2._x;
		result_point()->_y = point1._y + point2._y;
		return result_point++;
	};
	Point& operator -( const XPoint& point1, const XPoint& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1()->_x - point2()->_x;
		result_point()->_y = point1()->_y - point2()->_y;
		return result_point++;
	};
	Point& operator -( const XPoint& point1, const Point& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1()->_x - point2._x;
		result_point()->_y = point1()->_y - point2._y;
		return result_point++;
	};
	Point& operator -( const Point& point1, const XPoint& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1._x - point2()->_x;
		result_point()->_y = point1._y - point2()->_y;
		return result_point++;
	};
	Point& operator -( const Point& point1, const Point& point2 )
	{
		XPoint result_point;
		result_point()->_x = point1._x - point2._x;
		result_point()->_y = point1._y - point2._y;
		return result_point++;
	};
	::rux::int64 XPoint::get_X( void )
	{
		return (*this)()->_x;
	};
	::rux::int64 XPoint::get_Y( void )
	{
		return (*this)()->_y;
	};
	VectorF::VectorF( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
	};
	XVectorF::XVectorF()
	{	
	};
	XVectorF::XVectorF( const VectorF& vector )
	{
	  (*this) = vector;
	};
	XVectorF::XVectorF( const XObject& object )
	{
	  *static_cast<XGCHandle<VectorF >*>(this) = object;
	};
	XVectorF::XVectorF( const XVectorF& vector )
	{
	  (*(static_cast<XGCHandle<VectorF >*>( this ))) = vector;
	};
	XVectorF& XVectorF::operator =( const VectorF& vector )
	{
	  (*(static_cast<XGCHandle<VectorF >*>( this ))) = vector;
		return *this;
	};
	XVectorF& XVectorF::operator =( const XVectorF& vector )
	{
	  (*(static_cast<XGCHandle<VectorF >*>( this ))) = vector;
		return *this;
	};
	XVectorF::XVectorF( const XPointF& start , const XPointF& end )
	{
		(*this)()->_start()->_x = ::rux::math::round( start()->_x , 2 );
		(*this)()->_start()->_y = ::rux::math::round( start()->_y , 2 );
		(*this)()->_end()->_x = ::rux::math::round( end()->_x , 2 );
		(*this)()->_end()->_y = ::rux::math::round( end()->_y , 2 );
	};	
	PointF& XVectorF::get_Start( void )
	{
		return (*this)()->_start++;
	};
	PointF& XVectorF::get_End( void )
	{
		return (*this)()->_end++;
	};
	void XVectorF::set_Start( const XPointF& start )
	{
		(*this)()->_start()->_x = ::rux::math::round( start()->_x , 2 );
		(*this)()->_start()->_y = ::rux::math::round( start()->_y , 2 );
	};
	void XVectorF::set_End( const XPointF& end )
	{
		(*this)()->_end()->_x = ::rux::math::round( end()->_x , 2 );
		(*this)()->_end()->_y = ::rux::math::round( end()->_y , 2 );
	};
	bool operator ==( const XVectorF& vector1, const XVectorF& vector2 )
	{
		return ( vector1()->_start()->_x == vector2()->_start()->_x && vector1()->_start()->_y == vector2()->_start()->_y
			&& vector1()->_end()->_x == vector2()->_end()->_x && vector1()->_end()->_y == vector2()->_end()->_y )
			|| ( vector1()->_start()->_x == vector2()->_end()->_x && vector1()->_start()->_y == vector2()->_end()->_y
			&& vector1()->_end()->_x == vector2()->_start()->_x && vector1()->_end()->_y == vector2()->_start()->_y );
	};
	VectorD::VectorD( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
	};
	XVectorD::XVectorD()
	{	
	};
	XVectorD::XVectorD( const VectorD& vector )
	{
	  (*this) = vector;
	};
	XVectorD::XVectorD( const XObject& object )
	{
	  *static_cast<XGCHandle<VectorD >*>(this) = object;
	};
	XVectorD::XVectorD( const XVectorD& vector )
	{
	  (*(static_cast<XGCHandle<VectorD >*>( this ))) = vector;
	};
	XVectorD& XVectorD::operator =( const VectorD& vector )
	{
	  (*(static_cast<XGCHandle<VectorD >*>( this ))) = vector;
		return *this;
	};
	XVectorD& XVectorD::operator =( const XVectorD& vector )
	{
	  (*(static_cast<XGCHandle<VectorD >*>( this ))) = vector;
		return *this;
	};
	XVectorD::XVectorD( const XPointD& start , const XPointD& end )
	{
		(*this)()->_start()->_x = ::rux::math::round( start()->_x , 2 );
		(*this)()->_start()->_y = ::rux::math::round( start()->_y , 2 );
		(*this)()->_end()->_x = ::rux::math::round( end()->_x , 2 );
		(*this)()->_end()->_y = ::rux::math::round( end()->_y , 2 );
	};	
	PointD& XVectorD::get_Start( void )
	{
		return (*this)()->_start++;
	};
	PointD& XVectorD::get_End( void )
	{
		return (*this)()->_end++;
	};
	void XVectorD::set_Start( const XPointD& start )
	{
		(*this)()->_start()->_x = ::rux::math::round( start()->_x , 2 );
		(*this)()->_start()->_y = ::rux::math::round( start()->_y , 2 );
	};
	void XVectorD::set_End( const XPointD& end )
	{
		(*this)()->_end()->_x = ::rux::math::round( end()->_x , 2 );
		(*this)()->_end()->_y = ::rux::math::round( end()->_y , 2 );
	};
	bool operator ==( const XVectorD& vector1, const XVectorD& vector2 )
	{
		return ( vector1()->_start()->_x == vector2()->_start()->_x && vector1()->_start()->_y == vector2()->_start()->_y
			&& vector1()->_end()->_x == vector2()->_end()->_x && vector1()->_end()->_y == vector2()->_end()->_y )
			|| ( vector1()->_start()->_x == vector2()->_end()->_x && vector1()->_start()->_y == vector2()->_end()->_y
			&& vector1()->_end()->_x == vector2()->_start()->_x && vector1()->_end()->_y == vector2()->_start()->_y );
	};
	TriangleF::TriangleF( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
	};
	XTriangleF::XTriangleF()
	{	
	};
	XTriangleF::XTriangleF( const TriangleF& triangle )
	{
	  (*this) = triangle;
	};
	XTriangleF::XTriangleF( const XObject& object )
	{
	  *static_cast<XGCHandle<TriangleF >*>(this) = object;
	};
	XTriangleF::XTriangleF( const XTriangleF& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleF >*>( this ))) = triangle;
	};
	XTriangleF& XTriangleF::operator =( const TriangleF& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleF >*>( this ))) = triangle;
		return *this;
	};
	XTriangleF& XTriangleF::operator =( const XTriangleF& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleF >*>( this ))) = triangle;
		return *this;
	};
	XTriangleF::XTriangleF( const XPointF& point1 , const XPointF& point2 , const XPointF& point3 )
	{
		(*this)()->_point1()->_x = ::rux::math::round( point1()->_x , 2 );
		(*this)()->_point1()->_y = ::rux::math::round( point1()->_y , 2 );	
		(*this)()->_point2()->_x = ::rux::math::round( point2()->_x , 2 );
		(*this)()->_point2()->_y = ::rux::math::round( point2()->_y , 2 );	
		(*this)()->_point3()->_x = ::rux::math::round( point3()->_x , 2 );
		(*this)()->_point3()->_y = ::rux::math::round( point3()->_y , 2 );	
	};
	void XTriangleF::set_Point1( const XPointF& point1 )
	{
		(*this)()->_point1()->_x = ::rux::math::round( point1()->_x , 2 );
		(*this)()->_point1()->_y = ::rux::math::round( point1()->_y , 2 );	
	};
	void XTriangleF::set_Point2( const XPointF& point2 )
	{
		(*this)()->_point2()->_x = ::rux::math::round( point2()->_x , 2 );
		(*this)()->_point2()->_y = ::rux::math::round( point2()->_y , 2 );	
	};
	void XTriangleF::set_Point3( const XPointF& point3 )
	{
		(*this)()->_point3()->_x = ::rux::math::round( point3()->_x , 2 );
		(*this)()->_point3()->_y = ::rux::math::round( point3()->_y , 2 );	
	};
	PointF& XTriangleF::get_Point1( void )
	{
		return (*this)()->_point1++;
	};
	PointF& XTriangleF::get_Point2( void )
	{
		return (*this)()->_point2++;
	};
	PointF& XTriangleF::get_Point3( void )
	{
		return (*this)()->_point3++;
	};
	bool operator ==( const XTriangleF& triangle1, const XTriangleF& triangle2 )
	{
		return ( triangle1()->_point1()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point3()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point1()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point2()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point3()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point2()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point1()->_y );
	};
	::rux::uint8 XTriangleF::get_IsLine( void )
	{
		return ( (*this)()->_point1()->_x == (*this)()->_point2()->_x && (*this)()->_point2()->_x == (*this)()->_point3()->_x )
			|| ( (*this)()->_point1()->_y == (*this)()->_point2()->_y && (*this)()->_point2()->_y == (*this)()->_point3()->_y ) ? 1 : 0;
	};
	TriangleD::TriangleD( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
	};
	XTriangleD::XTriangleD()
	{	
	};
	XTriangleD::XTriangleD( const TriangleD& triangle )
	{
	  (*this) = triangle;
	};
	XTriangleD::XTriangleD( const XObject& object )
	{
	  *static_cast<XGCHandle<TriangleD >*>(this) = object;
	};
	XTriangleD::XTriangleD( const XTriangleD& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleD >*>( this ))) = triangle;
	};
	XTriangleD& XTriangleD::operator =( const TriangleD& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleD >*>( this ))) = triangle;
		return *this;
	};
	XTriangleD& XTriangleD::operator =( const XTriangleD& triangle )
	{
	  (*(static_cast<XGCHandle<TriangleD >*>( this ))) = triangle;
		return *this;
	};
	XTriangleD::XTriangleD( const XPointD& point1 , const XPointD& point2 , const XPointD& point3 )
	{
		(*this)()->_point1()->_x = ::rux::math::round( point1()->_x , 2 );
		(*this)()->_point1()->_y = ::rux::math::round( point1()->_y , 2 );	
		(*this)()->_point2()->_x = ::rux::math::round( point2()->_x , 2 );
		(*this)()->_point2()->_y = ::rux::math::round( point2()->_y , 2 );	
		(*this)()->_point3()->_x = ::rux::math::round( point3()->_x , 2 );
		(*this)()->_point3()->_y = ::rux::math::round( point3()->_y , 2 );	
	};
	void XTriangleD::set_Point1( const XPointD& point1 )
	{
		(*this)()->_point1()->_x = ::rux::math::round( point1()->_x , 2 );
		(*this)()->_point1()->_y = ::rux::math::round( point1()->_y , 2 );	
	};
	void XTriangleD::set_Point2( const XPointD& point2 )
	{
		(*this)()->_point2()->_x = ::rux::math::round( point2()->_x , 2 );
		(*this)()->_point2()->_y = ::rux::math::round( point2()->_y , 2 );	
	};
	void XTriangleD::set_Point3( const XPointD& point3 )
	{
		(*this)()->_point3()->_x = ::rux::math::round( point3()->_x , 2 );
		(*this)()->_point3()->_y = ::rux::math::round( point3()->_y , 2 );	
	};
	PointD& XTriangleD::get_Point1( void )
	{
		return (*this)()->_point1++;
	};
	PointD& XTriangleD::get_Point2( void )
	{
		return (*this)()->_point2++;
	};
	PointD& XTriangleD::get_Point3( void )
	{
		return (*this)()->_point3++;
	};
	bool operator ==( const XTriangleD& triangle1, const XTriangleD& triangle2 )
	{
		return ( triangle1()->_point1()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point3()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point1()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point2()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point3()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point1()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point2()->_y )
			|| ( triangle1()->_point1()->_x == triangle2()->_point3()->_x
			&& triangle1()->_point1()->_y == triangle2()->_point3()->_y
			&& triangle1()->_point2()->_x == triangle2()->_point2()->_x
			&& triangle1()->_point2()->_y == triangle2()->_point2()->_y
			&& triangle1()->_point3()->_x == triangle2()->_point1()->_x
			&& triangle1()->_point3()->_y == triangle2()->_point1()->_y );
	};
	::rux::uint8 XTriangleD::get_IsLine( void )
	{
		return ( (*this)()->_point1()->_x == (*this)()->_point2()->_x && (*this)()->_point2()->_x == (*this)()->_point3()->_x )
			|| ( (*this)()->_point1()->_y == (*this)()->_point2()->_y && (*this)()->_point2()->_y == (*this)()->_point3()->_y ) ? 1 : 0;
	};
	RectangleF::RectangleF( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
		_width = 0.00f;
		_height = 0.00f;
	};
	XRectangleF::XRectangleF()
	{	
	};
	XRectangleF::XRectangleF( const RectangleF& rectangle )
	{
	  (*this) = rectangle;
	};
	XRectangleF::XRectangleF( const XObject& object )
	{
	  *static_cast<XGCHandle<RectangleF >*>(this) = object;
	};
	XRectangleF::XRectangleF( const XRectangleF& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleF >*>( this ))) = rectangle;
	};
	XRectangleF& XRectangleF::operator =( const RectangleF& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleF >*>( this ))) = rectangle;
		return *this;
	};
	XRectangleF& XRectangleF::operator =( const XRectangleF& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleF >*>( this ))) = rectangle;
		return *this;
	};
	XRectangleF::XRectangleF( const XPointF& point , float width , float height , ::rux::uint8 round_values )
	{
		if( round_values )
		{
			(*this)()->_point()->_x = ::rux::math::round( point()->_x , 2 );
			(*this)()->_point()->_y = ::rux::math::round( point()->_y , 2 );	
			(*this)()->_width = ::rux::math::round( width , 2 );	
			(*this)()->_height = ::rux::math::round( height , 2 );	
		}
		else
		{
			(*this)()->_point()->_x = point()->_x;
			(*this)()->_point()->_y = point()->_y;
			(*this)()->_width = width;
			(*this)()->_height = height;
		}
	};	
	PointF& RectangleF::get_Point( void )
	{
		return _point++;
	};
	float RectangleF::get_Width( void )
	{
		return _width;
	};
	float RectangleF::get_Height( void )
	{
		return _height;
	};
	::rux::uint8 XRectangleF::IsPointInside( float x , float y )
	{
		return (*this)()->_point.get_X() <= x && (*this)()->_point.get_Y() <= y
			&& (*this)()->_point.get_X() + (*this)()->_width >= x
			&& (*this)()->_point.get_Y() + (*this)()->_height >= y ? 1 : 0;
	};
	bool operator ==( const XRectangleF& rectangle1, const XRectangleF& rectangle2 )
	{
		return rectangle1()->_point()->_x == rectangle2()->_point()->_x
			&& rectangle1()->_point()->_y == rectangle2()->_point()->_y
			&& rectangle1()->_width == rectangle2()->_width
			&& rectangle1()->_height == rectangle2()->_height;
	};
	RectangleD::RectangleD( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
		_width = 0.00;
		_height = 0.00;
	};
	XRectangleD::XRectangleD()
	{	
	};
	XRectangleD::XRectangleD( const RectangleD& rectangle )
	{
	  (*this) = rectangle;
	};
	XRectangleD::XRectangleD( const XObject& object )
	{
	  *static_cast<XGCHandle<RectangleD >*>(this) = object;
	};
	XRectangleD::XRectangleD( const XRectangleD& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleD >*>( this ))) = rectangle;
	};
	XRectangleD& XRectangleD::operator =( const RectangleD& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleD >*>( this ))) = rectangle;
		return *this;
	};
	XRectangleD& XRectangleD::operator =( const XRectangleD& rectangle )
	{
	  (*(static_cast<XGCHandle<RectangleD >*>( this ))) = rectangle;
		return *this;
	};
	XRectangleD::XRectangleD( const XPointD& point , double width , double height , ::rux::uint8 round_values )
	{
		if( round_values )
		{
			(*this)()->_point()->_x = ::rux::math::round( point()->_x , 2 );
			(*this)()->_point()->_y = ::rux::math::round( point()->_y , 2 );	
			(*this)()->_width = ::rux::math::round( width , 2 );	
			(*this)()->_height = ::rux::math::round( height , 2 );	
		}
		else
		{
			(*this)()->_point()->_x = point()->_x;
			(*this)()->_point()->_y = point()->_y;
			(*this)()->_width = width;
			(*this)()->_height = height;
		}
	};	
	implement_duplicate_internal_result_function( PointF& , RectangleF , get_Point );
	implement_duplicate_internal_result_function( float , RectangleF ,get_Width );
	implement_duplicate_internal_result_function( float , RectangleF ,get_Height );
	PointD& XRectangleD::get_Point( void )
	{
		return (*this)()->_point++;
	};
	double XRectangleD::get_Width( void )
	{
		return (*this)()->_width;
	};
	double XRectangleD::get_Height( void )
	{
		return (*this)()->_height;
	};
	bool operator ==( const XRectangleD& rectangle1, const XRectangleD& rectangle2 )
	{
		return rectangle1()->_point()->_x == rectangle2()->_point()->_x
			&& rectangle1()->_point()->_y == rectangle2()->_point()->_y
			&& rectangle1()->_width == rectangle2()->_width
			&& rectangle1()->_height == rectangle2()->_height;
	};	
	GridPosition::GridPosition( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{
		_cols = 0;
		_rows = 0;
		_x = 0;
		_y = 0;
		_dx = 0;
		_dy = 0;
	};
	XGridPosition::XGridPosition()
	{	
	};
	XGridPosition::XGridPosition( const GridPosition& grid_position )
	{
	  (*this) = grid_position;
	};
	XGridPosition::XGridPosition( const XObject& object )
	{
	  *static_cast<XGCHandle<GridPosition >*>(this) = object;
	};
	XGridPosition::XGridPosition( const XGridPosition& grid_position )
	{
	  (*(static_cast<XGCHandle<GridPosition >*>( this ))) = grid_position;
	};
	XGridPosition& XGridPosition::operator =( const GridPosition& grid_position )
	{
	  (*(static_cast<XGCHandle<GridPosition >*>( this ))) = grid_position;
		return *this;
	};
	XGridPosition& XGridPosition::operator =( const XGridPosition& grid_position )
	{
	  (*(static_cast<XGCHandle<GridPosition >*>( this ))) = grid_position;
		return *this;
	};
	XGridPosition::XGridPosition( ::rux::uint32 cols , ::rux::uint32 rows , ::rux::uint32 x , ::rux::uint32 y , ::rux::uint32 dx , ::rux::uint32 dy )
	{
		(*this)()->_cols = cols;
		(*this)()->_rows = rows;
		(*this)()->_x = x;
		(*this)()->_y = y;
		(*this)()->_dx = dx;
		(*this)()->_dy = dy;
	};	
	Vector::Vector( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{		
	};
	XVector::XVector()
	{	
	};
	XVector::XVector( const Vector& vector )
	{
	  (*this) = vector;
	};
	XVector::XVector( const XObject& object )
	{
	  *static_cast<XGCHandle<Vector>*>(this) = object;
	};
	XVector::XVector( const XVector& vector )
	{
	  (*(static_cast<XGCHandle<Vector>*>( this ))) = vector;
	};
	XVector& XVector::operator =( const Vector& vector )
	{
	  (*(static_cast<XGCHandle<Vector>*>( this ))) = vector;
		return *this;
	};
	XVector& XVector::operator =( const XVector& vector )
	{
	  (*(static_cast<XGCHandle<Vector>*>( this ))) = vector;
		return *this;
	};
	XVector::XVector( const XPoint& start , const XPoint& end )
	{
		(*this)()->_start = start;
		(*this)()->_end = end;
	};	
	Point& XVector::get_Start( void )
	{
		return (*this)()->_start++;
	};
	Point& XVector::get_End( void )
	{
		return (*this)()->_end++;
	};
	Math::Math( const char* , const char* , ::rux::int32 )
		: XGCRef( get_static_Type )
	{
	};
	XMath::XMath()
	{	
	};
	XMath::XMath( const Math& math )
	{
	  (*this) = math;
	};
	XMath::XMath( const XObject& object )
	{
	  *static_cast<XGCHandle<Math>*>(this) = object;
	};
	XMath::XMath( const XMath& math )
	{
	  (*(static_cast<XGCHandle<Math>*>( this ))) = math;
	};
	XMath& XMath::operator =( const Math& math )
	{
	  (*(static_cast<XGCHandle<Math>*>( this ))) = math;
		return *this;
	};
	XMath& XMath::operator =( const XMath& math )
	{
	  (*(static_cast<XGCHandle<Math>*>( this ))) = math;
		return *this;
	};
	::rux::uint32 XMath::CyclicLeftShift( ::rux::uint32 number , ::rux::uint32 bits )
	{
		bits = bits % ( sizeof( ::rux::uint32 ) * 8 );
		return ( number << bits ) | ( number >> ( sizeof( ::rux::uint32 ) * 8 - bits ) );
	};
	::rux::uint32 XMath::CyclicRightShift( ::rux::uint32 number , ::rux::uint32 bits )
	{
		bits = bits % ( sizeof( ::rux::uint32 ) * 8 );
		return ( number >> bits ) | ( number << ( sizeof( ::rux::uint32 ) * 8 - bits ) );
	};
	::rux::uint8 XMath::IsIntervalsIntersect( const XVector& interval1 , const XVector& interval2 )
	{
		XPoint straight1 = interval1()->_end - interval1()->_start;
		XPoint straight2 = interval2()->_end - interval2()->_start;	
		::rux::int64 a1 = - straight1()->_y;
		::rux::int64 b1 = + straight1()->_x;
		::rux::int64 d1 = - ( a1 * interval1()->_start()->_x + b1 * interval1()->_start()->_y );
		::rux::int64 a2 = - straight2()->_y;
		::rux::int64 b2 = + straight2()->_x;
		::rux::int64 d2 = - ( a2 * interval2()->_start()->_x + b2 * interval2()->_start()->_y );
		::rux::int64 seg1_line2_start = a2 * interval1()->_start()->_x + b2 * interval1()->_start()->_y + d2;
		::rux::int64 seg1_line2_end = a2 * interval1()->_end()->_x + b2 * interval1()->_end()->_y + d2;
		::rux::int64 seg2_line1_start = a1 * interval2()->_start()->_x + b1 * interval2()->_start()->_y + d1;
		::rux::int64 seg2_line1_end = a1 * interval2()->_end()->_x + b1 * interval2()->_end()->_y + d1;	
		if( seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 ) 
			return 0;
		return 1;
	};
	void XMath::polygon_max_min( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float& max_x , float& max_y , float& min_x , float& min_y )
	{		
		if( polygon_x.Count() > 2
			&& polygon_x.Count() == polygon_y.Count() )
		{				
			max_x = polygon_x[ 0 ];
			max_y = polygon_y[ 0 ];
			min_x = polygon_x[ 0 ];
			min_y = polygon_y[ 0 ];
			for( ::rux::uint32 index0 = 1 ; index0 < polygon_x.Count() ; index0++ )
			{
				if( polygon_x[ index0 ] > max_x )
					max_x = polygon_x[ index0 ];
				if( polygon_y[ index0 ] > max_y )
					max_y = polygon_y[ index0 ];
				if( polygon_x[ index0 ] < min_x )
					min_x = polygon_x[ index0 ];
				if( polygon_y[ index0 ] < min_y )
					min_y = polygon_y[ index0 ];
			}
		}
	};
	::rux::uint8 XMath::is_point_inside_polygon( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float x , float y )
	{
		if( polygon_x.Count() > 2
			&& polygon_x.Count() == polygon_y.Count() )
		{
			float max_x = 0.f , max_y = 0.f , min_x = 0.f , min_y = 0.f;
			polygon_max_min( polygon_x , polygon_y , max_x , max_y , min_x , min_y );
			if( x >= min_x && y >= min_y && x <= max_x && y <= max_y )
			{
				::rux::uint32 intersection_count = 0;
				for( ::rux::uint32 index0 = 0 ; index0 < polygon_x.Count() ; index0++ )
				{
					if( index0 == polygon_x.Count() - 1 )
					{
						if( is_segments_intersect( x , y , max_x + 64 , y , polygon_x[ index0 ] , polygon_y[ index0 ] , polygon_x[ 0 ] , polygon_y[ 0 ] , min_x , min_y , 0 ) == 1 )
							intersection_count++;
					}
					else
					{
						if( is_segments_intersect( x , y , max_x + 64 , y , polygon_x[ index0 ] , polygon_y[ index0 ] , polygon_x[ index0 + 1 ] , polygon_y[ index0 + 1 ] , min_x , min_y , 0 ) == 1 )
							intersection_count++;
					}
				}
				if( intersection_count > 0 )
				{
					if( intersection_count % 2 == 0 )
						return 0;
					else
						return 1;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
			return 0;
	};
	::rux::uint8 XMath::IsPointInsidePolygon( const XPoint& point , const ::rux::XArray< XPoint >& polygon , ::rux::XString& error )
	{
		error.Clear();
		if( polygon.Count() > 2 )
		{
			::rux::int64 max_x = 0;
			::rux::int64 max_y = 0;
			::rux::int64 min_x = 0xffffffff;
			::rux::int64 min_y = 0xffffffff;
			::rux::uint32 index0 = 0;
			for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
			{
				if( polygon[ index0 ]._x > max_x )
					max_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y > max_y )
					max_y = polygon[ index0 ]._y;
				if( polygon[ index0 ]._x < min_x )
					min_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y < min_y )
					min_y = polygon[ index0 ]._y;
			}
			if( point()->_x >= min_x && point()->_y >= min_y && point()->_x <= max_x && point()->_y <= max_y )
			{
				::rux::uint32 intersection_count = 0;
				XVector vector( point , XPoint( max_x + 64 , point()->_y ) );
				for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
				{
					if( index0 == polygon.Count() - 1 )
					{
						if( IsIntervalsIntersect( vector, XVector( polygon[ index0 ] , polygon[ 0 ] ) ) == 1 )
							intersection_count++;
					}
					else
					{
						if( IsIntervalsIntersect( vector, XVector( polygon[ index0 ] , polygon[ index0 + 1 ] ) ) == 1 )
							intersection_count++;
					}
				}
				if( intersection_count > 0 )
				{
					if( intersection_count % 2 == 0 )
						return 0;
					else
						return 1;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );
			return 0;
		}
	};
	::rux::uint8 XMath::polygon_bound( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float& x , float& y , float& width , float& height )
	{
		if( polygon_x.Count() > 0
			&& polygon_y.Count() == polygon_x.Count() )
		{
			x = polygon_x[ 0 ];
			y = polygon_y[ 0 ];
			width = polygon_x[ 0 ];
			height = polygon_y[ 0 ];
			for( ::rux::uint32 index0 = 1 ; index0 < polygon_x.Count() ; index0++ )
			{
				if( x > polygon_x[ index0 ] )
					x = polygon_x[ index0 ];
				if( y > polygon_y[ index0 ] )
					y = polygon_y[ index0 ];
				if( width < polygon_x[ index0 ] )
					width = polygon_x[ index0 ];
				if( height < polygon_y[ index0 ] )
					height = polygon_y[ index0 ];
			}
			width -= x;
			height -= y;
			return 1;
		}
		else
			return 0;
	};
	::rux::uint8 XMath::IsPointBelongsToInterval( const XPoint& point , const XVector& interval )
	{
		if( point()->_x == interval()->_start()->_x && point()->_y == interval()->_start()->_y )
			return 1;
		else if( point()->_x == interval()->_end()->_x && point()->_y == interval()->_end()->_y )
			return 1;
		else if( ( point()->_x > interval()->_start()->_x && point()->_x < interval()->_end()->_x )
			|| ( point()->_x < interval()->_start()->_x && point()->_x > interval()->_end()->_x ) )
		{
			if( ( ( point()->_x - interval()->_start()->_x ) * ( interval()->_end()->_y - interval()->_start()->_y ) - ( point()->_y - interval()->_start()->_y ) * ( interval()->_end()->_x - interval()->_start()->_x ) ) == 0 )
				return 1;
			else
				return 0;
		}
		else
			return 0;
	};
	float XMath::segment_length( float x1 , float y1 , float x2 , float y2 )
	{
		return sqrt( ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 ) );
	};
	::rux::uint8 XMath::is_point_belongs_to_segment( float x1 , float y1 , float x2 , float y2 , float x , float y , ::rux::uint8 precision )
	{
		x = ::rux::math::round( x , precision );
		x1 = ::rux::math::round( x1 , precision );
		x2 = ::rux::math::round( x2 , precision );
		y = ::rux::math::round( y , precision );
		y1 = ::rux::math::round( y1 , precision );
		y2 = ::rux::math::round( y2 , precision );
		if( x2 == x1 )
		{
			if( x2 == x )
			{
				if( y >= y1 && y <= y2 )
					return 1;
				else if( y >= y2 && y <= y1 )
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
		else if( y2 == y1 )
		{
			if( y2 == y )
			{
				if( x >= x1 && x <= x2 )
					return 1;
				else if( x >= x2 && x <= x1 )
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
		else
		{
			float left = .0f , right = .0f , top = .0f , bottom = .0f;
			float dx = x2 - x1;
			float dy = y2 - y1;
			float slope = dy / dx;
			float intercept = y1 - slope * x1;
			if( x1 < x2 )
			{
				left = x1;
				right = x2;
			}
			else
			{
				left = x2;
				right = x1;
			}
			if( y1 < y2 )
			{
				top = y1;
				bottom = y2;
			}
			else
			{
				top = y2;
				bottom = y1;
			}
			if( slope * x + intercept > ( y - 0.01 ) 
				&& slope * x + intercept < ( y + 0.01 ) )
			{
				if( x >= left && x <= right && 
					y >= top && y <= bottom )
					return 1;
				else
					return 0;
			}
			else
				return 0;
		}
	};
	::rux::uint8 XMath::is_lines_intersect( float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x4 , float y4 , float& x , float& y )
	{	
		float x12 = x1 - x2;
		float x34 = x3 - x4;
		float y12 = y1 - y2;
		float y34 = y3 - y4;
		float c = x12 * y34 - y12 * x34;
		if( fabs( c ) < 0.01 )
			return 0;
		else
		{			
			float a = x1 * y2 - y1 * x2;
			float b = x3 * y4 - y3 * x4;
			x = (a * x34 - b * x12) / c;
			y = (a * y34 - b * y12) / c;
			return 1;
		}
	};
	void XMath::segment_midpoint( float x1 , float y1 , float x2 , float y2 , float& x , float& y )
	{
		x = ( x1 + x2 ) / 2.f;
		y = ( y1 + y2 ) / 2.f;
	};
	::rux::uint8 XMath::is_segments_intersect( float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x4 , float y4 , float& x , float& y , ::rux::byte check_belong_to_segments , ::rux::uint8 precision )
	{
		x1 = ::rux::math::round( x1 , precision );
		x2 = ::rux::math::round( x2 , precision );
		x3 = ::rux::math::round( x3 , precision );
		x4 = ::rux::math::round( x4 , precision );
		y1 = ::rux::math::round( y1 , precision );
		y2 = ::rux::math::round( y2 , precision );
		y3 = ::rux::math::round( y3 , precision );
		y4 = ::rux::math::round( y4 , precision );
		float straight1_x = x2 - x1;
		float straight1_y = y2 - y1;
		float straight2_x = x4 - x3;
		float straight2_y = y4 - y3;
		float a1 = - straight1_y;
		float b1 = + straight1_x;
		float d1 = - ( a1 * x1 + b1 * y1 );
		float a2 = - straight2_y;
		float b2 = + straight2_x;
		float d2 = - ( a2 * x3 + b2 * y3 );
		float seg1_line2_start = a2 * x1 + b2 * y1 + d2;
		float seg1_line2_end = a2 * x2 + b2 * y2 + d2;
		float seg2_line1_start = a1 * x3 + b1 * y3 + d1;
		float seg2_line1_end = a1 * x4 + b1 * y4 + d1;
		if( seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 ) 
		{
			if( check_belong_to_segments )
			{
				if( XMath::is_point_belongs_to_segment( x3 , y3 , x4 , y4 , x1 , y1 , precision ) == 1 )
				{
					if( x1 != x3 || y1 != y3 )
					{
						if( XMath::is_point_belongs_to_segment( x2 , y2 , x3 , y3 , x1 , y1 , precision ) == 1 )
							return 0;
					}
					else if( x1 != x4 || y1 != y4 )
					{
						if( XMath::is_point_belongs_to_segment( x2 , y2 , x4 , y4 , x1 , y1 , precision ) == 1 )
							return 0;
					}
					x = x1;
					y = y1;
					return 1;
				}
				else if( XMath::is_point_belongs_to_segment( x3 , y3 , x4 , y4 , x2 , y2 , precision ) == 1 )
				{
					if( x2 != x3 || y2 != y3 )
					{
						if( XMath::is_point_belongs_to_segment( x1 , y1 , x3 , y3 , x2 , y2 , precision ) == 1 )
							return 0;
					}
					else if( x2 != x4 || y2 != y4 )
					{
						if( XMath::is_point_belongs_to_segment( x1 , y1 , x4 , y4 , x2 , y2 , precision ) == 1 )
							return 0;
					}
					x = x2;
					y = y2;
					return 1;
				}
				else if( XMath::is_point_belongs_to_segment( x1 , y1 , x2 , y2 ,  x3 , y3 , precision ) == 1 )
				{
					if( x3 != x1 || y3 != y1 )
					{
						if( XMath::is_point_belongs_to_segment( x4 , y4 , x1 , y1 , x3 , y3 , precision ) == 1 )
							return 0;
					}
					else if( x3 != x2 || y3 != y2 )
					{
						if( XMath::is_point_belongs_to_segment( x4 , y4 , x2 , y2 , x3 , y3 , precision ) == 1 )
							return 0;
					}
					x = x3;
					y = y3;
					return 1;
				}
				else if( XMath::is_point_belongs_to_segment( x1 , y1 , x2 , y2 , x4 , y4 , precision ) == 1 )
				{
					if( x4 != x1 || y4 != y1 )
					{
						if( XMath::is_point_belongs_to_segment( x3 , y3 , x1 , y1 , x4 , y4 , precision ) == 1 )
							return 0;
					}
					else if( x4 != x2 || y4 != y2 )
					{
						if( XMath::is_point_belongs_to_segment( x3 , y3 , x2 , y2 , x4 , y4 , precision ) == 1 )
							return 0;
					}
					x = x4;
					y = y4;
					return 1;
				}
			}
			return 0;
		}
		float u = seg1_line2_start / ( seg1_line2_start - seg1_line2_end );
		x = ::rux::math::round( x1 + u * straight1_x , precision );
		y = ::rux::math::round( y1 + u * straight1_y , precision );
		return 1;
	};
	::rux::uint8 XMath::IsIntervalsIntersect( const XVectorF& interval1 , const XVectorF& interval2 , XPointF& intersection )
	{
		XPointF straight1 = interval1()->_end - interval1()->_start;
		XPointF straight2 = interval2()->_end - interval2()->_start;	
		float a1 = - straight1()->_y;
		float b1 = + straight1()->_x;
		float d1 = - ( a1 * interval1()->_start()->_x + b1 * interval1()->_start()->_y );
		float a2 = - straight2()->_y;
		float b2 = + straight2()->_x;
		float d2 = - ( a2 * interval2()->_start()->_x + b2 * interval2()->_start()->_y );
		float seg1_line2_start = a2 * interval1()->_start()->_x + b2 * interval1()->_start()->_y + d2;
		float seg1_line2_end = a2 * interval1()->_end()->_x + b2 * interval1()->_end()->_y + d2;
		float seg2_line1_start = a1 * interval2()->_start()->_x + b1 * interval2()->_start()->_y + d1;
		float seg2_line1_end = a1 * interval2()->_end()->_x + b1 * interval2()->_end()->_y + d1;	
		if( seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 ) 
		{
			if( XMath::IsPointBelongsToInterval( interval1()->_start , interval2 ) == 1 )
			{
				if( interval1()->_start()->_x != interval2()->_start()->_x || interval1()->_start()->_y != interval2()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_start , XVectorF( interval1()->_end , interval2()->_start ) ) == 1 )
						return 0;
				}
				else if( interval1()->_start()->_x != interval2()->_end()->_x || interval1()->_start()->_y != interval2()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_start , XVectorF( interval1()->_end , interval2()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval1()->_start()->_x;
				intersection()->_y = interval1()->_start()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval1()->_end , interval2 ) == 1 )
			{
				if( interval1()->_end()->_x != interval2()->_start()->_x || interval1()->_end()->_y != interval2()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_end , XVectorF( interval1()->_start , interval2()->_start ) ) == 1 )
						return 0;
				}
				else if( interval1()->_end()->_x != interval2()->_end()->_x || interval1()->_end()->_y != interval2()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_end , XVectorF( interval1()->_start , interval2()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval1()->_end()->_x;
				intersection()->_y = interval1()->_end()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval2()->_start , interval1 ) == 1 )
			{
				if( interval2()->_start()->_x != interval1()->_start()->_x || interval2()->_start()->_y != interval1()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_start , XVectorF( interval2()->_end , interval1()->_start ) ) == 1 )
						return 0;
				}
				else if( interval2()->_start()->_x != interval1()->_end()->_x || interval2()->_start()->_y != interval1()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_start , XVectorF( interval2()->_end , interval1()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval2()->_start()->_x;
				intersection()->_y = interval2()->_start()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval2()->_end , interval1 ) == 1 )
			{
				if( interval2()->_end()->_x != interval1()->_start()->_x || interval2()->_end()->_y != interval1()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_end , XVectorF( interval2()->_start , interval1()->_start ) ) == 1 )
						return 0;
				}
				else if( interval2()->_end()->_x != interval1()->_end()->_x || interval2()->_end()->_y != interval1()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_end , XVectorF( interval2()->_start , interval1()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval2()->_end()->_x;
				intersection()->_y = interval2()->_end()->_y;
				return 1;
			}
			return 0;
		}
		float u = seg1_line2_start / ( seg1_line2_start - seg1_line2_end );
		intersection = interval1()->_start + u * straight1;
		return 1;
	};
	::rux::uint8 XMath::IsIntervalsIntersect( const XVectorD& interval1 , const XVectorD& interval2 , XPointD& intersection )
	{
		XPointD straight1 = interval1()->_end - interval1()->_start;
		XPointD straight2 = interval2()->_end - interval2()->_start;	
		double a1 = - straight1()->_y;
		double b1 = + straight1()->_x;
		double d1 = - ( a1 * interval1()->_start()->_x + b1 * interval1()->_start()->_y );
		double a2 = - straight2()->_y;
		double b2 = + straight2()->_x;
		double d2 = - ( a2 * interval2()->_start()->_x + b2 * interval2()->_start()->_y );
		double seg1_line2_start = a2 * interval1()->_start()->_x + b2 * interval1()->_start()->_y + d2;
		double seg1_line2_end = a2 * interval1()->_end()->_x + b2 * interval1()->_end()->_y + d2;
		double seg2_line1_start = a1 * interval2()->_start()->_x + b1 * interval2()->_start()->_y + d1;
		double seg2_line1_end = a1 * interval2()->_end()->_x + b1 * interval2()->_end()->_y + d1;	
		if( seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0 ) 
		{
			if( XMath::IsPointBelongsToInterval( interval1()->_start , interval2 ) == 1 )
			{
				if( interval1()->_start()->_x != interval2()->_start()->_x || interval1()->_start()->_y != interval2()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_start , XVectorD( interval1()->_end , interval2()->_start ) ) == 1 )
						return 0;
				}
				else if( interval1()->_start()->_x != interval2()->_end()->_x || interval1()->_start()->_y != interval2()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_start , XVectorD( interval1()->_end , interval2()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval1()->_start()->_x;
				intersection()->_y = interval1()->_start()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval1()->_end , interval2 ) == 1 )
			{
				if( interval1()->_end()->_x != interval2()->_start()->_x || interval1()->_end()->_y != interval2()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_end , XVectorD( interval1()->_start , interval2()->_start ) ) == 1 )
						return 0;
				}
				else if( interval1()->_end()->_x != interval2()->_end()->_x || interval1()->_end()->_y != interval2()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval1()->_end , XVectorD( interval1()->_start , interval2()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval1()->_end()->_x;
				intersection()->_y = interval1()->_end()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval2()->_start , interval1 ) == 1 )
			{
				if( interval2()->_start()->_x != interval1()->_start()->_x || interval2()->_start()->_y != interval1()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_start , XVectorD( interval2()->_end , interval1()->_start ) ) == 1 )
						return 0;
				}
				else if( interval2()->_start()->_x != interval1()->_end()->_x || interval2()->_start()->_y != interval1()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_start , XVectorD( interval2()->_end , interval1()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval2()->_start()->_x;
				intersection()->_y = interval2()->_start()->_y;
				return 1;
			}
			else if( XMath::IsPointBelongsToInterval( interval2()->_end , interval1 ) == 1 )
			{
				if( interval2()->_end()->_x != interval1()->_start()->_x || interval2()->_end()->_y != interval1()->_start()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_end , XVectorD( interval2()->_start , interval1()->_start ) ) == 1 )
						return 0;
				}
				else if( interval2()->_end()->_x != interval1()->_end()->_x || interval2()->_end()->_y != interval1()->_end()->_y )
				{
					if( XMath::IsPointBelongsToInterval( interval2()->_end , XVectorD( interval2()->_start , interval1()->_end ) ) == 1 )
						return 0;
				}
				intersection()->_x = interval2()->_end()->_x;
				intersection()->_y = interval2()->_end()->_y;
				return 1;
			}
			return 0;
		}
		double u = seg1_line2_start / ( seg1_line2_start - seg1_line2_end );
		intersection = interval1()->_start + u * straight1;
		return 1;
	};
	::rux::uint8 XMath::IsPointInsidePolygon( const XPointF& point , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error )
	{
		error.Clear();
		if( polygon.Count() > 2 )
		{
			float max_x = 0;
			float max_y = 0;
			float min_x = 999999.0f;
			float min_y = 999999.0f;
			::rux::uint32 index0 = 0;
			for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
			{
				if( polygon[ index0 ]._x > max_x )
					max_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y > max_y )
					max_y = polygon[ index0 ]._y;
				if( polygon[ index0 ]._x < min_x )
					min_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y < min_y )
					min_y = polygon[ index0 ]._y;
			}
			if( point()->_x >= min_x && point()->_y >= min_y && point()->_x <= max_x && point()->_y <= max_y )
			{
				XPointF intersection;
				::rux::uint32 intersection_count = 0;
				XVectorF vector( point , XPointF( max_x + 64 , point()->_y ) );
				for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
				{
					if( index0 == polygon.Count() - 1 )
					{
						if( IsIntervalsIntersect( vector, XVectorF( polygon[ index0 ] , polygon[ 0 ] ) , intersection ) == 1 )
							intersection_count++;
					}
					else
					{
						if( IsIntervalsIntersect( vector, XVectorF( polygon[ index0 ] , polygon[ index0 + 1 ] ) , intersection ) == 1 )
							intersection_count++;
					}
				}
				if( intersection_count > 0 )
				{
					if( intersection_count % 2 == 0 )
						return 0;
					else
						return 1;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );
			return 0;
		}
	};
	::rux::uint8 XMath::IsPointInsidePolygon( const XPointD& point , const ::rux::XArray< XPointD >& polygon , ::rux::XString& error )
	{
		error.Clear();
		if( polygon.Count() > 2 )
		{
			double max_x = 0;
			double max_y = 0;
			double min_x = DBL_MAX;
			double min_y = DBL_MAX;
			::rux::uint32 index0 = 0;
			for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
			{
				if( polygon[ index0 ]._x > max_x )
					max_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y > max_y )
					max_y = polygon[ index0 ]._y;
				if( polygon[ index0 ]._x < min_x )
					min_x = polygon[ index0 ]._x;
				if( polygon[ index0 ]._y < min_y )
					min_y = polygon[ index0 ]._y;
			}
			if( point()->_x >= min_x && point()->_y >= min_y && point()->_x <= max_x && point()->_y <= max_y )
			{
				XPointD intersection;
				::rux::uint32 intersection_count = 0;
				XVectorD vector( point , XPointD( max_x + 64 , point()->_y ) );
				for( index0 = 0 ; index0 < polygon.Count() ; index0++ )
				{
					if( index0 == polygon.Count() - 1 )
					{
						if( IsIntervalsIntersect( vector, XVectorD( polygon[ index0 ] , polygon[ 0 ] ) , intersection ) == 1 )
							intersection_count++;
					}
					else
					{
						if( IsIntervalsIntersect( vector, XVectorD( polygon[ index0 ] , polygon[ index0 + 1 ] ) , intersection ) == 1 )
							intersection_count++;
					}
				}
				if( intersection_count > 0 )
				{
					if( intersection_count % 2 == 0 )
						return 0;
					else
						return 1;
				}
				else
					return 0;
			}
			else
				return 0;
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );
			return 0;
		}
	};
	::rux::uint8 XMath::IsPointBelongsToInterval( const XPointF& point , const XVectorF& interval )
	{
		if( point()->_x == interval()->_start()->_x && point()->_y == interval()->_start()->_y )
			return 1;
		else if( point()->_x == interval()->_end()->_x && point()->_y == interval()->_end()->_y )
			return 1;
		else if( ( point()->_x > interval()->_start()->_x && point()->_x < interval()->_end()->_x )
			|| ( point()->_x < interval()->_start()->_x && point()->_x > interval()->_end()->_x ) )
		{
			if( ( ( point()->_x - interval()->_start()->_x ) * ( interval()->_end()->_y - interval()->_start()->_y ) - ( point()->_y - interval()->_start()->_y ) * ( interval()->_end()->_x - interval()->_start()->_x ) ) == 0 )
				return 1;
			else
				return 0;
		}
		else
			return 0;
	};
	::rux::uint8 XMath::IsPointBelongsToInterval( const XPointD& point , const XVectorD& interval )
	{
		if( point()->_x == interval()->_start()->_x && point()->_y == interval()->_start()->_y )
			return 1;
		else if( point()->_x == interval()->_end()->_x && point()->_y == interval()->_end()->_y )
			return 1;
		else if( ( point()->_x > interval()->_start()->_x && point()->_x < interval()->_end()->_x )
			|| ( point()->_x < interval()->_start()->_x && point()->_x > interval()->_end()->_x ) )
		{
			if( ( ( point()->_x - interval()->_start()->_x ) * ( interval()->_end()->_y - interval()->_start()->_y ) - ( point()->_y - interval()->_start()->_y ) * ( interval()->_end()->_x - interval()->_start()->_x ) ) == 0 )
				return 1;
			else
				return 0;
		}
		else
			return 0;
	};
	::rux::uint32 XMath::get_PolygonIntersectionPointsCount( const XVectorF& vector , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error )
	{
		::rux::uint32 intersection_points_count = 0;
		error.Clear();
		if( polygon.Count() > 2 )
		{
			::rux::uint32 index0 = 1;
			XPointF intersection;
			for( ; index0 < polygon.Count() ; index0++ )
			{
				if( ( vector()->_start()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_start()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_end()->_x == polygon[ index0 ]._x
					&& vector()->_end()->_y == polygon[ index0 ]._y )
					|| ( vector()->_end()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_end()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_start()->_x == polygon[ index0 ]._x
					&& vector()->_start()->_y == polygon[ index0 ]._y ) )
					continue;
				if( IsIntervalsIntersect( vector , XVectorF( polygon[ index0 - 1 ] , polygon[ index0 ] ) , intersection ) == 1 )
				{
					if( ( intersection()->_x == vector()->_start()->_x && intersection()->_y == vector()->_start()->_y )
						|| ( intersection()->_x == vector()->_end()->_x && intersection()->_y == vector()->_end()->_y ) )
						continue;
					intersection_points_count++;
				}
			}
			if( IsIntervalsIntersect( vector , XVectorF( polygon[ polygon.Count() - 1 ] , polygon[ 0 ] ) , intersection ) == 1 )
			{
				if( !( ( intersection()->_x == vector()->_start()->_x && intersection()->_y == vector()->_start()->_y )
					|| ( intersection()->_x == vector()->_end()->_x && intersection()->_y == vector()->_end()->_y ) ) )
					intersection_points_count++;
			}
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );		
		}
		return intersection_points_count;
	};
	::rux::uint32 XMath::get_PolygonIntersectionPointsCount( const XVectorD& vector , const ::rux::XArray< XPointD >& polygon , ::rux::XString& error )
	{
		::rux::uint32 intersection_points_count = 0;
		error.Clear();
		if( polygon.Count() > 2 )
		{
			::rux::uint32 index0 = 1;
			XPointD intersection;
			for( ; index0 < polygon.Count() ; index0++ )
			{
				if( ( vector()->_start()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_start()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_end()->_x == polygon[ index0 ]._x
					&& vector()->_end()->_y == polygon[ index0 ]._y )
					|| ( vector()->_end()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_end()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_start()->_x == polygon[ index0 ]._x
					&& vector()->_start()->_y == polygon[ index0 ]._y ) )
					continue;
				if( IsIntervalsIntersect( vector , XVectorD( polygon[ index0 - 1 ] , polygon[ index0 ] ) , intersection ) == 1 )
				{
					if( ( intersection()->_x == vector()->_start()->_x && intersection()->_y == vector()->_start()->_y )
						|| ( intersection()->_x == vector()->_end()->_x && intersection()->_y == vector()->_end()->_y ) )
						continue;
					intersection_points_count++;
				}
			}
			if( IsIntervalsIntersect( vector , XVectorD( polygon[ polygon.Count() - 1 ] , polygon[ 0 ] ) , intersection ) == 1 )
			{
				if( !( ( intersection()->_x == vector()->_start()->_x && intersection()->_y == vector()->_start()->_y )
					|| ( intersection()->_x == vector()->_end()->_x && intersection()->_y == vector()->_end()->_y ) ) )
					intersection_points_count++;
			}
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );		
		}
		return intersection_points_count;
	};
	Array< XPointF >& XMath::get_PolygonIntersectionPoints( const XVectorF& vector , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error )
	{
		XArray< XPointF > intersection_points;
		error.Clear();
		if( polygon.Count() > 2 )
		{
			::rux::uint32 index0 = 1 , index1 = 0;	
			::rux::uint8 min_type = 0;
			float min = 0.0f;
			if( vector()->_start()->_x == vector()->_end()->_x )
			{
				if( vector()->_start()->_y < vector()->_end()->_y )
				{
					min_type = 1;
					min = vector()->_end()->_y;
				}
				else
				{
					min_type = 2;
					min = vector()->_end()->_y;
				}
			}
			else if( vector()->_start()->_x < vector()->_end()->_x )
			{
				min_type = 3;
				min = vector()->_end()->_x;
			}
			else
			{
				min_type = 4;
				min = vector()->_end()->_x;
			}		
			for( ; index0 < polygon.Count() ; index0++ )
			{
				if( ( vector()->_start()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_start()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_end()->_x == polygon[ index0 ]._x
					&& vector()->_end()->_y == polygon[ index0 ]._y )
					|| ( vector()->_end()->_x == polygon[ index0 - 1 ]._x
					&& vector()->_end()->_y == polygon[ index0 - 1 ]._y
					&& vector()->_start()->_x == polygon[ index0 ]._x
					&& vector()->_start()->_y == polygon[ index0 ]._y ) )
					continue;
				XPointF intersection;
				if( IsIntervalsIntersect( vector , XVectorF( polygon[ index0 - 1 ] , polygon[ index0 ] ) , intersection ) == 1 )
				{
					if( ( intersection()->_x == vector()->_start()->_x && intersection()->_y == vector()->_start()->_y )
						|| ( intersection()->_x == vector()->_end()->_x && intersection()->_y == vector()->_end()->_y ) )
						continue;
					if( min_type == 1 )
					{
						if( min > intersection()->_y )
						{
							intersection_points.Insert( 0 , intersection );
							min = intersection()->_y;
						}
						else
						{
							for( index1 = intersection_points.Count() - 1 ; index1 > 0 ; index1-- )
							{
								if( intersection_points[ index1 ]._y < intersection()->_y )
									break;
							}
							intersection_points.Insert( index1 + 1 , intersection );
						}
					}
					else if( min_type == 2 )
					{
						if( min < intersection()->_y )
						{
							intersection_points.Insert( 0 , intersection );
							min = intersection()->_y;
						}
						else
						{
							for( index1 = intersection_points.Count() - 1 ; index1 > 0 ; index1-- )
							{
								if( intersection_points[ index1 ]._y > intersection()->_y )
									break;
							}
							intersection_points.Insert( index1 + 1 , intersection );
						}
					}
					else if( min_type == 3 )
					{
						if( min > intersection()->_x )
						{
							intersection_points.Insert( 0 , intersection );
							min = intersection()->_x;
						}
						else
						{
							for( index1 = intersection_points.Count() - 1 ; index1 > 0 ; index1-- )
							{
								if( intersection_points[ index1 ]._x < intersection()->_x )
									break;
							}
							intersection_points.Insert( index1 + 1 , intersection );
						}
					}
					else if( min_type == 4 )
					{
						if( min < intersection()->_y )
						{
							intersection_points.Insert( 0 , intersection );
							min = intersection()->_y;
						}
						else
						{
							for( index1 = intersection_points.Count() - 1 ; index1 > 0 ; index1-- )
							{
								if( intersection_points[ index1 ]._x > intersection()->_x )
									break;
							}
							intersection_points.Insert( index1 + 1 , intersection );
						}
					}				
				}
			}		
		}
		else
		{
			rux_write_error( "polygon has not ehough points" , error );		
		}
		return intersection_points++;
	};
	RectangleF& XMath::CalculateRect( const XRectangleF& rectangle , float x_transform , float y_transform )
	{
		float rounded_x_transform = ::rux::math::round( x_transform , 2 );
		float rounded_y_transform = ::rux::math::round( y_transform , 2 );
		XRectangleF calculated_rectangle;
		if( rounded_x_transform == rounded_y_transform )
		{
			calculated_rectangle()->_point()->_x = rectangle()->_point()->_x;
			calculated_rectangle()->_point()->_y = rectangle()->_point()->_y;
			calculated_rectangle()->_width = rectangle()->_width;
			calculated_rectangle()->_height = rectangle()->_height;
		}
		else
		{
			float x = rectangle()->_point()->_x;
			float y = rectangle()->_point()->_y;
			float width = rectangle()->_width;
			float height = rectangle()->_height;
			if( x >= 0 && y >= 0 && width > 0 && height > 0 && rounded_x_transform > 0 && rounded_y_transform > 0 )
			{
				float one_part_width = ::rux::math::round( width / rounded_x_transform , 2 );
				float one_part_height = ::rux::math::round( height / rounded_y_transform , 2 );
				float new_width = width;
				float new_x = x;
				float new_height = ::rux::math::round( one_part_width * rounded_y_transform , 2 );
				float new_y = ::rux::math::round( y + ( height - new_height ) / 2.0f , 2 );
				if( new_height > height )
				{
					new_height = height;
					new_width = ::rux::math::round( one_part_height * rounded_x_transform , 2 );
					new_x = ::rux::math::round( x + ( width - new_width ) / 2.0f , 2 );
					new_y = y;
				}
				calculated_rectangle()->_point()->_x = new_x;
				calculated_rectangle()->_point()->_y = new_y;
				calculated_rectangle()->_width = new_width;
				calculated_rectangle()->_height = new_height;
			}
		}
		return calculated_rectangle++;
	};
	RectangleD& XMath::CalculateRect( const XRectangleD& rectangle , double x_transform , double y_transform )
	{
		XRectangleD calculated_rectangle;
		double rounded_x_transform = ::rux::math::round( x_transform , 2 );
		double rounded_y_transform = ::rux::math::round( y_transform , 2 );
		if( rounded_x_transform == rounded_y_transform )
		{
			calculated_rectangle()->_point()->_x = rectangle()->_point()->_x;
			calculated_rectangle()->_point()->_y = rectangle()->_point()->_y;
			calculated_rectangle()->_width = rectangle()->_width;
			calculated_rectangle()->_height = rectangle()->_height;
		}
		else
		{
			double x = rectangle()->_point()->_x;
			double y = rectangle()->_point()->_y;
			double width = rectangle()->_width;
			double height = rectangle()->_height;
			if( x >= 0 && y >= 0 && width > 0 && height > 0 && rounded_x_transform > 0 && rounded_y_transform > 0 )
			{
				double one_part_width = ::rux::math::round( width / rounded_x_transform , 2 );
				double one_part_height = ::rux::math::round( height / rounded_y_transform , 2 );
				double new_width = width;
				double new_x = x;
				double new_height = ::rux::math::round( one_part_width * rounded_y_transform , 2 );
				double new_y = ::rux::math::round( y + ( height - new_height ) / 2.0f , 2 );
				if( new_height > height )
				{
					new_height = height;
					new_width = ::rux::math::round( one_part_height * rounded_x_transform , 2 );
					new_x = ::rux::math::round( x + ( width - new_width ) / 2.0f , 2 );
					new_y = y;
				}
				calculated_rectangle()->_point()->_x = new_x;
				calculated_rectangle()->_point()->_y = new_y;
				calculated_rectangle()->_width = new_width;
				calculated_rectangle()->_height = new_height;
			}
		}
		return calculated_rectangle++;
	};
	RectangleF& XMath::GridPositionToRect( const XGridPosition& grid_position , const XRectangleF& rectangle )
	{
		XRectangleF calculated_rectangle;
		::rux::uint32 rows = grid_position()->_rows;
		::rux::uint32 cols = grid_position()->_cols;
		::rux::uint32 x = grid_position()->_x;
		::rux::uint32 y = grid_position()->_y;
		::rux::uint32 dx = grid_position()->_dx;
		::rux::uint32 dy = grid_position()->_dy;
		float rectangle_x = rectangle()->_point()->_x;
		float rectangle_y = rectangle()->_point()->_y;
		float width = rectangle()->_width;
		float height = rectangle()->_height;
		if( width > 0 && height > 0 )
		{
			if( cols > 0 && rows > 0 && x > 0 && y > 0 && dx > 0 && dy > 0 )
			{
				width = ::rux::math::round( width / (float)cols , 2 );
				height = ::rux::math::round( height / (float)rows , 2 );
				rectangle_x = ::rux::math::round( rectangle_x + ( x - 1 ) * width , 2 );
				rectangle_y = ::rux::math::round( rectangle_y + ( y - 1 ) * height , 2 );
				width = ::rux::math::round( dx * width , 2 );
				height = ::rux::math::round( dy * height , 2 );
				calculated_rectangle()->_point()->_x = rectangle_x;
				calculated_rectangle()->_point()->_y = rectangle_y;
				calculated_rectangle()->_width = width;
				calculated_rectangle()->_height = height;
			}
		}		
		return calculated_rectangle++;
	};
	RectangleD& XMath::GridPositionToRect( const XGridPosition& grid_position , const XRectangleD& rectangle )
	{
		XRectangleD calculated_rectangle;
		::rux::uint32 rows = grid_position()->_rows;
		::rux::uint32 cols = grid_position()->_cols;
		::rux::uint32 x = grid_position()->_x;
		::rux::uint32 y = grid_position()->_y;
		::rux::uint32 dx = grid_position()->_dx;
		::rux::uint32 dy = grid_position()->_dy;
		double rectangle_x = rectangle()->_point()->_x;
		double rectangle_y = rectangle()->_point()->_y;
		double width = rectangle()->_width;
		double height = rectangle()->_height;
		if( rectangle_x >= 0 && rectangle_y >= 0	&& width > 0 	&& height > 0 )
		{
			if( cols > 0 && rows > 0 && x > 0 && y > 0 && dx > 0 && dy > 0 )
			{
				width = ::rux::math::round( width / (double)cols , 2 );
				height = ::rux::math::round( height / (double)rows , 2 );
				rectangle_x = ::rux::math::round( rectangle_x + ( x - 1 ) * width , 2 );
				rectangle_y = ::rux::math::round( rectangle_y + ( y - 1 ) * height , 2 );
				width = ::rux::math::round( dx * width , 2 );
				height = ::rux::math::round( dy * height , 2 );
				calculated_rectangle()->_point()->_x = rectangle_x;
				calculated_rectangle()->_point()->_y = rectangle_y;
				calculated_rectangle()->_width = width;
				calculated_rectangle()->_height = height;
			}
		}		
		return calculated_rectangle++;
	};
};