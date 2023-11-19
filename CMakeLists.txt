# Copyright (C) 2013-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 2.8)

project(CommonAPI-FDBus)

set(CMAKE_VERBOSE_MAKEFILE off)

set(LIBCOMMONAPI_FDBUS_MAJOR_VERSION 3)
set(LIBCOMMONAPI_FDBUS_MINOR_VERSION 1)
set(LIBCOMMONAPI_FDBUS_PATCH_VERSION 12)

message(STATUS "Project name: ${PROJECT_NAME}")

set(COMPONENT_VERSION ${LIBCOMMONAPI_FDBUS_MAJOR_VERSION}.${LIBCOMMONAPI_FDBUS_MINOR_VERSION}.${LIBCOMMONAPI_FDBUS_PATCH_VERSION})
set(COMMONAPI_API_HEADER_VERSION ${LIBCOMMONAPI_FDBUS_MAJOR_VERSION}.${LIBCOMMONAPI_FDBUS_MINOR_VERSION})

SET(PACKAGE_VERSION "${COMPONENT_VERSION}") # used in *.cmake.in

OPTION(USE_FILE "Set to OFF to disable file logging" OFF)
message(STATUS "USE_FILE is set to value: ${USE_FILE}")

OPTION(USE_CONSOLE "Set to OFF to disable console logging" OFF)
message(STATUS "USE_CONSOLE is set to value: ${USE_CONSOLE}")

SET(DEFAULT_SEND_TIMEOUT "5000" CACHE STRING "default send timeout")
message(STATUS "DEFAULT_SEND_TIMEOUT is set to value: ${DEFAULT_SEND_TIMEOUT} ms")

IF(USE_FILE)
    add_definitions(-DUSE_FILE)
ENDIF(USE_FILE)

IF(USE_CONSOLE)
    add_definitions(-DUSE_CONSOLE)
ENDIF(USE_CONSOLE)

add_definitions(-DDEFAULT_SEND_TIMEOUT=${DEFAULT_SEND_TIMEOUT})

# Set a default build type if none was specified
set(default_build_type "RelWithDebInfo")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

