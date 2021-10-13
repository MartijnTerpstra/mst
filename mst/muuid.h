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

#include <mcore.h>
#include <mx_string_utils.h>
#include <mx_uuid.h>
#include <cstring>
#include <string>

#if MST_UUID_NO_SIMD
#define _MST_UUID_SIMD_ENABLED 0
#elif MST_NO_SIMD
#define _MST_UUID_SIMD_ENABLED 0
#elif _MST_HAS_ARM
#define _MST_UUID_SIMD_ENABLED 0
#else
#define _MST_UUID_SIMD_ENABLED 1
#include <emmintrin.h>
#endif

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
		std::memcpy(&_Mydata, _data, sizeof(_Mydata));

		static_assert(sizeof(_data) == sizeof(_Mydata), "Invalid data size");
	}

#if _MST_UUID_SIMD_ENABLED
	inline explicit uuid(__m128i data) noexcept
		: _Mydata(data)
	{ }

	inline explicit uuid(const std::array<uint8_t, 16>& data) noexcept
	{
		std::memcpy(&_Mydata, data.data(), sizeof(_Mydata));

		static_assert(sizeof(data) == sizeof(_Mydata), "Invalid data size");
	}

#else

	inline explicit uuid(const std::array<uint8_t, 16>& data) noexcept
		: _Mydata(data)
	{ }

#endif

	static uuid create() noexcept
	{
		return uuid{ _Details::uuid_helper::generate() };
	}

#if _MST_HAS_INLINE_VARIABLES
	static const uuid zero;
