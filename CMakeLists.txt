cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

project (JumpingJimothy)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable (${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Find libs
find_library(ALLEGRO_LIBRARY NAMES allegro REQUIRED)
find_library(ALLEGRO_ACODEC_LIBRARY NAMES allegro_acodec REQUIRED)
find_library(ALLEGRO_AUDIO_LIBRARY NAMES allegro_audio REQUIRED)
find_library(ALLEGRO_COLOR_LIBRARY NAMES allegro_color REQUIRED)
find_library(ALLEGRO_DIALOG_LIBRARY NAMES allegro_dialog REQUIRED)
find_library(ALLEGRO_FONT_LIBRARY NAMES allegro_font REQUIRED)
find_library(ALLEGRO_IMAGE_LIBRARY NAMES allegro_image REQUIRED)
find_library(ALLEGRO_MAIN_LIBRARY NAMES allegro_main REQUIRED)
find_library(ALLEGRO_PRIMITIVES_LIBRARY NAMES allegro_primitives REQUIRED)
find_library(ALLEGRO_TTF_LIBRARY NAMES allegro_ttf REQUIRED)
find_library(BOX2D_LIBRARY NAMES Box2D REQUIRED)

# Link Libs
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_ACODEC_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_AUDIO_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_COLOR_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_DIALOG_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_FONT_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_IMAGE_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_MAIN_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_PRIMITIVES_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_TTF_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${BOX2D_LIBRARY})
