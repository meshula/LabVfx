
include_guard()

cmake_minimum_required (VERSION 3.10)
project(LabParticle)

set(LABPARTICLE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/scripts
                      ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/modules)

include(Settings)
include(Utilities)

lab_library(LabParticle
    TYPE STATIC
    ALIAS Lab::Particle

    LIBRARIES

    INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    PUBLIC_HEADERS
        include/LabParticle/LabParticle.h
        include/LabParticle/DataStripes.h
        include/LabParticle/Effect.h
        include/LabParticle/Integrator.h
        include/LabParticle/LifetimeRules.h
        include/LabParticle/Looks.h

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
    target_compile_definitions(LabParticle PUBLIC PLATFORM_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(LabParticle PUBLIC PLATFORM_MACOS=1)
else()
    target_compile_definitions(LabParticle PUBLIC PLATFORM_LINUX=1)
endif()


configure_file(cmake/LabParticleConfig.cmake.in
  "${PROJECT_BINARY_DIR}/LabParticleConfig.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/LabParticleConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
