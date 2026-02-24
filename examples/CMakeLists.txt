cmake_minimum_required(VERSION 4.2)
project(GeoJsonViewer)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
)
# ===== If LatteProj is installed use:
# find_package(LatteLong REQUIRED COMPONENTS LatteProj)
# ===== Otherwise ( using add_subdirectory(...) ) you are already be able to use it

add_executable(${PROJECT_NAME} main.cpp ProjComboBox.hpp)
target_link_libraries(${PROJECT_NAME} LatteLong::LatteLong LatteLong::LatteProj)