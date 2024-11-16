#pragma once
#include <cstdint>

namespace ktask {
// task_id.hpp
enum struct TaskId : std::uint64_t;

// task_status.hpp
enum class TaskStatus : int;

// task.hpp
class Task;

// queue.hpp
enum struct ThreadCount : std::uint8_t;
struct QueueCreateInfo;
class Queue;
} // namespace ktask
