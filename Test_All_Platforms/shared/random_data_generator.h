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

#include <random>
#include <vector>

namespace mst {
namespace tests {

class random_data_generator
{
public:
	template<typename Integer>
	Integer scalar_int(Integer minValue, Integer maxValue)
	{
		std::uniform_int_distribution<Integer> dist{ minValue, maxValue };

		return dist(m_engine);
	}

	template<typename Integer>
	std::vector<Integer> vector_int(size_t size, Integer minValue, Integer maxValue)
	{
		std::uniform_int_distribution<Integer> dist{ minValue, maxValue };

		std::vector<Integer> randomVector;

		randomVector.reserve(size);

		for(size_t i = 0; i < size; ++i)
		{
			randomVector.push_back(dist(m_engine));
		}

		return randomVector;
	}

	template<typename Integer>
	std::vector<Integer> vector_int(
		size_t minSize, size_t maxSize, Integer minValue, Integer maxValue)
	{
		std::uniform_int_distribution<Integer> dist{ minValue, maxValue };

		std::vector<Integer> randomVector;

		const auto size = scalar_int(minSize, maxSize);

		randomVector.reserve(size);

		for(size_t i = 0; i < size; ++i)
		{
			randomVector.push_back(dist(m_engine));
		}

		return randomVector;
	}

	template<typename Integer>
	std::vector<Integer> vector_int(size_t minSize, size_t maxSize, Integer value)
	{
		std::vector<Integer> randomVector;

		const auto size = scalar_int(minSize, maxSize);

		randomVector.reserve(size);

		for(size_t i = 0; i < size; ++i)
		{
			randomVector.push_back(value);
		}

		return randomVector;
	}

private:
	std::mt19937_64 m_engine;
};

}
}