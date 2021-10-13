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

#include <muuid.h>

#include <random>
#include <mutex>

::std::string mst::_Details::uuid_helper::to_string(std::array<uint8_t, 16> const& data)
{
	constexpr char chars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
		'D', 'E', 'F' };

	std::string retval;
	retval.reserve(36);

	for(size_t i = 0; i < 4; ++i)
	{
		retval.push_back(chars[data[i] >> 4]);
		retval.push_back(chars[data[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 4; i < 6; ++i)
	{
		retval.push_back(chars[data[i] >> 4]);
		retval.push_back(chars[data[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 6; i < 8; ++i)
	{
		retval.push_back(chars[data[i] >> 4]);
		retval.push_back(chars[data[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 8; i < 10; ++i)
	{
		retval.push_back(chars[data[i] >> 4]);
		retval.push_back(chars[data[i] & 15]);
	}

	retval.push_back('-');

	for(size_t i = 10; i < 16; ++i)
	{
		retval.push_back(chars[data[i] >> 4]);
		retval.push_back(chars[data[i] & 15]);
	}

	return retval;
}

static std::mutex g_mutex;
static uint64_t random_seed()
{
	static std::random_device seeder;

	std::lock_guard<std::mutex> _lock{ g_mutex };

	return seeder();
}

std::array<uint8_t, 16> mst::_Details::uuid_helper::generate()
{
	thread_local static std::mt19937_64 rng{ random_seed() };

	std::array numbers{ rng(), rng() };


	std::array<uint8_t, 16> data;
	static_assert(sizeof(numbers) == sizeof(data), "sizeof(bytes) != sizeof(data)");
	memcpy(data.data(), numbers.data(), sizeof(data));
	return data;
}