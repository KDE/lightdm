add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${QLIGHTDM_INCLUDE_DIR}
)


######## Package plugin ########

# Plugin for Plasmapkg to understand LightDM Themes

set(PLASMA_PACKAGE_SRCS
    lightdmpackagestructure.cpp
    lightdmpackageplugin.cpp
)

kde4_add_plugin( plasma_packagestructure_lightdm_theme ${PLASMA_PACKAGE_SRCS} )
target_link_libraries( plasma_packagestructure_lightdm_theme ${KDE4_KDEUI_LIBS} ${KDE4_PLASMA_LIBS} )
install( TARGETS plasma_packagestructure_lightdm_theme DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES plasma-packagestructure-lightdm.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

#Install a service type
install( FILES lightdmtheme.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} )

######## QML Plugin ########

# Library for all people making themes

set(QML_LIB_SRCS
    qmlplugin.cpp
    extrarowproxymodel.cpp
    sessionsmodel.cpp
    usersmodel.cpp
    screensmodel.cpp
    fakegreeter.cpp
    configwrapper.cpp
)

kde4_add_library(klightdmqmlplugin SHARED ${QML_LIB_SRCS})

# Link against the needed libraries
target_link_libraries(klightdmqmlplugin
  ${QT_QTDECLARATIVE_LIBRARY}
  ${QLIGHTDM_LIBRARIES}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_PLASMA_LIBS}
)

install(TARGETS klightdmqmlplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/lightdm/)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/lightdm/)
install(FILES qml/ScreenManager.qml DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/lightdm/)

macro_display_feature_log()


