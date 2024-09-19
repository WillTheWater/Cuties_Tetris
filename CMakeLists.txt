cmake_minimum_required(VERSION 3.30.3)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Copy the assets folder (with subfolders) to the output directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Copying assets folder to output directory"
)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
