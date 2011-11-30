
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${QLIGHTDM_INCLUDE_DIR}
  libkdeclarative
)

add_subdirectory(libkdeclarative)

# Insert your sources here
set(GREETER_SRCS
    greeterwindow.cpp
    main.cpp
    components/modelcombobox.cpp
    components/passwordlineedit.cpp
)

# Create the plasmoid
kde4_add_executable(lightdm-kde-greeter ${GREETER_SRCS})

# Link against the needed libraries
target_link_libraries(lightdm-kde-greeter
  ${QT_QTDECLARATIVE_LIBRARY}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_PLASMA_LIBS}
  ${QLIGHTDM_LIBRARIES}
  kdeclarative
)

install(TARGETS lightdm-kde-greeter RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/sbin/)

#FIXME find correct macro for /usr/[local]/share
install(FILES lightdm-kde-greeter.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/xgreeters)

macro_display_feature_log()


