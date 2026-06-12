cmake_minimum_required(VERSION 3.23)
project(3DEngineCpp)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# using cpp 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#compiler flags
add_compile_options(-Wall -Wextra)

#getting the source files
file(GLOB_RECURSE SOURCES 
	src/*.cpp
	src/*.c)

#Remove the main function
list(REMOVE_ITEM SOURCES ${3DEngineCpp_SOURCE_DIR}/src/main.cpp)

#Create a library
add_library(engine STATIC ${SOURCES})

#Setting variable to the cmake directory
set(3DEngineCpp_CMAKE_DIR ${3DEngineCpp_SOURCE_DIR}/cmake)

# adding dependencies
find_package(OpenGL REQUIRED)
INCLUDE(${3DEngineCpp_CMAKE_DIR}/FindGLEW.cmake)
INCLUDE(${3DEngineCpp_CMAKE_DIR}/FindSDL2.cmake)
INCLUDE(${3DEngineCpp_CMAKE_DIR}/FindASSIMP.cmake)


# showing the headers to the compiler
target_include_directories(engine PRIVATE
    ${3DEngineCpp_SOURCE_DIR}/src    # your own headers
    ${OPENGL_INCLUDE_DIRS}           # where GL/gl.h lives
    ${GLEW_INCLUDE_DIRS}             # where GL/glew.h lives
    ${SDL2_INCLUDE_DIRS}             # where SDL2/SDL.h lives
    ${ASSIMP_INCLUDE_DIRS}           # where assimp headers live
)

# showing linker where to link to 
target_link_libraries(engine PRIVATE
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${SDL2_LIBRARIES}
	${ASSIMP_LIBRARIES}

)

#created the executable file and linked it to our engine library 
add_executable(3DEngineCpp ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(3DEngineCpp engine)

#Make sure you have Gtest
find_package(GTest REQUIRED)

#find the test files
file(GLOB_RECURSE TESTS
	${CMAKE_SOURCE_DIR}/test/*.cpp
)

#allow testing
enable_testing()

# create executable
add_executable(testing ${TESTS})

# link lib + gtest to executable
target_link_libraries(testing 
	engine
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testing)









