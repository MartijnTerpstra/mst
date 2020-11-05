#pragma once

namespace mst {
namespace _Details {

template<typename T>
_MST_CONSTEXPR17 int32_t _Get_shift_value_cepr(T value)
{
	for(int i = 0; i < sizeof(T) * 8; ++i)
	{
		if(value >> i == 1)
			return i;
	}

	return -1;
}

template<int8_t Value>
struct _Get_shift_value_int8_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 7)))))));
	// clang-format on
};

template<uint8_t Value>
struct _Get_shift_value_uint8_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 7)))))));
	// clang-format on
};

template<int16_t Value>
struct _Get_shift_value_int16_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 15)))))))))))))));
	// clang-format on
};

template<uint16_t Value>
struct _Get_shift_value_uint16_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 15)))))))))))))));
	// clang-format on
};

template<int32_t Value>
struct _Get_shift_value_int32_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 : 
		(Value >> 30 == 1 ? 30 : 31)))))))))))))))))))))))))))))));
	// clang-format on
};

template<uint32_t Value>
struct _Get_shift_value_uint32_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 : 
		(Value >> 30 == 1 ? 30 : 31)))))))))))))))))))))))))))))));
	// clang-format on
};

template<int64_t Value>
struct _Get_shift_value_int64_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 :
		(Value >> 30 == 1 ? 30 :
		(Value >> 31 == 1 ? 31 :
		(Value >> 32 == 1 ? 32 :
		(Value >> 33 == 1 ? 33 :
		(Value >> 34 == 1 ? 34 :
		(Value >> 35 == 1 ? 35 :
		(Value >> 36 == 1 ? 36 :
		(Value >> 37 == 1 ? 37 :
		(Value >> 38 == 1 ? 38 :
		(Value >> 39 == 1 ? 39 :
		(Value >> 40 == 1 ? 40 :
		(Value >> 41 == 1 ? 41 :
		(Value >> 42 == 1 ? 42 :
		(Value >> 43 == 1 ? 43 :
		(Value >> 44 == 1 ? 44 :
		(Value >> 45 == 1 ? 45 :
		(Value >> 46 == 1 ? 46 :
		(Value >> 47 == 1 ? 47 :
		(Value >> 48 == 1 ? 48 :
		(Value >> 49 == 1 ? 49 :
		(Value >> 50 == 1 ? 50 :
		(Value >> 51 == 1 ? 51 :
		(Value >> 52 == 1 ? 52 : 
		(Value >> 53 == 1 ? 53 : 
		(Value >> 54 == 1 ? 54 : 
		(Value >> 55 == 1 ? 55 : 
		(Value >> 56 == 1 ? 56 :
		(Value >> 57 == 1 ? 57 :
		(Value >> 58 == 1 ? 58 :
		(Value >> 59 == 1 ? 59 :
		(Value >> 60 == 1 ? 60 :
		(Value >> 61 == 1 ? 61 :
		(Value >> 62 == 1 ? 62 : 63)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
	// clang-format on
};

template<uint64_t Value>
struct _Get_shift_value_uint64_t
{
	// clang-format off
	constexpr static const int32_t value = 
		(Value >> 0 == 1 ? 0 :
		(Value >> 1 == 1 ? 1 :
		(Value >> 2 == 1 ? 2 :
		(Value >> 3 == 1 ? 3 : 
		(Value >> 4 == 1 ? 4 :
		(Value >> 5 == 1 ? 5 : 
		(Value >> 6 == 1 ? 6 : 
		(Value >> 7 == 1 ? 7 : 
		(Value >> 8 == 1 ? 8 : 
		(Value >> 9 == 1 ? 9 : 
		(Value >> 10 == 1 ? 10 : 
		(Value >> 11 == 1 ? 11 : 
		(Value >> 12 == 1 ? 12 : 
		(Value >> 13 == 1 ? 13 : 
		(Value >> 14 == 1 ? 14 : 
		(Value >> 15 == 1 ? 15 : 
		(Value >> 16 == 1 ? 16 : 
		(Value >> 17 == 1 ? 17 : 
		(Value >> 18 == 1 ? 18 : 
		(Value >> 19 == 1 ? 19 : 
		(Value >> 20 == 1 ? 20 : 
		(Value >> 21 == 1 ? 21 : 
		(Value >> 22 == 1 ? 22 : 
		(Value >> 23 == 1 ? 23 : 
		(Value >> 24 == 1 ? 24 : 
		(Value >> 25 == 1 ? 25 :
		(Value >> 26 == 1 ? 26 :
		(Value >> 27 == 1 ? 27 : 
		(Value >> 28 == 1 ? 28 : 
		(Value >> 29 == 1 ? 29 :
		(Value >> 30 == 1 ? 30 :
		(Value >> 31 == 1 ? 31 :
		(Value >> 32 == 1 ? 32 :
		(Value >> 33 == 1 ? 33 :
		(Value >> 34 == 1 ? 34 :
		(Value >> 35 == 1 ? 35 :
		(Value >> 36 == 1 ? 36 :
		(Value >> 37 == 1 ? 37 :
		(Value >> 38 == 1 ? 38 :
		(Value >> 39 == 1 ? 39 :
		(Value >> 40 == 1 ? 40 :
		(Value >> 41 == 1 ? 41 :
		(Value >> 42 == 1 ? 42 :
		(Value >> 43 == 1 ? 43 :
		(Value >> 44 == 1 ? 44 :
		(Value >> 45 == 1 ? 45 :
		(Value >> 46 == 1 ? 46 :
		(Value >> 47 == 1 ? 47 :
		(Value >> 48 == 1 ? 48 :
		(Value >> 49 == 1 ? 49 :
		(Value >> 50 == 1 ? 50 :
		(Value >> 51 == 1 ? 51 :
		(Value >> 52 == 1 ? 52 : 
		(Value >> 53 == 1 ? 53 : 
		(Value >> 54 == 1 ? 54 : 
		(Value >> 55 == 1 ? 55 : 
		(Value >> 56 == 1 ? 56 :
		(Value >> 57 == 1 ? 57 :
		(Value >> 58 == 1 ? 58 :
		(Value >> 59 == 1 ? 59 :
		(Value >> 60 == 1 ? 60 :
		(Value >> 61 == 1 ? 61 :
		(Value >> 62 == 1 ? 62 : 63)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
	// clang-format on
};

template<typename T, T Value>
struct _Getshift_value_impl
{ };

template<int8_t Value>
struct _Getshift_value_impl<int8_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_int8_t<Value>::value;
};

template<uint8_t Value>
struct _Getshift_value_impl<uint8_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_uint8_t<Value>::value;
};

template<int16_t Value>
struct _Getshift_value_impl<int16_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_int16_t<Value>::value;
};

template<uint16_t Value>
struct _Getshift_value_impl<uint16_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_uint16_t<Value>::value;
};

template<int32_t Value>
struct _Getshift_value_impl<int32_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_int32_t<Value>::value;
};

template<uint32_t Value>
struct _Getshift_value_impl<uint32_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_uint32_t<Value>::value;
};

template<int64_t Value>
struct _Getshift_value_impl<int64_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_int64_t<Value>::value;
};

template<uint64_t Value>
struct _Getshift_value_impl<uint64_t, Value>
{
	constexpr static const int32_t value = _Get_shift_value_uint64_t<Value>::value;
};

#define _MST_GET_SHIFT(x) (::mst::_Details::_Getshift_value_impl<decltype(x), (x)>::value)

} // namespace _Details
} // namespace mst