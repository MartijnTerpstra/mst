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

#if !_MST_MATH_SIMD_ENABLED

#error "Should only be included when simd is enabled"

#else

namespace mst {
namespace math {

namespace _Details {

inline __m128 _VectorDot(__m128 V1, __m128 V2)
{
	__m128 vTemp2 = V2;
	__m128 vTemp = _mm_mul_ps(V1, vTemp2);
	vTemp2 = _mm_shuffle_ps(
		vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
	vTemp2 = _mm_add_ps(vTemp2, vTemp);			 // Add Z = X+Z; W = Y+W;
	vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0)); // Copy W to the Z position
	vTemp = _mm_add_ps(vTemp, vTemp2);								// Add Z and W together
	return _MST_PERMUTE_PS(vTemp, _MM_SHUFFLE(2, 2, 2, 2));			// Splat Z and return
}

} // namespace _Details

//////////////////////////////////
// matrix<float, 4, 3>          //
//////////////////////////////////

inline matrix<float, 4, 3> operator*(
	const matrix<float, 4, 3>& _Left, const matrix<float, 4, 3>& _Right) noexcept
{
	matrix<float, 4, 3> retval;

#if _MST_HAS_AVX
	__m128 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 0);
	__m128 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 1);
	__m128 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 2);
	__m128 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 3);
#else
	__m128 vW = _Left[0]._Simd;
	__m128 vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	__m128 lastRow = _mm_set_ps(1, 0, 0, 0);

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, lastRow);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[0]._Simd = vX;

#if _MST_HAS_AVX
	vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 0);
	vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 1);
	vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 2);
	vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 3);
#else
	vW = _Left[1]._Simd;
	vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, lastRow);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[1]._Simd = vX;

#if _MST_HAS_AVX
	vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 0);
	vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 1);
	vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 2);
	vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 3);
#else
	vW = _Left[2]._Simd;
	vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, lastRow);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[2]._Simd = vX;

	return retval;
}

//////////////////////////////////
// matrix<float, 4, 4>          //
//////////////////////////////////

template<>
inline vector<float, 4> operator*(
	const vector<float, 4>& _Left, const matrix<float, 4, 4>& _Right) noexcept
{
	vector<float, 4> retval;

#if _MST_HAS_AVX
	__m128 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left._Simd) + 0);
	__m128 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left._Simd) + 1);
	__m128 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left._Simd) + 2);
	__m128 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left._Simd) + 3);
#else
	__m128 vW = _Left._Simd;
	__m128 vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, _Right[3]._Simd);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval._Simd = vX;

	return retval;
}

template<>
inline matrix<float, 4, 4> operator*(
	const matrix<float, 4, 4>& _Left, const matrix<float, 4, 4>& _Right) noexcept
{
	matrix<float, 4, 4> retval;

#if _MST_HAS_AVX
	__m128 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 0);
	__m128 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 1);
	__m128 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 2);
	__m128 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[0]._Simd) + 3);
#else
	__m128 vW = _Left[0]._Simd;
	__m128 vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, _Right[3]._Simd);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[0]._Simd = vX;

#if _MST_HAS_AVX
	vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 0);
	vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 1);
	vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 2);
	vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[1]._Simd) + 3);
#else
	vW = _Left[1]._Simd;
	vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, _Right[3]._Simd);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[1]._Simd = vX;

#if _MST_HAS_AVX
	vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 0);
	vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 1);
	vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 2);
	vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[2]._Simd) + 3);
#else
	vW = _Left[2]._Simd;
	vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, _Right[3]._Simd);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[2]._Simd = vX;
#if _MST_HAS_AVX
	vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[3]._Simd) + 0);
	vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[3]._Simd) + 1);
	vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[3]._Simd) + 2);
	vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&_Left[3]._Simd) + 3);
#else
	vW = _Left[3]._Simd;
	vX = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = _MST_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif // _MST_HAS_AVX

	vX = _mm_mul_ps(vX, _Right[0]._Simd);
	vY = _mm_mul_ps(vY, _Right[1]._Simd);
	vZ = _mm_mul_ps(vZ, _Right[2]._Simd);
	vW = _mm_mul_ps(vW, _Right[3]._Simd);

	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);

	retval[3]._Simd = vX;

	return retval;
}

