//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsStdRTTI.h"
#include "Localization/BsStringTable.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT StringTableRTTI : public RTTIType<StringTable, Resource, StringTableRTTI>
	{
	private:
		Language& getActiveLanguage(StringTable* obj) { return obj->mActiveLanguage; }
		void setActiveLanguage(StringTable* obj, Language& val) { obj->mActiveLanguage = val; }

		LanguageData& getLanguageData(StringTable* obj, UINT32 idx) { return obj->mAllLanguages[idx]; }
		void setLanguageData(StringTable* obj, UINT32 idx, LanguageData& val) { obj->mAllLanguages[idx] = val; }
		UINT32 getNumLanguages(StringTable* obj) { return (UINT32)Language::Count; }
		void setNumLanguages(StringTable* obj, UINT32 val) { /* Do nothing */ }

		UnorderedSet<String>& getIdentifiers(StringTable* obj) { return obj->mIdentifiers; }
		void setIdentifiers(StringTable* obj, UnorderedSet<String>& val) { obj->mIdentifiers = val; }

	public:
		StringTableRTTI()
		{
			addPlainField("mActiveLanguage", 0, &StringTableRTTI::getActiveLanguage, &StringTableRTTI::setActiveLanguage);
			addPlainArrayField("mLanguageData", 1, &StringTableRTTI::getLanguageData, &StringTableRTTI::getNumLanguages,
				&StringTableRTTI::setLanguageData, &StringTableRTTI::setNumLanguages);
			addPlainField("mIdentifiers", 2, &StringTableRTTI::getIdentifiers, &StringTableRTTI::setIdentifiers);
		}

		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			StringTable* stringTable = static_cast<StringTable*>(obj);
			stringTable->setActiveLanguage(stringTable->mActiveLanguage);
		}

		const String& getRTTIName() override
		{
			static String name = "StringTable";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_StringTable;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return StringTable::_createPtr();
		}
	};

	/**
	 * RTTIPlainType for LanguageData.
	 * 			
	 * @see		RTTIPlainType
	 */
	template<>
	struct RTTIPlainType<LanguageData>
	{	
		enum { id = TID_LanguageData }; enum { hasDynamicSize = 1 };

		/** @copydoc RTTIPlainType::toMemory */
		static uint32_t toMemory(const LanguageData& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, [&data, &stream]()
			{
				uint32_t size = 0;

				auto numElements = (uint32_t)data.strings.size();
				size += rtti_write(numElements, stream);

				for (auto& entry : data.strings)
				{
					size += rtti_write(entry.first, stream);
					size += rtti_write(*entry.second, stream);
				}

				return size;
			});
		}

		/** @copydoc RTTIPlainType::fromMemory */
		static uint32_t fromMemory(LanguageData& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			uint32_t size = 0;
			rtti_read(size, stream);

			uint32_t numElements = 0;
			rtti_read(numElements, stream);

			data.strings.clear();
			for (uint32_t i = 0; i < numElements; i++)
			{
				String identifier;
				rtti_read(identifier, stream);

				SPtr<LocalizedStringData> entryData = bs_shared_ptr_new<LocalizedStringData>();
				rtti_read(*entryData, stream);

				data.strings[identifier] = entryData;
			}

			return size;
		}

		/** @copydoc RTTIPlainType::getDynamicSize */
		static uint32_t getDynamicSize(const LanguageData& data)
		{
			uint64_t dataSize = sizeof(uint32_t) * 2;

			for (auto& entry : data.strings)
			{
				dataSize += rtti_size(entry.first);
				dataSize += rtti_size(*entry.second);
			}

			assert(dataSize <= std::numeric_limits<uint32_t>::max());

			return (uint32_t)dataSize;
		}	
	};

	/**
	 * RTTIPlainType for LocalizedStringData.
	 * 			
	 * @see		RTTIPlainType
	 */
	template<>
	struct RTTIPlainType<LocalizedStringData>
	{	
		enum { id = TID_LocalizedStringData }; enum { hasDynamicSize = 1 };

		/** @copydoc RTTIPlainType::toMemory */
		static uint32_t toMemory(const LocalizedStringData& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, [&data, &stream]()
			{
				uint32_t size = 0;

				size += rtti_write(data.string, stream);
				size += rtti_write(data.numParameters, stream);

				for (uint32_t i = 0; i < data.numParameters; i++)
					size += rtti_write(data.parameterOffsets[i], stream);

				return size;
			});
		}

		/** @copydoc RTTIPlainType::fromMemory */
		static uint32_t fromMemory(LocalizedStringData& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			if (data.parameterOffsets != nullptr)
				bs_deleteN(data.parameterOffsets, data.numParameters);

			uint32_t size = 0;
			rtti_read(size, stream);

			rtti_read(data.string, stream);
			rtti_read(data.numParameters, stream);

			data.parameterOffsets = bs_newN<LocalizedStringData::ParamOffset>(data.numParameters);
			for (uint32_t i = 0; i < data.numParameters; i++)
				rtti_read(data.parameterOffsets[i], stream);

			return size;
		}

		/** @copydoc RTTIPlainType::getDynamicSize */
		static uint32_t getDynamicSize(const LocalizedStringData& data)
		{
			uint64_t dataSize = sizeof(uint32_t);

			dataSize += rtti_size(data.string);
			dataSize += rtti_size(data.numParameters);

			for (uint32_t i = 0; i < data.numParameters; i++)
				dataSize = rtti_size(data.parameterOffsets[i]);

			assert(dataSize <= std::numeric_limits<uint32_t>::max());

			return (uint32_t)dataSize;
		}	
	};

	BS_ALLOW_MEMCPY_SERIALIZATION(LocalizedStringData::ParamOffset);

	/** @} */
	/** @endcond */
}
