#pragma once

#include <tl/expected.hpp>

#include <cpprest/details/basic_types.h>

namespace LibRest
{

	template<class T> using Result = tl::expected<T, utility::string_t>;

}

