//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2022 Martinus Terpstra                                                 //
//                                                                                          //
//      Permission is hereby granted, free of charge, to any person obtaining a copy        //
//      of this software and associated documentation files (the "Software"), to deal       //
//      in the Software without restriction, including without limitation the rights        //
//      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell           //
//      copies of the Software, and to permit persons to whom the Software is               //
//      furnished to do so, subject to the following conditions:                            //
//                                                                                          //
//      The above copyright notice and this permission notice shall be included in          //
//      all copies or substantial portions of the Software.                                 //
//                                                                                          //
//      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          //
//      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            //
//      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE         //
//      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              //
//      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       //
//      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           //
//      THE SOFTWARE.                                                                       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mtype_traits.h>

namespace mst {

#define MST_DECLARE_PRESHIFTED_ENUM(EnumType)                                                      \
	template<>                                                                                     \
	struct ::mst::flag_traits<EnumType> : public ::mst::flag_traits_preshifted<EnumType>           \
	{ };

#define MST_DECLARE_NOT_PRESHIFTED_ENUM(EnumType)                                                  \
	template<>                                                                                     \
	struct ::mst::flag_traits<EnumType> : public ::mst::flag_traits_not_preshifted<EnumType>       \
	{ };

template<typename T>
struct flag_traits_not_preshifted
{
	typedef T value_type;
	typedef typename std::underlying_type<T>::type storage_type;

	static constexpr storage_type to_storage_type(const value_type& value) noexcept
	{
		return (storage_type(1) << static_cast<storage_type>(value));
	};

	/* initial value, default case is zero */
	static const storage_type init_value = numeric_traits<storage_type>::zero;

	/* max value, default case is the bitsize of the int type */
	static const storage_type max_value = numeric_traits<storage_type>::bits;

	static const bool preshifted = false;

}; // struct flag_traits_not_preshifted<T>

template<typename T>
struct flag_traits_preshifted
{
	typedef T value_type;
	typedef typename std::underlying_type<T>::type storage_type;

	static constexpr storage_type to_storage_type(const value_type& value) noexcept
	{
		return static_cast<storage_type>(value);
	};

	/* initial value, default case is zero */
	static const storage_type init_value = numeric_traits<storage_type>::zero;

	/* max value, default case is the maximum valie of the int type */
	static const storage_type max_value = numeric_traits<storage_type>::max;

	static const bool preshifted = true;

}; // struct flag_traits_preshifted<T>

/*
Specialize this when the enum flag style does not use the default setting
*/
template<typename T>
struct flag_traits
#if MST_USE_PRESHIFTED_FLAGS_AS_DEFAULT
	: public flag_traits_preshifted<T>
#else  // !MST_USE_PRESHIFTED_FLAGS_AS_DEFAULT
	: public flag_traits_not_preshifted<T>
#endif // MST_USE_PRESHIFTED_FLAGS_AS_DEFAULT
{ };

} // namespace mst