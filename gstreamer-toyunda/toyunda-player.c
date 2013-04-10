#include <string.h>
#include <gst/gst.h>
#include <glib.h>
#include <glib/gstdio.h>

#include <gst/interfaces/xoverlay.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

static GstBin*		videobin;
static GstBin*		audiobin;
static GstElement*	videosink;
static GstState		pipeline_state;
static GstElement*	pipeline;
static GstElement*	filesrc;
static GstElement*	toyunda;
static GstElement*	capsfilter;

static GtkWidget*	main_window;
static GtkWidget*	video_window;
static gint		original_width = 0;
static gint		original_height = 0;
static gint		scaled_height = 0;
static gint		scaled_width = 0;

static gulong video_window_xid = 0;

void	dispose_and_exit();
static void	realize_cb(GtkWidget*, void*);


/* scale the size to the video preserving the aspect-ratio
 */
static void	calc_scaled_size(gint wanted_width, gint wanted_height, gint* m_scaled_widht, gint* m_scaled_height)
{
	float	aspect_ratio = (float) original_width / original_height;
	float 	wanted_ratio = (float) wanted_width / wanted_height;
	
	if (wanted_ratio < aspect_ratio)
	{
		*m_scaled_widht = wanted_width;
		*m_scaled_height = ((float)wanted_width / original_width) * original_height;
	}
	else
	{
		*m_scaled_height = wanted_height;
		*m_scaled_widht = ((float)wanted_height / original_height) * original_width;
	}
}

static GstBusSyncReply
xoverlay_cb (GstBus * bus, GstMessage * message, GstPipeline * pipeline)
{
	bus = bus;
	pipeline = pipeline;
 // ignore anything but 'prepare-xwindow-id' element messages
	if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
		return GST_BUS_PASS;

	if (!gst_structure_has_name (message->structure, "prepare-xwindow-id"))
		return GST_BUS_PASS;
	
	if (video_window_xid != 0) {
		GstXOverlay *xoverlay;

		// GST_MESSAGE_SRC (message) will be the video sink element
		xoverlay = GST_X_OVERLAY (GST_MESSAGE_SRC (message));
		if (xoverlay == NULL)
			dispose_and_exit();
		gst_x_overlay_set_window_handle (xoverlay, video_window_xid);
		//gst_x_overlay_set_render_rectangle(xoverlay, 0, 0, 800, 600 );
	} else {
		g_warning ("Should have obtained video_window_xid by now!");
	}
	g_object_set (G_OBJECT (GST_MESSAGE_SRC(message)), "force-aspect-ratio", TRUE, NULL);
	gst_message_unref (message);
	return GST_BUS_DROP;
}

static void	close_win_cb(GtkWidget *wid, void *data)
{
	wid = wid;
	data = data;
	dispose_and_exit();
}

static gboolean
key_press_event_cb(GtkWidget *widget, GdkEventKey *event, gpointer data)	
{	
	GstCaps*	new_caps;
	gboolean is_fullscreen;

	data = data;
	widget = widget;
	if (event->keyval == 'f')
	{	    
		g_print("Toggling fullscreen\n");		
		is_fullscreen = (gdk_window_get_state(GDK_WINDOW(widget->window)) == GDK_WINDOW_STATE_FULLSCREEN);		
		if (is_fullscreen)
		{
			gtk_window_unfullscreen(GTK_WINDOW(widget));
			new_caps = gst_caps_new_simple	("video/x-raw-yuv",
						"width", G_TYPE_INT, scaled_width,
						"height", G_TYPE_INT, scaled_width,
						NULL);
			g_object_set(G_OBJECT(capsfilter), "caps", new_caps, NULL);
			gst_caps_unref(new_caps);
			gtk_widget_set_size_request(video_window, 800, 600);
			gtk_container_resize_children(GTK_CONTAINER(main_window));
		}
		else	
			gtk_window_fullscreen(GTK_WINDOW(widget));
	}
	if (event->keyval == 'q' || event->keyval == GDK_KEY_Escape)
		dispose_and_exit();
	if (event->keyval == GDK_space)
	{
		if (pipeline_state == GST_STATE_PAUSED)
			gst_element_set_state (pipeline, GST_STATE_PLAYING);
		else
			gst_element_set_state (pipeline, GST_STATE_PAUSED);
	}
	return TRUE;
}
	
