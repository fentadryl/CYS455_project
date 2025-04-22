cmake_minimum_required(VERSION 3.20)
project(MovieManager C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories - prefer target_include_directories
directories(MovieManager PRIVATE 
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

# Find OpenSSL package (required for SHA256)
find_package(OpenSSL REQUIRED)

# Link OpenSSL
target_link_libraries(MovieManager PRIVATE OpenSSL::Crypto)
