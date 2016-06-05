#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_memory_cluster.h>
namespace rux
{
	namespace memory
	{
		dll_internal XMemoryCluster* _memory_clusters[ RUX_MAX_MEMORY_CLUSTERS_COUNT ] = {0};
		dll_internal size_t _memory_clusters_count = 0;
		void* XMemoryCluster::alloc( size_t size )
		{
			return 0;
		};
		void XMemoryCluster::free( void* ptr )
		{

		};
	};
};