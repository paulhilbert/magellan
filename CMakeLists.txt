cmake_minimum_required(VERSION 2.8.8)
project(magellan)

set (magellan_VERSION_MAJOR 0)
set (magellan_VERSION_MINOR 1)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

if(CMAKE_COMPILER_IS_GNUCXX)
    if (NOT WIN32)
        add_definitions(-fPIC)
    else()
        add_definitions(-DM_LOG2E=1.44269504088896340736)
    endif()
	add_definitions(-O0)
	add_definitions(-g)
	add_definitions(-std=c++14)
    add_definitions(-Wall)
    add_definitions(-Wno-unused-function)
    add_definitions(-Wno-unused-local-typedefs)
    add_definitions(-Wno-deprecated-declarations)
    add_definitions(-Wno-write-strings)
    add_definitions(-DBOOST_USE_VALGRIND)
    add_definitions(-DBOOST_OPTIONAL_USE_OLD_DEFINITION_OF_NONE)
endif()

find_package(Boost COMPONENTS system filesystem coroutine)
find_package(Asio)

file (GLOB_RECURSE obj RELATIVE "${PROJECT_SOURCE_DIR}" "src/*.cpp")
if (ASIO_FOUND AND Boost_FOUND)
	include_directories(${ASIO_INCLUDE_DIRS})
	include_directories(${Boost_INCLUDE_DIRS})

	add_library(magellan SHARED ${obj})
	target_link_libraries(magellan ${Boost_LIBRARIES} "pthread")

    # echo example
	add_executable(echo_server "${PROJECT_SOURCE_DIR}/examples/echo_server.cpp")
	target_link_libraries(echo_server magellan)
	add_executable(echo_client "${PROJECT_SOURCE_DIR}/examples/echo_client.cpp")
	target_link_libraries(echo_client magellan)

    # send_container example
	add_executable(send_container_server "${PROJECT_SOURCE_DIR}/examples/send_container_server.cpp")
	target_link_libraries(send_container_server magellan)
	add_executable(send_container_client "${PROJECT_SOURCE_DIR}/examples/send_container_client.cpp")
	target_link_libraries(send_container_client magellan)

	#add_executable(stream_server ${obj} "${PROJECT_SOURCE_DIR}/apps/stream_server.cpp")
	#target_link_libraries(stream_server ${Boost_LIBRARIES} "pthread")

	# install binary
    install (TARGETS magellan DESTINATION lib)
	# install header
	install (DIRECTORY include/ DESTINATION include/magellan)
endif()

