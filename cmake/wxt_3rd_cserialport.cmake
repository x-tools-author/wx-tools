set(module_name CSerialPort)

wxt_find_module(${module_name})
if(${${module_name}_FOUND})
  return()
endif()

wxt_add_github_module(${module_name} "https://github.com/itas109/CSerialPort.git" "v4.3.2")
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name}/include)
wxt_install_module(${module_name})
