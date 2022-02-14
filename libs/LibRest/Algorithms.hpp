#pragma once

#include <map>

#include <boost/optional.hpp>

namespace LibRest::Algorithms
{

template <class Map, class Key> auto mapFind(const Map& map, const Key& key)
{
    using ValueType = std::decay_t<decltype(map.at(key))>;
    using ReturnType = boost::optional<ValueType>;
    const auto it = map.find(key);
    if(it == std::end(map))
        return ReturnType{boost::none};
    else
        return ReturnType{it->second};
}

template <class FCT> auto applyToTuple(FCT fct)
{
    return [f = std::move(fct)](auto tuple) { return std::apply(f, std::move(tuple)); };
}

} // namespace LibRest::Algorithms
