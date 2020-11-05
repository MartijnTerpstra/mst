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

#include <mmath2.h>

namespace mst {
namespace test {


template<typename MathType>
class math_approx : public Catch::MatcherBase<MathType>
{
	typedef typename MathType::value_type value_type;

public:
	math_approx(MathType right, value_type epsilon, value_type percentage)
		: m_right(std::move(right))
		, m_epsilon(epsilon)
		, m_percentage(percentage)
	{ }

	virtual std::string describe() const override
	{
		std::string retval = "doesnt match given value with an ";
		if(m_percentage == 0)
		{
			retval.append("epsilon of " + std::to_string(m_epsilon));
		}
		else
		{
			retval.append("allowed error percentage of " + std::to_string(m_percentage));
		}
		return retval + m_errors;
	}

public:
	bool match(const MathType& left) const override
	{
		return _Match(left, m_right, [this](const value_type& left, const value_type& right) {
			if(m_percentage == 0)
			{
				return abs(left - right) < m_epsilon;
			}
			else if(abs(right) < m_epsilon)
			{
				return left < m_epsilon;
			}
			else
			{
				return abs(left - right) / right < m_percentage;
			}
		});
	}

private:
	template<typename ValueType, size_t Columns, size_t Rows, typename Fn>
	bool _Match(const mst::math::matrix<ValueType, Columns, Rows>& left,
		const mst::math::matrix<ValueType, Columns, Rows>& right, Fn func) const
	{
		bool success = true;
		for(size_t x = 0; x < Rows; ++x)
			for(size_t y = 0; y < Columns; ++y)
			{
				if(!func(left[x][y], right[x][y]))
				{
					success = false;
					m_errors.append("|(" + std::to_string(x) + ", " + std::to_string(y) +
									") where " + std::to_string(left[x][y]) +
									" != " + std::to_string(right[x][y]));
				}
			}

		return success;
	}

	template<typename ValueType, size_t Elems, typename Fn>
	bool _Match(const mst::math::vector<ValueType, Elems>& left,
		const mst::math::vector<ValueType, Elems>& right, Fn func) const
	{
		bool success = true;
		for(size_t x = 0; x < Elems; ++x)
		{
			if(!func(left[x], right[x]))
			{
				success = false;
				m_errors.append("|(" + std::to_string(x) + ") where " + std::to_string(left[x]) +
								" != " + std::to_string(right[x]));
			}
		}

		return success;
	}

	template<typename ValueType, typename Fn>
	bool _Match(const mst::math::quaternion<ValueType>& left,
		const mst::math::quaternion<ValueType>& right, Fn func) const
	{
		bool success = true;
		for(size_t x = 0; x < 4; ++x)
		{
			if(!func(left[x], right[x]))
			{
				success = false;
				m_errors.append("|(" + std::to_string(x) + ") where " + std::to_string(left[x]) +
								" != " + std::to_string(right[x]));
			}
		}

		return success;
	}

private:
	MathType m_right;
	const value_type m_epsilon;
	const value_type m_percentage;
	mutable std::string m_errors;
};

template<typename MathType>
math_approx<MathType> approx_equal(MathType right, typename MathType::value_type epsilon)
{
	return math_approx<MathType>(std::move(right), epsilon, 0);
}

template<typename MathType>
math_approx<MathType> approx_equal(
	MathType right, typename MathType::value_type epsilon, typename MathType::value_type percentage)
{
	return math_approx<MathType>(std::move(right), epsilon, percentage);
}

}
}