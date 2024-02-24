#pragma once

#ifdef PATCHOULI_SHARED_LIBRARY
	#ifdef _MSC_VER
		#ifdef PATCHOULI_SHARED_LIBRARY_EXPORT
			#define PATCHOULI_API __declspec(dllexport)
		#else
			#define PATCHOULI_API __declspec(dllimport)
		#endif
	#elif defined(__GNUC__) || defined(__clang__)
		#ifdef PATCHOULI_DLL_EXPORT
			#define PATCHOULI_API __attribute__((visibility("default")))
		#else
			#define PATCHOULI_API
		#endif
	#endif
#elif defined(PATCHOULI_STATIC_LIBRARY)
	#define PATCHOULI_API
#endif
