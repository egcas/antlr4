cmake_minimum_required(VERSION 2.8.11)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)


find_package(Java COMPONENTS Development)

#due to a lots of warning while compiling antlr runtime, supress some warnings
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        string(APPEND CMAKE_CXX_FLAGS "-Wno-attributes")
endif()

if (NOT ${Java_FOUND})
        message( FATAL_ERROR  "Java compiler and/or runtime has not been found. Please install an appropriate java environment for your OS")
endif()

set(ANTLR4_VERSION "4.7.1")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(ENV{CLASSPATH} ".:${CMAKE_CURRENT_SOURCE_DIR}/antlr-${ANTLR4_VERSION}-complete.jar")
        set(ANTLR4_RUNTIME java -Xmx500M -cp "$ENV{CLASSPATH}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(ENV_CLASSPATH ".$<SEMICOLON>${CMAKE_CURRENT_SOURCE_DIR}/antlr-${ANTLR4_VERSION}-complete.jar")
        file(TO_NATIVE_PATH "${ENV_CLASSPATH}" ENV_CLASSPATH)
        set(ENV{CLASSPATH} "${ENV_CLASSPATH}")
        set(ANTLR4_RUNTIME java -Xmx500M -cp "${ENV_CLASSPATH}")
        file(TO_NATIVE_PATH "${ANTLR4_RUNTIME}" ANTLR4_RUNTIME)
endif()

if( NOT ANTLR4_GRAMMAR_FILE)
        message( FATAL_ERROR  "Variable ANTLR4_GRAMMAR_FILE not set, set ANTLR4_GRAMMAR_FILE to hold the location of the grammar file (*.g4) from within your program.")        
endif()
if( NOT ANTLR4_GRAMMAR_NAME)
        message( FATAL_ERROR  "Variable ANTLR4_GRAMMAR_NAME not set, set ANTLR4_GRAMMAR_NAME to hold the grammar name as defined in your *.g4 grammar file.")        
endif()
if( NOT ANTLR4_BASE_RULE_NAME)
        message( FATAL_ERROR  "Variable ANTLR4_BASE_RULE_NAME not set, set ANTLR4_BASE_RULE_NAME to hold the name of the first (base rule) rule name as defined in your *.g4 grammar file.")        
endif()


#if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        add_subdirectory(linux)
#elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#        add_subdirectory(windows)
#endif()


set(ANTLR_JAR_LOCATION "${CMAKE_CURRENT_LIST_DIR}/antlr-${ANTLR4_VERSION}-complete.jar")
set(PARSER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/parser_gen)
set(HEADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/include/antlr4-runtime")

include_directories(${PARSER_OUTPUT_DIR})
include_directories(${CMAKE_BINARY_DIR}/include)
# due to cpp runtime bug this include must also be set
include_directories(${CMAKE_BINARY_DIR}/include/antlr4-runtime)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_custom_command(OUTPUT ${HEADER_OUTPUT_DIR}/antlr4-runtime.h COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/linux/antlr4-headers-${ANTLR4_VERSION}" ${HEADER_OUTPUT_DIR})
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_custom_command(OUTPUT ${HEADER_OUTPUT_DIR}/antlr4-runtime.h COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/windows/antlr4-cpp-runtime-${ANTLR4_VERSION}-vs2015/antlr4-runtime" ${HEADER_OUTPUT_DIR})
endif()

add_custom_target(antlr4_run
                  COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.Tool ${ANTLR4_GRAMMAR_FILE} -o ${PARSER_OUTPUT_DIR}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "execute antr4")

set(ANTLR4_GENERATED_SRC 
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}BaseListener.cpp  
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}BaseVisitor.cpp  
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}Lexer.cpp  
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}Listener.cpp  
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}Parser.cpp  
    ${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}Visitor.cpp
)

add_custom_command(OUTPUT ${ANTLR4_GENERATED_SRC} 
                   COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.Tool -Dlanguage=Cpp -visitor ${ANTLR4_GRAMMAR_FILE} -o ${PARSER_OUTPUT_DIR} 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} 
                   DEPENDS ${ANTLR4_GRAMMAR_FILE}
                   COMMENT "generate library for generated parser" VERBATIM
)                 

add_library(antlr4_parser_generated_static STATIC ${ANTLR4_GENERATED_SRC} ${HEADER_OUTPUT_DIR}/antlr4-runtime.h)

set(ANTLR4_JAVA_BASE_NAME "${PARSER_OUTPUT_DIR}/${ANTLR4_GRAMMAR_NAME}")
set(ANTLR4_JAVA_SOURCE "${ANTLR4_JAVA_BASE_NAME}BaseListener.java" "${ANTLR4_JAVA_BASE_NAME}Listener.java" "${ANTLR4_JAVA_BASE_NAME}Lexer.java" "${ANTLR4_JAVA_BASE_NAME}Parser.java")                  
set(ANTLR4_JAVA_CLASSES "${ANTLR4_JAVA_BASE_NAME}BaseListener.class" "${ANTLR4_JAVA_BASE_NAME}Listener.class" "${ANTLR4_JAVA_BASE_NAME}Lexer.class" "${ANTLR4_JAVA_BASE_NAME}Parser.class" "${ANTLR4_JAVA_BASE_NAME}Parser$RContext.class") 
                  
add_custom_target(antlr4_grun COMMAND ${Java_JAVAC_EXECUTABLE} -cp "${CMAKE_CURRENT_SOURCE_DIR}/antlr-${ANTLR4_VERSION}-complete.jar:$ENV{CLASSPATH}" ${ANTLR4_JAVA_SOURCE}
                   WORKING_DIRECTORY ${PARSER_OUTPUT_DIR}
                   COMMENT "compile grammar (java)"
                   VERBATIM)                  
add_dependencies(antlr4_grun antlr4_run)                  

                  
add_custom_target(antlr4_grun_gui_run COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.gui.TestRig ${ANTLR4_GRAMMAR_NAME} ${ANTLR4_BASE_RULE_NAME} -gui
                  WORKING_DIRECTORY ${PARSER_OUTPUT_DIR} COMMENT "execute antr grun: Enter text and press Enter followed by Ctrl + D (Unix) or Ctrl + Z (Windows) (means EOF) - to see the gui")

add_dependencies(antlr4_grun_gui_run antlr4_grun)

add_custom_target(antlr4_grun_tree_run COMMAND ${ANTLR4_RUNTIME} org.antlr.v4.gui.TestRig ${ANTLR4_GRAMMAR_NAME} ${ANTLR4_BASE_RULE_NAME} -tree
                  WORKING_DIRECTORY ${PARSER_OUTPUT_DIR} COMMENT "execute antr grun: Enter text and press Enter followed by Ctrl + D (Unix) or Ctrl + Z (Windows) (means EOF) - to see the gui")

add_dependencies(antlr4_grun_tree_run antlr4_grun)

