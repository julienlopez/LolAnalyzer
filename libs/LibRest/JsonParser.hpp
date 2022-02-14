#pragma once

#include "Algorithms.hpp"
#include "Result.hpp"

#include <expected_task/expected_optional_bridge.hpp>

#include <cpprest/json.h>

namespace LibRest::JsonParser
{

	template <class T> inline Result<T> cast(const web::json::value& value)
	{
		return tl::make_unexpected(L"unsupported type");
	}

	template <> Result<std::size_t> inline cast<std::size_t>(const web::json::value& value)
	{
		if (value.is_number())
			return value.as_number().to_uint64();
		else
			return tl::make_unexpected(L"not the right type : number");
	}

	template <> Result<utility::string_t> inline cast<utility::string_t>(const web::json::value& value)
	{
		if (value.is_string())
			return value.as_string();
		else
			return tl::make_unexpected(L"not the right type : string");
	}

	template <class T>
	Result<std::tuple<T>> parseSimpleObject(const web::json::object& object,
		const utility::string_t& name)
	{
		return expected_task::from_optional(Algorithms::mapFind(object, name),
			U("Unable to find ") + name)
			.and_then(&cast<T>);
	}

	template <class TYPE1, class... TYPES, class... STRS>
	Result<std::tuple<TYPE1, TYPES...>> parseSimpleObject(const web::json::object& object,
		const utility::string_t& name1, STRS... names)
	{
		static_assert(sizeof...(TYPES) == sizeof...(STRS), "must be as many types as there are strings");
		auto res1 = parseSimpleObject<TYPE1>(object, name1);
		if (!res1) return tl::make_unexpected(res1.error());
		auto res2 = parseSimpleObject<TYPES...>(object, names...);
		if (!res2) return tl::make_unexpected(res2.error());
		return std::tuple_cat(std::move(*res1), std::move(*res2));
	}

	template <class TYPE1, class... TYPES, class... STRS>
    Result<std::tuple<TYPE1, TYPES...>> parseSimpleObject(const web::json::value& json,
		const utility::string_t& name1, STRS... names)
	{
        if(json.is_object())
            return parseSimpleObject<TYPE1, TYPES...>(json.as_object(), name1, names...);
		else return tl::make_unexpected(U("json is not an object"));
	}

} // namespace LibHttp::JsonParser
