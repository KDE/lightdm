project(lightdmkcm)


find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

# Add the compiler flags
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
#   ${QT_QTNETWORK_INCLUDE_DIR}
#   ${QT_QTDBUS_INCLUDE_DIR}
#   ${QT_QTSQL_INCLUDE_DIR}
#   ${QT_QTWEBKIT_INCLUDE_DIR}
  ${KDE4_INCLUDES}
 #   ${SOPRANO_INCLUDE_DIR}
)

# Insert your sources here
set(LIGHTDM_KCM_SRCS
    mainwindow.cpp
    main.cpp
    themesmodel.cpp
)

set(LIGHTDM_KCM_UI
    mainwindow.ui)

kde4_add_ui_files(LIGHTDM_KCM_SRCS ${LIGHTDM_KCM_UI})

# Create the plasmoid
kde4_add_executable(lightdmkcm ${LIGHTDM_KCM_SRCS})

# Link against the needed libraries
target_link_libraries(lightdmkcm
  ${KDE4_KDEUI_LIBS}
  ${QT_QTUITOOLS_LIBRARY}
)
# 
# install(TARGETS lightdmkcm ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES lightdmkcm.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

macro_display_feature_log()


