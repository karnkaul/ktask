#pragma once
#include <ktask/build_version.hpp>
#include <ktask/task_fwd.hpp>
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

	[[nodiscard]] auto get_id() const -> Id { return m_id; }
	[[nodiscard]] auto get_status() const -> Status { return m_status; }
	[[nodiscard]] auto is_busy() const -> bool { return m_status != Status::Completed && m_status != Status::Dropped; }

	void wait() { m_completed.wait(); }

  private:
	virtual void execute() = 0;
	virtual void drop() {}

	std::atomic<Status> m_status{};
	std::latch m_completed{1};
	Id m_id{Id::None};

	friend class Queue;
};
} // namespace ktask
