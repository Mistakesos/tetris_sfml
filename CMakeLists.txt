cmake_minimum_required(VERSION 3.29.1)

project(tetris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED graphics)

set(INC_LIST
    ${PROJECT_SOURCE_DIR}/include
)

include_directories(${INC_LIST})


aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# set(SRC_LIST
#     ${PROJECT_SOURCE_DIR}/source
# )

add_executable(tetris ${SRC_LIST})

target_link_libraries(tetris sfml-graphics sfml-system)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
