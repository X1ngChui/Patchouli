#include "Core/PObject.h"
#include "Log/Console.h"
#include <mimalloc.h>

namespace Patchouli
{
	void* PObject::operator new(std::size_t size)
	{
		return mi_new(size);
	}

	void* PObject::operator new[](std::size_t size)
	{
		return mi_new(size);
	}

	void* PObject::operator new(std::size_t size, std::align_val_t alignment)
	{
		return mi_new_aligned(size, static_cast<std::size_t>(alignment));
	}

	void* PObject::operator new[](std::size_t size, std::align_val_t alignment)
	{
		return mi_new_aligned(size, static_cast<std::size_t>(alignment));
	}

	void PObject::operator delete(void* ptr)
	{
		manager.push(ptr);
	}

	void PObject::operator delete[](void* ptr)
	{
		manager.push(ptr);
	}

	PObject::ObjectManager::ObjectManager()
	{
		mi_register_deferred_free([](bool force, unsigned long long heartbeat, void* arg) {
			std::size_t n = ((ObjectManager*)arg)->collect();
			Console::coreInfo("Collection {}: {} Objects collected", heartbeat, n);
			}, this);
	}

	PObject::ObjectManager::~ObjectManager()
	{
		mi_register_deferred_free(nullptr, nullptr);
		collect();
	}

	void PObject::ObjectManager::push(void* obj)
	{
		static_assert(sizeof(PObject) > sizeof(Node));
		Node* node = (Node*)obj;
		node->next = deferredList.load(std::memory_order_relaxed);
		while (!deferredList.compare_exchange_weak(node->next, node));
	}

	void* PObject::ObjectManager::pop()
	{
		Node* head = deferredList.load(std::memory_order_relaxed);
		do {
			if (head == nullptr) return nullptr;
		} while (!deferredList.compare_exchange_weak(head, head->next));

		return head;
	}

	std::size_t PObject::ObjectManager::collect()
	{
		std::lock_guard<std::mutex> lock(collectMutex);

		std::size_t n = 0;
		for (void* obj = pop(); obj != nullptr; obj = pop())
		{
			n++;
			mi_free(obj);
		}

		return n;
	}
}