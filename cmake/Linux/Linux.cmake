function(wxt_make_package target packetName friendlyName version)
  file(GLOB_RECURSE RES_FILES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/app/*)

  # Remove 'v' and 'V' from version string
  string(REGEX REPLACE "v" "" version ${version})
  string(REGEX REPLACE "V" "" version ${version})

  set(args "-DargTarget=${target}")
  list(APPEND args "-DargPacketName=${packetName}") # e.g. wx-tools
  list(APPEND args "-DargTargetName=${friendlyName}") # e.g. wxTools
  list(APPEND args "-DargVersion=${version}")
  list(APPEND args "-DargWorkingDir=${CMAKE_BINARY_DIR}/deb")
  list(APPEND args "-DargTool=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/tools/linuxdeployqt")
  list(APPEND args "-DargTargetBinaryDir=$<TARGET_FILE_DIR:${target}>")
  list(APPEND args "-DargTemplateDir=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/app")
  list(APPEND args "-DargIconFile=${CMAKE_SOURCE_DIR}/300x300.png")

  # cmake-format: off
  add_custom_target(${target}-deb
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/LinuxScript.cmake
    SOURCES ${RES_FILES})
  # cmake-format: on
endfunction()
