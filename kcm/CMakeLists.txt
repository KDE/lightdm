find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

# Add the compiler flags
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
)



# Insert your sources here
set(LIGHTDM_KCM_SRCS
    themeconfig.cpp
    themesmodel.cpp
    coreconfig.cpp
    lightdmkcm.cpp
)

set(LIGHTDM_KCM_UI
    themeconfig.ui
    coreconfig.ui)

kde4_add_ui_files(LIGHTDM_KCM_SRCS ${LIGHTDM_KCM_UI})

kde4_add_plugin(kcm_lightdm
                ${LIGHTDM_KCM_SRCS})


# Link against the needed libraries
target_link_libraries(kcm_lightdm
  ${KDE4_KDEUI_LIBS}
  ${QT_QTUITOOLS_LIBRARY}
  ${QT_QTDECLARATIVE_LIBRARY}
)

install(TARGETS kcm_lightdm
          DESTINATION ${PLUGIN_INSTALL_DIR})

install (FILES kcm_lightdm.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)


macro_display_feature_log()


