cmake_minimum_required(VERSION 2.8)
project(commsnode)
 
# Try to find the Boost libraries and includes automagically
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem regex system thread REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# Header files from this project
include_directories(include)
 
# Source files from this project
file(GLOB SOURCES "src/*.cpp")
 
add_executable(commsnode ${SOURCES})
target_link_libraries(commsnode ${Boost_LIBRARIES})
