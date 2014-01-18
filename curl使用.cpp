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


//=====================================================================

经典例子 说明很详细
//http://bbs.chinaunix.net/thread-3775662-1-1.html
size_t call_wirte_func(char *ptr, size_t size, size_t nmemb, std::string *stream)  
{  
    assert(stream != NULL);  
    size_t len  = size * nmemb;  
        stream->append(ptr, len);  
        //strcat( chsources, ptr );
                                
        return len;  
}  

CURL *curl;  
CURLcode code;  
std::string szbuffer;  
std::string szheader_buffer;  
char errorBuffer[CURL_ERROR_SIZE];  
std::string url = "http://www.baidu.com";  
std::string useragent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1";  
/* 
CURL_GLOBAL_ALL                //初始化所有的可能的调用。 
CURL_GLOBAL_SSL               //初始化支持 安全套接字层。 
CURL_GLOBAL_WIN32            //初始化win32套接字库。 
CURL_GLOBAL_NOTHING         //没有额外的初始化。 
*/  
curl_global_init(CURL_GLOBAL_ALL);  
curl = curl_easy_init();  
if(curl) {  
        // 远程URL，支持 http, https, ftp  
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());  
        curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent.c_str());
        // 官方下载的DLL并不支持GZIP，Accept-Encoding:deflate, gzip  
        curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");            
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//调试信息打开  
        //https 访问专用：start  
#ifdef SKIP_PEER_VERIFICATION  
        //跳过服务器SSL验证，不使用CA证书  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  
        //如果不跳过SSL验证，则可指定一个CA证书目录  
        //curl_easy_setopt(curl, CURLOPT_CAPATH, "this is ca ceat");  
#endif  
        
#ifdef SKIP_HOSTNAME_VERFICATION  
        //验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  
#endif  
        //https 访问专用：end  
        
        
        //发送cookie值给服务器  
        //curl_easy_setopt(curl, CURLOPT_COOKIE, "name1=var1; name2=var2;");   
        /* 与服务器通信交互cookie，默认在内存中，可以是不存在磁盘中的文件或留空 */  
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "./cookie.txt");   
        /* 与多个CURL或浏览器交互cookie，会在释放内存后写入磁盘文件 */  
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "./cookie.txt");   
        
        /* POST 数据 */  
        // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");  
        //设置重定向的最大次数  
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);  
        //设置301、302跳转跟随location  
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
        //抓取内容后，回调函数  
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, call_wirte_func);  
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &szbuffer);  
        //抓取头信息，回调函数  
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback );  
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);  
        
        /*  
        CURLE_OK    任务完成一切都好 
        CURLE_UNSUPPORTED_PROTOCOL  不支持的协议，由URL的头部指定 
        CURLE_COULDNT_CONNECT   不能连接到remote 主机或者代理 
        CURLE_REMOTE_ACCESS_DENIED  访问被拒绝 
        CURLE_HTTP_RETURNED_ERROR   Http返回错误 
        CURLE_READ_ERROR    读本地文件错误  
        CURLE_SSL_CACERT    访问HTTPS时需要CA证书路径 
        */  
        code = curl_easy_perform(curl);  
        if(CURLE_OK == code) {  
                double val;  
                
                /* check for bytes downloaded */  
                code = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &val);  
                if((CURLE_OK == code) && (val>0))  
                        printf("Data downloaded: %0.0f bytes.\n", val);  
                
                /* check for total download time */  
                code = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &val);  
                if((CURLE_OK == code) && (val>0))  
                        printf("Total download time: %0.3f sec.\n", val);  
                
                /* check for average download speed */  
                code = curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &val);  
                if((CURLE_OK == code) && (val>0))  
                        printf("Average download speed: %0.3f kbyte/sec.\n", val / 1024);  
                

                                        
                /* 将压缩内容保存到.gzip文件， 生成的gzip文件可以正常解压 */  
                CFile f;
                f.Open( "c:\\2.gzip", CFile::modeCreate|CFile::modeWrite );
                f.Write( (void*) szbuffer.c_str(), szbuffer.size() );
                f.Close();

                //解压缩buffer中的数据
                uLongf uDestBufferLen=1024000;//此处长度需要足够大以容纳解压缩后数据
                char* uDestBuffer=(char*)::calloc((uInt)uDestBufferLen, 1);
                int err; //错误变量的定义

                /* 用zlib的解压函数进行解压 */
                err=uncompress((Bytef*)uDestBuffer,(uLongf*)&uDestBufferLen,(Bytef*)szbuffer.c_str(),(uLongf)szbuffer.size());
                
                if(err!=Z_OK)
                {
                        cout<<"解压缩失败："<<err<<endl;  /*err总是返回Z_DATA_ERROR*/
                        return 1;
                }

        }   
        else {  
                fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);  
                // exit(EXIT_FAILURE);  
        }  
        
        /* 释放内存 */  
        curl_easy_cleanup(curl);  
}  
curl_global_cleanup(); 