#pragma once

template<typename T>
template<size_t MaxElements>
inline mst::dll_safe::string<T>::string(const ::mst::stack_string<T, MaxElements>& stackString)
{
	_Myptr = nullptr;
	_Mylength = 0;
	_Mycapacity = 0;

	size_t len = stackString.length();
	_Alloc(len);
	for(size_t i = 0; i < len; ++i)
	{
		_Myptr[i] = stackString[i];
	}
}

template<typename T, size_t MaxElements>
inline mst::stack_string<T, MaxElements>::stack_string(const ::mst::dll_safe::string<T>& dllString)
{
	memset(_Myptr, 0, sizeof(_Myptr));
	_Mylength = 0;

	auto bg = dllString.begin();
	auto ed = dllString.end();

	while(bg != ed)
	{
		_Myptr[_Mylength++] = *bg++;
	}
}

template<typename T, size_t MaxElements>
inline mst::stack_string<T, MaxElements>& mst::stack_string<T, MaxElements>::operator=(const ::mst::dll_safe::string<T>& dllString)
{
	memset(_Myptr, 0, sizeof(_Myptr));
	_Mylength = 0;

	auto begin = dllString.begin();
	auto end = dllString.end();

	while(begin != end)
	{
		_Myptr[_Mylength++] = *begin++;
	}

	return *this;
}

template<typename T, size_t MaxElements>
inline bool operator==(const ::mst::stack_string<T, MaxElements>& l, const ::mst::dll_safe::string<T>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), l.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements>
inline bool operator!=(const ::mst::stack_string<T, MaxElements>& l, const ::mst::dll_safe::string<T>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), l.size() * sizeof(T)) != 0);
}

template<typename T, size_t MaxElements>
inline bool operator==(const ::mst::dll_safe::string<T>& l, const ::mst::stack_string<T, MaxElements>& r)
{
	return l.size() == r.size() && (memcmp(l.data(), r.data(), r.size() * sizeof(T)) == 0);
}

template<typename T, size_t MaxElements>
inline bool operator!=(const ::mst::dll_safe::string<T>& l, const ::mst::stack_string<T, MaxElements>& r)
{
	return l.size() != r.size() || (memcmp(l.data(), r.data(), r.size() * sizeof(T)) != 0);
}