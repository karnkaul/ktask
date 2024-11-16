#pragma once
#include <ktask/task_id.hpp>
#include <ktask/task_status.hpp>
#include <atomic>
#include <latch>

namespace ktask {
class Task {
  public:
	using Status = TaskStatus;
	using Id = TaskId;

	virtual ~Task() = default;

	Task() = default;
	Task(Task const&) = delete;
	Task(Task&&) = delete;
	auto operator=(Task const&) -> Task& = delete;
	auto operator=(Task&&) -> Task& = delete;

	virtual void execute() = 0;

	[[nodiscard]] auto get_id() const -> Id { return m_id; }
	[[nodiscard]] auto get_status() const -> Status { return m_status; }

	void wait() { m_completed.wait(); }

  private:
	std::atomic<Status> m_status{};
	std::latch m_completed{1};
	Id m_id{Id::None};

	friend class Queue;
};
} // namespace ktask
