cmake_minimum_required(VERSION 2.8.12)
project(APP)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-std=c++17)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
link_libraries(stdc++fs)
endif()

include_directories(include)

find_library(GUROBI_LIBRARY_CPLUS libgurobi_c++.a OR gurobi_c++mdd2019.lib PATHS /opt/local/stow/gurobi910/linux64/lib/ /Library/gurobi903/mac64/lib/ C:/gurobi951/win64/lib/)
find_library(GUROBI_LIBRARY libgurobi91.so OR libgurobi90.dylib OR gurobi95.lib PATHS /opt/local/stow/gurobi910/linux64/lib/ /Library/gurobi903/mac64/lib/ C:/gurobi951/win64/lib/)

# if we find the library, we can use it
if(GUROBI_LIBRARY_CPLUS AND GUROBI_LIBRARY)
    set(GUROBI_LIBRARIES ${GUROBI_LIBRARY_CPLUS} ${GUROBI_LIBRARY})
    message(STATUS "GUROBI_LIBRARIES : ${GUROBI_LIBRARIES}")
    set(GUROBI_INCLUDE_DIR /opt/local/stow/gurobi910/linux64/include/ /Library/gurobi903/mac64/include/ C:/gurobi951/win64/include/)
    include_directories(include + ${GUROBI_INCLUDE_DIR})

    # mip executable
    file(GLOB SRC src/*.cpp src/Graph/*.cpp)
    file(GLOB SRC_MIP   src/mip_formulation.cpp src/file_io.cpp src/utils.cpp src/Graph/*.cpp)
    file(GLOB MIP src/exec/mip.cpp)
    add_executable(mip.out ${SRC_MIP} ${MIP})
    target_link_libraries(mip.out ${GUROBI_LIBRARIES})
endif()

# test executable
file(GLOB SRC_TEST src/brute_force.cpp src/file_io.cpp src/utils.cpp src/Graph/*.cpp)
file(GLOB TEST src/exec/test.cpp)
add_executable(test.out ${SRC_TEST} ${TEST})

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/data/ ${CMAKE_CURRENT_BINARY_DIR}/data)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/solution/ ${CMAKE_CURRENT_BINARY_DIR}/solution)
