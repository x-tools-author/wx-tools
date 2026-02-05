# https://github.com/simpleble/simpleble.git
set(package_version v0.11.0)

macro(wxt_remove_all_ble_files)
  file(GLOB_RECURSE BLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/BLE*.*")
  foreach(file ${BLE_FILES})
    list(REMOVE_ITEM WXT_SOURCES ${file})
    message(STATUS "[wxTools.BLE] Remove simple-ble file: ${file}")
  endforeach()
  add_compile_definitions(WXT_ENABLE_BLE=0)
endmacro()

if(ANDROID OR IOS)
  wxt_remove_all_ble_files()
  return()
endif()

option(WXT_ENABLE_BLE "Enable SimpleBLE support via simple-ble library" OFF)
if(NOT WXT_ENABLE_BLE)
  wxt_remove_all_ble_files()
  return()
endif()

add_compile_definitions(WXT_ENABLE_BLE=1)

# Using pre-build simple-ble libs
set(dst_dir ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/simpleble-${package_version})
if(EXISTS ${dst_dir}/include/simpleble/SimpleBLE.h)
  list(APPEND CMAKE_PREFIX_PATH ${dst_dir})
  find_package(simpleble REQUIRED)
  message(STATUS "[wxtTools] Found simple-ble: ${dst_dir}")
  list(APPEND WXT_LIBS simpleble::simpleble simpleble::simpleble-c)
  return()
endif()

# Build simple-ble from source
FetchContent_Declare(
  simpleble
  GIT_REPOSITORY https://github.com/simpleble/simpleble.git
  GIT_TAG ${package_version}
  SOURCE_SUBDIR simpleble)
FetchContent_MakeAvailable(simpleble)
list(APPEND WXT_LIBS simpleble::simpleble simpleble::simpleble-c)

# Install simple-ble
add_custom_target(
  simpleble_post ALL
  DEPENDS simpleble simpleble-c
  COMMAND ${CMAKE_COMMAND} --install . --prefix ${dst_dir}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/simpleble-build
  COMMENT "Installing simple-ble to ${dst_dir}")
