#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"
#include <concurrentqueue.h>

namespace Patchouli
{

	struct PATCHOULI_API PObject : public RefBase<PObject>
	{
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

            void free(PObject* obj);
            void collect();
        private:
            std::mutex mutex;
            moodycamel::ConcurrentQueue<PObject*> deferedList;
        };

        inline static PObjectManger objectManger;
	};
}