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
#include <mcore.h>
#include <mdebug.h>
#include <mx_storage_pair.h>

#include <vector>
#include <cstring>
#include <tuple>

namespace mst {

template<typename ElementType, typename IndexType>
class sparse_iterator;

template<typename ElementType, typename IndexType>
class const_sparse_iterator;

template<typename ElementType, typename IndexType = size_t, size_t PageSize = 32768U>
class sparse_set
{
	friend class sparse_iterator<ElementType, IndexType>;
	friend class const_sparse_iterator<ElementType, IndexType>;

public:
	typedef std::pair<const IndexType, ElementType> value_type;

	typedef sparse_iterator<ElementType, IndexType> iterator;
	typedef const_sparse_iterator<ElementType, IndexType> const_iterator;

	inline sparse_set() noexcept
	{ }

	inline ~sparse_set()
	{
		for(auto page : m_sparse)
		{
			delete[] page;
		}
	}

	inline sparse_set(const sparse_set& other)
		: m_data(other.m_data)
	{
		const auto dataSize = m_data.size();
		for(size_t i = 0; i < dataSize; ++i)
		{
			const auto index = m_data[i];

			const size_t idx = static_cast<size_t>(index);
			const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);
			const size_t elemIdx = idx & (PageSize - 1);

			ensure_page(pageIdx)[elemIdx] = static_cast<IndexType>(i);
		}
	}

	inline sparse_set(sparse_set&& other)
		: m_data(std::move(other.m_data))
		, m_sparse(std::move(other.m_sparse))
	{ }

	inline sparse_set& operator=(const sparse_set& other)
	{
		clear();

		m_data = other.m_data;

		const auto dataSize = m_data.size();
		for(size_t i = 0; i < dataSize; ++i)
		{
			const auto index = m_data[i];

			const size_t idx = static_cast<size_t>(index);
			const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);
			const size_t elemIdx = idx & (PageSize - 1);

			ensure_page(pageIdx)[elemIdx] = static_cast<IndexType>(i);
		}

		return *this;
	}

	inline sparse_set& operator=(sparse_set&& other)
	{
		m_data.swap(other.m_data);
		m_sparse.swap(other.m_sparse);

		return *this;
	}

	inline void reserve(IndexType count)
	{
		m_data.reserve(static_cast<size_t>(count));
	}

	template<typename... Args>
	inline value_type& emplace(IndexType index, Args&&... args)
	{
		MST_ASSERT(!contains(index), "index already exists in set");

		const size_t idx = static_cast<size_t>(index);
		const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);
		const size_t elemIdx = idx & (PageSize - 1);

		ensure_page(pageIdx)[elemIdx] = static_cast<IndexType>(m_data.size());

		m_data.emplace_back(std::piecewise_construct, std::forward_as_tuple(index),
			std::forward_as_tuple(std::forward<Args>(args)...));

		return m_data.back().get();
	}

	void erase(IndexType index) noexcept
	{
		MST_ASSERT(contains(index), "erase() requires an existing index");

		const size_t idx = static_cast<size_t>(index);
		const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);
		const size_t elemIdx = idx & (PageSize - 1);

		const auto dataIndex = m_sparse[pageIdx][elemIdx];
		m_sparse[pageIdx][elemIdx] = -1;

		if(static_cast<size_t>(dataIndex + 1) != m_data.size())
		{
			const size_t lastIdx = static_cast<size_t>(m_data.back().get().first);
			const size_t lastPageIdx = lastIdx >> _MST_GET_SHIFT(PageSize);
			const size_t lastElemIdx = lastIdx & (PageSize - 1);

			m_sparse[lastPageIdx][lastElemIdx] = dataIndex;
			m_data[static_cast<size_t>(dataIndex)] = m_data.back();
		}

		m_data.pop_back();
	}

	[[nodiscard]] inline bool contains(IndexType index) const noexcept
	{
		const size_t idx = static_cast<size_t>(index);
		const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);

		if(m_sparse.size() <= pageIdx)
			return false;

		if(!m_sparse[pageIdx])
			return false;

		const size_t elemIdx = idx & (PageSize - 1);

		return m_sparse[pageIdx][elemIdx] != static_cast<IndexType>(-1);
	}

	[[nodiscard]] inline const ElementType& at(IndexType index) const noexcept
	{
		MST_ASSERT(contains(index), "at() requires an existing index");

		return m_data[get_index_impl(index)].get().second;
	}

	[[nodiscard]] inline ElementType& at(IndexType index) noexcept
	{
		MST_ASSERT(contains(index), "at() requires an existing index");

		return m_data[get_index_impl(index)].get().second;
	}

	[[nodiscard]] inline bool empty() const noexcept
	{
		return m_data.empty();
	}

	[[nodiscard]] inline size_t size() const noexcept
	{
		return m_data.size();
	}

	[[nodiscard]] inline iterator begin() noexcept
	{
		return m_data.begin();
	}

	[[nodiscard]] inline const_iterator begin() const noexcept
	{
		return m_data.begin();
	}

	[[nodiscard]] inline const_iterator cbegin() const noexcept
	{
		return m_data.begin();
	}

	[[nodiscard]] inline iterator end() noexcept
	{
		return m_data.end();
	}

	[[nodiscard]] inline const_iterator end() const noexcept
	{
		return m_data.end();
	}

	[[nodiscard]] inline const_iterator cend() const noexcept
	{
		return m_data.end();
	}

	void clear()
	{
		m_data.clear();
		for(auto page : m_sparse)
		{
			delete[] page;
		}

		m_sparse.clear();
	}

