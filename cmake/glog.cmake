set(packet_name "glog-0.7.1")

# cmake-format: off
set(WITH_GFLAGS OFF)
set(BUILD_TESTING OFF)
set(WITH_TOOLS OFF CACHE BOOL "Reset option" FORCE)
# cmake-format: on

set(wxWidget_LIBS ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${packet_name})
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${wxWidget_LIBS})
find_package(glog QUIET)
if(glog_FOUND AND WXT_AUTO_USING_3RD_LIBS)
  # Nothing to do yet...
else()
  # Extract the glog packet
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${packet_name})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/3rd/${packet_name}.zip
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  endif()

  # Add the glog subdirectory
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${packet_name})

  # Install glog to the ${CMAKE_SOURCE_DIR}/libs directory
  set(prefix ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${packet_name})
  set(workdir ${CMAKE_BINARY_DIR}/3rd/${packet_name})
  add_custom_target(
    install-${packet_name}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${prefix}
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${prefix}
    SOURCES ${CMAKE_SOURCE_DIR}/cmake/glog.cmake
    WORKING_DIRECTORY ${workdir})
endif()
