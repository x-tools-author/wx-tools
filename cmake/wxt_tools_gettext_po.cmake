message(STATUS "[wxTools-po] I18N input dir: ${argInputDir}")
message(STATUS "[wxTools-po] I18N output dir: ${argOutputDir}")
message(STATUS "[wxTools-po] xgettext: ${argXgettext}")

# cmake-format: off
set(po_files af an ar ca co cs da de el es eu fa_IR fi fr gl_ES hi hr hu id it ja ka ko_KR lt lv ms nb ne nl pl pt_BR pt ro ru sk sl sq sr sv ta tr uk vi zh_CN zh_TW)
# cmake-format: on
file(GLOB_RECURSE src_files "${argInputDir}/*.h" "${argInputDir}/*.cpp")

# src_files相对于outputDir的相对路径
set(tmp "")
foreach(src_fil IN LISTS src_files)
  file(RELATIVE_PATH src_fil "${argInputDir}" "${src_fil}")
  list(APPEND tmp "${src_fil}")
endforeach()
set(src_files "${tmp}")

make_directory(${argOutputDir})
foreach(lang IN LISTS po_files)
  set(file_name "${argOutputDir}/${lang}.po")
  execute_process(COMMAND ${argXgettext} --join-existing --keyword=_ "-o" "${file_name}"
                          ${src_files} WORKING_DIRECTORY ${argInputDir})
  if(EXISTS "${file_name}")
    message(STATUS "[wxTools-po] Generated ${file_name}")
  endif()
endforeach()
