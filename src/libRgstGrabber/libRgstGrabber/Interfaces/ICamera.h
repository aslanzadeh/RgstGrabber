//
// Created by reza on 2020-10-03.
//

#pragma once
#include "../Queue.h"

namespace RgstGrabber {
    class ICamera {
	public:
		ICamera();
		bool virtual init_camera(Queue* image_queue, const Setting& setting = nullptr);
		void virtual uninit_camera();
		bool virtual start_camera();
		void virtual stop_camera();
		/*bool virtual set_parameter(std::string name, double value);
		bool virtual set_parameter(std::string name, bool value);
		double virtual get_parameter(std::string name);
		bool virtual get_parameter(std::string name);*/
		Mat get_frame();
		bool virtual is_opened();
		Setting _setting;
    };
}




