add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${QLIGHTDM_INCLUDE_DIR}
)

set(LIB_SRCS
    qmlplugin.cpp
    extrarowproxymodel.cpp
#     faceimageprovider.cpp
    sessionsmodel.cpp
    usersmodel.cpp
    screensmodel.cpp
    fakegreeter.cpp
)

kde4_add_library(klightdmqmlplugin SHARED ${LIB_SRCS})

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


