#pragma once

#include <mcore.h>
#include <mnone.h>

namespace mst {

template<typename T>
class optional
{
public:
	static_assert(!::std::is_reference<T>::value, "optional<T>: T should not be a reference type");

	inline optional()
		: _Is_initialized(false)
	{ }

	inline optional(none_t)
		: _Is_initialized(false)
	{ }

	inline optional(const optional& other)
		: _Is_initialized(other._Is_initialized)
	{
		if(_Is_initialized)
			_Init(*other);
	}

	inline optional(optional&& other)
		: _Is_initialized(other._Is_initialized)
	{
		if(_Is_initialized)
			_Init(::std::move(*other));
	}

	inline optional(const T& value)
		: _Is_initialized(true)
	{
		_Init(value);
	}

	inline optional(T&& value)
		: _Is_initialized(true)
	{
		_Init(::std::move(value));
	}

	~optional()
	{
		_Destroy_storage();
	}

	inline optional& operator=(none_t)
	{
		_Destroy_storage();
		_Is_initialized = false;
		return *this;
	}

	inline optional& operator=(const T& other)
	{
		if(_Is_initialized)
		{
			value() = other;
		}
		else
		{
			_Is_initialized = true;
			_Init(other);
		}

		return *this;
	}

	inline optional& operator=(T&& other)
	{
		if(_Is_initialized)
		{
			value() = ::std::move(other);
		}
		else
		{
			_Is_initialized = true;
			_Init(::std::move(other));
		}

		return *this;
	}

	inline optional& operator=(const optional& other)
	{
		if(other._Is_initialized)
		{
			*this = *other;
		}
		else
		{
			*this = none;
		}

		return *this;
	}

	inline optional& operator=(optional&& other)
	{
		if(other._Is_initialized)
		{
			*this = std::move(*other);
		}
		else
		{
			*this = none;
		}

		return *this;
	}

	inline bool has_value() const
	{
		return _Is_initialized;
	}

	inline const T& value() const
	{
		if(!has_value())
			throw std::exception("no value constructed");
		return **this;
	}

	inline T& value()
	{
		if(!has_value())
			throw std::exception("no value constructed");
		return **this;
	}

	inline const T& operator*() const
	{
		return *reinterpret_cast<const T*>(::std::addressof(_Mystorage));
	}

	inline T& operator*()
	{
		return *reinterpret_cast<T*>(::std::addressof(_Mystorage));
	}

	inline const T* operator->() const
	{
		return reinterpret_cast<const T*>(::std::addressof(_Mystorage));
	}

	inline T* operator->()
	{
		return reinterpret_cast<T*>(::std::addressof(_Mystorage));
	}

	inline explicit operator bool() const
	{
		return _Is_initialized;
	}

private:
	void _Destroy_storage()
	{
		if(_Is_initialized)
		{
			// reinterpret_cast<T*>(&_Mystorage)->~T();
			::mst::_Details::_Destroy_object(reinterpret_cast<T*>(&_Mystorage));
			_Is_initialized = false;
		}
	}

	void _Init(T&& value)
	{
		::new(reinterpret_cast<void*>(&_Mystorage)) T(::std::move(value));
	}

	void _Init(const T& value)
	{
		::new(reinterpret_cast<void*>(&_Mystorage)) T(value);
	}

private:
	bool _Is_initialized;
	typename ::std::aligned_storage<sizeof(T), ::std::alignment_of<T>::value>::type _Mystorage;

}; // class optional<T>

}; // namespace mst