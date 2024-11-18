#pragma once
#include <ktask/task_fwd.hpp>

namespace ktask {
enum class TaskStatus : int { None, Queued, Dropped, Executing, Completed };
} // namespace ktask
