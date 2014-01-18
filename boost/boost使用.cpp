b2 --with-system --build-type=complete toolset=msvc-7.1

原子操作
#include <boost/interprocess/detail/atomic.hpp>


//! Compare an boost::uint32_t's value with "cmp".
//! If they are the same swap the value with "with"
//! "mem": pointer to the value
//! "with" what to swap it with
//! "cmp": the value to compare it to
//! Returns the old value of *mem
inline boost::uint32_t atomic_cas32
   (volatile boost::uint32_t *mem, boost::uint32_t with, boost::uint32_t cmp)


volatile uint32_t shutdown = 1;
// shutdown的值和1比较，如果相同，就把shutdown赋值0
// 返回值始终是shutdown原来的值
uint32_t old = boost::interprocess::ipcdetail::atomic_cas32(&shutdown, 0, 1);
cout << old << shutdown << endl;


//-----------------------------------------------------------------------------


boost整数类
#include <boost/cstdint.hpp>

using boost::uint64_t;
using boost::uint32_t;
using boost::uint16_t;
using boost::uint8_t;

using boost::int64_t;
using boost::int32_t;
using boost::int16_t;
using boost::int8_t;

//-----------------------------------------------------------------------------

时间操作
#include <boost/date_time/posix_time/posix_time.hpp>

boost::posix_time::microsec_clock::local_time();
boost::posix_time::milliseconds(1000);

//-----------------------------------------------------------------------------
boost::ptree来将解析json

string str = "{\"51\":1,\"50\":1}";
stringstream stream;
stream<<str;
ptree tree;
read_json(stream, tree);

因为ptree 下层使用的 spirit 库默认不是线程安全的 。需要在使用 include spirit 头文件的时候使用 宏 
#define BOOST_SPIRIT_THREADSAFE

//-----------------------------------------------------------------------------