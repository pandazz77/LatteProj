add_executable(GeoJsonViewer main.cpp ProjComboBox.hpp)
target_link_libraries(GeoJsonViewer LatteLong::LatteProj)