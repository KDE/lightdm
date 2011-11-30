project(greeter-app)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(KDE4 REQUIRED)
find_package(QLightDM REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

add_subdirectory(greeter)

install(DIRECTORY themes
        DESTINATION ${DATA_INSTALL_DIR}/lightdm-kde-greeter)
