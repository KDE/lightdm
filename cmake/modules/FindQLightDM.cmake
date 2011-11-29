
set(QLIGHTDM_LIBRARIES_FIND_REQUIRED ${QLightDM_FIND_REQUIRED})
# if(QLIGHTDM_INCLUDE_DIR AND QLIGHTDM_LIBRARIES)
#   # Already in cache, be silent
#   set(QLIGHTDM_FIND_QUIETLY TRUE)
# endif(QLIGHTDM_INCLUDE_DIR AND QLIGHTDM_LIBRARIES)

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
     pkg_check_modules(PC_QLIGHTDM liblightdm-qt-1)
endif(PKG_CONFIG_FOUND)


find_path(QLIGHTDM_INCLUDE_DIR
          NAMES QLightDM/Greeter
          HINTS
          ${PC_QLIGHTDM_INCLUDEDIR}
          ${PC_QLIGHTDM_INCLUDE_DIRS}
)

find_library(QLIGHTDM_LIBRARIES
             NAMES lightdm-qt-1
             HINTS
             ${PC_QLIGHTDM_LIBDIR}
             ${PC_QLIGHTDM_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QLIGHTDM_DEFAULT_MSG
                                  QLIGHTDM_LIBRARIES QLIGHTDM_INCLUDE_DIR)
