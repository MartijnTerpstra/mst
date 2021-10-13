//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)2021 Martinus Terpstra                                                 //
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

#include <cstring>
#include <filesystem>

#include <fstream>
#include <mplatform.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <fstream>
#include <regex>
#include <vector>
#include <utility>
#include <stdexcept>
#include <chrono>
#include <ctime>

const std::regex g_oldHeader{
	R"(\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\n\/\/\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\tMST Utility Library\t\t\t\t\t\t\t \t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\tCopyright \(c\)\d\d\d\d Martinus Terpstra\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\tPermission is hereby granted, free of charge, to any person obtaining a copy\t\t\/\/\n\/\/\t\tof this software and associated documentation files \(the "Software"\), to deal\t\t\/\/\n\/\/\t\tin the Software without restriction, including without limitation the rights\t\t\/\/\n\/\/\t\tto use, copy, modify, merge, publish, distribute, sublicense, and\/or sell\t\t\t\/\/\n\/\/\t\tcopies of the Software, and to permit persons to whom the Software is\t\t\t\t\/\/\n\/\/\t\tfurnished to do so, subject to the following conditions:\t\t\t\t\t\t\t\/\/\n\/\/\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\tThe above copyright notice and this permission notice shall be included in\t\t\t\/\/\n\/\/\t\tall copies or substantial portions of the Software\.\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\tTHE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\t\t\t\/\/\n\/\/\t\tIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\t\t\t\/\/\n\/\/\t\tFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT\. IN NO EVENT SHALL THE\t\t\t\/\/\n\/\/\t\tAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\t\t\t\t\/\/\n\/\/\t\tLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\t\t\/\/\n\/\/\t\tOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\t\t\t\/\/\n\/\/\t\tTHE SOFTWARE\.\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\/\/\n\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/)"
};

const std::regex g_newOldHeaderCpp{
	R"(\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\n\/\/                                                                                          \/\/\n\/\/      MST Utility Library                                                                 \/\/\n\/\/      Copyright \(c\)\d\d\d\d Martinus Terpstra                                                 \/\/\n\/\/                                                                                          \/\/\n\/\/      Permission is hereby granted, free of charge, to any person obtaining a copy        \/\/\n\/\/      of this software and associated documentation files \(the "Software"\), to deal       \/\/\n\/\/      in the Software without restriction, including without limitation the rights        \/\/\n\/\/      to use, copy, modify, merge, publish, distribute, sublicense, and\/or sell           \/\/\n\/\/      copies of the Software, and to permit persons to whom the Software is               \/\/\n\/\/      furnished to do so, subject to the following conditions:                            \/\/\n\/\/                                                                                          \/\/\n\/\/      The above copyright notice and this permission notice shall be included in          \/\/\n\/\/      all copies or substantial portions of the Software\.                                 \/\/\n\/\/                                                                                          \/\/\n\/\/      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          \/\/\n\/\/      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            \/\/\n\/\/      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT\. IN NO EVENT SHALL THE         \/\/\n\/\/      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              \/\/\n\/\/      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       \/\/\n\/\/      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           \/\/\n\/\/      THE SOFTWARE\.                                                                       \/\/\n\/\/                                                                                          \/\/\n\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/)"
};

const std::regex g_newOldHeaderHash{
	R"(##############################################################################################\n##                                                                                          ##\n##      MST Utility Library                                                                 ##\n##      Copyright \(c\)\d\d\d\d Martinus Terpstra                                                 ##\n##                                                                                          ##\n##      Permission is hereby granted, free of charge, to any person obtaining a copy        ##\n##      of this software and associated documentation files \(the "Software"\), to deal       ##\n##      in the Software without restriction, including without limitation the rights        ##\n##      to use, copy, modify, merge, publish, distribute, sublicense, and#or sell           ##\n##      copies of the Software, and to permit persons to whom the Software is               ##\n##      furnished to do so, subject to the following conditions:                            ##\n##                                                                                          ##\n##      The above copyright notice and this permission notice shall be included in          ##\n##      all copies or substantial portions of the Software\.                                 ##\n##                                                                                          ##\n##      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          ##\n##      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            ##\n##      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT\. IN NO EVENT SHALL THE         ##\n##      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              ##\n##      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       ##\n##      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           ##\n##      THE SOFTWARE\.                                                                       ##\n##                                                                                          ##\n##############################################################################################)"
};

