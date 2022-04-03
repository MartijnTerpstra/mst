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
namespace _Details {

template<typename KeyType, typename Value_type>
class storage_pair
{
	typedef std::pair<const KeyType, Value_type> value_type;

public:
	template<typename... Args>
	storage_pair(Args&&... args)
	{
		new(&m_data) value_type(std::forward<Args>(args)...);
	}

	storage_pair(const storage_pair& other)
	{
		new(&m_data) value_type(other.get());
	}

	storage_pair(const value_type& other)
	{
		new(&m_data) value_type(other);
	}

	storage_pair(storage_pair&& other) noexcept
	{
		new(&m_data) value_type(std::move(other.get()));
	}

	storage_pair(value_type&& other) noexcept
	{
		new(&m_data) value_type(std::move(other));
	}

	~storage_pair()
	{
		get().~value_type();
	}

	storage_pair& operator=(const storage_pair& other)
	{
		get().~value_type();
		new(&m_data) value_type(other.get());
		return *this;
	}

	storage_pair& operator=(storage_pair&& other) noexcept
	{
		get().~value_type();
		new(&m_data) value_type(std::move(other.get()));
		return *this;
	}

	[[nodiscard]] value_type& get() noexcept
	{
		return *reinterpret_cast<value_type*>(&m_data);
	}

	[[nodiscard]] const value_type& get() const noexcept
	{
		return *reinterpret_cast<const value_type*>(&m_data);
	}

private:
	typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type m_data;
};

}
}