//////////////////////////////////////////////////////////////////////////////////////////////
//																							//
//		MST Utility Library							 										//
//		Copyright (c)2014 Martinus Terpstra													//
//																							//
//		Permission is hereby granted, free of charge, to any person obtaining a copy		//
//		of this software and associated documentation files (the "Software"), to deal		//
//		in the Software without restriction, including without limitation the rights		//
//		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell			//
//		copies of the Software, and to permit persons to whom the Software is				//
//		furnished to do so, subject to the following conditions:							//
//																							//
//		The above copyright notice and this permission notice shall be included in			//
//		all copies or substantial portions of the Software.									//
//																							//
//		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR			//
//		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
//		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE			//
//		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER				//
//		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		//
//		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN			//
//		THE SOFTWARE.																		//
//																							//
//////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mcore.h>
#include <sstream>

namespace mst {

class logger
{
	typedef int _Log_type;

	enum
	{
		_Log_info = 0,
		_Log_error,
		_Log_warning
	};
public:

#ifndef MST_DEBUG_LOGGING
	static inline void error(const std::string& text)
	{
		_Message(_Log_error, text);
	}

	static inline void warning(const std::string& text)
	{
		_Message(_Log_warning, text);
	}

	static inline void info(const std::string& text)
	{
		_Message(_Log_info, text);
	}

	template<typename _Type>
	static inline void error(const std::string& text, const _Type& value)
	{
		_Message(_Log_error, text, value);
	}

	template<typename _Type>
	static inline void warning(const std::string& text, const _Type& value)
	{
		_Message(_Log_warning, text, value);
	}

	template<typename _Type>
	static inline void info(const std::string& text, const _Type& value)
	{
		_Message(_Log_info, text, value);
	}

#else
	static inline void error(const std::string& text) {}

	static inline void warning(const std::string& text) {}

	static inline void info(const std::string& text) {}

	template<typename _Type>
	static inline void error(const std::string& text, _Type value) {}

	template<typename _Type>
	static inline void warning(const std::string& text, _Type value) {}

	template<typename _Type>
	static inline void info(const std::string& text, _Type value) {}

#endif

private:

	inline static void _Message(_Log_type type, const ::std::string& text)
	{
		::std::stringstream str;

		switch (type)
		{
		case _Log_info:
			str << "< INFO>: ";
			break;
		case _Log_warning:
			str << "< WARN>: ";
			break;
		case _Log_error:
			str << "<ERROR>: ";
			break;
		}

		str << text << ::std::endl;

		_Log(str.str());
	}

	template<typename _Type>
	inline static void _Message(_Log_type type, const ::std::string& text, const _Type& value)
	{
		::std::stringstream str;

		switch (type)
		{
		case _Log_info:
			str << "< INFO>: ";
			break;
		case _Log_warning:
			str << "< WARN>: ";
			break;
		case _Log_error:
			str << "<ERROR>: ";
			break;
		}

		str << text << ": " << value << ::std::endl;

		_Log(str.str());
	}

	static void _Log(const ::std::string&);
};

}