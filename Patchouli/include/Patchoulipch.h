#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <concepts>
#include <type_traits>
#include <memory>
#include <mutex>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <iterator>
#include <compare>

#include <atomic>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "Core/Base.h"
#include "Core/Log.h"

#include "ObjectPool.h"
#include "Identifier.h"
#include "ResourcePtr.h"

#ifdef PACHE_PLATFORM_WINDOWS
#include <Windows.h>
#endif