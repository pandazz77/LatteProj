set(DATASETS ${CMAKE_CURRENT_SOURCE_DIR}/datasets)

add_executable(csv_test csv_projection_test.cpp)
target_link_libraries(csv_test LatteProj)

add_test(NAME csv_test COMMAND csv_test ${DATASETS}/dataset0.csv)