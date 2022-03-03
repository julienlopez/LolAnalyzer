cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(EXE_TARGET_NAME LolAnalyzerServer)

add_executable(
  ${EXE_TARGET_NAME}
  "main.cpp" "RestServer.hpp" "RestServer.cpp")

target_link_libraries(${EXE_TARGET_NAME}
	PRIVATE
	LolAnalyzer::LibLolAnalyzer
)
