message(STATUS "[wxTools-i18n] I18N input dir: ${argInputDir}")
message(STATUS "[wxTools-i18n] I18N output dir: ${argOutputDir}")
message(STATUS "[wxTools-i18n] wxWidgets local dir: ${argWxWidgetsLocalDir}")
message(STATUS "[wxTools-i18n] msgfile: ${argMsgfmt}")

# wxTools i18n...
file(GLOB_RECURSE po_files "${argInputDir}/*.po")
foreach(po_file IN LISTS po_files)
  get_filename_component(tmp "${po_file}" NAME_WE)
  make_directory("${argOutputDir}/${tmp}")
  set(mo_file "${argOutputDir}/${tmp}/wxTools.mo")
  execute_process(COMMAND ${argMsgfmt} "-o" "${mo_file}" "${po_file}")

  if(EXISTS "${mo_file}")
    string(REPLACE "${argOutputDir}/" "" mo_file "${mo_file}")
    message(STATUS "[wxTools-i18n] Generated ${mo_file} from wxTools ${tmp}.po")
  endif()
endforeach()

# wxWidgets i18n...
if(NOT EXISTS ${argWxWidgetsLocalDir})
  return()
endif()

# * Get all .po files from wxWidgets local directory.
# * Generate .mo files from .po files, such en.po -> ${argOutputDir}/en/wxWidgets.mo
file(GLOB_RECURSE po_files "${argWxWidgetsLocalDir}/*.po")
foreach(po_file IN LISTS po_files)
  # If the file contains '@', ignore it.
  if("${po_file}" MATCHES "@")
    continue()
  endif()

  get_filename_component(tmp "${po_file}" NAME_WE)
  make_directory("${argOutputDir}/${tmp}")
  set(mo_file "${argOutputDir}/${tmp}/wxWidgets.mo")
  execute_process(COMMAND ${argMsgfmt} "-o" "${mo_file}" "${po_file}")

  string(REPLACE "${argOutputDir}/" "" mo_file "${mo_file}")
  set(tmp "${argWxWidgetsLocalDir}/${tmp}.po")
  message(STATUS "[wxTools-i18n] Generated ${mo_file} from wxWidgets ${tmp}.po")
endforeach()
