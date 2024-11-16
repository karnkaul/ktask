#pragma once
#include <ktask/fwd.hpp>

namespace ktask {
enum class TaskStatus : int { None, Queued, Dropped, Executing, Completed };
} // namespace ktask
