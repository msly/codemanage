参数说明
http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTWRITEFUNCTION

下载字节数 总时间 下载速度 解析速度 连接速度

/* check for bytes downloaded */ 
res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &val);
if((CURLE_OK == res) && (val>0))
	printf("Data downloaded: %0.0f bytes.\n", val);

/* check for total download time */ 
res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &val);
if((CURLE_OK == res) && (val>0))
	printf("Total download time: %0.3f sec.\n", val);

/* check for average download speed */ 
res = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &val);
if((CURLE_OK == res) && (val>0))
	printf("Average download speed: %0.3f kbyte/sec.\n", val / 1024);

/* check for name resolution time */ 
res = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &val);
if((CURLE_OK == res) && (val>0))
	printf("Name lookup time: %0.3f sec.\n", val);

/* check for connect time */ 
res = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &val);
if((CURLE_OK == res) && (val>0))
	printf("Connect time: %0.3f sec.\n", val);


//====================================================================
头 和 body写文件
static const char *headerfilename = "head.out";
FILE *headerfile;
static const char *bodyfilename = "body.out";
FILE *bodyfile;

/* we want the headers be written to this file handle */ 
curl_easy_setopt(curl_handle,   CURLOPT_WRITEHEADER, headerfile);

/* we want the body be written to this file handle instead of stdout */ 
curl_easy_setopt(curl_handle,   CURLOPT_WRITEDATA, bodyfile);


//=====================================================================
接收数据写文件
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

/* send all data to this function  */ 
curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

/* we want the body be written to this file handle instead of stdout */ 
curl_easy_setopt(curl_handle,   CURLOPT_WRITEDATA, bodyfile);

//=====================================================================

显示调试信息
/* Switch on full protocol/debug output */ 
curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);