static gboolean	event_config_cb(GtkWidget* wid, GdkEventConfigure* event, gpointer data)
{
	static gint old_width, old_height = 0;
	gint	new_height, new_width;
	GstCaps*	new_caps;
	GtkAllocation* al;

	wid = wid;
	data = data;
	//g_printf("____________________Size changed : %dx%d\n", event->width, event->height);
	if (event->height != old_height || event->width != old_width)
	{
		//g_printf("=================Size changed : %dx%d\n", event->width, event->height);
		calc_scaled_size(event->width, event->height, &new_width, &new_height);
		//g_printf("=================Scaled new size : %dx%d\n", new_width, new_height);
		new_caps = gst_caps_new_simple	("video/x-raw-yuv",
					"width", G_TYPE_INT, new_width,
					"height", G_TYPE_INT, new_height,
					NULL);
		g_object_set(G_OBJECT(capsfilter), "caps", new_caps, NULL);
		gst_caps_unref(new_caps);
		old_height = event->height;
		old_width = event->width;
		al = g_new(GtkAllocation, 1);
		al->x = 0;
		al->y = 0;
		al->width = event->width;
		al->height = event->height;
		gtk_widget_size_allocate(video_window, al);
		gtk_container_resize_children(GTK_CONTAINER(main_window));
		g_free(al);
	}
	return TRUE;
}

static void	create_ui()
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file("toyunda.tga", &error);

	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	video_window = gtk_drawing_area_new();
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	else
		gtk_window_set_icon(GTK_WINDOW(main_window), pixbuf);
	gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
	gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
	gtk_widget_set_size_request(video_window, 800, 600);
	gtk_widget_set_double_buffered (video_window, FALSE);
	gtk_container_add (GTK_CONTAINER (main_window), video_window);

	g_signal_connect (video_window, "realize", G_CALLBACK (realize_cb), NULL);
	g_signal_connect(main_window, "destroy", G_CALLBACK(close_win_cb), NULL);
	g_signal_connect(main_window, "configure-event", G_CALLBACK(event_config_cb), NULL);
	gtk_widget_set_events(main_window, GDK_KEY_PRESS_MASK);
	g_signal_connect(G_OBJECT(main_window), "key-press-event", G_CALLBACK(key_press_event_cb), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "GSToyunda Player");

	gtk_widget_show_all(main_window);	
}

static void realize_cb (GtkWidget *widget, void *data) {
	GdkWindow *window = gtk_widget_get_window (widget);
	guintptr window_handle;
	
	data = data;
	if (!gdk_window_ensure_native (window))
		g_error ("Couldn't create native window needed for GstXOverlay!");
	
	/* Retrieve window handler from GDK */
	#if defined (GDK_WINDOWING_WIN32)
	window_handle = (guintptr)GDK_WINDOW_HWND (window);
	#elif defined (GDK_WINDOWING_QUARTZ)
	window_handle = gdk_quartz_window_get_nsview (window);
	#elif defined (GDK_WINDOWING_X11)
	window_handle = GDK_WINDOW_XID (window);
	#endif
	video_window_xid = window_handle;
}


static gboolean
bus_call (GstBus     *bus,
	  GstMessage *msg,
	  gpointer  data)
{
	bus = bus;
	data = data;
  switch (GST_MESSAGE_TYPE (msg)) {
	  case GST_MESSAGE_STATE_CHANGED:
		{
			GstState	old;
			GstState	news;
			GstState	pending;
			gst_message_parse_state_changed(msg, &old, &news, &pending);
			pipeline_state = news;
		}
		break;
	case GST_MESSAGE_EOS:
		g_print ("End-of-stream\n");
		dispose_and_exit();
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
		dispose_and_exit();
		break;
	}
	default:
	break;
	}

  return TRUE;
}
void	on_autoplug_continue(GstElement* object,
                                                 GstPad* pad,
                                                 GstCaps* caps,
                                                 gpointer user_data)
{
	GstStructure*	str;
	GstCaps*	new_caps;
	pad = pad;
	object = object;
	user_data = user_data;

	str = gst_caps_get_structure(caps, 0);
	
	if (!gst_structure_get_int (str, "width", &original_width) ||
		!gst_structure_get_int (str, "height", &original_height))
		g_print ("No width/height available\n");
	else
	{
		g_printf("Width : %d, Height : %d\n", original_width, original_height);
		calc_scaled_size(800, 600, &scaled_width, &scaled_height);
		//g_printf("Width : %d, Height : %d\n",scaled_width, scaled_height);
		new_caps = gst_caps_new_simple	("video/x-raw-yuv",
					"width", G_TYPE_INT, scaled_width,
					"height", G_TYPE_INT, scaled_height,
					NULL);
		g_object_set(G_OBJECT(capsfilter), "caps", new_caps, NULL);
		gst_caps_unref(new_caps);
		
	}
}



