cmake_minimum_required(VERSION 3.12)


project(Spiel)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)  # Replace '11' with your desired C++ version
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/root/Spiel/build/_deps/nlohmann_json-src/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/root/Spiel/build/_deps/cli11-src/include")
find_package(cli11 QUIET)
if(NOT cli11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(cli11)
endif()

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)
endif()


add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" ) 

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
# Link against the C++ standard library
#target_link_libraries("${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" stdc++)
target_link_libraries(${PROJECT_NAME} CLI11::CLI11 nlohmann_json::nlohmann_json)
