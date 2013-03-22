#include <gst/gst.h>
#include <glib.h>
#include <glib/gstdio.h>

GstBin*		videobin;
GstBin*		audiobin;

static gboolean
bus_call (GstBus     *bus,
	  GstMessage *msg,
	  gpointer    data)
{
  GMainLoop *loop = data;

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_EOS:
      g_print ("End-of-stream\n");
      g_main_loop_quit (loop);
      exit(0);
      break;
    case GST_MESSAGE_ERROR: {
      gchar *debug = NULL;
      GError *err = NULL;

      gst_message_parse_error (msg, &err, &debug);

      g_print ("Error: %s\n", err->message);
      g_error_free (err);

      if (debug) {
        g_print ("Debug details: %s\n", debug);
        g_free (debug);
      }

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}


void	on_new_decoded_pad(GstElement *elem, GstPad	*pad, gboolean b, gpointer *data)
{
	GstCaps*	caps;
	GstStructure*	str;
	GstStructure*	str2;
	gchar*		name;
	GstPad		*audiopad, *videopad;

	
	caps = gst_pad_get_caps(pad);
	str = gst_caps_get_structure(caps, 0);
	audiopad = gst_element_get_static_pad(GST_ELEMENT(audiobin), "sink");
	videopad = gst_element_get_static_pad(GST_ELEMENT(videobin), "sink");
	
	g_print("======================Name %s\n", gst_structure_get_name(str));
	if ((name = gst_structure_get_name(str)) != NULL)
	{
		if (g_strrstr(name, "audio") != NULL)
		{
			g_print("====================Audio Linked=================\n");
			gst_pad_link(pad, audiopad);
		}
		if (g_strrstr(name, "video") != NULL)
		{
			g_print("====================Video Linked=================\n");
			gst_pad_link(pad, videopad);
		}
	}
}

int	main(int ac, char *ag[])
{

	GstElement	*pipeline, *filesrc, *decoder, *videosink, *audiosink, *queuea, *queuev;
	GstElement	*convert, *resample, *ffmpegrecolor, *videoscale;
	GstElement	*toyunda;
	GMainLoop*	loop;
	GstBus*		bus;
	GstPad*     gpad, *gpad2;
	guint		watch_id;
	GstStateChangeReturn ret;

	gst_init(&ac, &ag);
	loop = g_main_loop_new(NULL, FALSE);
	
	pipeline = gst_pipeline_new("toyundapipeline");
	bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	watch_id = gst_bus_add_watch (bus, bus_call, loop);
	gst_object_unref (bus);

	filesrc  = gst_element_factory_make ("filesrc", "file source");
	decoder  = gst_element_factory_make ("decodebin2", "decodebin2");
	g_signal_connect(decoder, "new-decoded-pad", G_CALLBACK(on_new_decoded_pad), NULL);
	gst_bin_add_many(GST_BIN(pipeline), filesrc, decoder, NULL);
	gst_element_link(filesrc, decoder);
	
	audiobin = gst_bin_new("audiobin");
	convert = gst_element_factory_make("audioconvert", "audioconvert");
	resample = gst_element_factory_make("audioresample", "audioresample");
	queuea = gst_element_factory_make("queue", "queue a");
	audiosink = gst_element_factory_make("autoaudiosink", "autoaudiosink");
	gst_bin_add_many(audiobin, queuea, convert, resample, audiosink, NULL);
	gst_element_link_many(queuea, convert, resample, audiosink, NULL);
	gpad = gst_ghost_pad_new("sink", gst_element_get_static_pad(queuea, "sink"));
	gst_element_add_pad(GST_ELEMENT(audiobin), gpad);
	
	
	videobin = gst_bin_new("videobin");
#ifdef G_OS_WIN32
	videosink = gst_element_factory_make("directdrawsink", "videosink");
#else
	videosink = gst_element_factory_make("autovideosink", "videosink");
#endif
	queuev = gst_element_factory_make("queue", "queue v");
	ffmpegrecolor = gst_element_factory_make("ffmpegcolorspace", "ffmpegcolor");
	videoscale = gst_element_factory_make("videoscale", "videoscale");
	toyunda = gst_element_factory_make("toyunda", "toyunda");
	if (toyunda == NULL)
	{
		g_print("Can't locate toyunda plugin\n");
		gst_element_set_state (pipeline, GST_STATE_NULL);
		gst_object_unref (pipeline);
		g_source_remove (watch_id);
		g_main_loop_unref (loop);
		gst_deinit();
		return 1;
	}
	gst_bin_add_many(videobin, queuev, toyunda, ffmpegrecolor, videosink, videoscale, NULL);
	gst_element_link(queuev, videoscale);
	gst_element_link(videoscale, toyunda);
	gst_element_link(toyunda, ffmpegrecolor);
	gst_element_link(ffmpegrecolor, videosink);
	gpad2 = gst_ghost_pad_new("sink", gst_element_get_static_pad(queuev, "sink"));
	gst_element_add_pad(GST_ELEMENT(videobin), gpad2);
	
	g_object_set(G_OBJECT(filesrc), "location", ag[1], NULL);
	//g_object_set(G_OBJECT(toyunda), "subfile", "C:\\toyunda\\GuiliGuili\\test karaoke dir\\Lyrics\\Air - AMV - Azurewind.txt", NULL);
	g_object_set(G_OBJECT(toyunda), "subfile", ag[2], NULL);
	
	gst_bin_add_many(GST_BIN(pipeline), videobin, audiobin, NULL);
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		GstMessage *msg;

		g_print ("Failed to start up pipeline!\n");

	/* check if there is an error message with details on the bus */
		msg = gst_bus_poll (bus, GST_MESSAGE_ERROR, 0);
		if (msg) {
			GError *err = NULL;

			gst_message_parse_error (msg, &err, NULL);
			g_print ("ERROR: %s\n", err->message);
			g_error_free (err);
			gst_message_unref (msg);
		
		}
	return -1;
	}

  g_main_loop_run (loop);

  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  g_source_remove (watch_id);
  g_main_loop_unref (loop);
  gst_deinit();
  return 0;
}
