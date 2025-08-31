set(file_name "asio-1.30.2")
set(file_url "https://codeload.github.com/chriskohlhoff/asio/zip/refs/tags/asio-1-30-2")

wxt_download_zip_file(${file_url} ${file_name})
if(NOT EXISTS ${WXT_3RD_DIR}/${file_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${WXT_3RD_DIR}/${file_name}.zip
                  WORKING_DIRECTORY ${WXT_3RD_DIR})
endif()
include_directories(${WXT_3RD_DIR}/asio-asio-1-30-2/asio/include)
