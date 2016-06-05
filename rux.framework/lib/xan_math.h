#ifndef XAN_XMATH_H
#define XAN_XMATH_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_string.h>
namespace rux
{
	class math
	{
	public:
		static ::rux::int32 gcd( ::rux::int32 number0 , ::rux::int32 number1 );
		static ::rux::uint32 gcd( ::rux::uint32 number0 , ::rux::uint32 number1 );
		static ::rux::int64 gcd( ::rux::int64 number0 , ::rux::int64 number1 );
		static ::rux::uint64 gcd( ::rux::uint64 number0 , ::rux::uint64 number1 );
		static void calculate_third_point_of_right_triangle( float x1 , float y1 , float x2 , float y2 , float cathetus , float& x3 , float& y3 , float& x4 , float& y4 ); 
		static ::rux::int32 quadratic_equation( float a , float b , float c , float& x1 , float& x2 );
		static ::rux::uint8 is_point_belongs_to_rectangle( float x0 , float y0 , float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x , float y , ::rux::uint8 precision = 2 );
		static float round( float number , ::rux::uint8 precision );
		static double round( double number , ::rux::uint8 precision );
	};
	class Point;
	class XPoint : public XGCHandle<Point>
	{	
		declare_rux_base_methods( Point );
	private:	
	public:
		declare_rux_set_operators( Point );
		XPoint( ::rux::int64 x , ::rux::int64 y );
		XPoint();
		XPoint( const Point& point );
		XPoint( const XPoint& point );
		explicit XPoint( const XObject& object );
		friend Point& operator +( const XPoint& point1, const XPoint& point2 );	
		friend Point& operator +( const XPoint& point1, const Point& point2 );	
		friend Point& operator +( const Point& point1, const XPoint& point2 );	
		friend Point& operator +( const Point& point1, const Point& point2 );		
		friend Point& operator -( const XPoint& point1, const XPoint& point2 );	
		friend Point& operator -( const XPoint& point1, const Point& point2 );	
		friend Point& operator -( const Point& point1, const XPoint& point2 );	
		friend Point& operator -( const Point& point1, const Point& point2 );		
		::rux::int64 get_X( void );
		::rux::int64 get_Y( void );
	};
	class Point : public XGCRef
	{
		friend class XPoint;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		::rux::int64 _x;
		::rux::int64 _y;
	public:
		explicit Point( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend Point& operator +( const XPoint& point1, const XPoint& point2 );	
		friend Point& operator +( const XPoint& point1, const Point& point2 );	
		friend Point& operator +( const Point& point1, const XPoint& point2 );	
		friend Point& operator +( const Point& point1, const Point& point2 );		
		friend Point& operator -( const XPoint& point1, const XPoint& point2 );	
		friend Point& operator -( const XPoint& point1, const Point& point2 );	
		friend Point& operator -( const Point& point1, const XPoint& point2 );	
		friend Point& operator -( const Point& point1, const Point& point2 );		
	};
	class PointF;
	class XPointF : public XGCHandle<PointF>
	{	
		declare_rux_base_methods( PointF );
	private:	
	public:
		declare_rux_set_operators( PointF );
		XPointF( float x , float y , ::rux::uint8 round_values = 1 );
		XPointF();
		XPointF( const PointF& point );
		XPointF( const XPointF& point );
		explicit XPointF( const XObject& object );
		friend PointF& operator *( float coef, const XPointF& point2 );	
		friend PointF& operator +( const XPointF& point1, const XPointF& point2 );	
		friend PointF& operator +( const XPointF& point1, const PointF& point2 );	
		friend PointF& operator +( const PointF& point1, const XPointF& point2 );	
		friend PointF& operator +( const PointF& point1, const PointF& point2 );		
		friend PointF& operator -( const XPointF& point1, const XPointF& point2 );	
		friend PointF& operator -( const XPointF& point1, const PointF& point2 );	
		friend PointF& operator -( const PointF& point1, const XPointF& point2 );	
		friend PointF& operator -( const PointF& point1, const PointF& point2 );		
		float get_X( void );
		float get_Y( void );
		void set_X( float x );
		void set_Y( float y );
		friend bool operator ==( const XPointF& point1, const XPointF& point2 );
		friend bool operator !=( const PointF& point1, const PointF& point2 );
	};
	class XVectorF;
	class XTriangleF;
	class XRectangleF;
	class PointF : public XGCRef
	{
		friend class XTriangleF;
		friend class XVectorF;
		friend class XPointF;
		friend class XMath;
		friend class XRectangleF;
		declare_rux_base_internal_methods();
	private:
		float _x;
		float _y;
	public:
		explicit PointF( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend PointF& operator *( float coef, const XPointF& point2 );	
		friend PointF& operator +( const XPointF& point1, const XPointF& point2 );	
		friend PointF& operator +( const XPointF& point1, const PointF& point2 );	
		friend PointF& operator +( const PointF& point1, const XPointF& point2 );	
		friend PointF& operator +( const PointF& point1, const PointF& point2 );		
		friend PointF& operator -( const XPointF& point1, const XPointF& point2 );	
		friend PointF& operator -( const XPointF& point1, const PointF& point2 );	
		friend PointF& operator -( const PointF& point1, const XPointF& point2 );	
		friend PointF& operator -( const PointF& point1, const PointF& point2 );		
		friend bool operator ==( const XVectorF& vector1, const XVectorF& vector2 );
		friend bool operator ==( const XTriangleF& triangle1, const XTriangleF& triangle2 );
		friend bool operator ==( const XPointF& point1, const XPointF& point2 );
		friend bool operator !=( const PointF& point1, const PointF& point2 );
		friend bool operator ==( const XRectangleF& rectangle1, const XRectangleF& rectangle2 );
		float get_X( void );
		float get_Y( void );
		void set_X( float x );
		void set_Y( float y );
	};
	class PointD;
	class XPointD : public XGCHandle<PointD>
	{	
		declare_rux_base_methods( PointD );
	private:	
	public:
		declare_rux_set_operators( PointD );
		XPointD( double x , double y , ::rux::uint8 round_values = 1 );
		XPointD();
		XPointD( const PointD& point );
		XPointD( const XPointD& point );
		explicit XPointD( const XObject& object );
		friend PointD& operator *( double coef, const XPointD& point2 );	
		friend PointD& operator +( const XPointD& point1, const XPointD& point2 );	
		friend PointD& operator +( const XPointD& point1, const PointD& point2 );	
		friend PointD& operator +( const PointD& point1, const XPointD& point2 );	
		friend PointD& operator +( const PointD& point1, const PointD& point2 );		
		friend PointD& operator -( const XPointD& point1, const XPointD& point2 );	
		friend PointD& operator -( const XPointD& point1, const PointD& point2 );	
		friend PointD& operator -( const PointD& point1, const XPointD& point2 );	
		friend PointD& operator -( const PointD& point1, const PointD& point2 );		
		double get_X( void );
		double get_Y( void );
		void set_X( double x );
		void set_Y( double y );
		friend bool operator ==( const XPointD& point1, const XPointD& point2 );
		friend bool operator !=( const PointD& point1, const PointD& point2 );
	};
	class XVectorD;
	class XTriangleD;
	class XRectangleD;
	class PointD : public XGCRef
	{
		friend class XTriangleD;
		friend class XVectorD;
		friend class XPointD;
		friend class XMath;
		friend class XRectangleD;
		declare_rux_base_internal_methods();
	private:
		double _x;
		double _y;
	public:
		explicit PointD( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend PointD& operator *( double coef, const XPointD& point2 );	
		friend PointD& operator +( const XPointD& point1, const XPointD& point2 );	
		friend PointD& operator +( const XPointD& point1, const PointD& point2 );	
		friend PointD& operator +( const PointD& point1, const XPointD& point2 );	
		friend PointD& operator +( const PointD& point1, const PointD& point2 );		
		friend PointD& operator -( const XPointD& point1, const XPointD& point2 );	
		friend PointD& operator -( const XPointD& point1, const PointD& point2 );	
		friend PointD& operator -( const PointD& point1, const XPointD& point2 );	
		friend PointD& operator -( const PointD& point1, const PointD& point2 );		
		friend bool operator ==( const XVectorD& vector1, const XVectorD& vector2 );
		friend bool operator ==( const XTriangleD& triangle1, const XTriangleD& triangle2 );
		friend bool operator ==( const XPointD& point1, const XPointD& point2 );
		friend bool operator !=( const PointD& point1, const PointD& point2 );
		friend bool operator ==( const XRectangleD& rectangle1, const XRectangleD& rectangle2 );			
	};
	class Vector : public XGCRef
	{
		friend class XVector;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPoint _start;
		XPoint _end;
	public:
		explicit Vector( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	};
	class XVector : public XGCHandle<Vector>
	{	
		declare_rux_base_methods( Vector );
	private:	
	public:
		declare_rux_set_operators( Vector );
		XVector( const XPoint& start , const XPoint& end );
		XVector();
		XVector( const Vector& vector );
		XVector( const XVector& vector );
		explicit XVector( const XObject& object );
		Point& get_Start( void );
		Point& get_End( void );
	};
	class VectorF;
	class XVectorF : public XGCHandle<VectorF >
	{	
		declare_rux_base_methods( VectorF );
	private:	
	public:
		declare_rux_set_operators( VectorF );
		XVectorF( const XPointF& start , const XPointF& end );
		XVectorF();
		XVectorF( const VectorF& vector );
		XVectorF( const XVectorF& vector );
		explicit XVectorF( const XObject& object );
		friend bool operator ==( const XVectorF& vector1, const XVectorF& vector2 );			
		PointF& get_Start( void );
		PointF& get_End( void );
		void set_Start( const XPointF& start );
		void set_End( const XPointF& end );
	};
	class VectorF : public XGCRef
	{
		friend class XVectorF;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointF _start;
		XPointF _end;
	public:
		explicit VectorF( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XVectorF& vector1, const XVectorF& vector2 );
	};
	class VectorD;
	class XVectorD : public XGCHandle<VectorD >
	{	
		declare_rux_base_methods( VectorD );
	private:	
	public:
		declare_rux_set_operators( VectorD );
		XVectorD( const XPointD& start , const XPointD& end );
		XVectorD();
		XVectorD( const VectorD& vector );
		XVectorD( const XVectorD& vector );
		explicit XVectorD( const XObject& object );
		friend bool operator ==( const XVectorD& vector1, const XVectorD& vector2 );			
		PointD& get_Start( void );
		PointD& get_End( void );
		void set_Start( const XPointD& start );
		void set_End( const XPointD& end );
	};
	class VectorD : public XGCRef
	{
		friend class XVectorD;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointD _start;
		XPointD _end;
	public:
		explicit VectorD( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XVectorD& vector1, const XVectorD& vector2 );
	};
	class TriangleF;
	class XTriangleF : public XGCHandle<TriangleF >
	{	
		declare_rux_base_methods( TriangleF );
	private:	
	public:
		declare_rux_set_operators( TriangleF );
		XTriangleF( const XPointF& point1 , const XPointF& point2 , const XPointF& point3 );
		XTriangleF();
		XTriangleF( const TriangleF& vector );
		XTriangleF( const XTriangleF& vector );
		explicit XTriangleF( const XObject& object );
		friend bool operator ==( const XTriangleF& vector1, const XTriangleF& vector2 );	
		void set_Point1( const XPointF& point1 );
		void set_Point2( const XPointF& point2 );
		void set_Point3( const XPointF& point3 );
		PointF& get_Point1( void );
		PointF& get_Point2( void );
		PointF& get_Point3( void );
		::rux::uint8 get_IsLine( void );
	};
	class TriangleF : public XGCRef
	{
		friend class XTriangleF;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointF _point1;
		XPointF _point2;
		XPointF _point3;
	public:
		explicit TriangleF( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XTriangleF& vector1, const XTriangleF& vector2 );
	};
	class TriangleD;
	class XTriangleD : public XGCHandle<TriangleD >
	{	
		declare_rux_base_methods( TriangleD );
	private:	
	public:
		declare_rux_set_operators( TriangleD );
		XTriangleD( const XPointD& point1 , const XPointD& point2 , const XPointD& point3 );
		XTriangleD();
		XTriangleD( const TriangleD& vector );
		XTriangleD( const XTriangleD& vector );
		explicit XTriangleD( const XObject& object );
		friend bool operator ==( const XTriangleD& vector1, const XTriangleD& vector2 );	
		void set_Point1( const XPointD& point1 );
		void set_Point2( const XPointD& point2 );
		void set_Point3( const XPointD& point3 );
		PointD& get_Point1( void );
		PointD& get_Point2( void );
		PointD& get_Point3( void );
		::rux::uint8 get_IsLine( void );
	};
	class TriangleD : public XGCRef
	{
		friend class XTriangleD;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointD _point1;
		XPointD _point2;
		XPointD _point3;
	public:
		explicit TriangleD( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XTriangleD& vector1, const XTriangleD& vector2 );
	};
	class RectangleF;
	class XRectangleF : public XGCHandle<RectangleF >
	{	
		declare_rux_base_methods( RectangleF );
	private:	
	public:
		declare_rux_set_operators( RectangleF );
		XRectangleF( const XPointF& point1 , float width , float height , ::rux::uint8 round_values = 1 );
		XRectangleF();
		XRectangleF( const RectangleF& rectangle );
		XRectangleF( const XRectangleF& rectangle );
		explicit XRectangleF( const XObject& object );
		friend bool operator ==( const XRectangleF& rectangle1 , const XRectangleF& rectangle2 );			
		PointF& get_Point( void );
		float get_Width( void );
		float get_Height( void );
		::rux::uint8 IsPointInside( float x , float y );
	};
	class RectangleF : public XGCRef
	{
		friend class XRectangleF;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointF _point;
		float _width;
		float _height;
	public:
		explicit RectangleF( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XRectangleF& rectangle1, const XRectangleF& rectangle2 );
		PointF& get_Point( void );
		float get_Width( void );
		float get_Height( void );
	};
	class RectangleD;
	class XRectangleD : public XGCHandle<RectangleD >
	{	
		declare_rux_base_methods( RectangleD );
	private:	
	public:
		declare_rux_set_operators( RectangleD );
		XRectangleD( const XPointD& point1 , double width , double height , ::rux::uint8 round_values = 1 );
		XRectangleD();
		XRectangleD( const RectangleD& rectangle );
		XRectangleD( const XRectangleD& rectangle );
		explicit XRectangleD( const XObject& object );
		friend bool operator ==( const XRectangleD& rectangle1, const XRectangleD& rectangle2 );			
		PointD& get_Point( void );
		double get_Width( void );
		double get_Height( void );
	};
	class RectangleD : public XGCRef
	{
		friend class XRectangleD;
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
		XPointD _point;
		double _width;
		double _height;
	public:
		explicit RectangleD( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
		friend bool operator ==( const XRectangleD& rectangle1, const XRectangleD& rectangle2 );
	};
	class GridPosition;
	class XGridPosition : public XGCHandle<GridPosition >
	{	
		declare_rux_base_methods( GridPosition );
	private:	
	public:
		declare_rux_set_operators( GridPosition );
		XGridPosition( ::rux::uint32 cols , ::rux::uint32 rows , ::rux::uint32 x , ::rux::uint32 y , ::rux::uint32 dx , ::rux::uint32 dy );
		XGridPosition();
		XGridPosition( const GridPosition& grid_position );
		XGridPosition( const XGridPosition& grid_position );
		explicit XGridPosition( const XObject& object );
	};
	class GridPosition : public XGCRef
	{
		friend class XGridPosition;
		friend class XMath;
		declare_rux_base_internal_methods();
	public:
		::rux::uint32 _cols;
		::rux::uint32 _rows;
		::rux::uint32 _x;
		::rux::uint32 _y;
		::rux::uint32 _dx;
		::rux::uint32 _dy;
	public:
		explicit GridPosition( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	};
	class Math : public XGCRef
	{
		friend class XMath;
		declare_rux_base_internal_methods();
	private:
	public:
		explicit Math( const char* variable_name = 0 , const char* __file__ = 0 , ::rux::int32 __line__ = 0 );
	};
	class XMath : public XGCHandle<Math>
	{
		declare_rux_base_methods( Math );
	private:
	public:
		declare_rux_set_operators( Math );
		XMath();
		XMath( const Math& math );
		XMath( const XMath& math );
		explicit XMath( const XObject& object );
		::rux::uint32 CyclicLeftShift( ::rux::uint32 number , ::rux::uint32 bits );
		::rux::uint32 CyclicRightShift( ::rux::uint32 number , ::rux::uint32 bits );
		static float segment_length( float x1 , float y1 , float x2 , float y2 ); 
		static void segment_midpoint( float x1 , float y1 , float x2 , float y2 , float& x , float& y );
		static ::rux::uint8 is_lines_intersect( float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x4 , float y4 , float& x , float& y );
		static ::rux::uint8 is_segments_intersect( float x1 , float y1 , float x2 , float y2 , float x3 , float y3 , float x4 , float y4 , float& x , float& y , ::rux::byte check_belong_to_segments = 1 , ::rux::uint8 precision = 2 );
		static void polygon_max_min( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float& max_x , float& max_y , float& min_x , float& min_y );
		static ::rux::uint8 is_point_inside_polygon( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float x , float y );
		static ::rux::uint8 polygon_bound( XMallocArray< float >& polygon_x , XMallocArray< float >& polygon_y , float& x , float& y , float& width , float& height );
		static ::rux::uint8 is_point_belongs_to_segment( float x1 , float y1 , float x2 , float y2 , float x , float y , ::rux::uint8 precision = 2 );
		static ::rux::uint8 IsIntervalsIntersect( const XVector& segment1 , const XVector& segment2 );
		static ::rux::uint8 IsPointInsidePolygon( const XPoint& point , const ::rux::XArray< XPoint >& polygon , ::rux::XString& error );
		static ::rux::uint8 IsPointBelongsToInterval( const XPoint& point , const XVector& interval );
		static ::rux::uint8 IsIntervalsIntersect( const XVectorF& segment1 , const XVectorF& segment2 , XPointF& intersection );
		static ::rux::uint8 IsIntervalsIntersect( const XVectorD& segment1 , const XVectorD& segment2 , XPointD& intersection );
		static ::rux::uint8 IsPointInsidePolygon( const XPointF& point , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error );
		static ::rux::uint8 IsPointInsidePolygon( const XPointD& point , const ::rux::XArray< XPointD >& polygon , ::rux::XString& error );
		static ::rux::uint8 IsPointBelongsToInterval( const XPointF& point , const XVectorF& interval );
		static ::rux::uint8 IsPointBelongsToInterval( const XPointD& point , const XVectorD& interval );
		static ::rux::Array< XPointF >& get_PolygonIntersectionPoints( const XVectorF& vector , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error );
		static ::rux::uint32 get_PolygonIntersectionPointsCount( const XVectorF& vector , const ::rux::XArray< XPointF >& polygon , ::rux::XString& error );
		static ::rux::uint32 get_PolygonIntersectionPointsCount( const XVectorD& vector , const ::rux::XArray< XPointD >& polygon , ::rux::XString& error );
		static RectangleF& CalculateRect( const XRectangleF& rectangle , float x_transform , float y_transform );
		static RectangleD& CalculateRect( const XRectangleD& rectangle , double x_transform , double y_transform );
		static RectangleF& GridPositionToRect( const XGridPosition& grdi_position , const XRectangleF& rectangle );
		static RectangleD& GridPositionToRect( const XGridPosition& grdi_position , const XRectangleD& rectangle );
	};
};
#endif
