cmake_minimum_required(VERSION 3.20)
# cmake-format: off
project(wxTools VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_AUTORCC ON)
# cmake-format: on

# cmake-format: off
if(NOT APPLE)
  set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared libraries" FORCE)
if(WIN32)
  set(wxBUILD_USE_STATIC_RUNTIME ON CACHE BOOL "Build static runtime" FORCE)
endif()
endif()
# cmake-format: on

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
  if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  elseif(MINGW)
    # set(CMAKE_EXE_LINKER_FLAGS -static)
  endif()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(WXT_RELEASE)
endif()

# Windows XP
option(_WIN32_WINNT "Windows Version" 0x0501)
# Windows 7 option(_WIN32_WINNT "Windows Version" 0x0601)

# --------------------------------------------------------------------------------------------------
# For portable edition
option(WXT_PORTABLE_EDITION "Portable edition" ON)
if(WXT_PORTABLE_EDITION)
  add_compile_definitions(WXT_PORTABLE_EDITION)
endif()

include(cmake/git.cmake)
wxt_git_get_latest_tag(${CMAKE_SOURCE_DIR} WXT)
wxt_git_get_latest_commit(${CMAKE_SOURCE_DIR} WXT)
wxt_git_get_latest_commit_time(${CMAKE_SOURCE_DIR} WXT)

include(cmake/fmt.cmake)
include(cmake/asio.cmake)
include(cmake/glog.cmake)
include(cmake/json.cmake)
include(cmake/wxTools.cmake)
include(cmake/mongoose.cmake)
include(cmake/wxWidgets.cmake)
include(cmake/Msix/Msix.cmake)
include(cmake/CSerialPort.cmake)

include_directories(src)

# --------------------------------------------------------------------------------------------------
# For mongoose, the default value is 32, I think it is too small.
add_compile_definitions(MG_DATA_SIZE=10240)

file(GLOB_RECURSE WX_TOOLS_SOURCE src/*.*)
list(APPEND WX_TOOLS_SOURCE wxTools.rc)
set(WXT_OUTPUT_DIR "${WX_TOOLS_BINARY_DIR}/wxTools")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WXT_OUTPUT_DIR})
add_executable(wxTools ${WX_TOOLS_SOURCE})

set(PROJECT_LIBS libcserialport mongoose wx::core wx::aui glog::glog fmt::fmt)
target_link_libraries(wxTools PRIVATE ${PROJECT_LIBS})
set_target_properties(
  wxTools
  PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
             ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
             TRUE WIN32_EXECUTABLE
             TRUE)

# --------------------------------------------------------------------------------------------------
# Copy res to build directory
# cmake-format: off
add_custom_command(TARGET wxTools POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/i18n" "$<TARGET_FILE_DIR:wxTools>/i18n"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/files" "$<TARGET_FILE_DIR:wxTools>/files"
)
# cmake-format: on

# --------------------------------------------------------------------------------------------------
# Make zip file
option(WXT_MAKE_ZIP "Make zip file" ON)
if(WXT_MAKE_ZIP)
  wxt_make_zip(wxTools)
endif()

# --------------------------------------------------------------------------------------------------
# Deploy local files
file(GLOB_RECURSE WX_TOOLS_LOCAL_FILES res/*.po)
add_custom_target(
  wxTools-locale
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res/i18n"
          "$<TARGET_FILE_DIR:wxTools>/i18n"
  SOURCES ${WX_TOOLS_LOCAL_FILES})

# --------------------------------------------------------------------------------------------------
# Generate MSI installer for Microsoft Store
wxt_build_for_store(wxTools)
option(WXT_GENERATE_MSIX "Generate MSIX" ON)
if(${CMAKE_BUILD_TYPE} STREQUAL "Release" AND WXT_GENERATE_MSIX)
  # Remove “v” from ${WXT_GIT_TAG}
  string(SUBSTRING ${WXT_GIT_TAG} 1 -1 WXT_GIT_TAG)
  wxt_generate_msix(wxTools "wxTools" "wxTools" ${WXT_GIT_TAG} OFF)
endif()
