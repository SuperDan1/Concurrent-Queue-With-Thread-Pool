cmake_minimum_required(VERSION 3.10)

project(concurrentQueue C)

set(CMAKE_C_STANDARD 11)

# default build type : Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src SEC_LIST)

find_package(Threads)

ADD_EXECUTABLE(concurrentQueue ${SEC_LIST})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
