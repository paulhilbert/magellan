cmake_minimum_required(VERSION 2.8.8)
project(magellan)

set (magellan_VERSION_MAJOR 0)
set (magellan_VERSION_MINOR 1)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-fPIC)
	add_definitions(-O3)
	add_definitions(-g)
	add_definitions(-std=c++11)
    add_definitions(-Wall)
    #add_definitions(-Wno-unused-function)
    #add_definitions(-Wno-unused-local-typedefs)
endif()

file (GLOB_RECURSE obj RELATIVE "${PROJECT_SOURCE_DIR}" "src/*.cpp")
#if (ASIO_FOUND)
    add_library(magellan SHARED ${obj})
    target_link_libraries(magellan "dl")

	# install binary
    install (TARGETS magellan DESTINATION lib)
	# install header
    install (DIRECTORY include/ DESTINATION include/magellan)
#endif()