private:
	[[nodiscard]] IndexType* ensure_page(size_t index)
	{
		if(m_sparse.size() <= index)
		{
			m_sparse.resize(index + 1);
		}

		if(!m_sparse[index])
		{
			m_sparse[index] = new IndexType[PageSize];
			memset(m_sparse[index], -1, PageSize * sizeof(IndexType));
		}

		return m_sparse[index];
	}

	[[nodiscard]] size_t get_index_impl(IndexType index) const noexcept
	{
		const size_t idx = static_cast<size_t>(index);
		const size_t pageIdx = idx >> _MST_GET_SHIFT(PageSize);
		const size_t elemIdx = idx & (PageSize - 1);

		return (size_t)m_sparse[pageIdx][elemIdx];
	}

	[[nodiscard]] ElementType& get_impl(IndexType index) noexcept
	{
		return reinterpret_cast<ElementType*>(&m_data[get_index_impl(index)]);
	}

	[[nodiscard]] const ElementType& get_impl(IndexType index) const noexcept
	{
		return reinterpret_cast<const ElementType*>(&m_data[get_index_impl(index)]);
	}

private:
	std::vector<::mst::_Details::storage_pair<IndexType, ElementType>> m_data;
	std::vector<IndexType*> m_sparse;
};

template<typename ElementType, typename IndexType>
class sparse_iterator
{
	typedef typename std::vector<::mst::_Details::storage_pair<IndexType, ElementType>>::iterator
		InternalIteratorType;
	friend class sparse_set<ElementType, IndexType>;

public:
	sparse_iterator() = default;
	sparse_iterator(const sparse_iterator&) = default;
	sparse_iterator(sparse_iterator&&) = default;

	sparse_iterator& operator=(const sparse_iterator&) = default;
	sparse_iterator& operator=(sparse_iterator&&) = default;

	inline sparse_iterator& operator++() noexcept
	{
		++m_iter;
		return *this;
	}

	[[nodiscard]] inline sparse_iterator operator++(int) noexcept
	{
		return sparse_iterator(m_iter++);
	}

	inline sparse_iterator& operator--() noexcept
	{
		--m_iter;
		return *this;
	}

	[[nodiscard]] inline sparse_iterator operator--(int) noexcept
	{
		return sparse_iterator(m_iter--);
	}

	inline sparse_iterator& operator+=(ptrdiff_t offset) noexcept
	{
		m_iter += offset;
		return *this;
	}

	[[nodiscard]] inline sparse_iterator operator+(ptrdiff_t offset) noexcept
	{
		return sparse_iterator(m_iter + offset);
	}

	inline sparse_iterator& operator-=(ptrdiff_t offset) noexcept
	{
		m_iter -= offset;
		return *this;
	}

	[[nodiscard]] inline sparse_iterator operator-(ptrdiff_t offset) noexcept
	{
		return sparse_iterator(m_iter - offset);
	}

	[[nodiscard]] inline bool operator==(const sparse_iterator& other) const noexcept
	{
		return m_iter == other.m_iter;
	}

