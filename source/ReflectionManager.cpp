#include <vxLib/ReflectionManager.h>
#include <vxLib/ReflectionData.h>
#include <memory>
#include <vxLib/string.h>

namespace vx
{
	ReflectionManager::Data* ReflectionManager::s_data{ nullptr };
	bool ReflectionManager::s_initialized{ false };

	const s32 g_reflectionMetaDataSize = sizeof(ReflectionData::size) + sizeof(ReflectionData::hash) + sizeof(ReflectionData::memberCount);
	static_assert(g_reflectionMetaDataSize == 12, "");

	struct ReflectionManager::Data
	{
		static const u32 DATA_CAPACITY{ 10 MBYTE };
		static const u32 ENTRY_CAPACITY{ 1024 * 32 };

		struct Entry
		{
			u32 key;
			u32 offset;
		};

		std::unique_ptr<Entry[]> m_entries;
		std::unique_ptr<u8[]> m_data;
		u32 m_offset;
		u32 m_size;

		Data();
		~Data();

		bool contains(u32 key) const;
		void addData(u32 key, const ReflectionData* p, u32 size);
		const ReflectionData* find(u32 key);
	};

	ReflectionManager::Data::Data()
		:m_offset(0),
		m_size(0),
		m_entries(std::make_unique<Entry[]>(ENTRY_CAPACITY)),
		m_data(std::make_unique<u8[]>(DATA_CAPACITY))
	{
	}

	ReflectionManager::Data::~Data()
	{
		s_data = nullptr;
	}

	bool ReflectionManager::Data::contains(uint32_t key) const
	{
		auto count = m_size;
		auto ptr = m_entries.get();
		auto end = ptr + count;

		while (ptr != end)
		{
			if (ptr->key == key)
				return true;

			++ptr;
		}

		return false;
	}

	void ReflectionManager::Data::addData(uint32_t key, const ReflectionData* p, uint32_t size)
	{
		VX_ASSERT(m_offset < DATA_CAPACITY || m_size < ENTRY_CAPACITY);

		::memcpy(m_data.get() + m_offset, p, size);

		m_entries[m_size++] = { key, m_offset };

		m_offset += size;
	}

	const ReflectionData* ReflectionManager::Data::find(uint32_t key)
	{
		auto count = m_size;
		auto ptr = m_entries.get();
		auto end = ptr + count;

		while (ptr != end)
		{
			if (ptr->key == key)
				return (const ReflectionData*)(m_data.get() + ptr->offset);

			++ptr;
		}

		return nullptr;
	}

	void ReflectionManager::initData()
	{
		static Data data{};

		s_data = &data;
		s_initialized = true;
	}

	void ReflectionManager::addData(const ReflectionData* p, size_t size)
	{
		if (!s_data)
			initData();

		auto hash = p->hash;

		auto &data = *s_data;
		if (!data.contains(hash))
		{
			data.addData(hash, p, size);
		}
	}

	const ReflectionData* ReflectionManager::find(u32 key)
	{
		return s_data->find(key);
	}

	void ReflectionManager::serializeImpl(OutStream* outStream, const u8* src, const vx::ReflectionData* reflection)
	{
		auto dataSize = reflection->size;
		auto hash = reflection->hash;
		auto memberCount = reflection->memberCount;

		outStream->write((u8*)&dataSize, sizeof(dataSize));
		outStream->write((u8*)&hash, sizeof(hash));
		outStream->write((u8*)&memberCount, sizeof(memberCount));

		if (memberCount == 0)
		{
			outStream->write(src, dataSize);
		}
		else
		{
			for (s32 i = 0; i < memberCount; ++i)
			{
				auto &memberReflection = reflection->members[i];
				auto offset = memberReflection.offset;

				serializeImpl(outStream, src + offset, &memberReflection.typeData);
			}
		}
	}

	bool ReflectionManager::deserializeImpl(InStream* inStream, u8* dst, const vx::ReflectionData* reflection)
	{
		auto dataSize = reflection->size;
		auto hash = reflection->hash;
		auto memberCount = reflection->memberCount;

		u32 readDataSize;
		inStream->read((u8*)&readDataSize, sizeof(readDataSize));

		u32 readHash;
		inStream->read((u8*)&readHash, sizeof(readHash));

		u32 readMemberCount;
		inStream->read((u8*)&readMemberCount, sizeof(readMemberCount));

		if (readDataSize != dataSize ||
			hash != readHash ||
			readMemberCount != memberCount)
			return false;

		if (memberCount == 0)
		{
			inStream->read(dst, readDataSize);
		}
		else
		{
			for (s32 i = 0; i < memberCount; ++i)
			{
				auto &memberReflection = reflection->members[i];
				auto offset = memberReflection.offset;

				if (!deserializeImpl(inStream, dst + offset, &memberReflection.typeData))
					return false;
			}
		}

		return true;
	}
}