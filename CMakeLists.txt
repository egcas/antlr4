cmake_minimum_required(VERSION 2.8.11)

##############################################################################

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE ON)

##############################################################################
##############################################################################

project(antlrv4)


if(NOT ANTLR4_GRAMMAR_FILE)
        message(WARNING "standard antlr grammar variables are used")
        set(ANTLR4_GRAMMAR_FILE "${CMAKE_CURRENT_SOURCE_DIR}/grammar/egc.g4")
        set(ANTLR4_GRAMMAR_NAME "egc")
        set(ANTLR4_BASE_RULE_NAME "formula")
else()
        message("using following variables: ")
        message("ANTLR4_GRAMMAR_FILE: ${ANTLR4_GRAMMAR_FILE}")
        message("ANTLR4_GRAMMAR_NAME: ${ANTLR4_GRAMMAR_NAME}")
        message("ANTLR4_BASE_RULE_NAME: ${ANTLR4_BASE_RULE_NAME}")
endif()

add_subdirectory(runtime)
add_subdirectory(example)

