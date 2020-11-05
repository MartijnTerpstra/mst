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

#include <mlogger.h>
#include <fstream>
#include <sys/file.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace mst;

std::string CreatePath()
{
	static int g_fileCounter = 0;

	return "/tmp/mst.logger" + std::to_string(g_fileCounter++) + ".log";
}

static std::string g_path = CreatePath();

void logger::_Log(const ::std::string& str)
{
	int fd = open("/var/lock/mst.logger.lock", O_CREAT, S_IRWXU);
	if(fd == -1)
		return;

	int lock = flock(fd, LOCK_SH);

	while(lock == -1)
	{
		lock = flock(fd, LOCK_SH);

		std::this_thread::yield();
	}

	std::cout << str << std::endl;

	std::ofstream logStream(g_path, std::ios::app | std::ios::ate);

	if(logStream.fail())
		return;

	while(logStream.tellp() > 1024 * 1024 * 10)
	{
		logStream.close();

		g_path = CreatePath();

		logStream.open(g_path, std::ios::app | std::ios::ate);

		if(logStream.fail())
			return;
	}

	logStream << str;

	logStream.close();

	flock(fd, LOCK_UN); // Unlock the file . . .

	close(fd);
}