#pragma once

#include <mcore.h>
#include <mtype_traits.h>
#include <type_traits>

namespace mst::concepts {

template<class T>
concept enum_type = std::is_enum_v<T>;

template<typename T>
concept integral_type = std::is_integral_v<T>;

template<typename T>
concept floating_point_type = std::is_floating_point_v<T>;

}