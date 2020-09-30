#include "main.h"

#include "HttpUpload.h"
#include "CaptureV4l2.h"

#include <stdio.h>
#include <unistd.h>

CaptureV4l2 cap;
HttpUpload httpUpload;

int main(int argc, char **argv)
{
	httpUpload.setUrl("http://localhost:1080/cap_upload.php");
	//httpUpload.startUpload("1.jpg");

	cap.init();
	cap.start();

	while(true)
	{
		cap.getFrame();
		sleep(1);
	}

	return 0;
}
