#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"
#include <concurrentqueue.h>

#define PATCHOULI_MAX_OBJECTS_FREE 1024
#define PATCHOULI_MAX_OBJECTS_REMAINING_FACTOR 0.25f

namespace Patchouli
{
	class PATCHOULI_API PObject : public RefBase<PObject>
	{
    public:
        PObject() noexcept = default;
        virtual ~PObject() noexcept = default;

        // Overloaded new operators for fast memory management
        void* operator new(std::size_t size);
        void* operator new[](std::size_t size);
        void* operator new(std::size_t size, std::align_val_t alignment);
        void* operator new[](std::size_t size, std::align_val_t alignment);

        // Overloaded delete operators for fast memory management
        void operator delete(void* ptr);
        void operator delete[](void* ptr);

    private:
        class PObjectManger
        {
        public:
            PObjectManger();
            ~PObjectManger();

            void free(void* obj);
            std::size_t collect(bool force);

        private:
            moodycamel::ConcurrentQueue<void*> deferredList;
        };

        inline static PObjectManger objectManger;
	};
}