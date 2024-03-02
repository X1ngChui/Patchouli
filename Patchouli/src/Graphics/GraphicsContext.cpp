#include "Graphics/GraphicsContext.h"
#include "Graphics/Vulkan/VulkanContext.h"

namespace Patchouli
{
	Ref<GraphicsContext> Patchouli::GraphicsContext::create(const GraphicsInfo& info)
	{
		switch (info.graphicsAPI)
		{
		case GraphicsAPI::Vulkan:
			return makeRef<VulkanContext>();
		}
		
		return nullptr;
	}
}
