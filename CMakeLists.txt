
include_guard()

cmake_minimum_required (VERSION 3.10)
project(LabVfx)

set(LABVFX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/scripts
                      ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/modules)

include(Settings)
include(Utilities)

lab_library(LabImath
    TYPE STATIC
    ALIAS Lab::Imath

    INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src

    PUBLIC_HEADERS
        include/LabImath/IlmBaseConfig.h
        include/LabImath/ImathErrors.h
        include/LabImath/ImathExport.h
        include/LabImath/ImathFun.h
        include/LabImath/ImathLimits.h
        include/LabImath/ImathMatrix.h
        include/LabImath/ImathNamespace.h
        include/LabImath/ImathPlatform.h
        include/LabImath/ImathShear.h
        include/LabImath/ImathVec.h

    CPPFILES
        include/LabImath/ImathFun.cpp
)

lab_library(LabVfx
    TYPE STATIC
    ALIAS Lab::Vfx

    LIBRARIES
        Lab::Imath

    INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src

    PUBLIC_HEADERS
        include/LabVfx/LabVfx.h
        include/LabVfx/DataStripes.h
        include/LabVfx/Effect.h
        include/LabVfx/Emitter.h
        include/LabVfx/Field.h
        include/LabVfx/Integrator.h
        include/LabVfx/LifetimeRules.h
        include/LabVfx/Looks.h
        include/LabVfx/Operator.h
        include/LabVfx/Random.h

    PRIVATE_HEADERS
        src/config.h

    CPPFILES
        src/CurlNoise.cpp
        src/DataStripes.cpp
        src/Effect.cpp
        src/Emitter.cpp
        src/Field.cpp
        src/Integrator.cpp
        src/LifetimeRules.cpp
        src/Looks.cpp
        src/Operator.cpp
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

#add_library(Lab::Vfx ALIAS LabVfx)
