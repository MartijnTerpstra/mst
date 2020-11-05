#pragma once
#include <mdebug.h>

namespace std {

template<typename _Ty, size_t _MaxSize>
class staticmap
{
public:
	typedef _Ty* pointer;
	typedef _Ty& reference;
	typedef const _Ty& const_reference;
	typedef _Ty* iterator;
	typedef const _Ty* const_iterator;

	staticmap()
	{
		_MyEnd = _MyBegin;
	}

	inline iterator begin()
	{
		return _MyBegin;
	}

	inline const_iterator begin() const
	{
		return _MyBegin;
	}

	inline const_iterator cbegin() const
	{
		return _MyBegin;
	}

	inline iterator end()
	{
		return _MyEnd;
	}

	inline const_iterator end() const
	{
		return _MyEnd;
	}

	inline const_iterator cend() const
	{
		return _MyEnd;
	}

	inline bool empty() const
	{
		return _MyEnd == _MyBegin;
	}

	inline void pop_back()
	{
		_CHECK_IF(empty(), "cannot call pop_back on empty staticmap");
		_MyEnd--;
	}

	inline void push_back(const _Ty& _Value)
	{
		_CHECK_IF(size() == _MaxSize, "cannot call push_back on full staticmap");
		memcpy(_MyEnd, _Value, sizeof(_Ty));
		_MyEnd++;
	}

	inline size_t size() const
	{
		return _MyEnd - _MyBegin;
	}

	inline reference operator[](size_t _Idx)
	{
		_CHECK_ARR_RANGE(_Idx, size(), "staticmap index out of range");
		return _Ty[_Idx];
	}

	inline const_reference operator[](size_t _Idx) const
	{
		_CHECK_ARR_RANGE(_Idx, size(), "staticmap index out of range");
		return _Ty[_Idx];
	}

	_Ty _MyBegin[_MaxSize];
	pointer _MyEnd;
};

};