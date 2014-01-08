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

----------
#include <sstream>
#include <iomanip>
os << hex << setw(2) << setfill('0') << uppercase << int(password[i]);
// 将输出1234.568，已经替我们做好了四舍五入
os<<fixed<<setprecision(3)<<1234.5678;
os << showbase 显示进制 如16进制显示“0X”
