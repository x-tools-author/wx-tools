# https://github.com/nlohmann/json
set(json_version "3.12.0")
set(file_name "json-${json_version}")
set(file_url "https://github.com/nlohmann/json/archive/refs/tags/v${json_version}.zip")

wxt_download_zip_file(${file_url} ${file_name})
if(NOT EXISTS ${WXT_3RD_DIR}/${file_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${WXT_3RD_DIR}/${file_name}.zip
                  WORKING_DIRECTORY ${WXT_3RD_DIR})
endif()
include_directories(${WXT_3RD_DIR}/${file_name}/include)
