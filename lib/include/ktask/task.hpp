#pragma once
#include <ktask/build_version.hpp>
#include <ktask/task_fwd.hpp>
#include <ktask/task_id.hpp>
#include <ktask/task_status.hpp>
#include <atomic>

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
	[[nodiscard]] auto is_busy() const -> bool { return m_busy; }

	void wait() { m_busy.wait(true); }

  protected:
	virtual void execute() = 0;

  private:
	void do_execute();
	void do_drop();

	void finalize();

	std::atomic<Status> m_status{};
	std::atomic<bool> m_busy{};
	Id m_id{Id::None};

	friend class Queue;
};
} // namespace ktask