const std::regex g_licence{ "Copyright (c) \\d\\d\\d\\d MartijnTerpstra" };

std::string g_newHeaderCpp =
	R"(//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//      MST Utility Library                                                                 //
//      Copyright (c)____ Martinus Terpstra                                                 //
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

std::string g_newHeaderHash =
	R"(##############################################################################################
##                                                                                          ##
##      MST Utility Library                                                                 ##
##      Copyright (c)____ Martinus Terpstra                                                 ##
##                                                                                          ##
##      Permission is hereby granted, free of charge, to any person obtaining a copy        ##
##      of this software and associated documentation files (the "Software"), to deal       ##
##      in the Software without restriction, including without limitation the rights        ##
##      to use, copy, modify, merge, publish, distribute, sublicense, and#or sell           ##
##      copies of the Software, and to permit persons to whom the Software is               ##
##      furnished to do so, subject to the following conditions:                            ##
##                                                                                          ##
##      The above copyright notice and this permission notice shall be included in          ##
##      all copies or substantial portions of the Software.                                 ##
##                                                                                          ##
##      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR          ##
##      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,            ##
##      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE         ##
##      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER              ##
##      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,       ##
##      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN           ##
##      THE SOFTWARE.                                                                       ##
##                                                                                          ##
##############################################################################################)";

std::string GetCurrentYear()
{
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);

	return std::to_string(tstruct.tm_year + 1900);
}

std::string_view DelimitedNewHeader(char delimiter)
{
	if(delimiter == '/')
	{
		if(auto yearpos = g_newHeaderCpp.find("____"); yearpos != std::string::npos)
		{
			auto year = GetCurrentYear();
			g_newHeaderCpp.replace(yearpos, 4, year.c_str());
		}
		return g_newHeaderCpp;
	}
	if(delimiter == '#')
	{
		if(auto yearpos = g_newHeaderHash.find("____"); yearpos != std::string::npos)
		{
			auto year = GetCurrentYear();
			g_newHeaderHash.replace(yearpos, 4, year.c_str());
		}
		return g_newHeaderHash;
	}
	throw new std::invalid_argument("delimiter");
}

const std::string_view g_newline = R"(
)";

using FileType = std::pair<std::regex, char>;

void FixFileHeader(const std::filesystem::path& path, char delimiter, std::string fileData)
{
	std::ofstream file(path.string(), std::ios::trunc | std::ios::binary);

	fileData = std::regex_replace(fileData, g_oldHeader, "");
	fileData = std::regex_replace(fileData, g_newOldHeaderCpp, "");
	fileData = std::regex_replace(fileData, g_newOldHeaderHash, "");

	auto newHeader = DelimitedNewHeader(delimiter);

	while(fileData.find(g_newline) == 0)
	{
		fileData = fileData.substr(g_newline.length());
	}
	file.write(newHeader.data(), newHeader.length());
	file.write(g_newline.data(), g_newline.length());
	file.write(g_newline.data(), g_newline.length());
	file.write(fileData.data(), fileData.length());
}

void ProcessFile(
	std::string& buffer, const std::filesystem::directory_entry& filenode, char delimiter)
{
	auto& path = filenode.path();

	std::ifstream file(path.string());

	auto filesize = filenode.file_size();

	auto newHeader = DelimitedNewHeader(delimiter);

	auto headerCheckSize = newHeader.length() + 2 * g_newline.length();

	if(buffer.length() < filesize)
	{
		buffer.resize(filesize);
	}

	if(filesize < headerCheckSize)
	{
		file.read(buffer.data(), filesize);
	}
	else
	{
		file.read(buffer.data(), headerCheckSize);
		if(memcmp(buffer.data(), newHeader.data(), newHeader.length()) == 0 &&
			memcmp(buffer.data() + newHeader.length(), g_newline.data(), g_newline.length()) == 0 &&
			memcmp(buffer.data() + newHeader.length() + g_newline.length(), g_newline.data(),
				g_newline.length()) == 0)
		{
			return;
		}
		file.read(buffer.data() + headerCheckSize, filesize - headerCheckSize);
	}
	file.close();
	FixFileHeader(path, delimiter, { buffer.data(), filesize });
}

