cmake_minimum_required(VERSION 2.8.9)
project(itd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(SDL2_INCLUDE_DIR /usr/include/SDL2)
set(SDL2_LIBRARY /usr/lib/libSDL2.so)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.c")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if (SDL2_LIBRARIES)
    message("Found SDL2: " ${SDL2_LIBRARIES})
endif()
if (OPENGL_LIBRARIES)
    message("Found OpenGL: " ${SDL2_LIBRARIES})
endif()


# include
include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL_TTF_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	include
	src
	lib
)

# source
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
	${PROJECT_NAME} 
	${OPENGL_LIBRARIES} 
	${SDL2_LIBRARIES}
	STB_IMAGE 
	-ldl
)

