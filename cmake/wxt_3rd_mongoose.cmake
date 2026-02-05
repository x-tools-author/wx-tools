# --------------------------------------------------------------------------------------------------
# For mongoose, the default value is 32, I think it is too small.
add_compile_definitions(MG_DATA_SIZE=10240)

set(mongoose_version 7.20)
set(file_name "mongoose-${mongoose_version}")
set(file_url "https://github.com/cesanta/mongoose/archive/refs/tags/${mongoose_version}.zip")

wxt_download_zip_file(${file_url} ${file_name})

if(NOT EXISTS ${WXT_3RD_DIR}/${file_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${WXT_3RD_DIR}/${file_name}.zip
                  WORKING_DIRECTORY ${WXT_3RD_DIR})
endif()

include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name})
set(tmp ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${file_name})
if(WIN32 AND MSVC)
  if(EXISTS ${tmp}/mongoose.lib)
    link_directories(${tmp})
    return()
  endif()
else()
  if(EXISTS ${tmp}/libmongoose.a)
    link_directories(${tmp})
    return()
  endif()
endif()

add_library(mongoose STATIC ${WXT_3RD_DIR}/${file_name}/mongoose.c)
if(WXT_AUTO_DEPLOY_3RD)
  set_target_properties(mongoose PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${tmp})
endif()