	[[nodiscard]] inline bool operator!=(const sparse_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	[[nodiscard]] inline std::pair<const IndexType, ElementType>& operator*() const noexcept
	{
		return *reinterpret_cast<std::pair<const IndexType, ElementType>*>(&*m_iter);
	}

	[[nodiscard]] inline std::pair<const IndexType, ElementType>* operator->() const noexcept
	{
		return reinterpret_cast<std::pair<const IndexType, ElementType>*>(&*m_iter);
	}

private:
	sparse_iterator(InternalIteratorType iter)
		: m_iter(iter)
	{ }

	InternalIteratorType m_iter;
};

template<typename ElementType, typename IndexType>
[[nodiscard]] inline sparse_iterator<ElementType, IndexType> operator+(
	ptrdiff_t offset, sparse_iterator<ElementType, IndexType> iter) noexcept
{
	return iter += offset;
}

template<typename ElementType, typename IndexType>
class const_sparse_iterator
{
	typedef
		typename std::vector<::mst::_Details::storage_pair<IndexType, ElementType>>::const_iterator
			InternalIteratorType;
	friend class sparse_set<ElementType, IndexType>;

public:
	const_sparse_iterator() = default;
	const_sparse_iterator(const const_sparse_iterator&) = default;
	const_sparse_iterator(const_sparse_iterator&&) = default;

	const_sparse_iterator(const sparse_iterator<ElementType, IndexType>& other)
		: m_iter(other.m_iter)
	{ }

	const_sparse_iterator(sparse_iterator<ElementType, IndexType>&& other)
		: m_iter(std::move(other.m_iter))
	{ }

	const_sparse_iterator& operator=(const const_sparse_iterator&) = default;
	const_sparse_iterator& operator=(const_sparse_iterator&&) = default;

	const_sparse_iterator& operator=(const sparse_iterator<ElementType, IndexType>& other)
	{
		m_iter = other.m_iter;
		return *this;
	}

	const_sparse_iterator& operator=(sparse_iterator<ElementType, IndexType>&& other)
	{
		m_iter = std::move(other.m_iter);
		return *this;
	}

	inline const_sparse_iterator& operator++() noexcept
	{
		++m_iter;
		return *this;
	}

	[[nodiscard]] inline const_sparse_iterator operator++(int) noexcept
	{
		return const_sparse_iterator(m_iter++);
	}

	inline const_sparse_iterator& operator--() noexcept
	{
		--m_iter;
		return *this;
	}

	[[nodiscard]] inline const_sparse_iterator operator--(int) noexcept
	{
		return const_sparse_iterator(m_iter--);
	}

	inline const_sparse_iterator& operator+=(ptrdiff_t offset) noexcept
	{
		m_iter += offset;
		return *this;
	}

	[[nodiscard]] inline const_sparse_iterator operator+(ptrdiff_t offset) noexcept
	{
		return const_sparse_iterator(m_iter + offset);
	}

	inline const_sparse_iterator& operator-=(ptrdiff_t offset) noexcept
	{
		m_iter -= offset;
		return *this;
	}

	[[nodiscard]] inline const_sparse_iterator operator-(ptrdiff_t offset) noexcept
	{
		return const_sparse_iterator(m_iter - offset);
	}

	[[nodiscard]] inline bool operator==(const const_sparse_iterator& other) const noexcept
	{
		return m_iter == other.m_iter;
	}

	[[nodiscard]] inline bool operator!=(const const_sparse_iterator& other) const noexcept
	{
		return !(*this == other);
	}

	[[nodiscard]] inline const std::pair<const IndexType, ElementType>& operator*() const noexcept
	{
		return *reinterpret_cast<const std::pair<const IndexType, ElementType>*>(&*m_iter);
	}

	[[nodiscard]] inline const std::pair<const IndexType, ElementType>* operator->() const noexcept
	{
		return reinterpret_cast<const std::pair<const IndexType, ElementType>*>(&*m_iter);
	}

private:
	const_sparse_iterator(InternalIteratorType iter)
		: m_iter(iter)
	{ }

private:
	InternalIteratorType m_iter;
};

template<typename ElementType, typename IndexType>
[[nodiscard]] inline const_sparse_iterator<ElementType, IndexType> operator+(
	ptrdiff_t offset, const_sparse_iterator<ElementType, IndexType> iter) noexcept
{
	return iter += offset;
}

} // namespace mst