# cmake-format: off
# Do something make CMake happy
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.20" CACHE STRING "CMake policy version" FORCE)
endif()
cmake_minimum_required(VERSION 3.20)
project(wxTools VERSION 1.0 LANGUAGES C CXX)
# cmake-format: on

if(IOS OR ANDROID)
  # Not supported on iOS and Android yet...
  message(FATAL_ERROR "wxTools is not supported on iOS and Android")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(WXT_RELEASE)
endif()

# * 为提高编译性能，可以添加Windows Defender排除路径：
# * 添加排除路径（管理员）：Add-MpPreference -ExclusionPath "C:\Users\user\Project"
# * 查看排除路径（管理员）：Get-MpPreference | Select-Object -ExpandProperty ExclusionPath
# * 移除排除路径（管理员）：Remove-MpPreference -ExclusionPath "C:\Users\user\Project"
if(MSVC)
  set(WXT_TARGET_ARCH ${CMAKE_EXE_LINKER_FLAGS})
  string(REPLACE "/machine:" "" WXT_TARGET_ARCH ${WXT_TARGET_ARCH})
else()
  if(APPLE)
    if(CMAKE_OSX_ARCHITECTURES)
      set(WXT_TARGET_ARCH ${CMAKE_OSX_ARCHITECTURES})
    else()
      set(CMAKE_OSX_ARCHITECTURES
          "arm64"
          CACHE STRING "Build architectures for macOS")
      set(WXT_TARGET_ARCH "arm64")
    endif()
  else()
    # 获取空指针大小
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
      set(WXT_TARGET_ARCH "x86")
    elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
      set(WXT_TARGET_ARCH "x64")
    else()
      set(WXT_TARGET_ARCH "unknown")
    endif()
  endif()
