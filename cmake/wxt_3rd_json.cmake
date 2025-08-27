set(module_name json)
wxt_add_github_module(${module_name} "https://github.com/nlohmann/json.git" "v3.11.3")
include_directories(${CMAKE_SOURCE_DIR}/3rd/${module_name}/include)