SET(MAX_LOG_LEVEL "DEBUG" CACHE STRING "maximum log level")
message(STATUS "MAX_LOG_LEVEL is set to value: ${MAX_LOG_LEVEL}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMONAPI_INTERNAL_COMPILATION")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMONAPI_FDBUS_VERSION_MAJOR=${LIBCOMMONAPI_FDBUS_MAJOR_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMONAPI_FDBUS_VERSION_MINOR=${LIBCOMMONAPI_FDBUS_MINOR_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMMONAPI_LOGLEVEL=COMMONAPI_LOGLEVEL_${MAX_LOG_LEVEL}")

# Package config module not found message macro
macro(pkg_config_module_not_found_message PKG_CONFIG_MODULE)
    message(FATAL_ERROR "pkg-config could not find the required module ${PKG_CONFIG_MODULE}!"
        " Please adjust your PKG_CONFIG_PATH environment variable accordingly.")
endmacro()

# ##################################################################################################
# see http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
# ##################################################################################################

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include/CommonAPI-${COMMONAPI_API_HEADER_VERSION} CACHE PATH "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR cmake)
else()
    set(DEF_INSTALL_CMAKE_DIR lib/cmake/CommonAPI-FDBus-${COMPONENT_VERSION})
endif()

set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)

    if(NOT IS_ABSOLUTE "${${var}}")
        set(ABSOLUTE_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# ##################################################################################################
if("${USE_INSTALLED_COMMONAPI}" STREQUAL "ON")
    FIND_PACKAGE(CommonAPI 3.1.12 REQUIRED CONFIG NO_CMAKE_PACKAGE_REGISTRY)
else()
    FIND_PACKAGE(CommonAPI 3.1.12 REQUIRED CONFIG NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
endif()

message(STATUS "CommonAPI_CONSIDERED_CONFIGS: ${CommonAPI_CONSIDERED_CONFIGS}")
message(STATUS "COMMONAPI_INCLUDE_DIRS: ${COMMONAPI_INCLUDE_DIRS}")
message(STATUS "CommonAPI Version: ${CommonAPI_VERSION}")

if(DEFINED ENV{FDBUS_INCLUDE_DIRS})
    MESSAGE(STATUS "FDBUS_INCLUDE_DIRS=$ENV{FDBUS_INCLUDE_DIRS}")
else()
    MESSAGE(FATAL_ERROR "FDBUS_INCLUDE_DIRS is missing!")
endif()

if(DEFINED ENV{FDBUS_LIB_DIRS})
    MESSAGE(STATUS "FDBUS_LIB_DIRS=$ENV{FDBUS_LIB_DIRS}")
else()
    MESSAGE(FATAL_ERROR "FDBUS_LIB_DIRS is missing!")
endif()

include_directories(
    include
    ${COMMONAPI_INCLUDE_DIRS}
    $ENV{FDBUS_INCLUDE_DIRS}
)

link_directories(
    $ENV{FDBUS_LIB_DIRS}
)

file(GLOB CommonAPI-FDBus_SRC "src/CommonAPI/FDBus/*.cpp")
list(SORT CommonAPI-FDBus_SRC)

# CommonAPI
add_library(CommonAPI-FDBus SHARED ${CommonAPI-FDBus_SRC})
set_target_properties(CommonAPI-FDBus PROPERTIES VERSION ${COMPONENT_VERSION} SOVERSION ${COMPONENT_VERSION})
target_link_libraries(CommonAPI-FDBus CommonAPI fdbus)

if(MSVC)
    target_link_libraries(CommonAPI-FDBus ws2_32 Rpcrt4)
endif()

# ##################################################################################################
file(GLOB_RECURSE CommonAPI-FDBus_INCLUDE_INSTALL_FILES "include/*.hpp")
list(SORT CommonAPI-FDBus_INCLUDE_INSTALL_FILES)

set_target_properties(CommonAPI-FDBus PROPERTIES PUBLIC_HEADER "${CommonAPI-FDBus_INCLUDE_INSTALL_FILES}")

install(
    TARGETS CommonAPI-FDBus

    # IMPORTANT: Add the CommonAPI-FDBus library to the "export-set"
    EXPORT CommonAPI-FDBusTargets
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/CommonAPI/FDBus"
)

# #############################################################################
# exporting, configuring and installing of cmake files

# Add all targets to the build-tree export set
export(TARGETS CommonAPI-FDBus
    FILE "${PROJECT_BINARY_DIR}/CommonAPI-FDBusTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE CommonAPI-FDBus)

# Create the CommonAPI-FDBusConfig.cmake and CommonAPI-FDBusConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${ABSOLUTE_INSTALL_CMAKE_DIR}" "${ABSOLUTE_INSTALL_INCLUDE_DIR}")

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPI-FDBusConfig.cmake.in
    "${PROJECT_BINARY_DIR}/CommonAPI-FDBusConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${COMMONAPI_FDBUS_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPI-FDBusConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CommonAPI-FDBusConfig.cmake" @ONLY)

# ... for both
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CommonAPI-FDBusConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/CommonAPI-FDBusConfigVersion.cmake" @ONLY)

# Install the CommonAPI-FDBusConfig.cmake and CommonAPI-FDBusConfigVersion.cmake
install(
    FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CommonAPI-FDBusConfig.cmake"
    "${PROJECT_BINARY_DIR}/CommonAPI-FDBusConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

# Install the export set for use with the install-tree
install(
    EXPORT CommonAPI-FDBusTargets
    DESTINATION "${INSTALL_CMAKE_DIR}"
)

# #############################################################################
# create pkg-config file
if(NOT WIN32)
    configure_file(CommonAPI-FDBus.pc.in ${PROJECT_BINARY_DIR}/CommonAPI-FDBus.pc @ONLY)
    install(FILES ${PROJECT_BINARY_DIR}/CommonAPI-FDBus.pc DESTINATION lib/pkgconfig)
endif()

# #############################################################################
# maintainer-clean
add_custom_target(maintainer-clean COMMAND rm -rf *)
