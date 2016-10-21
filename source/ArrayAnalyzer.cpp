#include <vxLib/ArrayAnalyzer.h>

namespace vx
{
	std::atomic_uint64_t ArrayAnalyzer::s_counter{0};
	ArrayStats* ArrayAnalyzer::s_data{nullptr};
}