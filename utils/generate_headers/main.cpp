#include <filesystem>

#include <fstream>
#include <mplatform.h>
#include <string>
#include <string_view>
#include <fstream>

const std::string_view g_oldHeader =
	R"(//////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////)";

const std::string_view g_newHeader =
	R"(//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2014 Martinus Terpstra                                                 //
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
//////////////////////////////////////////////////////////////////////////////////////////////)";

const std::string_view g_newline = R"(
)";

const std::string g_headerCheck = std::string(g_newHeader).append(g_newline).append(g_newline);

void FixFileHeader(const std::filesystem::path& path, std::string_view fileData)
{
	std::ofstream file(path.string(), std::ios::trunc);
	if(fileData.find(g_oldHeader) == 0)
	{
		fileData = fileData.substr(g_oldHeader.length());
	}

	if(fileData.find(g_newHeader) == 0)
	{
		fileData = fileData.substr(g_newHeader.length());
	}
	file.write(g_newHeader.data(), g_newHeader.length());
	if(fileData.length() < g_newline.length() || fileData.find(g_newline) != 0)
	{
		file.write(g_newline.data(), g_newline.length());
		file.write(g_newline.data(), g_newline.length());
	}
	else if(fileData.length() < 2 * g_newline.length() ||
			fileData.find(g_newline) != g_newline.length())
	{
		file.write(g_newline.data(), g_newline.length());
	}
	file.write(fileData.data(), fileData.length());
}

int main()
{
	auto directory = mst::platform::current_directory();

	auto files = std::filesystem::recursive_directory_iterator(directory);

	std::string buffer;
	buffer.resize(g_headerCheck.length());

	for(auto& filenode : files)
	{
		if(!filenode.is_regular_file())
			continue;

		auto& path = filenode.path();

		std::ifstream file(path.string());

		auto filesize = filenode.file_size();

		if(filesize < g_headerCheck.length())
		{
			file.read(buffer.data(), filesize);
		}
		else
		{
			file.read(buffer.data(), g_headerCheck.length());
			if(memcmp(buffer.data(), g_headerCheck.data(), g_headerCheck.length()) == 0)
				continue;

			if(buffer.length() < filesize)
			{
				buffer.resize(filesize);
			}
			file.read(buffer.data() + g_headerCheck.length(), filesize - g_headerCheck.length());
		}
		file.close();
		FixFileHeader({ buffer.data(), filesize });
	}
}