template<>
inline matrix<float, 4, 4> _Details::_Math_matrix_base<float, 4, 4>::get_transpose() const noexcept
{
	matrix<float, 4, 4> retval;

	__m128 vTemp1 = _mm_shuffle_ps(_Data[0]._Simd, _Data[1]._Simd, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 vTemp3 = _mm_shuffle_ps(_Data[0]._Simd, _Data[1]._Simd, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 vTemp2 = _mm_shuffle_ps(_Data[2]._Simd, _Data[3]._Simd, _MM_SHUFFLE(1, 0, 1, 0));
	__m128 vTemp4 = _mm_shuffle_ps(_Data[2]._Simd, _Data[3]._Simd, _MM_SHUFFLE(3, 2, 3, 2));

	retval[0]._Simd = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	retval[1]._Simd = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	retval[2]._Simd = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	retval[3]._Simd = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

	return retval;
}

template<>
inline matrix<float, 4, 4> _Details::_Math_matrix_square<float, 4>::get_inverse() const noexcept
{
	matrix<float, 4, 4> MT = get_transpose();

	__m128 V00 = _MST_PERMUTE_PS(MT[2]._Simd, _MM_SHUFFLE(1, 1, 0, 0));
	__m128 V10 = _MST_PERMUTE_PS(MT[3]._Simd, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 V01 = _MST_PERMUTE_PS(MT[0]._Simd, _MM_SHUFFLE(1, 1, 0, 0));
	__m128 V11 = _MST_PERMUTE_PS(MT[1]._Simd, _MM_SHUFFLE(3, 2, 3, 2));
	__m128 V02 = _mm_shuffle_ps(MT[2]._Simd, MT[0]._Simd, _MM_SHUFFLE(2, 0, 2, 0));
	__m128 V12 = _mm_shuffle_ps(MT[3]._Simd, MT[1]._Simd, _MM_SHUFFLE(3, 1, 3, 1));

	__m128 D0 = _mm_mul_ps(V00, V10);
	__m128 D1 = _mm_mul_ps(V01, V11);
	__m128 D2 = _mm_mul_ps(V02, V12);

	V00 = _MST_PERMUTE_PS(MT[2]._Simd, _MM_SHUFFLE(3, 2, 3, 2));
	V10 = _MST_PERMUTE_PS(MT[3]._Simd, _MM_SHUFFLE(1, 1, 0, 0));
	V01 = _MST_PERMUTE_PS(MT[0]._Simd, _MM_SHUFFLE(3, 2, 3, 2));
	V11 = _MST_PERMUTE_PS(MT[1]._Simd, _MM_SHUFFLE(1, 1, 0, 0));
	V02 = _mm_shuffle_ps(MT[2]._Simd, MT[0]._Simd, _MM_SHUFFLE(3, 1, 3, 1));
	V12 = _mm_shuffle_ps(MT[3]._Simd, MT[1]._Simd, _MM_SHUFFLE(2, 0, 2, 0));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	D0 = _mm_sub_ps(D0, V00);
	D1 = _mm_sub_ps(D1, V01);
	D2 = _mm_sub_ps(D2, V02);
	// V11 = D0Y,D0W,D2Y,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
	V00 = _MST_PERMUTE_PS(MT[1]._Simd, _MM_SHUFFLE(1, 0, 2, 1));
	V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
	V01 = _MST_PERMUTE_PS(MT[0]._Simd, _MM_SHUFFLE(0, 1, 0, 2));
	V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
	// V13 = D1Y,D1W,D2W,D2W
	__m128 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
	V02 = _MST_PERMUTE_PS(MT[3]._Simd, _MM_SHUFFLE(1, 0, 2, 1));
	V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
	__m128 V03 = _MST_PERMUTE_PS(MT[2]._Simd, _MM_SHUFFLE(0, 1, 0, 2));
	V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

	__m128 C0 = _mm_mul_ps(V00, V10);
	__m128 C2 = _mm_mul_ps(V01, V11);
	__m128 C4 = _mm_mul_ps(V02, V12);
	__m128 C6 = _mm_mul_ps(V03, V13);

	// V11 = D0X,D0Y,D2X,D2X
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
	V00 = _MST_PERMUTE_PS(MT[1]._Simd, _MM_SHUFFLE(2, 1, 3, 2));
	V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V01 = _MST_PERMUTE_PS(MT[0]._Simd, _MM_SHUFFLE(1, 3, 2, 3));
	V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
	// V13 = D1X,D1Y,D2Z,D2Z
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
	V02 = _MST_PERMUTE_PS(MT[3]._Simd, _MM_SHUFFLE(2, 1, 3, 2));
	V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
	V03 = _MST_PERMUTE_PS(MT[2]._Simd, _MM_SHUFFLE(1, 3, 2, 3));
	V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	C0 = _mm_sub_ps(C0, V00);
	C2 = _mm_sub_ps(C2, V01);
	C4 = _mm_sub_ps(C4, V02);
	C6 = _mm_sub_ps(C6, V03);

	V00 = _MST_PERMUTE_PS(MT[1]._Simd, _MM_SHUFFLE(0, 3, 0, 3));
	// V10 = D0Z,D0Z,D2X,D2Y
	V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
	V10 = _MST_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
	V01 = _MST_PERMUTE_PS(MT[0]._Simd, _MM_SHUFFLE(2, 0, 3, 1));
	// V11 = D0X,D0W,D2X,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
	V11 = _MST_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
	V02 = _MST_PERMUTE_PS(MT[3]._Simd, _MM_SHUFFLE(0, 3, 0, 3));
	// V12 = D1Z,D1Z,D2Z,D2W
	V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
	V12 = _MST_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
	V03 = _MST_PERMUTE_PS(MT[2]._Simd, _MM_SHUFFLE(2, 0, 3, 1));
	// V13 = D1X,D1W,D2Z,D2W
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
	V13 = _MST_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	__m128 C1 = _mm_sub_ps(C0, V00);
	C0 = _mm_add_ps(C0, V00);
	__m128 C3 = _mm_add_ps(C2, V01);
	C2 = _mm_sub_ps(C2, V01);
	__m128 C5 = _mm_sub_ps(C4, V02);
	C4 = _mm_add_ps(C4, V02);
	__m128 C7 = _mm_add_ps(C6, V03);
	C6 = _mm_sub_ps(C6, V03);

	C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
	C0 = _MST_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _MST_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _MST_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _MST_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));

	constexpr __m128 one{ 1.0f, 1.0f, 1.0f, 1.0f };

	// Get the determinate
	__m128 vTemp = _Details::_VectorDot(C0, MT[0]._Simd);
	vTemp = _mm_div_ps(one, vTemp);

	matrix<float, 4, 4> mResult;
	mResult[0]._Simd = _mm_mul_ps(C0, vTemp);
	mResult[1]._Simd = _mm_mul_ps(C2, vTemp);
	mResult[2]._Simd = _mm_mul_ps(C4, vTemp);
	mResult[3]._Simd = _mm_mul_ps(C6, vTemp);
	return mResult;
}

//////////////////////////////////
// matrix<int32_t, 4, 4>        //
//////////////////////////////////

#if _MST_HAS_AVX

template<>
inline vector<int32_t, 4> operator*(
	const vector<int32_t, 4>& _Left, const matrix<int32_t, 4, 4>& _Right) noexcept
{
	vector<int32_t, 4> retval;

	__m128i vX = _mm_set1_epi32(_Left[0]);
	__m128i vY = _mm_set1_epi32(_Left[1]);
	__m128i vZ = _mm_set1_epi32(_Left[2]);
	__m128i vW = _mm_set1_epi32(_Left[3]);

	vX = _mm_mullo_epi32(vX, _Right[0]._Simd);
	vY = _mm_mullo_epi32(vY, _Right[1]._Simd);
	vZ = _mm_mullo_epi32(vZ, _Right[2]._Simd);
	vW = _mm_mullo_epi32(vW, _Right[3]._Simd);

	vX = _mm_add_epi32(vX, vZ);
	vY = _mm_add_epi32(vY, vW);
	vX = _mm_add_epi32(vX, vY);

	retval._Simd = vX;

	return retval;
}

template<>
inline matrix<int32_t, 4, 4> operator*(
	const matrix<int32_t, 4, 4>& _Left, const matrix<int32_t, 4, 4>& _Right) noexcept
{
	matrix<int32_t, 4, 4> retval;

	__m128i vX = _mm_set1_epi32(_Left[0][0]);
	__m128i vY = _mm_set1_epi32(_Left[0][1]);
	__m128i vZ = _mm_set1_epi32(_Left[0][2]);
	__m128i vW = _mm_set1_epi32(_Left[0][3]);

	vX = _mm_mullo_epi32(vX, _Right[0]._Simd);
	vY = _mm_mullo_epi32(vY, _Right[1]._Simd);
	vZ = _mm_mullo_epi32(vZ, _Right[2]._Simd);
	vW = _mm_mullo_epi32(vW, _Right[3]._Simd);

	vX = _mm_add_epi32(vX, vZ);
	vY = _mm_add_epi32(vY, vW);
	vX = _mm_add_epi32(vX, vY);

	retval[0]._Simd = vX;

	vX = _mm_set1_epi32(_Left[1][0]);
	vY = _mm_set1_epi32(_Left[1][1]);
	vZ = _mm_set1_epi32(_Left[1][2]);
	vW = _mm_set1_epi32(_Left[1][3]);

	vX = _mm_mullo_epi32(vX, _Right[0]._Simd);
	vY = _mm_mullo_epi32(vY, _Right[1]._Simd);
	vZ = _mm_mullo_epi32(vZ, _Right[2]._Simd);
	vW = _mm_mullo_epi32(vW, _Right[3]._Simd);

	vX = _mm_add_epi32(vX, vZ);
	vY = _mm_add_epi32(vY, vW);
	vX = _mm_add_epi32(vX, vY);

	retval[1]._Simd = vX;

	vX = _mm_set1_epi32(_Left[2][0]);
	vY = _mm_set1_epi32(_Left[2][1]);
	vZ = _mm_set1_epi32(_Left[2][2]);
	vW = _mm_set1_epi32(_Left[2][3]);

	vX = _mm_mullo_epi32(vX, _Right[0]._Simd);
	vY = _mm_mullo_epi32(vY, _Right[1]._Simd);
	vZ = _mm_mullo_epi32(vZ, _Right[2]._Simd);
	vW = _mm_mullo_epi32(vW, _Right[3]._Simd);

	vX = _mm_add_epi32(vX, vZ);
	vY = _mm_add_epi32(vY, vW);
	vX = _mm_add_epi32(vX, vY);

	retval[2]._Simd = vX;

	vX = _mm_set1_epi32(_Left[3][0]);
	vY = _mm_set1_epi32(_Left[3][1]);
	vZ = _mm_set1_epi32(_Left[3][2]);
	vW = _mm_set1_epi32(_Left[3][3]);

	vX = _mm_mullo_epi32(vX, _Right[0]._Simd);
	vY = _mm_mullo_epi32(vY, _Right[1]._Simd);
	vZ = _mm_mullo_epi32(vZ, _Right[2]._Simd);
	vW = _mm_mullo_epi32(vW, _Right[3]._Simd);

	vX = _mm_add_epi32(vX, vZ);
	vY = _mm_add_epi32(vY, vW);
	vX = _mm_add_epi32(vX, vY);

	retval[3]._Simd = vX;

	return retval;
}

#endif

//////////////////////////////////
// matrix<double, 4, 4>         //
//////////////////////////////////

#if _MST_HAS_AVX

template<>
inline vector<double, 4> operator*(
	const vector<double, 4>& _Left, const matrix<double, 4, 4>& _Right) noexcept
{
	vector<double, 4> retval;

	__m256d vX = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left._Simd) + 0);
	__m256d vY = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left._Simd) + 1);
	__m256d vZ = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left._Simd) + 2);
	__m256d vW = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left._Simd) + 3);

	vX = _mm256_mul_pd(vX, _Right[0]._Simd);
	vY = _mm256_mul_pd(vY, _Right[1]._Simd);
	vZ = _mm256_mul_pd(vZ, _Right[2]._Simd);
	vW = _mm256_mul_pd(vW, _Right[3]._Simd);

	vX = _mm256_add_pd(vX, vZ);
	vY = _mm256_add_pd(vY, vW);
	vX = _mm256_add_pd(vX, vY);

	retval._Simd = vX;

	return retval;
}

