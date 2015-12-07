#pragma once

#include <vxLib/Allocator/Allocator.h>
#include <Windows.h>
#include <double-conversion/double-conversion.h>

namespace vx
{
	namespace detail
	{
		namespace print
		{
			extern bool int_to_string(s32 value, char** buffer, int* remainingBufferSize, int* size);
			extern bool int_to_string(u32 value, char** buffer, int* remainingBufferSize, int* size);
			extern bool int_to_string(s64 val, char** buffer, int* remainingBufferSize, int* size);
			extern bool int_to_string(u64 val, char** buffer, int* remainingBufferSize, int* size);

			extern int g_bufferSize;
			extern char* g_buffer;
			extern HANDLE g_consoleHandle;

			inline bool copy_string_until_var(char** dst_inout, const char** src_inout, int dstSize, int* written)
			{
				auto src = *src_inout;
				auto dst = *dst_inout;

				int count = 0;
				while (src[0] != '%' && src[1] != 'v')
				{
					if (src[0] == '\0')
					{
						*written = count;
						*dst_inout = dst;
						*src_inout = src;
						return true;
					}

					if (count >= dstSize)
					{
						*written = count;
						*dst_inout = dst;
						*src_inout = src;
						return false;
					}

					*dst++ = *src++;
					++count;
				}

				*dst_inout = dst;
				*src_inout = src + 2;
				*written = count;
				return true;
			}

			inline bool copy_string_until_end(char* dst, const char* src, int dstSize, int* written)
			{
				int count = 0;
				while (*src != '\0')
				{
					if (count >= dstSize)
					{
						*written = count;
						return false;
					}

					*dst++ = *src++;
					++count;
				}

				*written = count;
				return true;
			}

			inline void writeToConsole(const char* str, int size)
			{
				WriteConsoleA(g_consoleHandle, str, size, 0, 0);
			}

			inline void writeBufferToConsole(char** dst, int* size, int* remainingSize)
			{
				g_buffer[*size] = '\0';

				writeToConsole(g_buffer, *size);

				*dst = g_buffer;
				*size = 0;
				*remainingSize = g_bufferSize - 1;
			}

			inline void writeBufferToConsole(int size)
			{
				g_buffer[size] = '\0';

				writeToConsole(g_buffer, size);
			}

			struct Data
			{
				const char* format;
				char* dst;
				int size;
				int remainingSize;
			};

			template<typename T>
			struct ConvertImpl;

			struct ConvertFun
			{
				template<typename T>
				inline void operator()(Data* data, T arg)
				{
					auto format = data->format;
					auto dst = data->dst;
					auto size = data->size;
					auto remainingSize = data->remainingSize;

					int count = 0;
					while (!copy_string_until_var(&dst, &format, remainingSize, &count))
					{
						size += count;
						remainingSize -= count;

						writeBufferToConsole(&dst, &size, &remainingSize);
					}

					size += count;
					remainingSize -= count;

					if (!ConvertImpl<T>()(&dst, &remainingSize, &size, arg))
					{
						writeBufferToConsole(&dst, &size, &remainingSize);

						ConvertImpl<T>()(&dst, &remainingSize, &size, arg);
					}

					data->dst = dst;
					data->format = format;
					data->size = size;
					data->remainingSize = remainingSize;
				}
			};

			inline void printf_impl(Data* data)
			{
				auto format = data->format;
				auto dst = data->dst;
				auto size = data->size;
				auto remainingSize = data->remainingSize;

				if (format != '\0')
				{
					int writtenChars = 0;
					while (!copy_string_until_end(dst, format, remainingSize, &writtenChars))
					{
						size += writtenChars;

						writeBufferToConsole(&dst, &size, &remainingSize);
					}

					size += writtenChars;
				}

				writeBufferToConsole(size);
			}

			template<typename T, typename ...Args>
			void printf_impl(Data* data, T param, Args&& ...args)
			{
				ConvertFun()(data, param);

				printf_impl(data, args...);
			}

			template<>
			struct ConvertImpl<s32>
			{
				bool operator()(char** p, int* remainingSize, int* size, s32 arg)
				{
					return int_to_string(arg, p, remainingSize, size);
				}
			};

			template<>
			struct ConvertImpl<u32>
			{
				bool operator()(char** p, int* remainingSize, int* size, u32 arg)
				{
					return int_to_string(arg, p, remainingSize, size);
				}
			};

			template<>
			struct ConvertImpl<s64>
			{
				bool operator()(char** p, int* remainingSize, int* size, s64 arg)
				{
					return int_to_string(arg, p, remainingSize, size);
				}
			};

			template<>
			struct ConvertImpl<u64>
			{
				bool operator()(char** p, int* remainingSize, int* size, u64 arg)
				{
					return int_to_string(arg, p, remainingSize, size);
				}
			};

			template<>
			struct ConvertImpl<const char*>
			{
				bool operator()(char** p, int* remainingSize, int* size, const char* arg)
				{
					auto len = (int)strlen(arg);
					if (len > *remainingSize)
						return false;

					memcpy(*p, arg, len);

					*p += len;
					*remainingSize -= len;
					*size += len;

					return true;
				}
			};

			template<>
			struct ConvertImpl<f32>
			{
				bool operator()(char** p, int* remainingSize, int* size, float arg)
				{
					const auto minSize = 1 + double_conversion::DoubleToStringConverter::kMaxFixedDigitsBeforePoint + 1 + double_conversion::DoubleToStringConverter::kMaxFixedDigitsAfterPoint;
					if (minSize > *remainingSize)
						return false;

					double_conversion::DoubleToStringConverter c(double_conversion::DoubleToStringConverter::NO_FLAGS, "inf", "nan", 'e', -10, 10, 0, 0);
					double_conversion::StringBuilder builder(*p, *remainingSize);

					c.ToShortestSingle(arg, &builder);

					auto sz = builder.position();
					*size += sz;
					*remainingSize -= sz;
					*p += sz;

					return true;
				}
			};

			template<>
			struct ConvertImpl<f64>
			{
				bool operator()(char** p, int* remainingSize, int* size, double arg)
				{
					const auto minSize = 1 + double_conversion::DoubleToStringConverter::kMaxFixedDigitsBeforePoint + 1 + double_conversion::DoubleToStringConverter::kMaxFixedDigitsAfterPoint;
					if (minSize > *remainingSize)
						return false;

					double_conversion::DoubleToStringConverter c(double_conversion::DoubleToStringConverter::NO_FLAGS, "inf", "nan", 'e', -10, 10, 0, 0);
					double_conversion::StringBuilder builder(*p, *remainingSize);

					c.ToShortest(arg, &builder);

					auto sz = builder.position();
					*size += sz;
					*remainingSize -= sz;
					*p += sz;

					return true;
				}
			};
		}
	}

	extern void allocate_console(const vx::AllocatedBlock &block);
	extern vx::AllocatedBlock deallocate_console();

	template<typename ...Args>
	void printf(const char* format, Args&& ...args)
	{
		detail::print::Data data;
		data.dst = vx::detail::print::g_buffer;
		data.format = format;
		data.size = 0;
		data.remainingSize = vx::detail::print::g_bufferSize - 1;

		detail::print::printf_impl(&data, args...);
	}

	inline void printf(const char* format)
	{
		auto len = strlen(format);
		detail::print::writeToConsole(format, (int)len);
	}
}