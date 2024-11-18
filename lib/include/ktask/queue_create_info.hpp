#pragma once
#include <cstddef>
#include <cstdint>

namespace ktask {
enum struct ThreadCount : std::uint8_t { Default = 0, Minimum = 1 };
enum struct ElementCount : std::size_t { Unbounded = 0 };

struct QueueCreateInfo {
	ThreadCount thread_count{ThreadCount::Default};
	ElementCount max_elements{ElementCount::Unbounded};
};
} // namespace ktask
