#pragma once

#include <mcore.h>
#include <malgorithm.h>

namespace mst {

template<typename _Container>
void sort(_Container& _Cont)
{
	::std::sort(::std::begin(_Cont), ::std::end(_Cont));
}

template<typename _Container, typename _Predicate>
void sort(_Container& _Cont, _Predicate _Pred)
{
	::std::sort(::std::begin(_Cont), ::std::end(_Cont), ::std::move(_Pred));
}

template<typename _Container, typename _Type>
auto find(_Container& _Cont, const _Type& _Ty) -> decltype(::std::begin(_Cont))
{
	return ::std::find(::std::begin(_Cont), ::std::end(_Cont), _Ty);
}

template<typename _Container, typename _Predicate>
auto find_if(_Container& _Cont, _Predicate _Pred) -> decltype(::std::begin(_Cont))
{
	return ::std::find_if(::std::begin(_Cont), ::std::end(_Cont), ::std::move(_Pred));
}

template<typename _Container, typename _Type>
bool contains(_Container& _Cont, const _Type& _Ty)
{
	return ::std::find(::std::begin(_Cont), ::std::end(_Cont), _Ty) != ::std::end(_Cont);
}

}; // namespace mst