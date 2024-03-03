#pragma once
#include "Graphics/GraphicsObject.h"

namespace Patchouli
{
	class GraphicsSurface : public GraphicsObject
	{
	public:
		GraphicsSurface() = default;
		virtual ~GraphicsSurface() = default;
	};
}