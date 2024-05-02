#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"
#include <concurrentqueue.h>

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
        struct Node
        {
            Node* next = nullptr;
        };

        class ObjectManager
        {
        public:
            ObjectManager();
            ~ObjectManager();

            void push(void* obj);
            std::size_t collect();

        private:
            void* pop();

            std::mutex collectMutex;
            std::atomic<Node*> deferredList = nullptr;
        };

        inline static ObjectManager manager;
	};
}