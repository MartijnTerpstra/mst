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
class _Ray
{
public:
	typedef ::mst::math::vector<_Value_type, _Dimensions> vector_type;

	vector_type position, direction;

}; // class _Ray<_Value_type, _Dimensions>

} // namespace _Details

#undef _mst_declare_value

template<typename _Value_type, size_t _Dimensions>
class ray : public ::mst::math::_Details::_Ray<_Value_type, _Dimensions>
{
	constexpr ray() = default;

	inline ray(const vector<_Value_type, _Dimensions>& _position,
		const vector<_Value_type, _Dimensions>& _direction) noexcept
	{
		CHECK_IF(fabs(_direction.length() - (_Value_type)1) > _MST_EPSILON,
			"direction required a length of 1");

		this->position = _position;
		this->direction = _direction;
	}
};

} // namespace math
} // namespace mst