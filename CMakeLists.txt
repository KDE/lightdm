project(greeter-app)

# Versions
set(LIGHTDM_KDE_VERSION_MAJOR 0)
set(LIGHTDM_KDE_VERSION_MINOR 3)
set(LIGHTDM_KDE_VERSION_PATCH 0)
set(LIGHTDM_KDE_VERSION ${LIGHTDM_KDE_VERSION_MAJOR}.${LIGHTDM_KDE_VERSION_MINOR}.${LIGHTDM_KDE_VERSION_PATCH})

# Packaging
set(ARCHIVE_NAME lightdm-kde-${LIGHTDM_KDE_VERSION})
add_custom_target(dist
    COMMAND git archive --prefix=${ARCHIVE_NAME}/ HEAD
    | bzip2 > ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.tar.bz2
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

add_custom_target(distcheck
    COMMAND cd ${CMAKE_BINARY_DIR}
    && rm -rf ${ARCHIVE_NAME}
    && tar xf ${ARCHIVE_NAME}.tar.bz2
    && mkdir ${ARCHIVE_NAME}/build
    && cd ${ARCHIVE_NAME}/build
    && cmake -DCMAKE_INSTALL_PREFIX=/usr ..
    && make
    && make install DESTDIR=../install
    )
add_dependencies(distcheck dist)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set(LIGHTDM_CONFIG_DIR "/etc/lightdm"
    CACHE PATH "Directory where LightDM configuration files are kept")

find_package(KDE4 REQUIRED)

set(QLIGHTDM_MIN_VERSION 1.4.0)
find_package(QLightDM REQUIRED)

find_package(KDeclarative REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_subdirectory(greeter)
add_subdirectory(kcm)
add_subdirectory(themes)
