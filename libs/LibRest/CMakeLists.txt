set(LIBRARY_TARGET_NAME LibRest)

set(LIB_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

file(GLOB_RECURSE ${LIBRARY_TARGET_NAME}_HDR "./ExpectedTask.hpp" "./JsonParser.hpp" "./Result.hpp" "./Utils.hpp" "./RequestSplitter.hpp")
file(GLOB_RECURSE ${LIBRARY_TARGET_NAME}_SRC "./Utils.cpp" "./RequestSplitter.cpp")

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_HDR} ${${LIBRARY_TARGET_NAME}_SRC})

add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_include_directories(
  ${LIBRARY_TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                   "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

install(TARGETS ${LIBRARY_TARGET_NAME} EXPORT ${PROJECT_NAME})

install(FILES ${${LIBRARY_TARGET_NAME}_HDR} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}")

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC tl::expected cpprestsdk::cpprestsdk)

message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
