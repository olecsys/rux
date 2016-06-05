#ifndef XAN_THREAD_INFO_H
#define XAN_THREAD_INFO_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_defines.h>
#include <xan_crtsect_info.h>
#include <xan_rdwrlock_info.h>
#define RUX_MAX_LABELS_COUNT 16
#define RUX_MAX_GC_COUNT 64
#define RUX_CRTSECT_INFO_COUNT 16
#define RUX_RDWRLOCK_INFO_COUNT 16
class XGCRef;
class XMemoryManager;
class XThreadInfo
{
public:	
	::rux::byte _force_cpu;
	::rux::uint32 _activity_time;
	::rux::byte _is_unknown;
	rux_volatile _registered;
	rux_volatile _gcrefs_to_clean_locked;
	XGCRef* _gcrefs_to_clean[ RUX_MAX_GC_COUNT ];
	size_t _gcrefs_to_clean_count;
	size_t _gcrefs_to_clean_index;
	XGCRef* _gcref;
	rux::pid_t _tid;
	char _thread_name[ 16 ];
	rux::int32 _module_index;
	::rux::threading::RdWrLock _cs;
	size_t _count;
	rux::int32 _modules[ RUX_MAX_LABELS_COUNT ];
	const char* _filenames[ RUX_MAX_LABELS_COUNT ];
	const char* _functions[ RUX_MAX_LABELS_COUNT ];
	rux::int32 _lines[ RUX_MAX_LABELS_COUNT ];
	rux::uint64 _performance[ RUX_MAX_LABELS_COUNT ];
	const char* _other_modules[ RUX_MAX_LABELS_COUNT ];
	const char* _other_functions[ RUX_MAX_LABELS_COUNT ];
	rux::uint8 _check_delay[ RUX_MAX_LABELS_COUNT ];
	rux::uint64 _max_delay[ RUX_MAX_LABELS_COUNT ];
	rux::uint64 _delta[ RUX_MAX_LABELS_COUNT ];

	size_t _locked_crt_sects_count;
	XCrtSectInfo _locked_crt_sects[ RUX_CRTSECT_INFO_COUNT ];
	XRdWrLockInfo _write_locked_rdwrlocks[ RUX_RDWRLOCK_INFO_COUNT ];
	size_t _write_locked_rdwrlock_count;
	XRdWrLockInfo _read_locked_rdwrlocks[ RUX_RDWRLOCK_INFO_COUNT ];
	size_t _read_locked_rdwrlock_count;
	XThreadInfo();
	~XThreadInfo( void );
	void Add( const char* filename , const char* function , rux::int32 line , const char* other_module , const char* other_function , rux::int32 module_index , rux::uint8 check_delay , rux::uint32 max_delay , rux::pid_t* owner_tid );
	void Remove( void );
	void RemoveModuleLabels( rux::int32 module_index );
	rux::uint64 get_Timeout( rux::uint32 index );
	void lock( void );
	void unlock( void );
	void AddGCRef( const XGCRef* gc_ref_ptr );
	void CollectGCRefs( ::rux::byte force , ::rux::byte lock );
	void CollectModuleGCRefs( ::rux::int32 module_index );
	void CollectCrtSectInfos( ::rux::int32 module_index );
	static void add_label( size_t thread_index , const char* filename , const char* function , rux::int32 line , const char* other_module , const char* other_function , rux::int32 module_index , rux::uint8 check_delay , rux::uint32 max_delay );
	static void remove_label( size_t thread_index );
	static size_t add_thread_id_to_global( ::rux::pid_t thread_id , const char* thread_name , ::rux::int32 module_index , ::rux::byte is_unknown );
	static XMallocArray< XGCRef* >* get_all_threads( void );
	static void remove_thread_from_global( size_t index );
	static size_t add_thread_to_global( XGCRef* memory , ::rux::int32 module_index );
	static void free_all_threads( XMallocArray< XGCRef* >* threads );
	static void CollectGCRefs( ::rux::byte force , size_t thread_index );
	static void AddGCRef( rux::int32 module_index , const XGCRef* gc_ref_ptr );
	static void add_locked_crt_sect( size_t thread_index , XCrtSect* crt_sect , size_t& index , const char* file , ::rux::uint32 line , ::rux::int32 module_index );
	static void remove_locked_crt_sect( size_t thread_index , XCrtSect* crt_sect , size_t& index , ::rux::byte release );
	static size_t locked_crt_sect_count( size_t thread_index );
	static ::rux::byte can_add_write_or_read_locked_rdwrlock( void );
	static void add_write_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , const char* file , ::rux::uint32 line , ::rux::int32 module_index );
	static void remove_write_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , ::rux::byte release );
	static void add_read_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , const char* file , ::rux::uint32 line , ::rux::int32 module_index );
	static void remove_read_locked_rdwrlock( size_t thread_index , ::rux::threading::RdWrLock* rdwrlock , size_t& index , ::rux::byte release );
	static size_t read_locked_rdwrlock_count( size_t thread_index );
	static size_t write_locked_rdwrlock_count( size_t thread_index );
	static void write_diagnostics( XMemoryManager* _this , char* now_string , ::rux::uint8& is_interrupt );
	static void check_unknown_threads( void );
	void check_corrupted_memory( void );
	void unregister( void );
	void null( void );
	void remove_read_locked_rdwrlock( ::rux::threading::RdWrLock* rdwrlock , size_t& index , bool all );
	::rux::byte remove_write_locked_rdwrlock( ::rux::threading::RdWrLock* rdwrlock , size_t& index , bool all );
	::rux::byte remove_locked_crt_sect( XCrtSect* crt_sect , size_t& index , bool all );
	static size_t get_threads_count( void );
	static XThreadInfo* find_thread( ::rux::pid_t tid );
	static size_t find_thread_index( ::rux::pid_t tid );
	static size_t get_thread_index( ::rux::int32 module_index );
	static void set_thread_force_cpu( size_t thread_index , ::rux::byte force_cpu );
};
#endif
