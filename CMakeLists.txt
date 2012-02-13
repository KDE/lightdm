project(greeter-app)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

set(LIGHTDM_CONFIG_DIR "/etc/lightdm"
    CACHE PATH "Directory where LightDM configuration files are kept")

find_package(KDE4 REQUIRED)
find_package(QLightDM REQUIRED)
find_package(KDeclarative REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_subdirectory(greeter)
add_subdirectory(kcm)

install(DIRECTORY themes
        DESTINATION ${DATA_INSTALL_DIR}/lightdm-kde-greeter)
