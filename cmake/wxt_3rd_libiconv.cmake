# * https://www.gnu.org/software/libiconv/
# * https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
# * https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/v1.17-p1.zip
# * https://codeload.github.com/kiyolee/libiconv-win-build/zip/refs/tags/v1.17-p1
# * devenv libiconv.sln /Build

if(MINGW)
  message(STATUS "Using MinGW, skipping libiconv download and extraction")
  return()
endif()

if(ANDROID
   OR IOS
   OR MACOS)
  message(STATUS "Using Android or iOS, macOS, skipping libiconv download and extraction")
  return()
endif()

if(WIN32)
  if(${WXT_TARGET_ARCH} MATCHES "arm")
    return()
  endif()

  set(version_string "1.18-p1")
  set(file_name "v${version_string}")
  set(file_suffix "zip")
  set(file_url "https://github.com/kiyolee/libiconv-win-build/archive/refs/tags")
  set(file_url "${file_url}/${file_name}.${file_suffix}")
else()
  set(file_name "libiconv-1.18")
  set(file_suffix "tar.gz")
  set(file_url "https://ftp.gnu.org/pub/gnu/libiconv/${file_name}.${file_suffix}")
endif()

# Download libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix} AND NOT APPLE)
  message(STATUS "Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD ${file_url} ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix}
    STATUS download_status
    SHOW_PROGRESS)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "Failed to download ${file_name}: ${download_status}")
  endif()
endif()

# Extract libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name} AND NOT APPLE)
  message(STATUS "Extracting ${file_name}.${file_suffix}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${file_name}.${file_suffix}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  if(WIN32)
    set(tmp_dir libiconv-win-build-${version_string})
    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${tmp_dir} ${file_name}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  endif()
endif()

# Build libiconv if it does not exist
if(WIN32)
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-${WXT_VS})
  message(STATUS "[wxTools-libiconv] Working directory: ${working_dir}")

  string(TOLOWER ${WXT_TARGET_ARCH} arch)
  if(${arch} MATCHES "x86")
    set(arch "x86")
    set(lib_dir ${working_dir}/Release)
  elseif(${WXT_TARGET_ARCH} MATCHES "x64")
    set(arch "x64")
    set(lib_dir ${working_dir}/${arch}/Release)
  else()
    message(STATUS "[wxTools-libiconv] Unsupported architecture: ${WXT_TARGET_ARCH}")
    return()
  endif()

  message(STATUS "[wxTools-libiconv] Library directory: ${lib_dir}")
  if(NOT EXISTS ${lib_dir}/libiconv-static.lib)

    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.50")
      if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2026)
        set(src_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2022)
        set(dst_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2026)
        if(NOT EXISTS ${dst_dir})
          execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${src_dir} ${dst_dir})
          file(GLOB_RECURSE pro_files "${dst_dir}/*.vcxproj")
          foreach(pro_file IN LISTS pro_files)
            # Replace all occurrences of "v143" with "v145"
            set(old_text "v143")
            set(new_text "v145")
            execute_process(
              COMMAND
                powershell -Command
                "(Get-Content ${pro_file}) -replace '${old_text}', '${new_text}' | Set-Content ${pro_file}"
            )
          endforeach()
        endif()
      endif()

      set(WXT_VS "VS2026")
    elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30")
      set(WXT_VS "VS2022")
    elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.20")
      set(WXT_VS "VS2019")
    elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.10")
      set(WXT_VS "VS2017")
    elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.00")
      set(WXT_VS "VS2015")
    else()
      set(WXT_VS "VS2013")
    endif()

    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    set(devenv ${COMPILER_PATH}/../../../../../../../Common7/IDE/devenv.exe)
    message(STATUS "[wxTools-libiconv] ${devenv}")
    message(STATUS "[wxTools-libiconv] Building libiconv using Visual Studio")
    if(${arch} STREQUAL "x86")
      set(arch "Win32")
    endif()
    execute_process(COMMAND ${devenv} libiconv.sln /Build "Release|${arch}"
                    WORKING_DIRECTORY ${working_dir})
  endif()

  if(NOT EXISTS ${lib_dir}/libiconv-static.lib)
    message(STATUS "[wxTools-libiconv] libiconv-static.lib not found")
    return()
  endif()

  add_compile_definitions(WXT_ENABLE_ICONV)
  include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name}/include)
  link_directories(${lib_dir})
  list(APPEND WXT_LIBS "libiconv-static")
  message(STATUS "[wxTools-libiconv] Found libiconv-static.lib")
elseif(APPLE)
  list(APPEND WXT_LIBS iconv)
  add_compile_definitions(WXT_ENABLE_ICONV)
  # * set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  # * if(NOT EXISTS ${working_dir}/out/lib/libiconv.la)
  # * execute_process(COMMAND ./configure --prefix=${working_dir}/out
  # * WORKING_DIRECTORY ${working_dir})
  # * execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
  # * execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
  # * endif() *
  # * if(EXISTS ${working_dir}/out/lib/libiconv.la)
  # * add_compile_definitions(WXT_ENABLE_ICONV)
  # * include_directories(${working_dir}/out/include)
  # * link_directories(${working_dir}/out/lib)
  # * list(APPEND WXT_LIBS iconv)
  # * message(STATUS "[wxTools-libiconv] Found libiconv.la")
  # * endif()
else()
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  if(NOT EXISTS ${working_dir}/out/lib/libiconv.a)
    execute_process(COMMAND ./configure --prefix=${working_dir}/out --enable-static --disable-shared
                    WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
  endif()

  if(EXISTS ${working_dir}/out/lib/libiconv.a)
    add_compile_definitions(WXT_ENABLE_ICONV)
    include_directories(${working_dir}/out/include)
    link_directories(${working_dir}/out/lib)
    list(APPEND WXT_LIBS ${working_dir}/out/lib/libiconv.a)
  endif()
endif()
