#include "Queue.h"

namespace RgstGrabber
{
	Queue::Queue(int maxCapacityIn)
	{
		_max_capacity = maxCapacityIn;
	}

	Queue::~Queue()
	{

	}

	bool Queue::push(const std::pair<Timestamp, Mat>& data)
	{
		bool results = true;
		std::unique_lock<std::mutex> lock(_q_mutex);
		if(_queue.size() >= _max_capacity)
		{
			results = false;
		}
		else
		{
			_queue.push_back(data);
		}

		_cond.notify_one();
		return results;
	}

	void Queue::pushForce(const std::pair<Timestamp, Mat>& data)
	{
		bool results = true;
		std::unique_lock<std::mutex> lock(_q_mutex);
		if(_queue.size() >= _max_capacity)
		{
			_queue.pop_front();
		}
		_queue.push_back(data);

		_cond.notify_one();
	}

	void Queue::pop(std::pair<Timestamp, Mat>& popped_data)
	{
		std::unique_lock<std::mutex> lock(_q_mutex);

		while(_queue.empty() == true)
		{
			_cond.wait(lock);
		}

		popped_data = _queue.front();
		_queue.pop_front();
	}

	void Queue::restart()
	{
		_queue = deque<std::pair<Timestamp, Mat>>();
	}
} // namespace RgstGrabber