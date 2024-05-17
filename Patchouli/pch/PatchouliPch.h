#pragma once

// Standard C++ Library Headers
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <iterator>
#include <compare>
#include <ranges>

// Standard C Library Headers
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <cassert>

// Filesystem and File I/O
#include <filesystem>
#include <fstream>

// Type Traits and Concepts
#include <concepts>
#include <type_traits>

// Standard Library Headers
#include <atomic>
#include <random>

// Memory allocator
#include <mimalloc.h>

[[nodiscard]] inline void* operator new  (std::size_t count) { return mi_new(count); }
[[nodiscard]] inline void* operator new[](std::size_t count) { return mi_new(count); }
[[nodiscard]] inline void* operator new  (std::size_t count, std::align_val_t al) {
    return mi_new_aligned(count, static_cast<std::size_t>(al));
}

[[nodiscard]] inline void* operator new[](std::size_t count, std::align_val_t al) {
    return mi_new_aligned(count, static_cast<std::size_t>(al));
}

[[nodiscard]] inline void* operator new  (std::size_t count, const std::nothrow_t& tag) noexcept {
    return mi_new_nothrow(count);
}

[[nodiscard]] inline void* operator new[](std::size_t count, const std::nothrow_t& tag) noexcept {
    return mi_new_nothrow(count);
}

[[nodiscard]] inline void* operator new  (std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept {
    return mi_new_aligned_nothrow(count, static_cast<std::size_t>(al));
}

[[nodiscard]] inline void* operator new[](std::size_t count, std::align_val_t al, const std::nothrow_t&) noexcept {
    return mi_new_aligned_nothrow(count, static_cast<std::size_t>(al));
}

inline void operator delete  (void* ptr) noexcept { mi_free(ptr); }
inline void operator delete[](void* ptr) noexcept { mi_free(ptr); }
inline void operator delete  (void* ptr, std::align_val_t al) noexcept {
    mi_free_aligned(ptr, static_cast<uint32_t>(al));
}

inline void operator delete[](void* ptr, std::align_val_t al) noexcept {
    mi_free_aligned(ptr, static_cast<uint32_t>(al));
}

inline void operator delete  (void* ptr, std::size_t sz) noexcept { mi_free_size(ptr, sz); }
inline void operator delete[](void* ptr, std::size_t sz) noexcept { mi_free_size(ptr, sz); }

inline void operator delete  (void* ptr, std::size_t sz, std::align_val_t al) noexcept {
    mi_free_size_aligned(ptr, sz, static_cast<uint32_t>(al));
}

inline void operator delete[](void* ptr, std::size_t sz, std::align_val_t al) noexcept {
    mi_free_size_aligned(ptr, sz, static_cast<uint32_t>(al));
}

inline void operator delete  (void* ptr, const std::nothrow_t& tag) noexcept { mi_free(ptr); }
inline void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept { mi_free(ptr); }

void operator delete  (void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept {
    mi_free_aligned(ptr, static_cast<std::size_t>(al));
}

void operator delete[](void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept {
    mi_free_aligned(ptr, static_cast<std::size_t>(al));
}