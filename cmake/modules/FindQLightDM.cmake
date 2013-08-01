# Try to find the QLightDM library
# Once done this will define
#
#   QLIGHTDM_FOUND          Indicates that QLightDM was found
#   QLIGHTDM_LIBRARIES      Libraries needed to use QLightDM
#   QLIGHTDM_LIBRARY_DIRS   Paths needed for linking against QLightDM
#   QLIGHTDM_INCLUDE_DIR    Path needed for finding QLightDM include files
#
# Copyright 2012 David Edmundson <kde@davidedmundson.co.uk>
#
# Redistribution and use is allowed according to the terms of the BSD license.

set(QLIGHTDM_LIBRARIES_FIND_REQUIRED ${QLightDM_FIND_REQUIRED})
# if(QLIGHTDM_INCLUDE_DIR AND QLIGHTDM_LIBRARIES)
#   # Already in cache, be silent
#   set(QLIGHTDM_FIND_QUIETLY TRUE)
# endif(QLIGHTDM_INCLUDE_DIR AND QLIGHTDM_LIBRARIES)

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
    if (QLIGHTDM_MIN_VERSION)
        PKG_CHECK_MODULES(PC_QLIGHTDM liblightdm-qt-3>=${QLIGHTDM_MIN_VERSION})
    else (QLIGHTDM_MIN_VERSION)
        PKG_CHECK_MODULES(PC_QLIGHTDM liblightdm-qt-3)
    endif (QLIGHTDM_MIN_VERSION)
    SET (QLIGHTDM_API 3)
    if (NOT PC_QLIGHTDM_FOUND)
        if (QLIGHTDM_MIN_VERSION)
            PKG_CHECK_MODULES(PC_QLIGHTDM liblightdm-qt-2>=${QLIGHTDM_MIN_VERSION})
        else (QLIGHTDM_MIN_VERSION)
            PKG_CHECK_MODULES(PC_QLIGHTDM liblightdm-qt-2)
        endif (QLIGHTDM_MIN_VERSION)
        SET (QLIGHTDM_API 2)
    endif (NOT PC_QLIGHTDM_FOUND)
endif(PKG_CONFIG_FOUND)


find_path(QLIGHTDM_INCLUDE_DIR
          NAMES QLightDM/Greeter
          HINTS
          ${PC_QLIGHTDM_INCLUDEDIR}
          ${PC_QLIGHTDM_INCLUDE_DIRS}
)

find_library(QLIGHTDM_LIBRARIES
             NAMES lightdm-qt-${QLIGHTDM_API}
             HINTS
             ${PC_QLIGHTDM_LIBDIR}
             ${PC_QLIGHTDM_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QLIGHTDM_DEFAULT_MSG
                                  QLIGHTDM_LIBRARIES QLIGHTDM_INCLUDE_DIR)