endif()
message(STATUS "------------------------------------------------------------")
message(STATUS "[wxTools] CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "[wxTools] CMAKE_GENERATOR: ${CMAKE_GENERATOR}")
message(STATUS "[wxTools] CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "[wxTools] CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "[wxTools] CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "[wxTools] CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "[wxTools] CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "[wxTools] CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "[wxTools] CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "[wxTools] CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "[wxTools] CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "[wxTools] WXT_TARGET_ARCH: ${WXT_TARGET_ARCH}")

# --------------------------------------------------------------------------------------------------
# Project options
# cmake-format: off
set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared libraries" FORCE)
set(wxBUILD_USE_STATIC_RUNTIME ON CACHE BOOL "Build static runtime" FORCE)
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Reset option" FORCE)
endif()

option(WXT_PORTABLE_EDITION "Portable edition" ON)
option(WXT_DAILY_BUILD "Daily build" OFF)
if(WXT_PORTABLE_EDITION)
  add_compile_definitions(WXT_PORTABLE_EDITION)
endif()
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Reset option" FORCE)
# cmake-format: on

# Git information
include(cmake/wxt_git.cmake)
wxt_git_get_latest_tag(${CMAKE_SOURCE_DIR} WXT)
wxt_git_get_latest_commit(${CMAKE_SOURCE_DIR} WXT)
wxt_git_get_latest_commit_time(${CMAKE_SOURCE_DIR} WXT)

# Global variables
set(WXT_VERSION "0.0.0")
if(WXT_GIT_TAG)
  set(WXT_VERSION ${WXT_GIT_TAG})
endif()

# Something about the 3rd party libraries
set(WXT_3RD_DIR "${CMAKE_SOURCE_DIR}/3rd")
set(WXT_LIBS_DIR "${CMAKE_SOURCE_DIR}/libs")
set(cxx_compiler_flag "${CMAKE_CXX_COMPILER_ID}${CMAKE_CXX_COMPILER_VERSION}")
set(WXT_BUILD_FLAG "${CMAKE_BUILD_TYPE}/${WXT_TARGET_ARCH}/${cxx_compiler_flag}")
set(WX_TOOLS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)

# Do change WXT_MAGIC unless you know what you are doing...
option(WXT_MAGIC "The magic option..." OFF)
if(WXT_MAGIC)
  add_compile_definitions(WXT_MAGIC)
endif()

# Enable IPv6 options
option(WXT_ENABLE_IPV6 "Enable IPv6" OFF)
if(WXT_ENABLE_IPV6)
  add_compile_definitions(WXT_ENABLE_IPV6)
endif()

# --------------------------------------------------------------------------------------------------
# CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(wxt)
include(wxt_3rd)

# --------------------------------------------------------------------------------------------------
# wxTools applications
include_directories(src)
file(GLOB_RECURSE WX_TOOLS_SOURCE src/*.*)
list(APPEND WX_TOOLS_SOURCE wxTools.rc)
set(WXT_OUTPUT_DIR "${WX_TOOLS_BINARY_DIR}/wxTools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WXT_OUTPUT_DIR})
add_executable(wxTools ${WX_TOOLS_SOURCE})
if(APPLE)
  set(PROJECT_LIBS glog::glog fmt::fmt libcserialport mongoose)
else()
  set(PROJECT_LIBS glog::glog fmt::fmt libcserialport mongoose)
endif()
target_link_libraries(wxTools PRIVATE ${wxWidgets_LIBRARIES} ${PROJECT_LIBS})
message(STATUS "[wxTools] Libraries of wxTools: ${wxWidgets_LIBRARIES} ${PROJECT_LIBS}")

# --------------------------------------------------------------------------------------------------
# Copy res to build directory
# cmake-format: off
if(WIN32)
  add_custom_command(TARGET wxTools POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/i18n" "$<TARGET_FILE_DIR:wxTools>/i18n"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/files" "$<TARGET_FILE_DIR:wxTools>/files")
else(APPLE)
  add_custom_command(TARGET wxTools POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/i18n" "$<TARGET_FILE_DIR:wxTools>/i18n"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/files" "$<TARGET_FILE_DIR:wxTools>/files")
endif()
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# Do something for Windows
if(WIN32)
  set_target_properties(wxTools PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)

  option(WXT_MAKE_ZIP "Make zip file" ON)
  if(WXT_MAKE_ZIP)
    wxt_make_zip(wxTools)
  endif()

  wxt_build_for_store(wxTools)
  option(WXT_GENERATE_MSIX "Generate MSIX" ON)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Release" AND WXT_GENERATE_MSIX)
    include(cmake/Msix/Msix.cmake)
    wxt_generate_msix(wxTools "wxTools" "wxTools" ${WXT_VERSION} OFF)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Do something for Linux
if(LINUX)
  include(cmake/Linux/Linux.cmake)
  wxt_make_package(wxTools "wx-tools" "wxTools" ${WXT_VERSION} ON)
  wxt_make_package(wxTools "wx-tools" "wxTools" ${WXT_VERSION} OFF)
endif()

# --------------------------------------------------------------------------------------------------
# Do something for Apple
if(APPLE)
  # Set the icon file for the macOS bundle
  set(MACOSX_BUNDLE_ICON_FILE wxTools.icns)
  set(wxTools_ICON ${CMAKE_SOURCE_DIR}/wxTools.icns)
  set_source_files_properties(${wxTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  target_sources(wxTools PRIVATE ${wxTools_ICON})

  # Remove the last section from WXT_VERSION, such "1.0.0" -> "1.0"
  string(REGEX REPLACE "\\.[0-9]+$" "" short_ver ${WXT_VERSION})
  set(WX_TOOLS_BUNDLE_VERSION ${WXT_VERSION})
  set(WX_TOOLS_BUNDLE_SHORT_VERSION_STRING "${short_ver}")

  string(TIMESTAMP CURRENT_YEAR "%Y")
  message(STATUS "Current year: ${CURRENT_YEAR}")
  # cmake-format: off
  set(WX_TOOLS_YYYY "${CURRENT_YEAR}" CACHE STRING "The current year")
  # cmake-format: on

  set(out_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/wxTools.app/Contents/Info.plist)
  configure_file(res/apple/macos/wxTools.plist.in ${CMAKE_BINARY_DIR}/wxTools.plist)

  # Set the bundle properties
  # cmake-format: off
  if(BUNDLE_ID_OPTION)
    set_target_properties(wxTools PROPERTIES ${BUNDLE_ID_OPTION})
  endif()
  set_target_properties(wxTools PROPERTIES MACOSX_BUNDLE TRUE)
  set_target_properties(wxTools PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION ${WX_TOOLS_BUNDLE_VERSION})
  set_target_properties(wxTools PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "${WX_TOOLS_BUNDLE_SHORT_VERSION_STRING}")
  # cmake-format: on

  # Create pak file
  include(cmake/MacOS/MacOS.cmake)
  # cmake-format: off
  set(WXTOOLS_DEV_ID_APP "xxx_id_developer" CACHE STRING "The app's developer ID")
  set(WXTOOLS_DEV_ID_INSTALLER "xxx_id_installer" CACHE STRING "The installer's developer ID")
  wxt_make_pkg(wxTools ${WXTOOLS_DEV_ID_APP} ${WXTOOLS_DEV_ID_INSTALLER})
  # cmake-format: on
endif()
