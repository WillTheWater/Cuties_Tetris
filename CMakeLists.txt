cmake_minimum_required(VERSION 3.30.3)
project(Cuties_Tetris LANGUAGES CXX)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Create assets and include directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

# Include FetchContent for SFML
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Add source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Add executable and specify source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Add include directory and make it visible in VS
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio)

# Ensure C++20 is enabled
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Define SFML_STATIC if building statically (optional)
if(WIN32 AND NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
endif()

# Copy the assets folder (with subfolders) to the output directory after build
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets folder to output directory"
)

# Copy the assets folder to the build root directory (where the project files are)
add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets folder to build directory"
)

# Copy OpenAL DLL for Windows builds
if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
