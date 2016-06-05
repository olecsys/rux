#ifndef XAN_MEMORY_CLUSTER_H
#define XAN_MEMORY_CLUSTER_H
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_debug.h>
#include <xan_memory_helper.h>                                                                    
#include <xan_thread.h>
#include <xan_thread_info.h>
#define RUX_MAX_MEMORY_CLUSTERS_COUNT 200
namespace rux
{
	namespace memory
	{
		class XMemoryCluster
		{
		private:

		public:
			static void* alloc( size_t size );
			static void free( void* ptr );
		};
		extern dll_internal XMemoryCluster* _memory_clusters[ RUX_MAX_MEMORY_CLUSTERS_COUNT ];
		extern dll_internal size_t _memory_clusters_count;
	};
};
#endif
