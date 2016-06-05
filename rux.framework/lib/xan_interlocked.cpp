#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_interlocked.h>
rux_interlocked_return XInterlocked::Increment( rux_volatile* lpAddend )
{
#ifdef __WINDOWS__
	return ::InterlockedIncrement( lpAddend );
#endif
#ifdef __SOLARIS__
	return atomic_add_32_nv( lpAddend , 1 );
#endif
#ifdef __LINUX__
	return __sync_add_and_fetch( lpAddend , 1 );
#endif
};
rux_interlocked_return XInterlocked::Decrement( rux_volatile* lpAppend )
{
#ifdef __WINDOWS__
	return ::InterlockedDecrement( lpAppend );
#endif
#ifdef __SOLARIS__
	return atomic_dec_32_nv( lpAppend );
#endif
#ifdef __LINUX__
	return __sync_sub_and_fetch( lpAppend , 1 );
#endif
};
rux_interlocked_return XInterlocked::Exchange( rux_volatile* lpTarget , rux_interlocked_return lValue )
{
#ifdef __WINDOWS__
	return ::InterlockedExchange( lpTarget , lValue );
#endif
#ifdef __LINUX__
	return __sync_lock_test_and_set( lpTarget , lValue );
#endif
#ifdef __SOLARIS__
	return atomic_swap_32( lpTarget , lValue );
#endif
};
rux_interlocked_return XInterlocked::CompareExchange( rux_volatile* target , rux_interlocked_return exchange , rux_interlocked_return comparand )
{
#ifdef __WINDOWS__
	return ::InterlockedCompareExchange( target , exchange , comparand );
#endif
#ifdef __LINUX__
	return __sync_val_compare_and_swap( target , comparand , exchange );
#endif
#ifdef __SOLARIS__
	return atomic_cas_32( target , comparand , exchange );
#endif
};
rux_interlocked_return XInterlocked::ExchangeAdd( rux_volatile* target , rux_interlocked_return value )
{
#ifdef __WINDOWS__
	return ::InterlockedExchangeAdd( target , value );
#elif defined( __SOLARIS__ )
	return atomic_add_32( target , value );
#elif defined( __LINUX__ )
	return __sync_fetch_and_add( target , value );
#endif
};
rux_interlocked_return XInterlocked::ExchangeSubtract( rux_volatile* target , rux_interlocked_return value )
{
#ifdef __WINDOWS__
	return ::InterlockedExchangeAdd( target , -value );
#elif defined( __SOLARIS__ )
	return atomic_add_32( target , -value );
#elif defined( __LINUX__ )
	return __sync_fetch_and_sub( target , value );
#endif
};
void* XInterlocked::CompareExchangePointer( void* volatile* target , void* exchange , void* comparand )
{
#ifdef __WINDOWS__
	return ::InterlockedCompareExchangePointer( target , exchange , comparand );
#endif
#ifdef __LINUX__
	return __sync_val_compare_and_swap( target , comparand , exchange );
#endif
#ifdef __SOLARIS__
	return atomic_cas_ptr( *target , comparand , exchange );
#endif
};