cmake_minimum_required(VERSION 2.8.9)
project(itd)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

#----------------------------------DEBUG
if (SDL2_LIBRARIES)
    message("Found SDL2: " ${SDL2_LIBRARIES})
endif()
if (OPENGL_LIBRARIES)
    message("Found OpenGL: " ${OPENGL_LIBRARIES})
endif()


# include
include_directories(
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	include
	src
	lib
)

# source
file(GLOB SOURCES "src/*.cpp")
add_library(STB_IMAGE STATIC "lib/stb_image/stb_image.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
	${PROJECT_NAME} 
	${OPENGL_LIBRARIES} 
	${SDL2_LIBRARIES}
	STB_IMAGE 
	-ldl
)

