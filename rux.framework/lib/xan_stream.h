#ifndef XAN_STREAM_H
#define XAN_STREAM_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xan_includes.h"
namespace rux
{
	namespace io
	{
		enum XEnum_SetPosition
		{
			XEnum_SetPosition_FromBegin = SEEK_SET ,
			XEnum_SetPosition_FromEnd = SEEK_END ,
			XEnum_SetPosition_FromCurrentPosition = SEEK_CUR
		};
		class stream
		{
		public:
			virtual bool opened( void ) = 0;
			virtual bool read_bytes( char* buffer , size_t buffer_size , size_t& readen ) = 0;
			virtual bool write_bytes( char* buffer , size_t buffer_size , size_t& written ) = 0;
			virtual bool seek( ::rux::uint64 position , XEnum_SetPosition type = XEnum_SetPosition_FromBegin ) = 0;
			virtual ::rux::uint64 position( void ) = 0;
			void release( void ) const;
			virtual ~stream();
		};
		class memory_stream : public ::rux::io::stream
		{
			::rux::byte* _buffer;
			size_t _buffer_allocated_size;
			size_t _buffer_size;
			size_t _buffer_offset;
			::rux::byte _delete_here;
		public:
			memory_stream( void );
			memory_stream( ::rux::byte* buffer , size_t buffer_size , ::rux::byte copy = 0 , ::rux::byte delete_here = 1 );
			virtual ~memory_stream( void );
			void set_buffer( ::rux::byte* buffer , size_t buffer_size , ::rux::byte copy = 0 , ::rux::byte delete_here = 1 );
			void free_buffer( void );
			bool opened( void );
			bool read_bytes( char* buffer , size_t buffer_size , size_t& readen );
			bool write_bytes( char* buffer , size_t buffer_size , size_t& written );
			bool seek( ::rux::uint64 position , XEnum_SetPosition type = XEnum_SetPosition_FromBegin );
			::rux::uint64 position( void );
		};
	};
};
#endif