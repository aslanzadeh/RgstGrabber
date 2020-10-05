// GstreamerCamera.h :

#pragma once

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gio/gio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "Queue.h"
#include "Interfaces/ICamera.h"

namespace RgstGrabber
{
	class GstreamerCamera final : public
	{
	public:
		GstreamerCamera();
		bool init_camera(Queue* image_queue, const Setting& setting = nullptr) override;
		void uninit_camera() override;
		bool start_camera() override;
		void stop_camera() override;
		Mat get_frame() override;
		bool is_opened() override;
		/*bool set_parameter(std::string name, double value) override;
		bool set_parameter(std::string name, bool value) override;
		double get_parameter(std::string name) override;
		bool get_parameter(std::string name) override;*/
	protected:
		GstElement* pipeline;
		GstMessage* msg;
		GSocketService* service;
		GMainLoop* loop;
		Mat _last_image;
		bool _is_opened;
	};
} // namespace RgstGrabber
