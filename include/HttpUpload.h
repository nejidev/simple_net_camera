/**
 * @defgroup   HTTPUPLOAD Http Upload
 *
 * @brief      This file implements Http Upload.
 *
 * @author     nejidev
 * @date       2020-10-01 10:37
 */

#ifndef __HTTP_UPLOAD_H__
#define __HTTP_UPLOAD_H__

#include <string>

#ifdef __cplusplus
extern "C"
{
#endif
#include <curl/curl.h>

#ifdef __cplusplus
}
#endif

using namespace std;

class HttpUpload {

public:
	HttpUpload();
	~HttpUpload();
	HttpUpload(string url);
	void setUrl(string url);
	bool startUpload(string file_path);

private:
	string m_url;
	CURL *m_curl;

};

#endif /* __HTTP_UPLOAD_H__ */
