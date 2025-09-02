# https://github.com/google/glog
set(file_name "glog-0.7.1")
set(file_url "https://github.com/google/glog/archive/refs/tags/v0.7.1.zip")

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} glog)
