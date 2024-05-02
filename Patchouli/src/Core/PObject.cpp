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
		objectManger.free(ptr);
	}

	void PObject::operator delete[](void* ptr)
	{
		objectManger.free(ptr);
	}

	PObject::PObjectManger::PObjectManger()
		: deferredList(PATCHOULI_MAX_OBJECTS_FREE)
	{
		mi_register_deferred_free([](bool force, unsigned long long heartbeat, void* arg) {
			std::size_t n = ((PObjectManger*)arg)->collect(force);
			Console::coreInfo("Collection {}: {} Objects collected", heartbeat, n);
			},
			this
		);
	}

	PObject::PObjectManger::~PObjectManger()
	{
		mi_register_deferred_free(nullptr, nullptr);
		collect(true);
	}

	void PObject::PObjectManger::free(void* obj)
	{
		thread_local moodycamel::ProducerToken ptoken(deferredList);
		deferredList.enqueue(ptoken, obj);
	}

	std::size_t PObject::PObjectManger::collect(bool force)
	{
		thread_local moodycamel::ConsumerToken ctoken(deferredList);
		std::array<void*, PATCHOULI_MAX_OBJECTS_FREE> objBuffer;

		std::size_t total = 0, nObjects = 0, maxRemain = force ? 0 : PATCHOULI_MAX_OBJECTS_REMAINING_FACTOR * PATCHOULI_MAX_OBJECTS_FREE;

		do {
			nObjects = deferredList.try_dequeue_bulk(ctoken, objBuffer.data(), objBuffer.size());
			total += nObjects;

			for (std::size_t i = 0; i < nObjects; i++)
				mi_free(objBuffer[i]);
		} while (deferredList.size_approx() > maxRemain);

		return total;
	}
}