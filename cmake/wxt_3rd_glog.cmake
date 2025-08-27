wxt_find_module(glog)
if(${glog_FOUND})
  return()
endif()

set(module_name glog)
wxt_add_github_module(${module_name} "https://github.com/google/glog.git" "v0.7.1")
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name})
wxt_install_module(${module_name})
