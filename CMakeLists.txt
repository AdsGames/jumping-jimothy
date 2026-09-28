cmake_minimum_required(VERSION 3.24)

project(JumpingJimothy VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

include(cmake/get_cpm.cmake)
CPMAddPackage("gh:adsgames/asw@0.12.1")
CPMAddPackage("gh:zeux/pugixml@1.16")
CPMAddPackage(
  NAME box2d
  GITHUB_REPOSITORY erincatto/box2d
  VERSION 2.4.2
  OPTIONS
    "BOX2D_BUILD_UNIT_TESTS OFF"
    "BOX2D_BUILD_TESTBED OFF"
    "BOX2D_BUILD_DOCS OFF"
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(${PROJECT_NAME} PRIVATE
  asw
  box2d
  pugixml::pugixml
)

if(EMSCRIPTEN)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "index"
    SUFFIX ".html"
  )

  target_link_options(${PROJECT_NAME} PRIVATE
    -sWASM=1
    -sALLOW_MEMORY_GROWTH=1
    -sMAXIMUM_MEMORY=1gb
    -sSTACK_SIZE=32mb
    -sINITIAL_MEMORY=64mb
  )

  set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "--preload-file ${CMAKE_CURRENT_LIST_DIR}/assets@/assets --use-preload-plugins --shell-file ${CMAKE_CURRENT_LIST_DIR}/index.html"
  )
endif()

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)
