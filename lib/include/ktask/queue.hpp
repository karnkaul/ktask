#pragma once
#include <ktask/queue_create_info.hpp>
#include <ktask/queue_fwd.hpp>
#include <ktask/task.hpp>
#include <memory>
#include <span>

namespace ktask {
class Queue {
  public:
	using CreateInfo = QueueCreateInfo;

	static auto get_max_threads() -> ThreadCount;

	explicit Queue(CreateInfo create_info = {});

	[[nodiscard]] auto thread_count() const -> ThreadCount;
	[[nodiscard]] auto max_elements() const -> ElementCount;
	[[nodiscard]] auto enqueued_count() const -> std::size_t;
	[[nodiscard]] auto is_empty() const -> bool { return enqueued_count() == 0; }
	[[nodiscard]] auto can_enqueue(std::size_t count = 1) const -> bool;

	auto enqueue(Task& task) -> bool;
	auto enqueue(std::span<Task* const> tasks) -> bool;
	auto fork_join(std::span<Task* const> tasks) -> TaskStatus;

	void pause();
	void resume();
	void drain_and_wait();
	void drop_enqueued();

  private:
	struct Impl;
	struct Deleter {
		void operator()(Impl* ptr) const noexcept;
	};
	std::unique_ptr<Impl, Deleter> m_impl{};
};
} // namespace ktask
