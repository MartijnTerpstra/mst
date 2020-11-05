//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mcore.h>
#include <emmintrin.h>
#include <mx_string_utils.h>
#include <mx_uuid.h>
#include <cstring>

namespace mst {

class alignas(16) uuid
{
public:
	inline uuid() = default;
	inline uuid(uint8_t _B0, uint8_t _B1, uint8_t _B2, uint8_t _B3, uint8_t _B4, uint8_t _B5,
		uint8_t _B6, uint8_t _B7, uint8_t _B8, uint8_t _B9, uint8_t _B10, uint8_t _B11,
		uint8_t _B12, uint8_t _B13, uint8_t _B14, uint8_t _B15) noexcept
	{
		uint8_t _data[16]{ _B0, _B1, _B2, _B3, _B4, _B5, _B6, _B7, _B8, _B9, _B10, _B11, _B12, _B13,
			_B14, _B15 };
		memcpy(&_Mydata, _data, sizeof(_Mydata));

		static_assert(sizeof(_data) == sizeof(_Mydata), "Invalid data size");
	}

	inline explicit uuid(__m128i data) noexcept
		: _Mydata(data)
	{ }

	static uuid create() noexcept
	{
		uuid _retval;
		_Details::uuid_helper::generate(_retval._Mydata);
		return _retval;
	}

#if _MST_HAS_INLINE_VARIABLES
	static const uuid zero;
#endif

	inline bool operator==(const uuid& _Other) const noexcept
	{
		const auto _cmp = _mm_cmpeq_epi32(_Mydata, _Other._Mydata);
		return _mm_movemask_epi8(_cmp) == 0xFFFF;
	}

	inline bool operator!=(const uuid& _Other) const noexcept
	{
		const auto _cmp = _mm_cmpeq_epi32(_Mydata, _Other._Mydata);
		return _mm_movemask_epi8(_cmp) != 0xFFFF;
	}

	inline ::std::string to_string() const
	{
		return _Details::uuid_helper::to_string(_Mydata);
	}

	inline static bool try_parse(_Details::string_view_type _Str, uuid& _Out_value) noexcept
	{
		if(_Str.length() != 36) // _MST_UNLIKELY
			return false;

		return _Details::uuid_helper::try_parse(_Str.data(), _Out_value._Mydata);
	}

	inline static uuid parse(_Details::string_view_type _Str) noexcept
	{
		if(_Str.length() < 36) // _MST_UNLIKELY
			std::abort();

		uuid retval;
		retval._Mydata = _Details::uuid_helper::parse(_Str.data());
		return retval;
	}

private:
	__m128i _Mydata;
	// static_assert(alignof(_Mydata) == 16, "uuid should be have 16 bytes aligned data");
	static_assert(sizeof(_Mydata) == 16, "uuid should be have 16 bytes of data");
};

#if _MST_HAS_INLINE_VARIABLES
inline const uuid uuid::zero = uuid(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#endif

inline uuid create_zero_uuid() noexcept
{
	return uuid(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

} // namespace mst
