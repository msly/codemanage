字符替换
http://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replace(std::string& str, const std::string& from, const std::string& to) {   
    size_t start_pos = str.find(from);   
    if(start_pos == std::string::npos)   
        return false;   
    str.replace(start_pos, from.length(), to);   
    return true;   
}
std::string string("hello $name");
replace(string, "$name", "Somename");
In response to a comment, I think replaceAll would probably look something like this:
void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
--------------
字符分割
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


----------
CString转string
void ConvertCString2string(CString& strSrc,std::string& strDes) 
{ 
    #ifndef UNICODE 
    strDes = strSrc; 
     
    #else 
    USES_CONVERSION; 
     
    strDes = W2A(strSrc.LockBuffer()); 
    strSrc.UnlockBuffer(); 
    #endif 
 
}

---------
CString 多字节转unicode
#include <atlbase.h>

CString strText(_T("Hello,World!"));
USES_CONVERSION;

WCHAR* pBuf = T2W(strText.GetBuffer(strText.GetLength()));
AfxMessageBox(pBuf);
strText.ReleaseBuffer(strText.GetLength());


----------
#include <sstream>
#include <iomanip>
os << hex << setw(2) << setfill('0') << uppercase << int(password[i]);
// 将输出1234.568，已经替我们做好了四舍五入
os<<fixed<<setprecision(3)<<1234.5678;
os << showbase 显示进制 如16进制显示“0X”

----------
vector转stringstream
std::vector<Foo> v = ...;
std::ostringstream s;
std::copy(v.begin(), v.end(), std::ostream_iterator<Foo>(s));

------------
tr1 正则表达式
#include <iostream>  
#include <string>  
#include <regex>  
#include <cassert>  
using namespace std;  
int main()  
{  
    string str = "Hello world";  
    tr1::regex rx("ello");  
    // [1] regex_search  
    //assert( regex_search(str.begin(), str.end(), rx) );  
    if (regex_search(str.begin(), str.end(), rx))  
    {  
        cout<<"find it!"<<endl;// ok  
    }   
    else  
    {  
        cout<<"not find!"<<endl;  
    }  
    // [2] How Do I Do a Match? regex_match  
    str = "Hello world";  
    rx = "ello";  
    if (regex_match(str.begin(), str.end(), rx))  
    {  
        cout<<"find it!"<<endl;  
    }   
    else  
    {  
        cout<<"not find!"<<endl;// ok  
    }  
    // [3] How Do I Retrieve a Match?   
    tr1::cmatch res;  
    str = "<h2>Egg prices</h2>";  
    rx = "<h(.)>([^<]+)";  
    tr1::regex_search(str.c_str(), res, rx);  
    cout << res[1] << ". " << res[2] << "/n";// 2. Egg prices  
    // [4] How Do I Do a Replace? regex_replace  
    str = "Hello world";  
    rx = "world";  
    string replacement = "planet";  
    string str2 = std::tr1::regex_replace(str, rx, replacement);  
    cout<<str2<<endl;// Hello planet  
    // [5] How Do I Make a Regular Expression Case-insensitive?  
    str = "My name is wcdj";  
    tr1::regex rx2("WcdJ",std::tr1::regex_constants::icase);  
    //assert( regex_search(str.begin(), str.end(), rx) );  
    if (regex_search(str.begin(), str.end(), rx2))  
    {  
        cout<<"find it!"<<endl;// ok  
    }   
    else  
    {  
        cout<<"not find!"<<endl;  
    }  
    str = "My name is wcdj";  
    tr1::regex rx3("WcdJ");  
    //assert( regex_search(str.begin(), str.end(), rx) );  
    if (regex_search(str.begin(), str.end(), rx3))  
    {  
        cout<<"find it!"<<endl;  
    }   
    else  
    {  
        cout<<"not find!"<<endl;// ok  
    }  
    return 0;  
}  