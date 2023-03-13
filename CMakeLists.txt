cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)

project(entropy)


# Add the header-only library
file(GLOB HEADER_FILES "include/*.hpp")
file(GLOB SRC_FILES "src/*.cpp")

# Add the header files to your project
add_library(entropy STATIC ${HEADER_FILES} ${SRC_FILES}) 
target_include_directories(entropy PUBLIC include)

# Add the executable to test the library
add_executable(test_entropy test/main.cpp)

# Link the executable with the library
target_link_libraries(test_entropy PRIVATE entropy)

if(MSVC)
  target_compile_options(test_entropy PRIVATE /W4 /WX)
else()
  target_compile_options(test_entropy PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
