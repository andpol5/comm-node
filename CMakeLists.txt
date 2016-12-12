cmake_minimum_required(VERSION 2.8.8)
set(PROJECT_NAME_STR commsnode)
project(${PROJECT_NAME_STR} C CXX)

find_package(Threads REQUIRED)
 
# Try to find the Boost libraries and includes automagically
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS filesystem regex system thread date_time REQUIRED)
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIR})
endif()

# Set C++11 flag
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wall -Wextra -Werror -pthread)
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-std=c++11 -Wall -Wextra -Werror -D_VARIADIC_MAX=10")
endif() 

#--------------------------------------
# Main module lib (all source files except main.cpp)
#--------------------------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_lib)
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})
# Source the cpp files  
set(COMMSNODE_LIB_SOURCES 
    src/CommNodeMap.cpp
    src/CommsNodeAsyncTcpServer.cpp
    src/CommsNodeClient.cpp
    src/CommsNodeDiscoverableService.cpp
    src/CommsNodeDiscoveryService.cpp
    src/DiscoveryMulticastMessage.cpp
    src/UuidGenerator.cpp
    )
add_library(${PROJECT_LIB_NAME} ${COMMSNODE_LIB_SOURCES})

#--------------------------------------
# Main binary (main.cpp + the library)
#--------------------------------------
set(MAIN_SOURCE src/main.cpp)
add_executable(${PROJECT_NAME_STR} ${MAIN_SOURCE})
target_link_libraries(${PROJECT_NAME_STR} ${PROJECT_LIB_NAME} ${Boost_LIBRARIES})

#--------------------------------------
# Test
#--------------------------------------
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

add_definitions(${MSVC_COMPILER_DEFS})
enable_testing()

set(PROJECT_TEST_NAME test_${PROJECT_NAME_STR})
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)


if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
else()
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
endif()

target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_LIB_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
add_test(test1 ${PROJECT_TEST_NAME})

