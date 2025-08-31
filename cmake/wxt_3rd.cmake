include(FetchContent)
function(wxt_add_github_module module repository tag)
  FetchContent_Declare(
    ${module}
    GIT_REPOSITORY ${repository}
    GIT_TAG ${tag}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/${module})
  FetchContent_MakeAvailable(${module})
endfunction()

function(wxt_install_module module_name)
  if(ANDROID AND NOT IOS)
    return()
  endif()

  if(APPLE)
    string(TOLOWER ${module_name} module_name_lower)
  elseif(LINUX)
    string(TOLOWER ${module_name} module_name_lower)
  endif()
  set(module_src_dir ${CMAKE_BINARY_DIR}/_deps/${module_name_lower}-build)
  set(module_dst_dir ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${module_name})
  add_custom_target(${module_name}_auto_install ALL
                    COMMAND ${CMAKE_COMMAND} --install ${module_src_dir} --prefix ${module_dst_dir})
  set_property(TARGET ${module_name}_auto_install PROPERTY FOLDER "module")
endfunction()

function(wxt_find_module module_name)
  set(module_dst_dir ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${module_name})
  if(EXISTS ${module_dst_dir}/include)
    set(CMAKE_PREFIX_PATH ${module_dst_dir} ${CMAKE_PREFIX_PATH})
    find_package(${module_name} REQUIRED)
    # cmake-format: off
    if(${module_name}_FOUND)
      include_directories(${module_dst_dir}/include)
      link_directories(${module_dst_dir}/lib)
      message(STATUS "[wxTools] Found ${module_name}: ${module_dst_dir}")
    else()
      set(${module_name}_FOUND FALSE PARENT_SCOPE)
      message(FATAL_ERROR "[wxTools] ${module_name} not found")
    endif()
    # cmake-format: on
  endif()
endfunction()

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
  if(EXISTS ${package_dst_dir}/include)
    set(CMAKE_PREFIX_PATH ${package_dst_dir} ${CMAKE_PREFIX_PATH})
    find_package(${package_name} REQUIRED)
    message(STATUS "[PUMA]Found ${package_dir_name}: ${package_dst_dir}")
  else()
    add_subdirectory(${WXT_3RD_DIR}/${package_dir_name})

    if(NOT ANDROID AND NOT IOS)
      add_custom_target(
        ${package_dir_name}_auto_install ALL
        COMMAND ${CMAKE_COMMAND} --install . --prefix ${package_dst_dir}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_dir_name}
        COMMENT "Installing ${package_dir_name} to ${package_dst_dir}")
      add_dependencies(${package_dir_name}_auto_install ${package_name} ${ARGN})
      set_property(TARGET ${package_dir_name}_auto_install PROPERTY FOLDER "3rd")
    endif()
  endif()
endfunction()

include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_fmt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_asio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_glog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_mongoose.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_cserialport.cmake)

add_compile_definitions(_UNICODE)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_wxwidgets.cmake)
