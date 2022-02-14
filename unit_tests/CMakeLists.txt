cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(EXE_TARGET_NAME UnitTests)

find_package(Catch2)

add_executable(
  ${EXE_TARGET_NAME}
  "main.cpp")

target_link_libraries(${EXE_TARGET_NAME}
	PRIVATE
	LolAnalyzer::LibLolAnalyzer
	Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(${EXE_TARGET_NAME})
