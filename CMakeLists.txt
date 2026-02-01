cmake_minimum_required(VERSION 4.2)
project(LatteProj)

find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Widgets
)
find_package(LatteLong)
find_package(PROJ REQUIRED CONFIG)

set(SRCS
    src/Latte/Projection/CRSProjection.cpp
)

set(HRDS
    include/Latte/Projection/CRSProjection.h
)

add_library(${PROJECT_NAME} ${SRCS} ${HDRS})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} LatteLong::LatteLong PROJ::proj Qt6::Core Qt6::Widgets)


option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(./tests)
endif()