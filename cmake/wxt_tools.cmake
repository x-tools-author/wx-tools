function(wxt_download_and_extract url file_name file_suffix make_dir)
  set(cooked_url "${url}/${file_name}.${file_suffix}")
  # Download...
  if(NOT EXISTS "${WXT_3RD_DIR}/${file_name}.${file_suffix}")
    message(STATUS "[wxTools] Downloading ${file_name}.${file_suffix} from ${cooked_url}")
    file(
      DOWNLOAD "${cooked_url}" "${WXT_3RD_DIR}/${file_name}.${file_suffix}"
      SHOW_PROGRESS
      STATUS status)
    message(STATUS "[wxTools] Downloading ${file_name}.${file_suffix} status: ${status}")

    # The status is a list, such as: 28;"Timeout was reached"
    list(GET status 0 status_code)
    if(status_code EQUAL 0)
      message(STATUS "[wxTools] Downloaded ${file_name}.${file_suffix} successfully.")
    else()
      message(WARNING "[wxTools] Failed to download ${file_name}.${file_suffix}.")
    endif()
  endif()

  if(EXISTS "${WXT_3RD_DIR}/${file_name}")
    return()
  endif()

  # Make directory...
  if(make_dir)
    if(NOT EXISTS "${WXT_3RD_DIR}/${file_name}")
      file(MAKE_DIRECTORY "${WXT_3RD_DIR}/${file_name}")
    endif()
  endif()

  # Extract...
  if(make_dir)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ../${file_name}.${file_suffix}
                    WORKING_DIRECTORY ${WXT_3RD_DIR}/${file_name})
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${file_name}.${file_suffix}
                    WORKING_DIRECTORY ${WXT_3RD_DIR})
  endif()
endfunction()

include(cmake/wxt_tools_gettext.cmake)
