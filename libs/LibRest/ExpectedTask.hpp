#pragma once

#include "expected_task/expected_task.hpp"

#include <cpprest/details/basic_types.h>

namespace LibRest
{

	template<class T> using ExpectedTask = expected_task::expected_task<T, utility::string_t>;

}

