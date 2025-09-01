set(file_name "cserialport-4.3.2")
set(file_url "https://github.com/itas109/CSerialPort/archive/refs/tags/v4.3.2.zip")

set(CSERIALPORT_BUILD_EXAMPLES
    OFF
    CACHE BOOL "Build CSerialPort examples")
set(CSERIALPORT_BUILD_TEST
    OFF
    CACHE BOOL "Build CSerialPort tests")
set(CSERIALPORT_BUILD_DOC
    OFF
    CACHE BOOL "Build CSerialPort documentation")

# Move CSerialPort to cserialport to avoid name conflicts
if(NOT WIN32)
  string(TOLOWER cserialport_name ${file_name})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E rename ${WXT_3RD_DIR}/${file_name}
            ${WXT_3RD_DIR}/${cserialport_name}
    RESULT_VARIABLE rename_result
    OUTPUT_VARIABLE rename_output
    ERROR_VARIABLE rename_error
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
endif()

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} cserialport)
include_directories(${WXT_3RD_DIR}/${file_name}/include)

if(EXISTS ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${file_name}/include)
  set(wxtSerialPortLib ${CSerialPort_LIBRARY})
else()
  set(wxtSerialPortLib libcserialport)
endif()
