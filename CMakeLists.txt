cmake_minimum_required(VERSION 3.20)
project(MovieManager C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(MovieManager PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
)

# Source files
set(SOURCES
        src/movie.c
        src/file_io.c
        src/ui.c
        src/auth.c
        src/main.c
)

# Create executable
add_executable(MovieManager ${SOURCES})

