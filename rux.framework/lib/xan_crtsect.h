#ifndef XAN_CRTSECT_H
#define XAN_CRTSECT_H
#include <xan_includes.h>
class XThreadInfo;
class XMemoryManager;
class XCrtSect
{
	friend class XThreadInfo;
	friend class XMemoryManager;
private:
	size_t _crt_sect_index;
	char _name[ 24 ];
	::rux::uint8 _is_spin_lock;
#ifdef __WINDOWS__
	CRITICAL_SECTION _cs;
#endif
#ifdef __UNIX__
#ifndef __ANDROID__
	pthread_spinlock_t _pthread_spin;
#endif
	pthread_mutex_t _pthread_mutex;
	pthread_mutexattr_t _pthread_mutexattr;
#endif
public:
 XCrtSect();
	~XCrtSect();
	void Lock( ::rux::uint8 is_code_block = 1 , const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 , ::rux::byte rux_gui_pump_message = 1 );
	void UnLock( ::rux::uint8 is_code_block = 1 , const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 );
	bool TryLock( ::rux::uint8 is_code_block = 1 , const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 );
};
namespace rux
{
	namespace threading
	{
		class RdWrLock
		{
			friend class ::XThreadInfo;
			friend class ::XMemoryManager;
		public:
			RdWrLock( void );
			~RdWrLock( void );
			void ReadLock( const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 , ::rux::uint8 debug = 1 );
			void WriteLock( const char* file = NULL , const char* function = NULL , ::rux::int32 line = 0 , ::rux::uint8 debug = 1 );
			void ReadUnlock( void );
			void WriteUnlock( void );
			bool TryWriteLock( const char* file , const char* function , ::rux::int32 line , ::rux::uint8 debug );
		private:    
			static const ::rux::int32 WRITER_BIT = 1L << ( sizeof( ::rux::int32 ) * 8 - 2 );
			rux_volatile _writer_readers;
			rux_volatile _writer_recursion;
			::rux::pid_t _writer_thread;
			size_t _write_rdwrlock_index;
			size_t _read_rdwrlock_index;
		public:
			char _name[ 24 ];
		};
	};
};
#endif

