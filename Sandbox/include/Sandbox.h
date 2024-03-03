#include "Patchouli.h"

namespace Sandbox
{
	class Sandbox : public Patchouli::Application
	{
	public:
		Sandbox(const Patchouli::ApplicationInfo& info);
		virtual ~Sandbox() = default;
	private:
		Patchouli::Ref<Patchouli::GraphicsContext> graphicsContext;
	};
}
