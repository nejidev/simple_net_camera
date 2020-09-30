#include "CaptureV4l2.h"

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include <poll.h>

#include "log.h"

CaptureV4l2::CaptureV4l2():m_fd{-1}
{
	LOG_DEBUG("CaptureV4l2");
}

CaptureV4l2::~CaptureV4l2()
{
	int i = 0;

	LOG_DEBUG("~CaptureV4l2");

	for(i = 0; i < m_rb_count; i++)
	{
		if(m_vide_buff[i])
		{
			munmap(m_vide_buff[i], m_max_vide_buff_size);
			m_vide_buff[i] = NULL;
		}
	}

	if(-1 != m_fd)
	{
		close(m_fd);
	}
}

bool CaptureV4l2::init()
{
	int ret = 0;
	int i   = 0;

	LOG_DEBUG("init");

	m_fd = open(VIDEO_DEV, O_RDWR);

	if(-1 == m_fd)
	{
		LOG_ERROR("%s open failed", VIDEO_DEV);
		return false;
	}

	memset(&m_cap, 0, sizeof(m_cap));
	if(-1 == ioctl(m_fd, VIDIOC_QUERYCAP, &m_cap))
	{
		LOG_ERROR("%s ioctl failed", VIDEO_DEV);
		return false;
	}

	if(0 == (m_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
	{
		LOG_ERROR("video capture not supported");
		return false;
	}

	if(m_cap.capabilities & V4L2_CAP_STREAMING)
	{
		LOG_DEBUG("support streaming i/o");
	}

	if(m_cap.capabilities & V4L2_CAP_READWRITE) 
	{
		LOG_DEBUG("support read i/o");
	}

	//support format
	memset(&m_desc_fmt, 0, sizeof(m_desc_fmt));
	m_desc_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	while(0 <= (ret = ioctl(m_fd, VIDIOC_ENUM_FMT, &m_desc_fmt))) 
	{
		LOG_DEBUG("pixelformat: 0x%x", m_desc_fmt.pixelformat);

		if(V4L2_PIX_FMT_MJPEG == m_desc_fmt.pixelformat)
		{
			LOG_DEBUG("support pixelformat MJPEG");
		}

		if(V4L2_PIX_FMT_RGB565 == m_desc_fmt.pixelformat)
		{
			LOG_DEBUG("support pixelformat RGB565");
		}

		if(V4L2_PIX_FMT_YUYV == m_desc_fmt.pixelformat)
		{
			LOG_DEBUG("support pixelformat YUYV");
		}

		m_frmsize.pixel_format = m_desc_fmt.pixelformat;
		m_frmsize.index = 0;
		while(0 <= ioctl(m_fd, VIDIOC_ENUM_FRAMESIZES, &m_frmsize))
		{
			if(V4L2_FRMSIZE_TYPE_DISCRETE == m_frmsize.type ||
				V4L2_FRMSIZE_TYPE_STEPWISE == m_frmsize.type)
			{
				LOG_DEBUG("support frmsize:%d-%d",
						m_frmsize.discrete.width, m_frmsize.discrete.height);
			}
			m_frmsize.index++;
		}

		m_desc_fmt.index++;
	}

	//set format
	memset(&m_fmt, 0, sizeof(m_fmt));
	m_fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_fmt.fmt.pix.width       = VIDEO_WIDTH;
	m_fmt.fmt.pix.height      = VIDEO_HEIGHT;
	m_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; 
	m_fmt.fmt.pix.field       = V4L2_FIELD_ANY;

	ret = ioctl(m_fd, VIDIOC_S_FMT, &m_fmt);

	if(0 > ret)
	{
		LOG_ERROR("%s Unable to set format", VIDEO_DEV);
		return false;
	}

	memset(&m_rb, 0, sizeof(m_rb));
	m_rb.count  = NB_BUFFER;
	m_rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_rb.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(m_fd, VIDIOC_REQBUFS, &m_rb);

	if(0 > ret)
	{
		LOG_ERROR("Unable to allocate buffers");
		return false;
	}

	m_rb_count = NB_BUFFER;

	if(m_cap.capabilities & V4L2_CAP_STREAMING)
	{
		LOG_DEBUG("support cap stream");

		/* map the buffers */
		for(i = 0; i < NB_BUFFER; i++)
		{
			memset(&m_buf, 0, sizeof(m_buf));
			m_buf.index  = i;
			m_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			m_buf.memory = V4L2_MEMORY_MMAP;

			ret = ioctl(m_fd, VIDIOC_QUERYBUF, &m_buf);

			if(0 > ret)
			{
				LOG_ERROR("Unable to query buffer");
				return false;
			}

			m_max_vide_buff_size = m_buf.length;
			m_vide_buff[i] = (unsigned char *)mmap(0 /* start anywhere */ ,
					m_buf.length, PROT_READ, MAP_SHARED, m_fd, m_buf.m.offset);

			if(MAP_FAILED == m_vide_buff[i])
			{
				LOG_ERROR("Unable to map buffer");
				return false;
			}
		}

		/* Queue the buffers. */
		for(i = 0; i < NB_BUFFER; ++i)
		{
			memset(&m_buf, 0, sizeof(m_buf));
			m_buf.index  = i;
			m_buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			m_buf.memory = V4L2_MEMORY_MMAP;

			ret = ioctl(m_fd, VIDIOC_QBUF, &m_buf);

			if(0 > ret)
			{
				LOG_ERROR("Unable to queue buffer");
				return false;
			}
		}
	}
	else if(m_cap.capabilities & V4L2_CAP_READWRITE)
	{
		LOG_ERROR("support cap RW");
		return false;
	}

	return true;
}

bool CaptureV4l2::start()
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(m_fd, VIDIOC_STREAMON, &type)) 
	{
		LOG_ERROR("Unable to start capture");
		return false;
	}

	return true;
}

bool CaptureV4l2::stop()
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(ioctl(m_fd, VIDIOC_STREAMOFF, &type)) 
	{
		LOG_ERROR("Unable to stop capture");
		return false;
	}

	return true;
}

bool CaptureV4l2::getFrame()
{
	struct pollfd fds[1];
	int ret = 0;
	struct v4l2_buffer buffer;

	/* poll */
	fds[0].fd     = m_fd;
	fds[0].events = POLLIN;

	ret = poll(fds, 1, -1);

	if(0 >= ret)
	{
		LOG_ERROR("poll error!");
		return false;
	}

	/* VIDIOC_DQBUF */
	memset(&buffer, 0, sizeof(buffer));
	buffer.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buffer.memory = V4L2_MEMORY_MMAP;

	if(0 > ioctl(m_fd, VIDIOC_DQBUF, &buffer)) 
	{
		LOG_ERROR("Unable to dequeue buffer");
		return false;
	}

	m_rb_current = buffer.index;
	m_total_bytes = buffer.bytesused;

	return true;
}
