#include "Core/PObject.h"
#include <mimalloc.h>

#define PATCHOULI_MAXN_FREE_OBJECTS 256

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
		objectManger.free((PObject*)ptr);
	}

	void PObject::operator delete[](void* ptr)
	{
		objectManger.free((PObject*)ptr);
	}

	PObject::PObjectManger::PObjectManger()
	{
		mi_register_deferred_free([](bool force, unsigned long long heartbeat, void* arg) {
			((PObjectManger*)arg)->collect();
			}, this);
	}

	PObject::PObjectManger::~PObjectManger()
	{
		mi_register_deferred_free(nullptr, nullptr);
		PObject* obj;
		while (deferedList.try_dequeue_non_interleaved(obj))
			mi_free(obj);
	}

	void PObject::PObjectManger::free(PObject* obj)
	{
		deferedList.enqueue(obj);
	}

	void PObject::PObjectManger::collect()
	{
		std::array<PObject*, PATCHOULI_MAXN_FREE_OBJECTS> objBuffer;
		std::size_t n = deferedList.try_dequeue_bulk(objBuffer.data(), objBuffer.size());

		for (std::size_t i = 0; i < n; i++)
			mi_free(objBuffer[i]);
	}
}