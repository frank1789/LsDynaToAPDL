set(CPACK_PACKAGE_NAME "LsDynaToAPDL")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "LsDynaToAPDL")
set(CPACK_PACKAGE_VENDOR "https://github.com/frank1789/LsDynaToAPDL.git")
set(CPACK_PACKAGE_DESCRIPTION "Application converts LS-Dyna script to Ansys APDL.")

set(CPACK_SOURCE_IGNORE_FILES 
    ".svn" 
    ".git"
    ".chglog" 
    ".settings" 
    "${PROJECT_SOURCE_DIR}/build/[a-zA-Z0-9_]+" 
    "~$" 
    "${PROJECT_SOURCE_DIR}/.*${PROJECT_PREFIX}"
    "${PROJECT_SOURCE_DIR}/.*${PROJECT_NAME}"
    "${PROJECT_SOURCE_DIR}/.*[tT]est"
    "${PROJECT_SOURCE_DIR}/.*[eE]xample"
    ".DS_Store"
)

if(APPLE)
  configure_file("${LsDynaToApdl_SOURCE_DIR}/LICENSE" "${LsDynaToApdl_BINARY_DIR}/COPYING.txt" @ONLY)
  set(CPACK_RESOURCE_FILE_LICENSE "${LsDynaToApdl_BINARY_DIR}/COPYING.txt")
  set(CPACK_PACKAGE_ICON "${LsDynaToApdl_SOURCE_DIR}/Resources/Icon/generic.icns")
  set(CPACK_BUNDLE_ICON "${CPACK_PACKAGE_ICON}")

  if(${CMAKE_VERSION} VERSION_GREATER "3.19.0") 
    # add the codesign options to the package
    configure_file("${CMAKE_CURRENT_LIST_DIR}/deploy-osx.cmake.in" "${LsDynaToApdl_BINARY_DIR}/deploy-osx.cmake" @ONLY)
    set(CPACK_PRE_BUILD_SCRIPTS "${LsDynaToApdl_BINARY_DIR}/deploy-osx.cmake")
  endif()
else()
  set(CPACK_RESOURCE_FILE_LICENSE "${LsDynaToApdl_SOURCE_DIR}/LICENSE")
endif()

set(CPACK_PACKAGE_EXECUTABLES "LsDynaToApdl" "LSDynaToAPDL")
set(CPACK_CREATE_DESKTOP_LINKS "LsDynaToApdl")

configure_file("${CMAKE_CURRENT_LIST_DIR}/LsdynatoApdlCPackOptions.cmake.in"
  "${LsDynaToApdl_BINARY_DIR}/LsdynatoApdlCPackOptions.cmake" @ONLY)
set(CPACK_PROJECT_CONFIG_FILE
  "${LsDynaToApdl_BINARY_DIR}/LsdynatoApdlCPackOptions.cmake")

# Should we add extra install rules to make a self-contained bundle, this is
# usually only required when attempting to create self-contained installers.
if(APPLE OR WIN32)
  set(INSTALL_BUNDLE_FILES ON)
else()
  option(INSTALL_BUNDLE_FILES "Add install rules to bundle files" OFF)
endif()
if(INSTALL_BUNDLE_FILES)
  install(DIRECTORY "${LsDynaToApdl_LIBRARY_DIR}"
    DESTINATION ${INSTALL_LIBRARY_DIR})
endif()

include(CPack)