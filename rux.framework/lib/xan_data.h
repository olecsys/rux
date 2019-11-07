#ifndef XAN_DATA_H
#define XAN_DATA_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
struct XData
{
	void* _data;
	size_t _data_size;
	size_t _allocated_size;
	size_t _offset;
	XData();
	XData( void* data , size_t data_size );
	XData( size_t data_size );
	~XData();
	void Copy( void* data , size_t data_size , const char* file = 0 , ::rux::int32 line = 0 );
	void ReAlloc( size_t data_size , const char* file = 0 , ::rux::int32 line = 0 );
	void Concatenate( void* data , size_t data_size , const char* file = 0 , ::rux::int32 line = 0 );
	void* Read( size_t read_size );
};
#endif