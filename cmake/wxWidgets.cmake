# https://wxwidgets.org/downloads/

# The version of wxWidgets must be equal to or greater than 3.2.0
set(X_WXW_VERSION
    "3.3.1"
    CACHE STRING "Select the version of wxWidgets to be used")

set(packet_version "${X_WXW_VERSION}")
set(packet_name "wxWidgets-${packet_version}")
set(packet_url_base "https://github.com/wxWidgets/wxWidgets/releases/download")
set(packet_url "${packet_url_base}/v${packet_version}/${packet_name}.zip")

message(STATUS "[wxWidgets] packet_version ${packet_version}")
message(STATUS "[wxWidgets] packet_name ${packet_name}")
message(STATUS "[wxWidgets] packet_url_base ${packet_url_base}")
message(STATUS "[wxWidgets] packet_url ${packet_url}")

# Download wxWidgets if not already present
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${packet_name}.zip)
  message(STATUS "Downloading wxWidgets from ${packet_url}...")
  file(
    DOWNLOAD ${packet_url} ${CMAKE_SOURCE_DIR}/3rd/${packet_name}.zip
    STATUS download_status
    SHOW_PROGRESS)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "Failed to download wxWidgets: ${download_status}")
  endif()
endif()

# Using wxWidgets from the libs directory
set(wxWidget_LIBS ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${packet_name})
message(STATUS "[wxWidgets] wxWidget_LIBS ${wxWidget_LIBS}")

if(EXISTS ${wxWidget_LIBS})
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${wxWidget_LIBS})
  message(STATUS "[wxWidgets] CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}")

  find_package(wxWidgets QUIET COMPONENTS core base aui)
  include(${wxWidgets_USE_FILE})
  include_directories(${wxWidget_LIBS}/include/wx-3.2)
  message(STATUS "wxWidgets found in ${wxWidgets_INCLUDE_DIRS}")
  return()
endif()

# Extract the wxWidgets packet
message(STATUS "wxWidgets not found, extracting ${packet_name}...")
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${packet_name}/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${packet_name}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/3rd/${packet_name}.zip
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd/${packet_name})
endif()

# Add the wxWidgets subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${packet_name})
include_directories(${CMAKE_SOURCE_DIR}/3rd/${packet_name}/include)
set(wxWidgets_LIBRARIES wx::core wx::aui)

# Install wxWidgets to the ${CMAKE_SOURCE_DIR}/libs directory
set(prefix ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${packet_name})
set(workdir ${CMAKE_BINARY_DIR}/3rd/${packet_name})
add_custom_target(
  install-${packet_name}
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${prefix}
  COMMAND ${CMAKE_COMMAND} --install . --prefix ${prefix}
  SOURCES ${CMAKE_SOURCE_DIR}/cmake/wxWidgets.cmake
  WORKING_DIRECTORY ${workdir})
