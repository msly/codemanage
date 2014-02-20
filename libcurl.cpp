"Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/32.0.1700.76 Safari/537.36"
"Mozilla / 5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident / 6.0)"



curl_slist *headers = NULL;
headers = curl_slist_append(headers, "Host: host");
headers = curl_slist_append(headers, "Referer: referer");
headers = curl_slist_append(headers, "x-requested-with: XMLHttpRequest");
headers = curl_slist_append(headers, "Cache-Control: no-cache");



