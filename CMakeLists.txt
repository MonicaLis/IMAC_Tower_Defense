cmake_minimum_required(VERSION 2.8.9)
project(itd)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# includes cmake/FindSDL2.cmake and ttf
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.c")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

if (SDL2_LIBRARIES)
    message("Found SDL2: " ${SDL2_LIBRARIES})
endif()
if (OPENGL_LIBRARIES)
    message("Found OpenGL: " ${OPENGL_LIBRARIES})
endif()
if (SDL2TTF_LIBRARY)
    message("Found TTF: " ${SDL2TTF_LIBRARY})
endif()

# include
include_directories(
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIR}
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
	${SDL2TTF_LIBRARY}
	STB_IMAGE 
	-ldl
)

