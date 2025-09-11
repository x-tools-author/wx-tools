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
