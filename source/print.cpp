#include <vxLib/print.h>

namespace vx
{
	namespace detail
	{
		namespace print
		{
			thread_local int g_bufferSize{ 0 };
			thread_local char* g_buffer{ nullptr };
			thread_local HANDLE g_consoleHandle{ nullptr };
		}
	}

	void allocate_console(const vx::AllocatedBlock &block)
	{
		VX_ASSERT(block.size == (s32)block.size);
		vx::detail::print::g_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		vx::detail::print::g_buffer = (char*)block.ptr;
		vx::detail::print::g_bufferSize = (s32)block.size;
	}

	vx::AllocatedBlock deallocate_console()
	{
		vx::AllocatedBlock block = { (u8*)vx::detail::print::g_buffer, (size_t)vx::detail::print::g_bufferSize };

		vx::detail::print::g_consoleHandle = nullptr;

		vx::detail::print::g_buffer = nullptr;
		vx::detail::print::g_bufferSize = 0;

		return block;
	}
}