#include <vxLib/print.h>
#include <cstring>

namespace vx
{
	namespace detail
	{
		namespace print
		{
			const char digit_pairs[201] =
			{
				"00010203040506070809"
				"10111213141516171819"
				"20212223242526272829"
				"30313233343536373839"
				"40414243444546474849"
				"50515253545556575859"
				"60616263646566676869"
				"70717273747576777879"
				"80818283848586878889"
				"90919293949596979899"
			};

			static const int BUFFER_SIZE_32 = 11;
			static const int BUFFER_SIZE_64 = 21;

			template<typename T, size_t BUFFER_SIZE>
			bool sint_to_string_impl(T value, char** buffer, int* remainingBufferSize, int* sizeOut)
			{
				if (*remainingBufferSize < BUFFER_SIZE)
					return false;

				auto dst = *buffer;

				if (value == 0)
				{
					*dst++ = '0';
					*buffer = dst;
					--(*remainingBufferSize);
					++(*sizeOut);
					return true;
				}

				char buf[BUFFER_SIZE];
				char *it = &buf[BUFFER_SIZE - 2];

				if (value > 0)
				{
					T div = value / 100;
					while (div)
					{
						*((unsigned short*)it) = *(unsigned short*)&digit_pairs[2 * (value - div * 100)];
						value = div;
						it -= 2;
						div = value / 100;
					}

					*reinterpret_cast<unsigned short*>(it) = *reinterpret_cast<const unsigned short*>(&digit_pairs[2 * value]);

					if (value < 10)
						it++;
				}
				else
				{
					T div = value / 100;
					while (div)
					{
						*reinterpret_cast<unsigned short*>(it) = *(unsigned short*)&digit_pairs[-2 * (value - div * 100)];
						value = div;
						it -= 2;
						div = value / 100;
					}
					*reinterpret_cast<unsigned short*>(it) = *reinterpret_cast<const unsigned short*>(&digit_pairs[-2 * value]);
					if (value <= -10)
						it--;

					*it = '-';
				}

				const auto size = static_cast<int>(&buf[BUFFER_SIZE] - it);
				memcpy(dst, it, size);

				*buffer = dst + size;
				*remainingBufferSize -= size;
				*sizeOut += size;

				return true;
			}

			template<typename T, size_t BUFFER_SIZE>
			bool uint_to_string(T val, char** buffer, int* remainingBufferSize, int* sizeOut)
			{
				if (*remainingBufferSize < BUFFER_SIZE)
					return false;

				auto dst = *buffer;

				if (val == 0)
				{
					*dst++ = '0';
					*buffer = dst;
					--(*remainingBufferSize);
					++(*sizeOut);
					return true;
				}

				char buf[BUFFER_SIZE];
				char *it = (char*)&buf[BUFFER_SIZE - 2];

				T div = val / 100;
				while (div)
				{
					memcpy(it, &digit_pairs[2 * (val - div * 100)], 2);
					val = div;
					it -= 2;
					div = val / 100;
				}

				memcpy(it, &digit_pairs[2 * val], 2);
				if (val < 10)
					it++;

				const auto size = static_cast<int>(&buf[BUFFER_SIZE] - it);
				memcpy(dst, it, size);

				*buffer = dst + size;
				*remainingBufferSize -= size;
				*sizeOut += size;

				return true;
			}

			bool int_to_string(s32 value, char** buffer, int* remainingBufferSize, int* size)
			{
				return sint_to_string_impl<int, BUFFER_SIZE_32>(value, buffer, remainingBufferSize, size);
			}

			bool int_to_string(u32 val, char** buffer, int* remainingBufferSize, int* size)
			{
				return uint_to_string<unsigned int, BUFFER_SIZE_32>(val, buffer, remainingBufferSize, size);
			}

			bool int_to_string(s64 value, char** buffer, int* remainingBufferSize, int* size)
			{
				return sint_to_string_impl<long long, BUFFER_SIZE_64>(value, buffer, remainingBufferSize, size);
			}

			bool int_to_string(u64 val, char** buffer, int* remainingBufferSize, int* size)
			{
				return uint_to_string<unsigned long long, BUFFER_SIZE_64>(val, buffer, remainingBufferSize, size);
			}
		}
	}
}