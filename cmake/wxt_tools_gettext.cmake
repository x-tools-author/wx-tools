# https://github.com/mlocati/gettext-iconv-windows/releases/download/v0.26-v1.17/gettext0.26-iconv1.17-shared-32.zip
set(tag "0.26-v1.17")
set(file_name "gettext0.26-iconv1.17-shared-32")
set(base_url "https://github.com/mlocati/gettext-iconv-windows/releases/download")
set(url "${base_url}/v${tag}/${file_name}.zip")

if(NOT EXISTS "${WXT_3RD_DIR}/${file_name}.zip")
  message(STATUS "[wxTools] Downloading ${file_name} from ${url}...")
  file(
    DOWNLOAD "${url}" "${WXT_3RD_DIR}/${file_name}"
    SHOW_PROGRESS
    STATUS status)
  message(STATUS "[wxTools] Downloading ${file_name} status: ${status}")

  # The status is a list, such as: 28;"Timeout was reached"
  list(GET status 0 status_code)
  if(status_code EQUAL 0)
    message(STATUS "[wxTools] Downloaded ${file_name} successfully.")
  else()
    message(WARNING "[wxTools] Failed to download ${file_name}.")
  endif()
endif()
