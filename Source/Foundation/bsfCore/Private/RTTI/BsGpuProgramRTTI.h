//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsRTTIType.h"
#include "RTTI/BsStringRTTI.h"
#include "RTTI/BsStdRTTI.h"
#include "RTTI/BsDataBlobRTTI.h"
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsGpuParamDesc.h"
#include "Managers/BsGpuProgramManager.h"

namespace bs
{
	/** @cond RTTI */
	/** @addtogroup RTTI-Impl-Core
	 *  @{
	 */

	class BS_CORE_EXPORT GpuProgramBytecodeRTTI : public RTTIType<GpuProgramBytecode, IReflectable, GpuProgramBytecodeRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(instructions, 0)
			BS_RTTI_MEMBER_REFLPTR(paramDesc, 1)
			BS_RTTI_MEMBER_PLAIN(vertexInput, 2)
			BS_RTTI_MEMBER_PLAIN(messages, 3)
			BS_RTTI_MEMBER_PLAIN(compilerId, 4)
			BS_RTTI_MEMBER_PLAIN(compilerVersion, 5)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "GpuProgramBytecode";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_GpuProgramBytecode;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<GpuProgramBytecode>();
		}
	};

	class BS_CORE_EXPORT GpuParamDescRTTI : public RTTIType<GpuParamDesc, IReflectable, GpuParamDescRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(paramBlocks, 0)
			BS_RTTI_MEMBER_PLAIN(params, 1)
			BS_RTTI_MEMBER_PLAIN(samplers, 2)
			BS_RTTI_MEMBER_PLAIN(textures, 3)
			BS_RTTI_MEMBER_PLAIN(loadStoreTextures, 4)
			BS_RTTI_MEMBER_PLAIN(buffers, 5)
		BS_END_RTTI_MEMBERS

	public:
		const String& getRTTIName() override
		{
			static String name = "GpuParamDesc";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_GpuParamDesc;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return bs_shared_ptr_new<GpuParamDesc>();
		}
	};

	class BS_CORE_EXPORT GpuProgramRTTI : public RTTIType<GpuProgram, IReflectable, GpuProgramRTTI>
	{
	private:
		BS_BEGIN_RTTI_MEMBERS
			BS_RTTI_MEMBER_PLAIN(mType, 2)
			BS_RTTI_MEMBER_PLAIN(mNeedsAdjacencyInfo, 3)
			BS_RTTI_MEMBER_PLAIN(mEntryPoint, 4)
			BS_RTTI_MEMBER_PLAIN(mSource, 6)
			BS_RTTI_MEMBER_PLAIN(mLanguage, 7)
		BS_END_RTTI_MEMBERS

	public:
		void onSerializationStarted(IReflectable* obj, SerializationContext* context) override
		{
			// Need to ensure the core thread object is initialized
			GpuProgram* gpuProgram = static_cast<GpuProgram*>(obj);
			gpuProgram->blockUntilCoreInitialized();
		}

		void onDeserializationEnded(IReflectable* obj, SerializationContext* context) override
		{
			GpuProgram* gpuProgram = static_cast<GpuProgram*>(obj);
			gpuProgram->initialize();
		}

		const String& getRTTIName() override
		{
			static String name = "GpuProgram";
			return name;
		}

		UINT32 getRTTIId() override
		{
			return TID_GpuProgram;
		}

		SPtr<IReflectable> newRTTIObject() override
		{
			return GpuProgramManager::instance().createEmpty("", GPT_VERTEX_PROGRAM); // Params don't matter, they'll get overwritten
		}
	};

	template<> struct RTTIPlainType<GpuParamDataDesc>
	{
		enum { id = TID_GpuParamDataDesc }; enum { hasDynamicSize = 1 };
		static constexpr uint32_t VERSION = 1;

		static uint32_t toMemory(const GpuParamDataDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, [&data, &stream]()
			{
				uint32_t size = 0;
				size += rtti_write(VERSION, stream);

				size += rtti_write(data.name, stream);
				size += rtti_write(data.elementSize, stream);
				size += rtti_write(data.arraySize, stream);
				size += rtti_write(data.arrayElementStride, stream);
				size += rtti_write(data.type, stream);

				size += rtti_write(data.paramBlockSlot, stream);
				size += rtti_write(data.paramBlockSet, stream);
				size += rtti_write(data.gpuMemOffset, stream);
				size += rtti_write(data.cpuMemOffset, stream);

				return size;
			});
		}

		static uint32_t fromMemory(GpuParamDataDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			uint32_t size;
			rtti_read(size, stream);

			uint32_t version = 0;
			rtti_read(version, stream);
			assert(version == VERSION);

			rtti_read(data.name, stream);
			rtti_read(data.elementSize, stream);
			rtti_read(data.arraySize, stream);
			rtti_read(data.arrayElementStride, stream);
			rtti_read(data.type, stream);

			rtti_read(data.paramBlockSlot, stream);
			rtti_read(data.paramBlockSet, stream);
			rtti_read(data.gpuMemOffset, stream);
			rtti_read(data.cpuMemOffset, stream);

			return size;
		}

		static uint32_t getDynamicSize(const GpuParamDataDesc& data)
		{
			uint32_t dataSize = rtti_size(VERSION) + rtti_size(data.name) + rtti_size(data.elementSize) +
				rtti_size(data.arraySize) + rtti_size(data.arrayElementStride) + rtti_size(data.type) +
				rtti_size(data.paramBlockSlot) + rtti_size(data.paramBlockSet) +
				rtti_size(data.gpuMemOffset) + rtti_size(data.cpuMemOffset) + sizeof(uint32_t);

			return dataSize;
		}
	};

	template<> struct RTTIPlainType<GpuParamObjectDesc>
	{
		enum { id = TID_GpuParamObjectDesc }; enum { hasDynamicSize = 1 };
		static constexpr uint32_t VERSION = 2;

		static uint32_t toMemory(const GpuParamObjectDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, [&data, &stream]
			{
				uint32_t size = 0;
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.type, stream);
				size += rtti_write(data.slot, stream);
				size += rtti_write(data.set, stream);
				size += rtti_write(data.elementType, stream);

				return size;
			});
		}

		static uint32_t fromMemory(GpuParamObjectDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			uint32_t size;
			rtti_read(size, stream);

			uint32_t version = 0;
			rtti_read(version, stream);

			rtti_read(data.name, stream);
			rtti_read(data.type, stream);
			rtti_read(data.slot, stream);
			rtti_read(data.set, stream);

			if (version > 1)
				rtti_read(data.elementType, stream);

			return size;
		}

		static uint32_t getDynamicSize(const GpuParamObjectDesc& data)
		{
			uint32_t dataSize = rtti_size(VERSION) + rtti_size(data.name) + rtti_size(data.type) +
				rtti_size(data.slot) + rtti_size(data.set) + rtti_size(data.elementType) + sizeof(uint32_t);

			return dataSize;
		}
	};

	template<> struct RTTIPlainType<GpuParamBlockDesc>
	{
		enum { id = TID_GpuParamBlockDesc }; enum { hasDynamicSize = 1 };
		static constexpr uint32_t VERSION = 1;

		static uint32_t toMemory(const GpuParamBlockDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			return rtti_write_with_size_header(stream, [&data, &stream]
			{
				uint32_t size = 0;
				size += rtti_write(VERSION, stream);
				size += rtti_write(data.name, stream);
				size += rtti_write(data.set, stream);
				size += rtti_write(data.slot, stream);
				size += rtti_write(data.blockSize, stream);
				size += rtti_write(data.isShareable, stream);

				return size;
			});
		}

		static uint32_t fromMemory(GpuParamBlockDesc& data, Bitstream& stream, const RTTIFieldInfo& fieldInfo, bool compress)
		{
			uint32_t size;
			rtti_read(size, stream);

			uint32_t version = 0;
			rtti_read(version, stream);
			assert(version == VERSION);

			rtti_read(data.name, stream);
			rtti_read(data.set, stream);
			rtti_read(data.slot, stream);
			rtti_read(data.blockSize, stream);
			rtti_read(data.isShareable, stream);

			return size;
		}

		static uint32_t getDynamicSize(const GpuParamBlockDesc& data)
		{
			uint32_t dataSize = rtti_size(VERSION) + rtti_size(data.name) + rtti_size(data.set) +
				rtti_size(data.slot) + rtti_size(data.blockSize) + rtti_size(data.isShareable) +
				sizeof(uint32_t);

			return dataSize;
		}
	};

	/** @} */
	/** @endcond */
}
