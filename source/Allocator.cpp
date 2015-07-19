#include <vxLib/Allocator/Allocator.h>

namespace vx
{
	thread_local AllocationProfiler* Allocator::s_allocationProfiler{nullptr};
}