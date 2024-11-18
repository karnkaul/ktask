#include <ktask/queue.hpp>
#include <ktest/ktest.hpp>
#include <array>
#include <random>
#include <thread>

namespace {
using namespace ktask;
using namespace std::chrono_literals;

constexpr auto create_info_v = QueueCreateInfo{
	.thread_count = ThreadCount{2},
	.max_elements = ElementCount{10},
};

auto get_random_duration(std::chrono::milliseconds const hi = 2s) {
	static auto engine = std::default_random_engine{std::random_device{}()};
	return std::chrono::milliseconds{std::uniform_int_distribution<>{0, int(hi.count())}(engine)};
}

auto create_queue() -> Queue { return Queue{create_info_v}; }

struct WaitTask : Task {
	inline static std::atomic<int> s_executed{};
	std::chrono::milliseconds duration;

	WaitTask(std::chrono::milliseconds duration = get_random_duration(50ms)) : duration(duration) {}

	void execute() final {
		std::this_thread::sleep_for(duration);
		++s_executed;
	}
};

TEST(queue_drain_and_wait) {
	auto queue = create_queue();
	queue.pause();
	static constexpr auto run_count_v = 3;
	auto tasks = std::array<WaitTask, run_count_v>{};
	for (auto& task : tasks) { queue.enqueue(task); }
	WaitTask::s_executed = 0;
	EXPECT(queue.enqueued_count() == std::size_t(run_count_v));
	queue.drain_and_wait();
	EXPECT(queue.is_empty());
	EXPECT(WaitTask::s_executed == 3);

	queue.drain_and_wait(); // test empty_cv.lock() when queue is already empty
}

TEST(queue_drain_restart) {
	auto queue = create_queue();
	static constexpr auto run_count_v = 3;

	{
		auto tasks = std::array<WaitTask, run_count_v>{};
		queue.pause();
		for (auto& task : tasks) { queue.enqueue(task); }
		queue.drain_and_wait();
	}

	auto tasks = std::array<WaitTask, run_count_v>{};
	auto to_enqueue = std::array<Task*, run_count_v>{};
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	for (std::size_t i = 0; i < run_count_v; ++i) { to_enqueue[i] = &tasks[i]; }
	WaitTask::s_executed = 0;
	queue.enqueue(to_enqueue);
	queue.drain_and_wait();
	EXPECT(queue.is_empty());
	EXPECT(WaitTask::s_executed == 3);
}

TEST(queue_task_wait) {
	auto queue = create_queue();
	queue.pause();
	WaitTask::s_executed = 0;
	auto task = WaitTask{200ms};
	queue.enqueue(task);
	EXPECT(task.get_status() == TaskStatus::Queued);
	EXPECT(task.is_busy());
	queue.resume();
	task.wait();
	EXPECT(WaitTask::s_executed == 1);
	EXPECT(task.get_status() == TaskStatus::Completed);
	EXPECT(!task.is_busy());
	EXPECT(queue.is_empty());
}

TEST(queue_task_drop) {
	auto queue = create_queue();
	queue.pause();
	WaitTask::s_executed = 0;
	auto task = WaitTask{10s};
	queue.enqueue(task);
	EXPECT(task.is_busy());
	queue.drop_enqueued();
	EXPECT(WaitTask::s_executed == 0);
	EXPECT(task.get_status() == TaskStatus::Dropped);
	EXPECT(queue.is_empty());
}
} // namespace
