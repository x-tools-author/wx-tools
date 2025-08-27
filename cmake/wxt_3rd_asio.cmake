set(module_name asio)
wxt_add_github_module(${module_name} "https://github.com/chriskohlhoff/asio.git" "asio-1-30-2")
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name}/asio/include)