template<>
inline matrix<double, 4, 4> operator*(
	const matrix<double, 4, 4>& _Left, const matrix<double, 4, 4>& _Right) noexcept
{
	matrix<double, 4, 4> retval;

	__m256d vX = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[0]._Simd) + 0);
	__m256d vY = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[0]._Simd) + 1);
	__m256d vZ = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[0]._Simd) + 2);
	__m256d vW = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[0]._Simd) + 3);

	vX = _mm256_mul_pd(vX, _Right[0]._Simd);
	vY = _mm256_mul_pd(vY, _Right[1]._Simd);
	vZ = _mm256_mul_pd(vZ, _Right[2]._Simd);
	vW = _mm256_mul_pd(vW, _Right[3]._Simd);

	vX = _mm256_add_pd(vX, vZ);
	vY = _mm256_add_pd(vY, vW);
	vX = _mm256_add_pd(vX, vY);

	retval[0]._Simd = vX;

	vX = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[1]._Simd) + 0);
	vY = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[1]._Simd) + 1);
	vZ = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[1]._Simd) + 2);
	vW = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[1]._Simd) + 3);

	vX = _mm256_mul_pd(vX, _Right[0]._Simd);
	vY = _mm256_mul_pd(vY, _Right[1]._Simd);
	vZ = _mm256_mul_pd(vZ, _Right[2]._Simd);
	vW = _mm256_mul_pd(vW, _Right[3]._Simd);

	vX = _mm256_add_pd(vX, vZ);
	vY = _mm256_add_pd(vY, vW);
	vX = _mm256_add_pd(vX, vY);

	retval[1]._Simd = vX;

	vX = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[2]._Simd) + 0);
	vY = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[2]._Simd) + 1);
	vZ = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[2]._Simd) + 2);
	vW = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[2]._Simd) + 3);

	vX = _mm256_mul_pd(vX, _Right[0]._Simd);
	vY = _mm256_mul_pd(vY, _Right[1]._Simd);
	vZ = _mm256_mul_pd(vZ, _Right[2]._Simd);
	vW = _mm256_mul_pd(vW, _Right[3]._Simd);

	vX = _mm256_add_pd(vX, vZ);
	vY = _mm256_add_pd(vY, vW);
	vX = _mm256_add_pd(vX, vY);

	retval[2]._Simd = vX;

	vX = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[3]._Simd) + 0);
	vY = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[3]._Simd) + 1);
	vZ = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[3]._Simd) + 2);
	vW = _mm256_broadcast_sd(reinterpret_cast<const double*>(&_Left[3]._Simd) + 3);

	vX = _mm256_mul_pd(vX, _Right[0]._Simd);
	vY = _mm256_mul_pd(vY, _Right[1]._Simd);
	vZ = _mm256_mul_pd(vZ, _Right[2]._Simd);
	vW = _mm256_mul_pd(vW, _Right[3]._Simd);

	vX = _mm256_add_pd(vX, vZ);
	vY = _mm256_add_pd(vY, vW);
	vX = _mm256_add_pd(vX, vY);

	retval[3]._Simd = vX;

	return retval;
}

#endif

} // namespace math
} // namespace mst

#endif