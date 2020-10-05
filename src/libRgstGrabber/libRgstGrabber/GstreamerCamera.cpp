    
#include "GstreamerCamera.h"
#include "spdlog/spdlog.h"


GstFlowReturn new_preroll(GstAppSink *appsink, gpointer data) {
g_print ("Got preroll!\n");
return GST_FLOW_OK;
}

GstFlowReturn new_sample(GstAppSink *appsink, gpointer data) {
    GstSample *sample = gst_app_sink_pull_sample(appsink);
    GstCaps *caps = gst_sample_get_caps(sample);
    GstBuffer *buffer = gst_sample_get_buffer(sample);

    GstMapInfo map;
    gst_buffer_map(buffer, &map, GST_MAP_READ);

    //TODO 
	int res;
	gint width, height;
	res = gst_structure_get_int(s, "width", &width);
	res |= gst_structure_get_int(s, "height", &height);
	if(!res)
	g_printerr("could not get snapshot dimension\n");
	cv::Mat frame(height, width, CV_8UC3, (char*)map.data);
    
    // TODO do the action

    gst_buffer_unmap(buffer, &map);

    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

static gboolean on_message (GstBus *bus, GstMessage *message, gpointer data) {
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR: {
            GError *err;
            gchar *debug;
            gst_message_parse_error (message, &err, &debug);
            g_print ("Error: %s\n", err->message);
            g_error_free (err);
            g_free (debug);
            break;
        }
        case GST_MESSAGE_WARNING:{
            gchar *debug;
            GError *err;
            gst_message_parse_warning (message, &err, &debug);
            g_print ("Warning %s\n", err->message);
            g_error_free (err);
            g_free (debug);
            break;
        }
        case GST_MESSAGE_EOS:
            /* end-of-stream */
            g_print ("EOS");
            break;
        default:
            /* unhandled message */
            break;
    }
    return TRUE;
}

namespace RgstGrabber
{
	GstreamerCamera::GstreamerCamera()
	{
		_is_opened = false;
	}

	bool GstreamerCamera::init_camera(const Setting& setting)
	{
		g_print("gstreamer based camera interface initialization started\n");

		GError* error = NULL;
		string outputPort;

		if(!gst_init_check(nullptr, nullptr, &error))
		{
			g_print("gst initialize error: %s\n", error->message);
			return false;
		}
		std::string pipeline_str = "";
		//setting.color_format
		//setting.src_type
		//setting.conversition_is_needed
		//setting.is_http_server_needed
		//setting.send_protocol

		// TODO build pipline base on setting 
		

		pipeline = gst_parse_launch(pipeline_str.c_str(), &error);

		if(!pipeline)
		{
			g_print("Parse error: %s\n", error->message);
			return false;
		}


		/* get sink */
		GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink"); 
		if(sink != NULL)
		{
			gst_app_sink_set_emit_signals((GstAppSink*)sink, true); 
			gst_app_sink_set_drop((GstAppSink*)sink, true);
			gst_app_sink_set_max_buffers((GstAppSink*)sink, 1); 
			GstAppSinkCallbacks callbacks = {NULL, new_preroll, new_sample};
			gst_app_sink_set_callbacks(GST_APP_SINK(sink), &callbacks, NULL, NULL);
		}
		else
		{
			g_printerr("Could not get instance of appsink\n");
		}

		if(gst_debug_is_active())
		{
			g_print("pipeline dot file is writed on GST_DEBUG_DUMP_DOT_DIR/pipline.dot\n");
			GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "pipeline");
		}

		guint bus_watch_id;
		GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
		bus_watch_id = gst_bus_add_watch(bus, on_message, NULL);
		gst_object_unref(bus);

		_is_opened = true;
		if(!g_socket_service_is_active(service))
			spdlog::error("Soket service is not started!");
		if(!gst_is_initialized())
			spdlog::error("gst initilization is not successful");

		return true;
	}


	void GstreamerCamera::uninit_camera()
	{
		gst_message_unref(msg);
		g_socket_service_stop(service);
		g_object_unref(service);
		gst_element_set_state(pipeline, GST_STATE_NULL);
		gst_object_unref(pipeline);
		g_main_loop_unref(loop);
	}


	bool GstreamerCamera::start_camera()
	{
		if(gst_element_set_state(pipeline, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE)
		{
			spdlog::error("Could not open stream!");
			return false;
		}
		return true;
	}


	void GstreamerCamera::stop_camera()
	{
		gst_element_set_state(pipeline, GST_STATE_NULL);
		gst_object_unref(pipeline);
	}

	bool GstreamerCamera::is_opened()
	{
		return _is_opened;
	}

} // namespace RgstGrabber