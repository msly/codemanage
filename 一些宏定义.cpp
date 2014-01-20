# pragma warning(disable:4251) // Disable VC warning about dll linkage required (for private members?)
# pragma warning(disable:4275) // disable warning about non dll-interface base class.
# pragma warning(disable:4996) // Disable VC warning that std library may be unsafe
# pragma warning(disable:4290) // C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
# pragma warning(disable:4396) // Disable 'boost::operator !=' : the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
// boost::unordered_set triggers this.  I think it's a bug somewhere, but it doesn't
// cause any problems because the code never compares boost::unordered sets
#pragma warning(disable:4820)  // 'n' bytes padding added after data member
#pragma warning(disable:4127)  // Conditonal expression is constant (particularly in templates)
#pragma warning(disable:4100)  // Disable: unreferenced formal parameter (/W4 warning: common case for virtual methods)

