function(wxt_make_zip target)
  if(WIN32)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E tar "cf" ${WXT_ASSETS_NAME}.zip "--format=zip" ${target}
      WORKING_DIRECTORY ${WX_TOOLS_BINARY_DIR}
      COMMENT "Creating zip package")
  endif()
endfunction()

function(wxt_build_for_store target)
  if(NOT WIN32)
    return()
  endif()

  # cmake-format: off
  add_custom_target(${target}-store
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/msix/make-msix.bat ${target}
    SOURCES ${CMAKE_SOURCE_DIR}/scripts/msix/make-msix.bat
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts/msix)
  # cmake-format: on
endfunction()
