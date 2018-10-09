# CMake
cmake_minimum_required (VERSION 3.8)

# Variables
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(MICRO_VERSION 0)

# Definitions
add_definitions(-DNPE_VERSION="${MAJOR_VERSION}.${MINOR_VERSION}.${MICRO_VERSION}")

# Directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIBNPEPIPE_DIR ${CMAKE_SOURCE_DIR}/src/libnpepipe)
set(NPECLIENT_DIR ${CMAKE_SOURCE_DIR}/src/npeclient)
set(NPESERVER_DIR ${CMAKE_SOURCE_DIR}/src/npeserver)
set(NPEOUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NPEOUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NPEOUTPUT_DIR})

# Files
file(GLOB_RECURSE PUBLIC_HEADERS ${INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE LIBNPEPIPE_SOURCES ${LIBNPEPIPE_DIR}/*.cpp)
file(GLOB_RECURSE NPESERVER_SOURCES ${NPESERVER_DIR}/*.cpp)
file(GLOB_RECURSE NPECLIENT_SOURCES ${NPECLIENT_DIR}/*.cpp)

# Libpipe target
add_library(libnpepipe STATIC ${LIBNPEPIPE_SOURCES})
target_include_directories(libnpepipe PUBLIC ${INCLUDE_DIR})

# Server target
add_executable(npeserver ${NPESERVER_SOURCES}  ${PUBLIC_HEADERS})
target_include_directories(npeserver PUBLIC ${INCLUDE_DIR})
target_link_libraries(npeserver libnpepipe)

# Client target
add_executable(npeclient ${NPECLIENT_SOURCES} ${PUBLIC_HEADERS})
target_include_directories(npeclient PUBLIC ${INCLUDE_DIR})
target_link_libraries(npeclient libnpepipe)
