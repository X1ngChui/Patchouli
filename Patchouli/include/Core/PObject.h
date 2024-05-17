#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"

namespace Patchouli
{
    class PATCHOULI_API PObject : public RefBase<PObject>
    {
    public:
        PObject() noexcept = default;
        virtual ~PObject() noexcept = default;
    };
}