# https://github.com/mlocati/gettext-iconv-windows/releases/download/v0.26-v1.17/gettext0.26-iconv1.17-shared-32.zip
set(tag "0.26-v1.17")
set(file_name "gettext0.26-iconv1.17-shared-32")
set(base_url "https://github.com/mlocati/gettext-iconv-windows/releases/download")
set(url "${base_url}/v${tag}/${file_name}.zip")

wxt_download_and_extract("${url}" "${file_name}" "zip" TRUE)

set(msgfmt "${WXT_3RD_DIR}/${file_name}/bin/msgfmt.exe")
set(xgettext "${WXT_3RD_DIR}/${file_name}/bin/xgettext.exe")
set(po_script_file "${CMAKE_CURRENT_LIST_DIR}/wxt_tools_gettext_po.cmake")
set(mo_script_file "${CMAKE_CURRENT_LIST_DIR}/wxt_tools_gettext_mo.cmake")

# * Convert .po files to .mo files.
# * All .po files will be converted and placed in the output directory.
# * Input directory framework will be keep.
function(wxt_generate_mo target wxLocalDir)
  set(args -DargMsgfmt=${msgfmt})
  list(APPEND args -DargWxWidgetsLocalDir=${wxLocalDir})
  list(APPEND args -DargInputDir=${CMAKE_SOURCE_DIR}/res/i18n)
  list(APPEND args -DargOutputDir=$<TARGET_FILE_DIR:${target}>/i18n)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} ${args} -P ${mo_script_file})
  add_custom_target(${target}-mo COMMAND ${CMAKE_COMMAND} ${args} -P ${mo_script_file})
endfunction()

function(wxt_generate_po target in_dir out_dir)
  set(args -DargInputDir=${in_dir} -DargOutputDir=${out_dir} -DargXgettext=${xgettext})
  add_custom_target(${target}-po COMMAND ${CMAKE_COMMAND} ${args} -P ${po_script_file})
endfunction()
