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

#include <muuid.h>

#include <random>
#include <mutex>

::std::string mst::_Details::uuid_helper::to_string(__m128i data)
{
	static const char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
		'D', 'E', 'F' };

	byte bytes[16];

	static_assert(sizeof(bytes) == sizeof(data), "sizeof(bytes) != sizeof(data)");

	memcpy(bytes, &data, sizeof(bytes));

	std::string retval;
	retval.reserve(36);

	for(size_t i = 0; i < 4; ++i)
	{
		retval.push_back(chars[bytes[i] >> 4]);
		retval.push_back(chars[bytes[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 4; i < 6; ++i)
	{
		retval.push_back(chars[bytes[i] >> 4]);
		retval.push_back(chars[bytes[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 6; i < 8; ++i)
	{
		retval.push_back(chars[bytes[i] >> 4]);
		retval.push_back(chars[bytes[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 8; i < 10; ++i)
	{
		retval.push_back(chars[bytes[i] >> 4]);
		retval.push_back(chars[bytes[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 10; i < 16; ++i)
	{
		retval.push_back(chars[bytes[i] >> 4]);
		retval.push_back(chars[bytes[i] & 15]);
	}

	return retval;
}

bool mst::_Details::uuid_helper::try_parse(const char* str, __m128i& data) noexcept
{
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

	const auto hipart = _mm_set_epi8(str[34], str[32], str[30], str[28], str[26], str[24], str[21],
		str[19], str[16], str[14], str[11], str[9], str[6], str[4], str[2], str[0]);
	const auto lopart = _mm_set_epi8(str[35], str[33], str[31], str[29], str[27], str[25], str[22],
		str[20], str[17], str[15], str[12], str[10], str[7], str[5], str[3], str[1]);

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

	data = result;
	const auto combined_has_any = _mm_and_si128(has_any_result_lo, has_any_result_hi);
	return _mm_movemask_epi8(combined_has_any) == 0xFFFF;
}

__m128i mst::_Details::uuid_helper::parse(const char* str) noexcept
{
	static const __m128i chars[][3] = {
		{ _mm_set1_epi8('1'), _mm_set1_epi8((char)0x10), _mm_set1_epi8(0x1) },
		{ _mm_set1_epi8('2'), _mm_set1_epi8((char)0x20), _mm_set1_epi8(0x2) },
		{ _mm_set1_epi8('3'), _mm_set1_epi8((char)0x30), _mm_set1_epi8(0x3) },
		{ _mm_set1_epi8('4'), _mm_set1_epi8((char)0x40), _mm_set1_epi8(0x4) },
		{ _mm_set1_epi8('5'), _mm_set1_epi8((char)0x50), _mm_set1_epi8(0x5) },
		{ _mm_set1_epi8('6'), _mm_set1_epi8((char)0x60), _mm_set1_epi8(0x6) },
		{ _mm_set1_epi8('7'), _mm_set1_epi8((char)0x70), _mm_set1_epi8(0x7) },
		{ _mm_set1_epi8('8'), _mm_set1_epi8((char)0x00), _mm_set1_epi8(0x8) },
		{ _mm_set1_epi8('9'), _mm_set1_epi8((char)0x10), _mm_set1_epi8(0x9) },
		{ _mm_set1_epi8('A'), _mm_set1_epi8((char)0x20), _mm_set1_epi8(0xA) },
		{ _mm_set1_epi8('B'), _mm_set1_epi8((char)0x30), _mm_set1_epi8(0xB) },
		{ _mm_set1_epi8('C'), _mm_set1_epi8((char)0x40), _mm_set1_epi8(0xC) },
		{ _mm_set1_epi8('D'), _mm_set1_epi8((char)0x50), _mm_set1_epi8(0xD) },
		{ _mm_set1_epi8('E'), _mm_set1_epi8((char)0x60), _mm_set1_epi8(0xE) },
		{ _mm_set1_epi8('F'), _mm_set1_epi8((char)0x70), _mm_set1_epi8(0xF) },
	};

	const auto hipart = _mm_set_epi8(str[34], str[32], str[30], str[28], str[26], str[24], str[21],
		str[19], str[16], str[14], str[11], str[9], str[6], str[4], str[2], str[0]);
	const auto lopart = _mm_set_epi8(str[35], str[33], str[31], str[29], str[27], str[25], str[22],
		str[20], str[17], str[15], str[12], str[10], str[7], str[5], str[3], str[1]);

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

	return result;
}

static std::mutex g_mutex;
static uint64_t random_seed()
{
	static std::random_device seeder;

	std::lock_guard<std::mutex> _lock{ g_mutex };

	return seeder();
}

void mst::_Details::uuid_helper::generate(__m128i& data)
{
	byte bytes[16];

	static_assert(sizeof(bytes) == sizeof(data), "sizeof(bytes) != sizeof(data)");

	thread_local static std::mt19937_64 rng{ random_seed() };

	uint64_t low, high;
	{
		low = rng();
		high = rng();
	}

	data = _mm_set_epi64x(static_cast<int64_t>(low), static_cast<int64_t>(high));
}