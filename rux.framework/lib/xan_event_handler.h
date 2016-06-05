#ifndef XAN_EVENT_HANDLER_H
#define XAN_EVENT_HANDLER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_malloc_array.h>
#include <xan_crtsect.h>
namespace rux
{
	template< class function_typedef >
	class EventHandler
	{	
	private:
		::rux::threading::RdWrLock _cs_functions;
		XMallocArray< function_typedef , 1 > _functions;
	public:
		void add( function_typedef function )
		{
			if( function )
			{
				_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
				_functions.FillEmptyOrAdd( function );
				_cs_functions.WriteUnlock();
			}
		};
		EventHandler& operator +=( function_typedef function )
		{
			add( function );
			return *this;
		};
		void remove( function_typedef function )
		{
			if( function )
			{
				_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
				for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
				{
					if( _functions[ index0 ] == function )
					{
						_functions.set_EmptyAt( index0 );
						break;
					}
				}
				_cs_functions.WriteUnlock();
			}
		};
		EventHandler& operator -=( function_typedef function )
		{
			remove( function );
			return *this;
		};
		void clear( void )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			_functions.Clear();
			_cs_functions.WriteUnlock();
		};
		EventHandler& operator =( function_typedef function )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			clear();
			add( function );
			_cs_functions.WriteUnlock();
			return *this;
		};
		void raise( void )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]();
			}
			_cs_functions.WriteUnlock();
		};
		void operator ()( void )
		{
			raise();
		};
		template< class P0 >
		void raise( P0 param0 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 >
		void operator ()( P0 param0 )
		{
			raise< P0 >( param0 );
		};
		template< class P0 , class P1 >
		void raise( P0 param0 , P1 param1 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 >
		void operator ()( P0 param0 , P1 param1 )
		{
			raise< P0 , P1 >( param0 , param1 );
		};
		template< class P0 , class P1 , class P2 >
		void raise( P0 param0 , P1 param1 , P2 param2 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 )
		{
			raise< P0 , P1 , P2 >( param0 , param1 , param2 );
		};
		template< class P0 , class P1 , class P2 , class P3 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 )
		{
			raise< P0 , P1 , P2 , P3 >( param0 , param1 , param2 , param3 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 )
		{
			raise< P0 , P1 , P2 , P3 , P4 >( param0 , param1 , param2 , param3 , param4 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 , param5 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 )
		{
			raise< P0 , P1 , P2 , P3 , P4 , P5 >( param0 , param1 , param2 , param3 , param4 , param5 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 , param5 , param6 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 )
		{
			raise< P0 , P1 , P2 , P3 , P4 , P5 , P6 >( param0 , param1 , param2 , param3 , param4 , param5 , param6 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 )
		{
			raise< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7 >( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 )
		{
			raise< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 >( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 );
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
		void raise( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 , P9 param9 )
		{
			_cs_functions.WriteLock( __FILE__" _cs_functions" , __FUNCTION__ , __LINE__ );
			for( size_t index0 = 0 ; index0 < _functions.Count() ; index0++ )
			{
				if( _functions[ index0 ] )
					_functions[ index0 ]( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 );
			}
			_cs_functions.WriteUnlock();
		};
		template< class P0 , class P1 , class P2 , class P3 , class P4 , class P5 , class P6 , class P7 , class P8 , class P9 >
		void operator ()( P0 param0 , P1 param1 , P2 param2 , P3 param3 , P4 param4 , P5 param5 , P6 param6 , P7 param7 , P8 param8 , P9 param9 )
		{
			raise< P0 , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 , P9 >( param0 , param1 , param2 , param3 , param4 , param5 , param6 , param7 , param8 , param9 );
		};
	};
};
#endif
