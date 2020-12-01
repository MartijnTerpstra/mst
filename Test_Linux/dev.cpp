
#if __GNUC__
#include <X11/Xlib.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <thread>

#include <mplatform.h>
#include <mcommon.h>

int main(void)
{
	const auto features = mst::platform::get_cpu_features();
	// mst::printf(L"Test %hhd, or maybe say '%ls', %.2f, %u, %c", 111, std::wstring(L"Hello there
	// 34523 45325 256 2465 1341345 1345 3154 315 135 31513"), 0.6512f, 2U, 'u');

	std::cin.get();

#if __GNUC__

	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout << "platform: " << mst::platform::full_name()) << std::endl;

	Display* display;
	Window window;
	XEvent event;
	int s;


	/* open connection with the server */
	display = XOpenDisplay(":0.0");
	if(display == NULL)
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}

	s = DefaultScreen(display);

	/* create window */
	window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 200, 200, 1,
		BlackPixel(display, s), WhitePixel(display, s));

	/* select kind of events we are interested in */
	XSelectInput(display, window, ExposureMask | KeyPressMask);

	/* map (show) the window */
	XMapWindow(display, window);

	/* event loop */
	for(;;)
	{
		XNextEvent(display, &event);

		/* draw or redraw the window */
		if(event.type == Expose)
		{
			auto os = mst::platform::full_name();

			XFillRectangle(display, window, DefaultGC(display, s), 20, 20, 10, 10);
			XDrawString(display, window, DefaultGC(display, s), 50, 50, os.c_str(), os.length());
		}
		/* exit on key press */
		if(event.type == KeyPress)
			break;
	}

	/* close connection to server */
	XCloseDisplay(display);
#else
	printf("Press enter to continue...");
	std::string s;
	std::getline(std::cin, s);
#endif
}