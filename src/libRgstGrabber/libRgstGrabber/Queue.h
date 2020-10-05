//Queue.h

#pragma once

#include <opencv2/core/core.hpp>
#include <deque>
#include <functional>
#include <chrono>
#include <mutex>

namespace RgstGrabber
{
	using Timestamp = std::chrono::system_clock::time_point;
	using CurrentTime = std::chrono::system_clock::now();
	using Mat = cv::Mat;

	class Queue
	{
	public:
		Queue(int maxCapacity = 100000);
		~Queue();
		bool push(const std::pair<Timestamp, Mat>& data);
		void pushForce(const std::pair<Timestamp, Mat>& data);
		void pop(std::pair<Timestamp, Mat>& popped_data);
		void restart();

	private:
		std::deque<std::pair<Timestamp, Mat>> _queue;
		mutable std::mutex _q_mutex;
		std::condition_variable _cond;
		int _max_capacity;


	};
} // namespace RgstGrabber
#endif /*_ImageSavingQueue_h_*/
