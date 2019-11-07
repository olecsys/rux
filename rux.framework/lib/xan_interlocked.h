#ifndef XAN_INTERLOCKED_H
#define XAN_INTERLOCKED_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
class XInterlocked
{
private:
public:
	static rux_interlocked_return Increment( rux_volatile* lpAddend );
	static rux_interlocked_return Decrement( rux_volatile* lpAddend );
	static rux_interlocked_return Exchange( rux_volatile* lpTarget , rux_interlocked_return lValue );
	static rux_interlocked_return CompareExchange( rux_volatile* target , rux_interlocked_return exchange , rux_interlocked_return comparand );	
	static rux_interlocked_return ExchangeAdd( rux_volatile* lpTarget , rux_interlocked_return lValue );
	static rux_interlocked_return ExchangeSubtract( rux_volatile* lpTarget , rux_interlocked_return lValue );
	static void* CompareExchangePointer( void* volatile* target , void* exchange , void* comparand );
};
#endif

