#include <vxLib/math/half.h>

namespace HalfCpp
{
	u16 basetable[512];
	u8 shifttable[512];

	u32 mantissatable[2048];
	u16 offsettable[64];
	u32 exponenttable[64];

	void generatetables()
	{
		unsigned int i;
		int e;
		for (i = 0; i < 256; ++i)
		{
			e = i - 127;
			if (e < -24) { // Very small numbers map to zero
				basetable[i | 0x000] = 0x0000;
				basetable[i | 0x100] = 0x8000;
				shifttable[i | 0x000] = 24;
				shifttable[i | 0x100] = 24;
			}
			else if (e < -14) { // Small numbers map to denorms
				basetable[i | 0x000] = (0x0400 >> (-e - 14));
				basetable[i | 0x100] = (0x0400 >> (-e - 14)) | 0x8000;
				shifttable[i | 0x000] = -e - 1;
				shifttable[i | 0x100] = -e - 1;
			}
			else if (e <= 15) { // Normal numbers just lose precision
				basetable[i | 0x000] = ((e + 15) << 10);
				basetable[i | 0x100] = ((e + 15) << 10) | 0x8000;
				shifttable[i | 0x000] = 13;
				shifttable[i | 0x100] = 13;
			}
			else if (e < 128) { // Large numbers map to Infinity
				basetable[i | 0x000] = 0x7C00;
				basetable[i | 0x100] = 0xFC00;
				shifttable[i | 0x000] = 24;
				shifttable[i | 0x100] = 24;
			}
			else { // Infinity and NaN's stay Infinity and NaN's
				basetable[i | 0x000] = 0x7C00;
				basetable[i | 0x100] = 0xFC00;
				shifttable[i | 0x000] = 13;
				shifttable[i | 0x100] = 13;
			}
		}
	}

	unsigned int convertmantissa(unsigned int i)
	{
		unsigned int m = i << 13; // Zero pad mantissa bits
		unsigned int e = 0;
		while (!(m & 0x00800000)) { // While not normalized
			e -= 0x00800000; // Decrement exponent (1<<23)
			m <<= 1; // Shift mantissa
		}
		m &= ~0x00800000; // Clear leading 1 bit
		e += 0x38800000; // Adjust bias ((127-14)<<23)
		return m | e; // Return combined number

	}

	void generateTablesF16()
	{
		mantissatable[0] = 0;

		for (int i = 1; i < 1024; ++i)
		{
			mantissatable[i] = convertmantissa(i);// for i = 1..1023
		}

		for (int i = 1024; i < 2048; ++i)
		{
			mantissatable[i] = 0x38000000 + ((i - 1024) << 13);// for i = 1024...2047
		}

		exponenttable[0] = 0;
		for (int i = 1; i < 31; ++i)
		{
			exponenttable[i] = i << 23;// for i = 1..30
		}
		exponenttable[31] = 0x47800000;
		exponenttable[32] = 0x80000000;

		for (int i = 33; i < 63; ++i)
		{
			exponenttable[i] = 0x80000000 + ((i - 32) << 23);// for i = 33..62
		}
		exponenttable[63] = 0xC7800000;

		offsettable[0] = 0;
		offsettable[32] = 0;
		for (int i = 1; i < 64; ++i)
		{
			offsettable[i] = 1024;
		}
	}

	u16 f32ToF16(f32 f)
	{
		unsigned int ui = *(unsigned*)&f;
		return basetable[(ui >> 23) & 0x1ff] + ((ui & 0x007fffff) >> shifttable[(ui >> 23) & 0x1ff]);;
	}

	f32 f16ToF32(u16 f16)
	{
		auto tmp = mantissatable[offsettable[f16 >> 10] + (f16 & 0x3ff)] + exponenttable[f16 >> 10];

		float result = *(float*)&tmp;
		return result;
	};

	struct Initializer
	{
		Initializer()
		{
			generatetables();
			generateTablesF16();
		}
	};

	Initializer g_initializer{};
}

u16 f16::toF16(f32 f)
{
	return HalfCpp::f32ToF16(f);
}

f32 f16::toF32() const
{
	return HalfCpp::f16ToF32(m_value);
}