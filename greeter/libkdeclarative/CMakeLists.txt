project(kdeclarative)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${KDE4_KIO_INCLUDES}
   )

set(kdeclarative_LIB_SRCS
    kdeclarative.cpp
    private/engineaccess.cpp
    private/kiconprovider.cpp
    bindings/qscriptnonguibookkeeping.cpp
    bindings/i18n.cpp
    bindings/icon.cpp
    bindings/url.cpp)

kde4_add_library(kdeclarative STATIC ${kdeclarative_LIB_SRCS})

set_target_properties(kdeclarative PROPERTIES VERSION ${KDE_NON_GENERIC_LIB_VERSION} SOVERSION ${KDE_NON_GENERIC_LIB_SOVERSION})

target_link_libraries(kdeclarative ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTSCRIPT_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY})


