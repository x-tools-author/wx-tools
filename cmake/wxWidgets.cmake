set(packet_name "wxWidgets-3.2.7")

set(wxWidget_LIBS ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${packet_name})
if(EXISTS ${wxWidget_LIBS})
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${wxWidget_LIBS})
  find_package(wxWidgets QUIET COMPONENTS core base aui)
  include(${wxWidgets_USE_FILE})
  message(STATUS "wxWidgets found in ${wxWidgets_INCLUDE_DIRS}")
else()
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
    WORKING_DIRECTORY ${workdir})
endif()
