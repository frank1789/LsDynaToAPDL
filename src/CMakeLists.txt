
add_subdirectory(logtools)
add_subdirectory(common)
add_subdirectory(lsdyna)
add_subdirectory(core)





if(APPLE)
  set_target_properties(${NAME_PROJECT} PROPERTIES
    MACOSX_BUNDLE ON
    MACOSX_BUNDLE_BUNDLE_NAME "${NAME_PROJECT}"
    #MACOSX_BUNDLE_BUNDLE_IDENTIFIER "cz.mosra.magnum.my-application"
  )
endif()
