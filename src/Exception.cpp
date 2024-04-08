#include "gmshparsercpp/Exception.h"

namespace gmshparsercpp {

const char *
Exception::what() const noexcept
{
    return this->msg.c_str();
}

} // namespace gmshparsercpp
