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

wxt_download_zip_file(${file_url} ${file_name})
wxt_auto_import_package(${file_name} cserialport)
include_directories(${WXT_3RD_DIR}/${file_name}/include)

if(EXISTS ${WXT_LIBS_DIR}/${WXT_BUILD_FLAG}/${file_name}/include)
  set(wxtSerialPortLib ${CSerialPort_LIBRARY})
else()
  set(wxtSerialPortLib libcserialport)
endif()
