/**
 * @defgroup   MAIN main
 *
 * @brief      This file implements main.
 *
 * @author     nejidev
 * @date       2020-10-01 10:38
 */
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>

#include "HttpUpload.h"
#include "CaptureV4l2.h"
#include "log.h"

#define FRAME_LIMIT 30

using namespace std;

bool thread_runing = true;

void capture_run(const char *dev, const char *url)
{
	int i = -1;
	CaptureV4l2 cap;
	HttpUpload httpUpload;

	if(! cap.init(dev) || ! cap.start())
	{
		LOG_ERROR("capture init failed");
		exit(-1);
	}

	if(0 < strlen(url))
	{
		httpUpload.setUrl(url);
	}

        while(thread_runing)
        {
		this_thread::sleep_for(std::chrono::seconds(1));
                i++;

		if(0 < i % FRAME_LIMIT) continue;

                auto time = chrono::system_clock::to_time_t(chrono::system_clock::now());
                stringstream file_name;

                file_name<<put_time(localtime(&time),"%Y-%m-%d-%H-%M-%S")<<".jpg";
                cap.getFrame();
                cap.frameSaveImage(file_name.str());

		if(0 < strlen(url))
        	{
        		httpUpload.startUpload(file_name.str());
        	}

                LOG_DEBUG("save:%s", file_name.str().c_str());
        }
}

int main(int argc, char **argv)
{
	char dev[256] = {0};
	char url[256] = {0};

	strcpy(dev, "/dev/video0");

	if(2 <= argc)
	{
		strcpy(dev, argv[1]);
	}
	if (3 == argc)
	{
		snprintf(url, sizeof(url), "%s", argv[2]);
	}

	LOG_DEBUG("capture dev:%s upload url:%s", dev, url);

	thread capture(capture_run, dev, url);
	string cmd;

	while(true)
	{
		getline(cin, cmd);
		if("q" == cmd) break;
	}

	LOG_DEBUG("wait thread quit");

	thread_runing = false;
	capture.join();

	return 0;
}
