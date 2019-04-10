cmake_minimum_required(VERSION 2.8.9)
project(itd)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if (SDL2_LIBRARIES)
    message("Found SDL2: " ${SDL2_LIBRARIES})
endif()

# include
include_directories(
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	include
	src
)

# source
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} -ldl)

