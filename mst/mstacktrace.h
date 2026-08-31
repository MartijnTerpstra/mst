//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2026 Martinus Terpstra                                                 //
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
#include <mx_stacktrace.h>
#include <cstdint>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace mst {

class stacktrace_entry
{
public:
	using native_handle_type = void*;

	stacktrace_entry() noexcept = default;

	explicit stacktrace_entry(native_handle_type handle) noexcept
		: _Handle(handle)
	{ }

	native_handle_type native_handle() const noexcept
	{
		return _Handle;
	}

	explicit operator bool() const noexcept
	{
		return _Handle != nullptr;
	}

	::std::string description() const
	{
		return _Resolve().description;
	}

	::std::string source_file() const
	{
		return _Resolve().sourceFile;
	}

	uint32_t source_line() const
	{
		return _Resolve().sourceLine;
	}

	friend bool operator==(const stacktrace_entry& lhs, const stacktrace_entry& rhs) noexcept
	{
		return lhs._Handle == rhs._Handle;
	}

	friend bool operator!=(const stacktrace_entry& lhs, const stacktrace_entry& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	const ::mst::_Details::stacktrace_symbol_info& _Resolve() const
	{
		if(!_Cache)
		{
			_Cache = _Handle ? ::mst::_Details::stacktrace_resolve_impl(_Handle)
							 : ::mst::_Details::stacktrace_symbol_info{};
		}
		return *_Cache;
	}

	native_handle_type _Handle = nullptr;
	mutable ::std::optional<::mst::_Details::stacktrace_symbol_info> _Cache;
};

inline ::std::string to_string(const stacktrace_entry& entry)
{
	if(!entry)
		return ::std::string();

	const auto description = entry.description();
	const auto sourceFile = entry.source_file();

	::std::ostringstream stream;

	if(!description.empty())
	{
		stream << description;
	}
	else
	{
		stream << "0x" << ::std::hex << reinterpret_cast<uintptr_t>(entry.native_handle());
	}

	if(!sourceFile.empty())
	{
		stream << " at " << sourceFile;

		if(const auto line = entry.source_line(); line != 0)
		{
			stream << ":" << line;
		}
	}

	return stream.str();
}

inline ::std::ostream& operator<<(::std::ostream& stream, const stacktrace_entry& entry)
{
	return stream << to_string(entry);
}

class stacktrace
{
public:
	using value_type = stacktrace_entry;
	using reference = stacktrace_entry&;
	using const_reference = const stacktrace_entry&;
	using const_iterator = ::std::vector<stacktrace_entry>::const_iterator;
	using iterator = const_iterator;
	using size_type = size_t;
	using difference_type = ::std::ptrdiff_t;

	stacktrace() noexcept = default;

	static stacktrace current(size_t skip = 0, size_t maxDepth = 64) noexcept
	{
		stacktrace result;

		if(maxDepth == 0)
			return result;

		::std::vector<void*> frames(maxDepth);

		const auto count =
			::mst::_Details::stacktrace_capture_impl(frames.data(), frames.size(), skip + 1);

		result._Entries.reserve(count);

		for(size_t i = 0; i < count; ++i)
		{
			result._Entries.emplace_back(frames[i]);
		}

		return result;
	}

	bool empty() const noexcept
	{
		return _Entries.empty();
	}

	size_type size() const noexcept
	{
		return _Entries.size();
	}

	const_iterator begin() const noexcept
	{
		return _Entries.begin();
	}

	const_iterator end() const noexcept
	{
		return _Entries.end();
	}

	const_iterator cbegin() const noexcept
	{
		return _Entries.cbegin();
	}

	const_iterator cend() const noexcept
	{
		return _Entries.cend();
	}

	const_reference operator[](size_type index) const noexcept
	{
		return _Entries[index];
	}

	const_reference at(size_type index) const
	{
		return _Entries.at(index);
	}

	const_reference front() const noexcept
	{
		return _Entries.front();
	}

	const_reference back() const noexcept
	{
		return _Entries.back();
	}

	void swap(stacktrace& other) noexcept
	{
		_Entries.swap(other._Entries);
	}

	friend bool operator==(const stacktrace& lhs, const stacktrace& rhs) noexcept
	{
		return lhs._Entries == rhs._Entries;
	}

	friend bool operator!=(const stacktrace& lhs, const stacktrace& rhs) noexcept
	{
		return !(lhs == rhs);
	}

private:
	::std::vector<stacktrace_entry> _Entries;
};

inline ::std::string to_string(const stacktrace& trace)
{
	::std::string result;
	size_t index = 0;

	for(const auto& entry : trace)
	{
		result += "#";
		result += ::std::to_string(index);
		result += " ";
		result += to_string(entry);
		result += "\n";

		++index;
	}

	return result;
}

inline ::std::ostream& operator<<(::std::ostream& stream, const stacktrace& trace)
{
	return stream << to_string(trace);
}

} // namespace mst
