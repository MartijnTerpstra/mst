cmake_minimum_required (VERSION 3.13)

## X11 windowing system
find_package(X11 REQUIRED)

file (GLOB SOURCES *.cpp)

add_executable(dev ${SOURCES})

target_link_libraries(dev PRIVATE
    mst::mst
	${X11_LIBRARIES})