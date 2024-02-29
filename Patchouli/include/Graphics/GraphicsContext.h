#pragma once
#include "Core/Base.h"
#include "Util/Util.h"

namespace Patchouli
{
	enum class WindowAPI
	{
		None = 0, GLFW
	};

	struct GraphicsInfo
	{
		const char* appName;
		uint32_t appVersion;
		WindowAPI windowAPI;
	};

	class PATCHOULI_API GraphicsContext : public RefBase<GraphicsContext>
	{
	public:
		GraphicsContext() = default;
		virtual ~GraphicsContext() = default;

		static Ref<GraphicsContext> create(const GraphicsInfo& info);
	};
}