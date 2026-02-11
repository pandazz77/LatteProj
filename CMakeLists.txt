cmake_minimum_required(VERSION 4.2)
project(LatteProj VERSION 0.0.1)

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

add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})
add_library(LatteLong::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Widgets
        LatteLong::LatteLong
        PROJ::proj
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME}
    EXPORT "LatteLong${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl"
)

install(EXPORT "LatteLong${PROJECT_NAME}Targets"
    FILE LatteLong${PROJECT_NAME}Targets.cmake
    NAMESPACE LatteLong::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LatteLong
)

include(CMakePackageConfigHelpers)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LatteLong${PROJECT_NAME}Extra.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LatteLong${PROJECT_NAME}Extra.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LatteLong${PROJECT_NAME}Extra.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LatteLong
)


# ---------------- uninstall target ----------------
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
# --------------------------------------------------

option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(./examples)
endif()

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()