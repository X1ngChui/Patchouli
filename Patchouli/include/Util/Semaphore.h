#include "Core/Base.h"
#include <Windows.h>

namespace Patchouli
{
	class PATCHOULI_API Semaphore
	{
	public:
		Semaphore(uint32_t count)
			: handle(CreateSemaphore(nullptr, count, LONG_MAX, nullptr))
		{
		}

		~Semaphore()
		{
			CloseHandle(handle);
		}

		void signal()
		{
			ReleaseSemaphore(handle, 1, nullptr);
		}

		void wait()
		{
			WaitForSingleObject(handle, INFINITE);
		}
	private:
		HANDLE handle;
	};
}
