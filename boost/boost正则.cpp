

// -------------------------
using namespace boost::xpressive;

sregex srx = sregex::compile("日期:(\\d+)年(\\d+)月(\\d+)日");
smatch what;
std::string nDate;
if (regex_search(content, what, srx))
{
    nDate.append(what[1]);
    nDate.append("-");
    nDate.append(what[2]);
    nDate.append("-");
    nDate.append(what[3]);

    std::cout << what[0] << std::endl;
    std::cout << what[1] << std::endl;
    std::cout << what[2] << std::endl;
    std::cout << what[3] << std::endl;
}

// -------------------------
using namespace boost::xpressive;

sregex_iterator cur(str.begin(), str.end(), g_srx_list_1);
sregex_iterator end;
smatch match_1;
for (; cur!=end; ++cur)
{
    match_1 = *cur;
    for (size_t i=1; i<match_1.size(); ++i)
    {
       std::cout << match_1[i] << std::endl;
    }
}