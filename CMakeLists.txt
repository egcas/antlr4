cmake_minimum_required(VERSION 2.8.11)

##############################################################################

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)

##############################################################################
##############################################################################

project(antlrv4)

set(ANTLR4_GRAMMAR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/grammar/egc.g4")
set(ANTLR4_GRAMMAR_NAME "egc")
set(ANTLR4_BASE_RULE_NAME "formula")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

endif()

add_subdirectory(runtime)
add_subdirectory(example)

