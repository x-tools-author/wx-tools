# --------------------------------------------------------------------------------------------------
function(wxt_download_zip_file url file_name)
  if(NOT EXISTS ${WXT_3RD_DIR}/${file_name}.zip)
    file(
      DOWNLOAD ${url} ${WXT_3RD_DIR}/${file_name}.zip
      SHOW_PROGRESS
      STATUS status)

    # The status is a list, such as: 28;"Timeout was reached"
    list(GET status 0 status_code)
    if(status_code EQUAL 0)
      message(STATUS "[wxTools] Downloaded ${file_name} successfully.")
    else()
      message(WARNING "[wxTools] Failed to download ${file_name}.")
    endif()
  endif()
endfunction()

# --------------------------------------------------------------------------------------------------
function(wxt_auto_import_package package_zip_name package_name)
  # Extract the package name from the path
  if(NOT EXISTS ${WXT_3RD_DIR}/${package_zip_name})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_zip_name}.zip
                    WORKING_DIRECTORY ${WXT_3RD_DIR})
  endif()

  wxt_auto_import_package_dir(${package_zip_name} ${package_name} ${ARGN})
endfunction()

# --------------------------------------------------------------------------------------------------
function(wxt_auto_import_package_dir package_dir_name package_name)
  # Add module...
  set(package_dst_dir ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${package_dir_name})
  if(EXISTS ${package_dst_dir}/include AND WXT_AUTO_DEPLOY_3RD)
    set(CMAKE_PREFIX_PATH ${package_dst_dir} ${CMAKE_PREFIX_PATH})
    find_package(${package_name} REQUIRED)

    if(${package_name}_FOUND)
      include_directories(${package_dst_dir}/include)
      message(STATUS "[wxTools] Found ${package_dir_name}: ${package_dst_dir}")
    else()
      message(WARNING "[wxTools] ${package_dir_name} not found")
    endif()

    set(${package_name}_FOUND
        ${${package_name}_FOUND}
        PARENT_SCOPE)
  else()
    add_subdirectory(${WXT_3RD_DIR}/${package_dir_name})

    if(NOT ANDROID
       AND NOT IOS
       AND WXT_AUTO_DEPLOY_3RD)
      wxt_install_package(${package_dir_name})
    endif()
  endif()
endfunction()

function(wxt_install_package package_dir_name)
  set(package_dst_dir ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${package_dir_name})
  add_custom_target(
    ${package_dir_name}_auto_install
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${package_dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_dir_name}
    COMMENT "Installing ${package_dir_name} to ${package_dst_dir}")
endfunction()

include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_fmt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_lua.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_asio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_glog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_mongoose.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_libiconv.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_cserialport.cmake)

add_compile_definitions(_UNICODE)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_wxwidgets.cmake)
