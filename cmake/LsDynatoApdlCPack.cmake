include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "LsDynaToAPDL")
set(CPACK_PACKAGE_CONTACT "Francesco Argentieri <francesco.argentieri89@gmail.com>")
set(HOMEPAGE "https://github.com/frank1789/LsDynaToAPDL")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${VERSION_SHORT})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "LsDynaToAPDL")
set(CPACK_PACKAGE_VENDOR "https://github.com/frank1789/LsDynaToAPDL.git")
set(CPACK_PACKAGE_DESCRIPTION "Application converts LsDyna script to Ansys APDL.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/out")


# LsDynaToAPDL is a command line converter: it has no GUI, so it deliberately
# declares no CPACK_PACKAGE_EXECUTABLES / CPACK_CREATE_DESKTOP_LINKS. Those
# would add a start menu or desktop shortcut that launches nothing useful.
set(CPACK_STRIP_FILES TRUE)

if(WIN32 AND NOT UNIX)
    #--------------------------------------------------------------------------
    # Windows specific
    set(CPACK_GENERATOR "STGZ;ZIP")
    message(STATUS "Package generation - Windows")
    message(STATUS "   + STGZ                                 YES ")
    message(STATUS "   + ZIP                                  YES ")

    # NSIS windows installer
    find_program(NSIS_PATH nsis PATH_SUFFIXES nsis)
    if(NSIS_PATH)
        set(CPACK_GENERATOR "${CPACK_GENERATOR};NSIS")
        message(STATUS "   + NSIS                                 YES ")
        # Note: There is a bug in NSI that does not handle full unix paths properly. Make
        # sure there is at least one set of four (4) backlasshes.
        set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
        # No CPACK_NSIS_MUI_ICON: the repository ships no .ico, and NSIS fails
        # outright when the icon it is told to embed does not exist.
        # set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
        # set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
        set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
        set(CPACK_NSIS_MODIFY_PATH ON)
    else()
        message(STATUS "   + NSIS                                 NO ")
    endif()

elseif(APPLE)
    #--------------------------------------------------------------------------
    # Apple specific
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_VOLUME_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_SYSTEM_NAME "osx")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/")

    set(MACOSX_BUNDLE_BUNDLE_NAME ${CPACK_PACKAGE_NAME})
    set(MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.LsDynaToAPDL.LsDynaToAPDL")
    # MACOSX_BUNDLE_ICON_FILE names the file inside Contents/Resources, so it
    # must stay a bare filename; an absolute path yields a bundle with no icon.
    set(MACOSX_BUNDLE_ICON_FILE "generic.icns")
    set(MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/shared/macos/MacOSXBundleInfo.plist.in)
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION_SHORT}")

    set_source_files_properties(${PROJECT_SOURCE_DIR}/shared/icons/generic.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set(CPACK_DMG_VOLUME_NAME "LsDynaToAPDL")
    set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${PROJECT_SOURCE_DIR}/shared/macos/CMakeDMGSetup.scpt")
    # No CPACK_DMG_BACKGROUND_IMAGE: shared/macos ships no background artwork.
    set(CPACK_OSX_PACKAGE_VERSION "10.10") # min package version
else()
    #-----------------------------------------------------------------------------
    # Linux specific
    set(CPACK_GENERATOR "DEB;TBZ2;TXZ")
    message(STATUS "Package generation - UNIX")
    message(STATUS "   + DEB                                  YES ")
    message(STATUS "   + TBZ2                                 YES ")
    message(STATUS "   + TXZ                                  YES ")

    find_program(RPMBUILD_PATH rpmbuild)
    if(RPMBUILD_PATH)
        message(STATUS "   + RPM                                  YES ")
        set(CPACK_GENERATOR "${CPACK_GENERATOR};RPM")
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        # set(CPACK_RPM_PACKAGE_REQUIRES "gtkmm30")
        # exclude folders which clash with default ones
        set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST
            ${CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST}
            /usr
            /usr/bin
            /usr/share
            /usr/share/doc)
    else()
        message(STATUS "   + RPM                                  NO ")
    endif()

    # TODO do this better
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${HOMEPAGE}")
    # set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
    # set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtkmm-3.0")



    # No .desktop entry or hicolor/gnome icon: this is a terminal-only
    # converter, so a launcher would place a dead entry in the application
    # menu.
    # License file
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE
        DESTINATION share/doc/${PROJECT_NAME}/
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
        RENAME copyright)
endif()

include(CPack)
