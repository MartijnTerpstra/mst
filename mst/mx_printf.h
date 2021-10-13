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

// note no include guards

namespace _Details {

template<typename First MST_VARIADIC_TEMPLATE>
inline void _Check_printf(const char* formatString, size_t strIndex, size_t strLength,
	First first MST_VARIADIC_TEMPLATE_REFFS_COMMA)
{
	typedef typename ::std::remove_cv<typename ::std::remove_reference<First>::type>::type Type;

	for(size_t i = strIndex; i < strLength - 1; ++i)
	{
		if(formatString[i] == '%')
		{
			switch(formatString[i + 1])
			{
			case 'd':
			case 'i':
				// int
				if(!::std::is_integral<Type>::value || !::std::is_signed<Type>::value)
				{
					ERROR_MESG("%d or %i must be used on signed integer values");
				}
				break;
			case 'u':
			case 'o':
			case 'x':
			case 'X':
				// unsigned int
				if(!::std::is_integral<Type>::value || !::std::is_unsigned<Type>::value)
				{
					ERROR_MESG("%u, %o, %x or %X must be used on unsigned integer values");
				}
				break;
			case 'f':
			case 'F':
			case 'e':
			case 'E':
			case 'g':
			case 'G':
			case 'a':
			case 'A':
				// floating point
				if(!::std::is_floating_point<Type>::value)
				{
					ERROR_MESG(
						"%f, %F, %e, %E, %g, %G, %a or %A must be used on floating point values");
				}
				break;
			case 'c':
				// char
				if(!::std::is_same<First, char>::value)
				{
					ERROR_MESG("%c must be used on char values");
				}
				break;
			case 's':
				// c-string
				if(!::std::is_same<Type, const char*>::value && !::std::is_same<Type, char*>::value)
				{
					ERROR_MESG("%s must be used on c-string values");
				}
				break;
			case 'p':
				// pointer
				if(!::std::is_pointer<Type>::value &&
					!::std::is_same<Type, ::std::nullptr_t>::value)
				{
					ERROR_MESG("%p must be used on pointer values");
				}
				break;
			default:
				continue;
			}

			_Check_printf(formatString, i + 1, strLength MST_VARIADIC_TEMPLATE_FORWARDS_COMMA);
			return;
		}
	}
}

} // namespace _Details

MST_VARIADIC_TEMPLATE_DECL
inline void printf(const char* formatString MST_VARIADIC_TEMPLATE_REFFS_COMMA)
{
#if MST_DEBUGMODE
	::mst::_Details::_Check_printf(
		formatString, 0, strlen(formatString) MST_VARIADIC_TEMPLATE_FORWARDS_COMMA);
#endif
	printf(formatString MST_VARIADIC_TEMPLATE_FORWARDS_COMMA);
}

template<size_t N MST_VARIADIC_TEMPLATE>
inline void sprintf(
	char (&outputString)[N], const char* formatString MST_VARIADIC_TEMPLATE_REFFS_COMMA)
{
#if MST_DEBUGMODE
	::mst::_Details::_Check_printf(
		formatString, 0, strlen(formatString) MST_VARIADIC_TEMPLATE_FORWARDS_COMMA);
#endif
	sprintf(outputString, formatString MST_VARIADIC_TEMPLATE_FORWARDS_COMMA);
}