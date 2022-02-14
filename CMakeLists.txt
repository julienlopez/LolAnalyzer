cmake_minimum_required(VERSION 3.16)

project(LolAnalyzer CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
  set(CMAKE_DEBUG_POSTFIX "d")
else()
  add_definitions("-fprofile-arcs")
  add_definitions("-ftest-coverage")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      add_definitions("-fconcepts")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" OFF)
option(ENABLE_TESTING "Enable unit tests" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS ${CMAKE_BINARY_DIR}/conan.cmake)
  message(STATUS "Downloading conan.cmake 0.17.0.")
  file(
    DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
    ${CMAKE_BINARY_DIR}/conan.cmake
    EXPECTED_HASH SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
    TLS_VERIFY ON)
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/conan.cmake)

set(CONAN_PACKAGES tl-expected/20190710 ms-gsl/3.1.0 cpprestsdk/2.10.18)
if(ENABLE_TESTING)
    message("Enabling Tests.")
    list(APPEND CONAN_PACKAGES catch2/2.13.8)
endif()
message("installing ${CONAN_PACKAGES}")
conan_cmake_configure(REQUIRES ${CONAN_PACKAGES} GENERATORS cmake_find_package BUILD missing)

conan_cmake_autodetect(settings)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # gcc
    conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings} 
                    SETTINGS compiler.libcxx=libstdc++11)
else()                
    conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})
endif()

find_package(tl-expected)
find_package(cpprestsdk)

include_directories(SYSTEM 3rd_party/expected_task/lib)
include_directories(SYSTEM 3rd_party/higherorderfunctions/include)

add_subdirectory(libs)
add_subdirectory(exe)

if(ENABLE_TESTING)
    enable_testing()
    message("Enabling Tests.")
    add_subdirectory(unit_tests)
endif()
