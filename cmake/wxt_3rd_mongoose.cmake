# --------------------------------------------------------------------------------------------------
# For mongoose, the default value is 32, I think it is too small.
add_compile_definitions(MG_DATA_SIZE=10240)

set(module_name mongoose)
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name})
set(tmp ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${module_name})
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

wxt_add_github_module(${module_name} "https://github.com/cesanta/mongoose.git" "7.19")
add_library(${module_name} STATIC ${CMAKE_SOURCE_DIR}/3rd/${module_name}/mongoose.c)
set_target_properties(${module_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${tmp})
