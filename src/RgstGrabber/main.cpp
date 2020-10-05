//
// Created by reza on 2020-10-03.
//
#include <iostream>
#include "spdlog/spdlog.h"
#include <libRgstGrabber/Grabber.h>


int main() {
	spdlog::info("grabber process created");

	//command line parse

	RgstGrabber::Grabber gstGrabber;
	gstGrabber.Start();

	return 0;
}
