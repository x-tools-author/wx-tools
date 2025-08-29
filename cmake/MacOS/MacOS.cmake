function(wxt_make_pkg target dev_id_app dev_id_installer)
  # cmake-format: off
  add_custom_target(${target}_pkg WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/assets VERBATIM
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Application: ${dev_id_app}"
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Installer: ${dev_id_installer}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/assets/${target}_pkg "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/assets/${target}_pkg "||" ${CMAKE_COMMAND} -E true
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_BINARY_DIR}/assets/${target}.app ${CMAKE_BINARY_DIR}/assets/${target}_pkg/${target}.app
    COMMAND ${CMAKE_COMMAND} -E echo "codesign --deep --force --verbose --sign ${dev_id_app} ${target}.app"
    COMMAND codesign --deep --force --verbose --sign "${dev_id_app}" "${target}.app"
    COMMENT "Making pkg...")
  # cmake-format: on
endfunction()
