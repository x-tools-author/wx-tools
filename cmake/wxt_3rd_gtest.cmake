# https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
option(WXT_BUILD_TESTS "Build wxTools tests" ON)
if(NOT WXT_BUILD_TESTS)
  return()
endif()

set(google_test_version
    1.17.0
    CACHE STRING "Google Test version")
set(file_name "googletest-${google_test_version}")
set(file_url "https://github.com/google/googletest/archive/refs/tags/v${google_test_version}.zip")

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} GTest)
