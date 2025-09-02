# https://github.com/fmtlib/fmt
set(fmt_version "11.2.0")
set(file_name "fmt-${fmt_version}")
set(file_url
    "https://github.com/fmtlib/fmt/releases/download/${fmt_version}/fmt-${fmt_version}.zip")

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} fmt)
