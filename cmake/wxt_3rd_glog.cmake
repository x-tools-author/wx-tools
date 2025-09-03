# https://github.com/google/glog

set(WITH_GTEST
    OFF
    CACHE BOOL "Disable Google Test" FORCE)
set(WITH_GFLAGS
    OFF
    CACHE BOOL "Disable Google Flags" FORCE)
set(BUILD_TESTING
    OFF
    CACHE BOOL "Disable building tests" FORCE)

set(glog_version
    0.7.1
    CACHE STRING "glog version")
set(file_name "glog-${glog_version}")
set(file_url "https://github.com/google/glog/archive/refs/tags/v${glog_version}.zip")

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} glog)
