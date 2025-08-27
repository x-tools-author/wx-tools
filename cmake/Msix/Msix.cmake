function(wxt_generate_msix target packet_name display_name packet_version rename_target)
  # Remove 'v' and 'V' from version string
  string(REGEX REPLACE "v" "" packet_version ${packet_version})
  string(REGEX REPLACE "V" "" packet_version ${packet_version})

  set(root_dir ${WX_TOOLS_BINARY_DIR})
  set(template ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/MsixTemplate.xml)
  set(args -DargTarget=${target})
  list(APPEND args -DargRoot=${root_dir})
  list(APPEND args -DargTemplate=${template})
  list(APPEND args -DargPacketName=${packet_name})
  list(APPEND args -DargPacketVersion=${packet_version})
  list(APPEND args -DargPackageDisplayName=${display_name})
  list(APPEND args -DargRenameTarget:BOOL=${rename_target})
  message(STATUS "root_dir(${target}): ${root_dir}")
  message(STATUS "template(${target}): ${template}")
  set(msix_source ${template})
  list(APPEND msix_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Msix.cmake)
  list(APPEND msix_source ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/MsixScript.cmake)
  add_custom_target(
    ${target}_msix
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/MsixScript.cmake
    SOURCES ${msix_source}
    DEPENDS ${target}
    WORKING_DIRECTORY ${root_dir}
    COMMENT "Start making msix packet for ${target}")
endfunction()