std::vector<FileType> mathingFileTypes;

int main(int argc, const char* const* argv)
{
	if(argc < 2)
	{
		printf("Please pass the project folder");
		return 1;
	}
	auto directory = argv[1];

	std::ifstream checkFile{ std::string{ directory } + mst::platform::directory_separator() +
							 "CMakeLists.txt" };

	if(checkFile.fail())
	{
		printf("Failed to open the root CMakeLists.txt");
	}

	if(checkFile)
	{
		std::string line;
		while(!checkFile.eof())
		{
			std::getline(checkFile, line);

			if(line.empty())
				continue;

			if(line.find("project (mst CXX)") == 0)
			{
				break;
			}
		}
		if(checkFile.eof())
		{
			printf("Could not find 'project (mst CXX)' in the root CMakeLists.txt");
			return 1;
		}
		checkFile.close();
	}

	mathingFileTypes.emplace_back(std::regex{ ".+\\.cpp$" }, '/');
	mathingFileTypes.emplace_back(std::regex{ ".+\\.h$" }, '/');
	mathingFileTypes.emplace_back(std::regex{ ".+\\.inl$" }, '/');
	mathingFileTypes.emplace_back(std::regex{ "^mst\\.natvis$" }, '/');
	mathingFileTypes.emplace_back(
		std::regex{ "^CMakeLists\\.txt$", std::regex_constants::ECMAScript }, '#');

	std::string buffer;

	std::vector<std::string> folders = { "mst", "src", "tests", "dependencies", "utils" };

	for(auto& folder : folders)
	{
		auto files = std::filesystem::recursive_directory_iterator(
			std::string(directory) + mst::platform::directory_separator() + folder);

		for(auto& filenode : files)
		{
			if(!filenode.is_regular_file())
				continue;

			auto filename = filenode.path().filename().string();

			FileType* foundType = nullptr;
			for(auto& ft : mathingFileTypes)
			{
				if(std::regex_match(filename, ft.first))
				{
					foundType = &ft;
					break;
				}
			}

			if(!foundType)
			{
				printf("Skipping '%s': no file type matched\n", filename.c_str());
				continue;
			}

			ProcessFile(buffer, filenode, foundType->second);
		}
	}

	std::filesystem::directory_entry clang_format{
		std::string(directory) + mst::platform::directory_separator() + ".clang-format"
	};

	std::filesystem::directory_entry git_ignore{
		std::string(directory) + mst::platform::directory_separator() + ".gitignore"
	};

	std::filesystem::directory_entry rootCMakeLists{
		std::string(directory) + mst::platform::directory_separator() + "CMakeLists.txt"
	};

	ProcessFile(buffer, clang_format, '#');
	ProcessFile(buffer, git_ignore, '#');
	ProcessFile(buffer, rootCMakeLists, '#');

	std::ifstream licence{ std::string(directory) + mst::platform::directory_separator() +
							   "LICENSE",
		std::ios::binary | std::ios::ate };
	if(licence.is_open())
	{
		std::string licenseBody;
		licenseBody.resize(licence.tellg());
		licence.seekg(0);
		licence.read(licenseBody.data(), licenseBody.length());
		licence.close();

		auto newBody =
			std::regex_replace(licenseBody, std::regex{ "\\d\\d\\d\\d" }, GetCurrentYear());

		std::ofstream newLicense{ std::string(directory) + mst::platform::directory_separator() +
									  "LICENSE",
			std::ios::binary };

		newLicense.write(newBody.data(), newBody.length());
	}
}