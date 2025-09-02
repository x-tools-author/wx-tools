# https://github.com/chriskohlhoff/asio
set(asio_version
    1.36.0
    CACHE STRING "Asio version")
string(REPLACE "." "-" asio_version_dash ${asio_version})
set(file_name "asio-${asio_version}")
set(base_url "https://codeload.github.com/chriskohlhoff/asio")
set(file_url "${base_url}/zip/refs/tags/asio-${asio_version_dash}")
# https://codeload.github.com/chriskohlhoff/asio/zip/refs/tags/asio-1-36-0

wxt_download_zip_file(${file_url} ${file_name})
if(NOT EXISTS ${WXT_3RD_DIR}/${file_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${WXT_3RD_DIR}/${file_name}.zip
                  WORKING_DIRECTORY ${WXT_3RD_DIR})
endif()
include_directories(${WXT_3RD_DIR}/asio-asio-${asio_version_dash}/asio/include)
