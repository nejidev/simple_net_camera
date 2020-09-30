#include "HttpUpload.h"

#include "log.h"

#define TIMEOUT 30

HttpUpload::HttpUpload():m_url{""}
{
	LOG_DEBUG("HttpUpload");
}

HttpUpload::HttpUpload(string url):m_url{url},m_curl{nullptr}
{
	LOG_DEBUG("HttpUpload url:%s", m_url.c_str());
}

HttpUpload::~HttpUpload()
{
	LOG_DEBUG("~HttpUpload");
}

void HttpUpload::setUrl(string url)
{
	this->m_url = url;
}

bool HttpUpload::startUpload(string file_path)
{
	LOG_DEBUG("startUpload");

	bool ret = true;
	CURLcode curl_ret = CURLE_OK;
	struct curl_httppost *form_post = NULL;
	struct curl_httppost *form_last = NULL;

	m_curl = curl_easy_init();

	curl_formadd(&form_post, &form_last, CURLFORM_PTRNAME, "file", CURLFORM_FILE,
				file_path.c_str(), CURLFORM_END);

	curl_easy_setopt(m_curl, CURLOPT_POST,           1);
	curl_easy_setopt(m_curl, CURLOPT_URL,            this->m_url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL,       1);
	curl_easy_setopt(m_curl, CURLOPT_HEADER,         0);
	curl_easy_setopt(m_curl, CURLOPT_HTTPPOST,       form_post);
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT,        TIMEOUT);

	curl_ret = curl_easy_perform(m_curl);

	LOG_DEBUG("curl_easy_perform ret:%d", curl_ret);

	if(CURLE_OK != curl_ret)
	{
		LOG_ERROR("failed ret:%d msg:%s", curl_ret, curl_easy_strerror(curl_ret));
		ret = false;
	}

	curl_formfree(form_post);
	curl_easy_cleanup(m_curl);

	return ret;
}