#endif

	inline ::std::string to_string() const
	{
#if _MST_UUID_SIMD_ENABLED
		std::array<uint8_t, 16> bytes;
		memcpy(bytes.data(), &_Mydata, sizeof(bytes));
		return _Details::uuid_helper::to_string(bytes);
#else
		return _Details::uuid_helper::to_string(_Mydata);
#endif
	}

	inline bool operator==(const uuid& _Other) const noexcept
	{
#if _MST_UUID_SIMD_ENABLED
		const auto _cmp = _mm_cmpeq_epi32(_Mydata, _Other._Mydata);
		return _mm_movemask_epi8(_cmp) == 0xFFFF;
#else
		return _Mydata == _Other._Mydata;
#endif
	}

	inline bool operator!=(const uuid& _Other) const noexcept
	{
#if _MST_UUID_SIMD_ENABLED
		const auto _cmp = _mm_cmpeq_epi32(_Mydata, _Other._Mydata);
		return _mm_movemask_epi8(_cmp) != 0xFFFF;
#else
		return _Mydata != _Other._Mydata;
#endif
	}

	inline static bool try_parse(_Details::string_view_type _Str, uuid& _Out_value) noexcept
	{
		if(_Str.length() != 36) // _MST_UNLIKELY
			return false;

		for(auto index : { 8, 13, 18, 23 })
		{
			if(_Str[index] != '-')
				return false;
		}

#if _MST_UUID_SIMD_ENABLED
		const auto zeroChar = _mm_set1_epi8('0');

		static const __m128i chars[][3] = {
			{ _mm_set1_epi8('1'), _mm_set1_epi8(static_cast<char>(0x10)), _mm_set1_epi8(0x1) },
			{ _mm_set1_epi8('2'), _mm_set1_epi8(static_cast<char>(0x20)), _mm_set1_epi8(0x2) },
			{ _mm_set1_epi8('3'), _mm_set1_epi8(static_cast<char>(0x30)), _mm_set1_epi8(0x3) },
			{ _mm_set1_epi8('4'), _mm_set1_epi8(static_cast<char>(0x40)), _mm_set1_epi8(0x4) },
			{ _mm_set1_epi8('5'), _mm_set1_epi8(static_cast<char>(0x50)), _mm_set1_epi8(0x5) },
			{ _mm_set1_epi8('6'), _mm_set1_epi8(static_cast<char>(0x60)), _mm_set1_epi8(0x6) },
			{ _mm_set1_epi8('7'), _mm_set1_epi8(static_cast<char>(0x70)), _mm_set1_epi8(0x7) },
			{ _mm_set1_epi8('8'), _mm_set1_epi8(static_cast<char>(0x80)), _mm_set1_epi8(0x8) },
			{ _mm_set1_epi8('9'), _mm_set1_epi8(static_cast<char>(0x90)), _mm_set1_epi8(0x9) },
			{ _mm_set1_epi8('A'), _mm_set1_epi8(static_cast<char>(0xA0)), _mm_set1_epi8(0xA) },
			{ _mm_set1_epi8('B'), _mm_set1_epi8(static_cast<char>(0xB0)), _mm_set1_epi8(0xB) },
			{ _mm_set1_epi8('C'), _mm_set1_epi8(static_cast<char>(0xC0)), _mm_set1_epi8(0xC) },
			{ _mm_set1_epi8('D'), _mm_set1_epi8(static_cast<char>(0xD0)), _mm_set1_epi8(0xD) },
			{ _mm_set1_epi8('E'), _mm_set1_epi8(static_cast<char>(0xE0)), _mm_set1_epi8(0xE) },
			{ _mm_set1_epi8('F'), _mm_set1_epi8(static_cast<char>(0xF0)), _mm_set1_epi8(0xF) },
		};

		const auto hipart = _mm_set_epi8(_Str[34], _Str[32], _Str[30], _Str[28], _Str[26], _Str[24],
			_Str[21], _Str[19], _Str[16], _Str[14], _Str[11], _Str[9], _Str[6], _Str[4], _Str[2],
			_Str[0]);
		const auto lopart = _mm_set_epi8(_Str[35], _Str[33], _Str[31], _Str[29], _Str[27], _Str[25],
			_Str[22], _Str[20], _Str[17], _Str[15], _Str[12], _Str[10], _Str[7], _Str[5], _Str[3],
			_Str[1]);

		auto result = _mm_setzero_si128();
		auto has_any_result_hi = _mm_cmpeq_epi8(hipart, zeroChar);
		auto has_any_result_lo = _mm_cmpeq_epi8(lopart, zeroChar);

		for(const auto& _c : chars)
		{
			const auto hieq = _mm_cmpeq_epi8(hipart, _c[0]);
			const auto himask = _mm_and_si128(hieq, _c[1]);

			has_any_result_hi = _mm_or_si128(has_any_result_hi, hieq);
			result = _mm_or_si128(result, himask);

			const auto loeq = _mm_cmpeq_epi8(lopart, _c[0]);
			const auto lomask = _mm_and_si128(loeq, _c[2]);

			has_any_result_lo = _mm_or_si128(has_any_result_lo, loeq);
			result = _mm_or_si128(result, lomask);
		}

		_Out_value._Mydata = result;
		const auto combined_has_any = _mm_and_si128(has_any_result_lo, has_any_result_hi);
		return _mm_movemask_epi8(combined_has_any) == 0xFFFF;
#else
		constexpr int8_t indices[32]{ 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 17, 19, 20,
			21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

		for(auto index : indices)
		{
			if(_Str[index] < '0' && _Str[index] > '9' && _Str[index] < 'A' && _Str[index] > 'Z')
				return false;
		}

		constexpr uint8_t hi['F' - '0' + 1]{ 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
			0x90, 255, 255, 255, 255, 255, 255, 255, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 };

		constexpr uint8_t lo['F' - '0' + 1]{ 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 255,
			255, 255, 255, 255, 255, 255, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };

		for(int8_t i = 0; i < 32; i += 2)
		{
			_Out_value._Mydata[i >> 1] =
				hi[(_Str[indices[i]] - '0')] | lo[_Str[indices[i + 1]] - '0'];
		}
		return true;
#endif
	}

	inline static uuid parse(_Details::string_view_type _Str) noexcept
	{
#if _MST_UUID_SIMD_ENABLED
		static const __m128i chars[][3] = {
			{ _mm_set1_epi8('1'), _mm_set1_epi8(static_cast<char>(0x10)), _mm_set1_epi8(0x1) },
			{ _mm_set1_epi8('2'), _mm_set1_epi8(static_cast<char>(0x20)), _mm_set1_epi8(0x2) },
			{ _mm_set1_epi8('3'), _mm_set1_epi8(static_cast<char>(0x30)), _mm_set1_epi8(0x3) },
			{ _mm_set1_epi8('4'), _mm_set1_epi8(static_cast<char>(0x40)), _mm_set1_epi8(0x4) },
			{ _mm_set1_epi8('5'), _mm_set1_epi8(static_cast<char>(0x50)), _mm_set1_epi8(0x5) },
			{ _mm_set1_epi8('6'), _mm_set1_epi8(static_cast<char>(0x60)), _mm_set1_epi8(0x6) },
			{ _mm_set1_epi8('7'), _mm_set1_epi8(static_cast<char>(0x70)), _mm_set1_epi8(0x7) },
			{ _mm_set1_epi8('8'), _mm_set1_epi8(static_cast<char>(0x80)), _mm_set1_epi8(0x8) },
			{ _mm_set1_epi8('9'), _mm_set1_epi8(static_cast<char>(0x90)), _mm_set1_epi8(0x9) },
			{ _mm_set1_epi8('A'), _mm_set1_epi8(static_cast<char>(0xA0)), _mm_set1_epi8(0xA) },
			{ _mm_set1_epi8('B'), _mm_set1_epi8(static_cast<char>(0xB0)), _mm_set1_epi8(0xB) },
			{ _mm_set1_epi8('C'), _mm_set1_epi8(static_cast<char>(0xC0)), _mm_set1_epi8(0xC) },
			{ _mm_set1_epi8('D'), _mm_set1_epi8(static_cast<char>(0xD0)), _mm_set1_epi8(0xD) },
			{ _mm_set1_epi8('E'), _mm_set1_epi8(static_cast<char>(0xE0)), _mm_set1_epi8(0xE) },
			{ _mm_set1_epi8('F'), _mm_set1_epi8(static_cast<char>(0xF0)), _mm_set1_epi8(0xF) },
		};

		const auto hipart = _mm_set_epi8(_Str[34], _Str[32], _Str[30], _Str[28], _Str[26], _Str[24],
			_Str[21], _Str[19], _Str[16], _Str[14], _Str[11], _Str[9], _Str[6], _Str[4], _Str[2],
			_Str[0]);
		const auto lopart = _mm_set_epi8(_Str[35], _Str[33], _Str[31], _Str[29], _Str[27], _Str[25],
			_Str[22], _Str[20], _Str[17], _Str[15], _Str[12], _Str[10], _Str[7], _Str[5], _Str[3],
			_Str[1]);

		auto result = _mm_setzero_si128();

		for(const auto& _c : chars)
		{
			const auto hieq = _mm_cmpeq_epi8(hipart, _c[0]);
			const auto himask = _mm_and_si128(hieq, _c[1]);

			result = _mm_or_si128(result, himask);

			const auto loeq = _mm_cmpeq_epi8(lopart, _c[0]);
			const auto lomask = _mm_and_si128(loeq, _c[2]);

			result = _mm_or_si128(result, lomask);
		}

		return uuid{ result };
#else
		constexpr int8_t indices[32]{ 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 17, 19, 20,
			21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };

		constexpr uint8_t hi['F' - '0' + 1]{ 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
			0x90, 255, 255, 255, 255, 255, 255, 255, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 };

		constexpr uint8_t lo['F' - '0' + 1]{ 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 255,
			255, 255, 255, 255, 255, 255, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };

		std::array<uint8_t, 16> parsed;
		for(int8_t i = 0; i < 32; i += 2)
		{
			parsed[i >> 1] = hi[(_Str[indices[i]] - '0')] | lo[_Str[indices[i + 1]] - '0'];
		}
		return uuid{ parsed };
#endif
	}

private:
#if _MST_UUID_SIMD_ENABLED
	__m128i _Mydata;
#else
	std::array<uint8_t, 16> _Mydata;
#endif
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
