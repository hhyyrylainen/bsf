#pragma once

#include "CmPrerequisites.h"
#include "CmRTTIType.h"
#include "CmGpuProgramManager.h"
#include "CmD3D9GpuProgram.h"

namespace BansheeEngine
{
	class CM_D3D9_EXPORT D3D9GpuProgramRTTI : public RTTIType<D3D9GpuProgram, GpuProgram, D3D9GpuProgramRTTI>
	{
	public:
		D3D9GpuProgramRTTI()
		{ }

		virtual const String& getRTTIName()
		{
			static String name = "D3D9GpuProgram";
			return name;
		}

		virtual UINT32 getRTTIId()
		{
			return TID_D3D9_GpuProgram;
		}

		virtual std::shared_ptr<IReflectable> newRTTIObject()
		{
			CM_EXCEPT(InvalidStateException, "Cannot create an instance of an abstract class.");
		}
	};

	class CM_D3D9_EXPORT D3D9GpuVertexProgramRTTI : public RTTIType<D3D9GpuVertexProgram, D3D9GpuProgram, D3D9GpuVertexProgramRTTI>
	{
	public:
		D3D9GpuVertexProgramRTTI()
		{ }

		virtual const String& getRTTIName()
		{
			static String name = "D3D9GpuVertexProgram";
			return name;
		}

		virtual UINT32 getRTTIId()
		{
			return TID_D3D9_GpuVertexProgram;
		}

		virtual std::shared_ptr<IReflectable> newRTTIObject()
		{
			return GpuProgramManager::instance().createEmpty("hlsl", GPT_VERTEX_PROGRAM);
		}
	};

	class CM_D3D9_EXPORT D3D9GpuFragmentProgramRTTI : public RTTIType<D3D9GpuFragmentProgram, D3D9GpuProgram, D3D9GpuFragmentProgramRTTI>
	{
	public:
		D3D9GpuFragmentProgramRTTI()
		{ }

		virtual const String& getRTTIName()
		{
			static String name = "D3D9GpuFragmentProgram";
			return name;
		}

		virtual UINT32 getRTTIId()
		{
			return TID_D3D9_GpuFragmentProgram;
		}

		virtual std::shared_ptr<IReflectable> newRTTIObject()
		{
			return GpuProgramManager::instance().createEmpty("hlsl", GPT_FRAGMENT_PROGRAM);
		}
	};
}