#pragma once
#include <ktask/task.hpp>
#include <cstdint>
#include <memory>

namespace ktask {
enum struct ThreadCount : std::uint8_t { Default = 0, Minimum = 1 };
enum struct ElementCount : std::size_t { Unbounded = 0 };

struct QueueCreateInfo {
	ThreadCount thread_count{ThreadCount::Default};
	ElementCount max_elements{ElementCount::Unbounded};
};

class Queue {
  public:
	using CreateInfo = QueueCreateInfo;

	static auto get_max_threads() -> ThreadCount;

	explicit Queue(CreateInfo create_info = {});

	[[nodiscard]] auto thread_count() const -> ThreadCount;
	[[nodiscard]] auto max_elements() const -> ElementCount;
	[[nodiscard]] auto enqueued_count() const -> std::size_t;
	[[nodiscard]] auto is_empty() const -> bool { return enqueued_count() == 0; }
	[[nodiscard]] auto can_enqueue() const -> bool;

	auto enqueue(std::shared_ptr<Task> task) -> bool;

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
