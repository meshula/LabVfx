
include_guard()

cmake_minimum_required (VERSION 3.10)
project(LabVfx)

set(LABVFX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/scripts
                      ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/modules)

include(Settings)
include(Utilities)

lab_library(LabVfx
    TYPE STATIC
    ALIAS Lab::Particle

    LIBRARIES

    INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    PUBLIC_HEADERS
        include/LabVfx/LabVfx.h
        include/LabVfx/DataStripes.h
        include/LabVfx/Effect.h
        include/LabVfx/Integrator.h
        include/LabVfx/LifetimeRules.h
        include/LabVfx/Looks.h

    PRIVATE_HEADERS
        src/config.h

    CPPFILES
        src/CurlNoise.cpp
        src/DataStripes.cpp
        src/Effect.cpp
        src/Emitter.cpp
        src/Integrator.cpp
        src/LifetimeRules.cpp
        src/Looks.cpp
)

IF(WIN32)
    target_compile_definitions(LabVfx PUBLIC PLATFORM_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(LabVfx PUBLIC PLATFORM_MACOS=1)
else()
    target_compile_definitions(LabVfx PUBLIC PLATFORM_LINUX=1)
endif()


configure_file(cmake/LabVfxConfig.cmake.in
  "${PROJECT_BINARY_DIR}/LabVfxConfig.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/LabVfxConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
