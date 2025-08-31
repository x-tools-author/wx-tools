set(file_name "fmt-11.1.4")
set(file_url "https://github.com/fmtlib/fmt/releases/download/11.1.4/fmt-11.1.4.zip")

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} fmt)
