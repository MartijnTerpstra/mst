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

// Force-included (via /FI, see SETUP_TARGET in the top-level CMakeLists.txt) into every MSVC
// Debug test translation unit. Without this, a checked-iterator violation, heap corruption, or
// similar Debug-CRT check pops a modal "Debug Assertion Failed!" dialog that hangs the process
// until someone dismisses it - fatal for an unattended/CI run. This redirects that reporting to
// stderr instead: the check still runs and the process still aborts, only how the failure is
// reported changes, so a broken test fails loudly in the log rather than hanging on a dialog.

#pragma once

#include <mcore.h>

#if defined(_WIN32) && defined(MST_DEBUGMODE)

#include <crtdbg.h>

namespace mst {
namespace _Details {

struct crt_report_redirect
{
	crt_report_redirect() noexcept
	{
		::_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
		::_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	}
};

static const crt_report_redirect _CrtReportRedirectInstance;

} // namespace _Details
} // namespace mst

#endif // defined(_WIN32) && defined(MST_DEBUGMODE)
