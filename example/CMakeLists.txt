cmake_minimum_required(VERSION 2.8.11)


include_directories(${CMAKE_BINARY_DIR}/include)
# due to cpp runtime bug this include must also be set
include_directories(${CMAKE_BINARY_DIR}/include/antlr4-runtime)
include_directories(${CMAKE_BINARY_DIR}/parser_gen)


add_executable(egc_gramar_test
        example.cpp 
        visitor.cpp 
)

set_property(TARGET egc_gramar_test PROPERTY CXX_STANDARD 11)
set_property(TARGET egc_gramar_test PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(egc_gramar_test antlr4_parser_generated_static antlr4_static)

  
