#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanContext.h"

namespace Patchouli
{
	Ref<GraphicsContext> Patchouli::GraphicsContext::create(const GraphicsInfo& info)
	{
		return makeRef<VulkanContext>(info);
	}
}
