set(module_name fmt)
wxt_find_module(${module_name})
if(${fmt_FOUND})
  return()
endif()

wxt_add_github_module(${module_name} "https://github.com/fmtlib/fmt.git" "11.2.0")
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name})
wxt_install_module(${module_name})
