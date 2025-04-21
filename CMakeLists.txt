cmake_minimum_required(VERSION 3.20)
project(MovieManager C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
        src/movie.c
        src/file_io.c
        src/ui.c
        src/auth.c
        include/movie.h
        include/file_io.h
        include/auth.h
        include/ui.h
        src/movie.c
        src/file_io.c
        src/ui.c
        src/main.c
)

# Create executable
add_executable(MovieManager ${SOURCES})

# For OpenSSL if you're using password hashing
#find_package(OpenSSL REQUIRED)
#target_link_libraries(MovieManager PRIVATE OpenSSL::Crypto)