void	on_new_decoded_pad(GstElement *elem, GstPad	*pad, gboolean b, gpointer *data)
{
	GstCaps*	caps;
	GstStructure*	str;
	gchar*		name;
	GstPad		*audiopad, *videopad;

	b = b;
	data = data;
	elem = elem;
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

void	gstreamer_create_pipeline()
{
	GstElement	*decoder, *audiosink, *queuea, *queuev;
	GstElement	*convert, *resample, *ffmpegrecolor, *videoscale;
	GstBus*		bus;
	GstPad*		gpad, *gpad2;
	guint		watch_id;

	pipeline = gst_pipeline_new("toyundapipeline");
	bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
	watch_id = gst_bus_add_watch (bus, bus_call, NULL);

	filesrc  = gst_element_factory_make ("filesrc", "file source");
	decoder  = gst_element_factory_make ("decodebin2", "decodebin2");
	g_signal_connect(decoder, "new-decoded-pad", G_CALLBACK(on_new_decoded_pad), NULL);
	g_signal_connect(decoder, "autoplug-continue", G_CALLBACK(on_autoplug_continue), NULL);
	gst_bin_add_many(GST_BIN(pipeline), filesrc, decoder, NULL);
	gst_element_link(filesrc, decoder);
	gst_bus_set_sync_handler (bus, (GstBusSyncHandler) xoverlay_cb, pipeline);
	gst_object_unref (bus);
	
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
	capsfilter = gst_element_factory_make("capsfilter", "capsfilter");
	toyunda = gst_element_factory_make("toyunda", "toyunda");
	if (toyunda == NULL)
	{
		g_print("Can't locate toyunda plugin\n");
		dispose_and_exit();
	}
	gst_bin_add_many(videobin, queuev, capsfilter, toyunda, ffmpegrecolor, videosink, videoscale, NULL);
	//gst_bin_add_many(videobin, queuev, ffmpegrecolor, videosink, videoscale, NULL);
	gst_element_link(queuev, videoscale);
	gst_element_link(videoscale, capsfilter);
	gst_element_link(capsfilter, toyunda);
	gst_element_link(toyunda, ffmpegrecolor);
	//gst_element_link(videoscale, ffmpegrecolor);
	gst_element_link(ffmpegrecolor, videosink);
	gpad2 = gst_ghost_pad_new("sink", gst_element_get_static_pad(queuev, "sink"));
	gst_element_add_pad(GST_ELEMENT(videobin), gpad2);
	
	
	gst_bin_add_many(GST_BIN(pipeline), GST_ELEMENT(videobin), GST_ELEMENT(audiobin), NULL);
}

void	gst_start(char *video, char * sub)
{
	GstStateChange	ret;
	char*		tmpstr;
	GFile		*fi;

	g_object_set(G_OBJECT(filesrc), "location", video, NULL);
	g_object_set(G_OBJECT(toyunda), "subfile", sub, NULL);
	
	ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		GstMessage *msg;

		g_print ("Failed to start up pipeline!\n");

		/* check if there is an error message with details on the bus */
		msg = gst_bus_poll(gst_pipeline_get_bus(GST_PIPELINE(pipeline)), GST_MESSAGE_ERROR, 0);
		if (msg) {
			GError *err = NULL;

			gst_message_parse_error (msg, &err, NULL);
			g_print ("ERROR: %s\n", err->message);
			g_error_free (err);
			gst_message_unref (msg);
			
		}
		dispose_and_exit();
	}
	fi = g_file_parse_name(sub);

	tmpstr = g_new(char, strlen("GSToyunda Player") + strlen(g_file_get_basename(fi)) + 5);
	strcpy(tmpstr, "GSToyunda Player : ");
	strcat(tmpstr, g_file_get_basename(fi));
	gtk_window_set_title(GTK_WINDOW(main_window), tmpstr);
	g_free(tmpstr);
}

int	main(int ac, char *ag[])
{
	//putenv("GST_PLUGIN_PATH=C:\\toyunda\\build\\");

	gtk_init(&ac, &ag);
	gst_init(&ac, &ag);
	
	
	create_ui();
	gstreamer_create_pipeline();
	gst_start(ag[1], ag[2]);
	gtk_main();
	dispose_and_exit();
	return 0;
}

void	dispose_and_exit()
{
	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref (pipeline);
	gst_deinit();
	exit(0);
}