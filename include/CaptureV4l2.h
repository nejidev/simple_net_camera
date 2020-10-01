/**
 * @defgroup   CAPTUREV4L2 Capture V 4 l 2
 *
 * @brief      This file implements Capture V 4 l 2.
 *
 * @author     nejidev
 * @date       2020-10-01 10:37
 */

#ifndef __CAPTURE_V4L2_H__
#define __CAPTURE_V4L2_H__

#include <string>
#include <linux/videodev2.h>

using namespace std;

#define VIDEO_WIDTH  640
#define VIDEO_HEIGHT 480
#define NB_BUFFER 4

class CaptureV4l2 {

public:
	CaptureV4l2();
	~CaptureV4l2();
	bool init(const char *dev);
	bool start();
	bool stop();
	bool getFrame();
	bool putFrame();
	bool frameSaveImage(string file_path);

private:
	int                        m_fd;
	int                        m_rb_count;
	int                        m_rb_current;
	int                        m_total_bytes;
	struct v4l2_capability     m_cap;
	struct v4l2_format         m_fmt;
	struct v4l2_fmtdesc        m_desc_fmt;
	struct v4l2_requestbuffers m_rb;
	struct v4l2_buffer         m_buf;
	struct v4l2_frmsizeenum    m_frmsize;
	unsigned char *m_vide_buff[NB_BUFFER];
	int                        m_max_vide_buff_size;

};

#endif /* __CAPTURE_V4L2_H__ */
