#pragma once
#include "Core/Base.h"
#include "Util/Util.h"

namespace Patchouli
{
	struct GraphicsInfo
	{
		const char* appName;
		uint32_t appVersion;
	};

	class PATCHOULI_API GraphicsContext : public RefCounted<GraphicsContext>
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

		static Ref<GraphicsContext> create(const GraphicsInfo& info);
	};
}