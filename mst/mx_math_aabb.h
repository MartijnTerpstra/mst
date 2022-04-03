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

namespace mst {
namespace math {

namespace _Details {

template<typename _Value_type, size_t _Dimensions>
class _Aabb
{
public:
	typedef ::mst::math::vector<_Value_type, _Dimensions> vector_type;
	static const size_t dimensions = _Dimensions;

public:
	inline _Aabb()
	{
		min = vector_type((_Value_type)0);
		max = vector_type((_Value_type)0);
	}

	inline _Aabb(const vector_type& _min, const vector_type& _max)
	{
		min = _min;
		max = _max;
	}

	inline bool contains(const vector_type& point)
	{
		for(size_t i = 0; i < _Dimensions; ++i)
		{
			if(point[i] < min[i] ||
				point[i] > max[i])
			{
				return false;
			}
		}

		return true;
	}

public:
	vector_type min, max;
};

} // namespace _Details

template<typename _Value_type, size_t _Dimensions>
class aabb : public ::mst::math::_Details::_Aabb<_Value_type, _Dimensions>
{
};

template<typename _Value_type>
class aabb<_Value_type, 2> : public ::mst::math::_Details::_Aabb<_Value_type, 2>
{
public:
	inline aabb()
	{
	}

	inline aabb(const vector<_Value_type, 2>& min, const vector<_Value_type, 2>& max)
		: ::mst::math::_Details::_Aabb<_Value_type, 2>(min, max)
	{
	}

	inline aabb(_Value_type left, _Value_type right,
		_Value_type bottom, _Value_type top)
		: ::mst::math::_Details::_Aabb<_Value_type, 2>(vector<_Value_type, 2>(left, bottom), vector<_Value_type, 2>(right, top))
	{
	}

}; // class aabb<_Value_type, 2>

template<typename _Value_type>
class aabb<_Value_type, 3> : public ::mst::math::_Details::_Aabb<_Value_type, 3>
{
public:
	inline aabb()
	{
	}

	inline aabb(const vector<_Value_type, 3>& min, const vector<_Value_type, 3>& max)
		: ::mst::math::_Details::_Aabb<_Value_type, 3>(min, max)
	{
	}

	inline aabb(_Value_type left, _Value_type right,
		_Value_type bottom, _Value_type top,
		_Value_type front, _Value_type back)
		: ::mst::math::_Details::_Aabb<_Value_type, 3>(vector<_Value_type, 3>(left, bottom, front), vector<_Value_type, 3>(right, top, back))
	{
	}

}; // class aabb<_Value_type, 3>

#undef _mst_declare_value
#undef _mst_declare_value2

} // namespace math
} // namespace mst