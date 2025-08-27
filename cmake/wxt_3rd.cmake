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
      set(${module_name}_FOUND TRUE PARENT_SCOPE)
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

include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_fmt.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_json.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_asio.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_glog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_mongoose.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_cserialport.cmake)

add_compile_definitions(_UNICODE)
include(${CMAKE_CURRENT_LIST_DIR}/wxt_3rd_wxwidgets.cmake)
