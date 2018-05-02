CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
SET( MODULE_NAME chordpro2html )
GET_FILENAME_COMPONENT( CONFIGURATION_PROJECT ${CMAKE_BINARY_DIR} NAME )


####################################################################
#                                                                  #
#                        PClinux                                   #
#                                                                  #
####################################################################
IF( CONFIGURATION_PROJECT STREQUAL "PClinux.release" )
    MESSAGE( "=== PClinux.release ===" )
    ADD_DEFINITIONS( Di586)

    # Variabili da Settare
    SET( ARCHITETTURA                    i386 )
    SET( PREFIX_CROSS_COMPILER           "" )
    SET( CMAKE_CXX_COMPILER              ${PREFIX_CROSS_COMPILER}c++ )
    SET( CMAKE_C_COMPILER                ${PREFIX_CROSS_COMPILER}gcc )
    SET( CMAKE_LINKER                    ${PREFIX_CROSS_COMPILER}ld )

    PROJECT( ${MODULE_NAME})

    SET( CMAKE_INSTALL_PREFIX            ${PROJECT_SOURCE_DIR}/release/i586 )
    SET( BASE_DEVELOP                    "" )

    SET( CMAKE_FLAGS "-DCAN_OVER_ETHERNET -DDRIVE_CAN_NAME='FlexCAN' -D__CAN_USE_DRV_ETHERCAN -DPCLINUX")


ELSEIF( CONFIGURATION_PROJECT STREQUAL "PClinux.debug" )
    MESSAGE( "=== PClinux.debug ===" )
    ADD_DEFINITIONS( -DDEBUG_I586  -Di586)

    # Variabili da Settare
    SET( ARCHITETTURA                    i386 )
    SET( PREFIX_CROSS_COMPILER           "" )
    SET( CMAKE_CXX_COMPILER              ${PREFIX_CROSS_COMPILER}c++ )
    SET( CMAKE_C_COMPILER                ${PREFIX_CROSS_COMPILER}gcc )
    SET( CMAKE_LINKER                    ${PREFIX_CROSS_COMPILER}ld )

    PROJECT( ${MODULE_NAME} )

    SET( CMAKE_INSTALL_PREFIX           ${PROJECT_SOURCE_DIR}/release/i586 )
    SET( BASE_DEVELOP                   "" )

    SET( CMAKE_FLAGS "-DCAN_OVER_ETHERNET -DDRIVE_CAN_NAME='FlexCAN' -D__CAN_USE_DRV_ETHERCAN -DPCLINUX")

ENDIF ()



####################################################################
#                                                                  #
#                        PARTE GENERALE                            #
#                                                                  #
####################################################################
# SET( CMAKE_EXE_LINKER_FLAGS "")
SET(CMAKE_CXX_FLAGS "${CMAKE_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS   "${CMAKE_FLAGS}   -std=c11")
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../inc
)

MESSAGE(  ${BASE_DEVELOP}/${LINUX_VERSION_DIR}/include )

SET( chordpro2html_SRCS
	../src/chordpro_data.cpp
	../src/chordpro_parser.cpp
	../src/console_utf8.cpp
	../src/file_system.cpp
	../src/html_writer.cpp
	../src/main.cpp
)

SET( LINK_LIBRARIES pthread rt)

ADD_EXECUTABLE( chordpro2html ${chordpro2html_SRCS} )
TARGET_LINK_LIBRARIES( chordpro2html ${LINK_LIBRARIES} )
INSTALL( TARGETS ${MODULE_NAME} DESTINATION home/treccani/chordpro2html )

