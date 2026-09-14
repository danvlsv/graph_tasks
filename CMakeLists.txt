cmake_minimum_required(VERSION 3.28)

project (graph_tasks
  VERSION 0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE PERCOLATION_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(graph_core STATIC ${PERCOLATION_SOURCES})
target_include_directories(graph_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(graph_core PUBLIC fmt::fmt)

target_compile_features(graph_core PUBLIC cxx_std_23)

file(GLOB TASK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tasks/*.cpp"
)

foreach(TASK_SRC IN LISTS TASK_SOURCES)
    get_filename_component(TASK_NAME ${TASK_SRC} NAME_WE)
    add_executable(${TASK_NAME} ${TASK_SRC})
    target_link_libraries(${TASK_NAME} PRIVATE graph_core)
endforeach()
