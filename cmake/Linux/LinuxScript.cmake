message(STATUS "[wxTools-packing] Packaging ${argTarget}......")
message(STATUS "[wxTools-packing] Working dir: ${argWorkingDir}")
message(STATUS "[wxTools-packing] Packet name: ${argPacketName}") # e.g. wx-tools
message(STATUS "[wxTools-packing] Target name: ${argTargetName}") # e.g. wxTools
message(STATUS "[wxTools-packing] Version: ${argVersion}")
message(STATUS "[wxTools-packing] Tool path: ${argTool}")
message(STATUS "[wxTools-packing] Target binary dir: ${argTargetBinaryDir}")
message(STATUS "[wxTools-packing] Template dir: ${argTemplateDir}")
message(STATUS "[wxTools-packing] Icon file: ${argIconFile}")

# Remove argTargetName dir from argWorkingDir
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${argWorkingDir}/${argTargetName} "||" ${CMAKE_COMMAND} -E true)
message(STATUS "[wxTools-packing] Removed old tmp dir: ${argWorkingDir}/${argTargetName}")

# Copy argTemplateDir to argWorkingDir/argTargetName
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${argTemplateDir} ${argWorkingDir}/${argTargetName})
message(STATUS "[wxTools-packing] Copied template files from ${argTemplateDir} to ${argWorkingDir}/${argTargetName}")

# Create opt dir
make_directory(${argWorkingDir}/${argTargetName}/opt)

# Copy argTargetBinaryDir to opt/argFriendlyName
set(src_dir ${argTargetBinaryDir})
set(dst_dir ${argWorkingDir}/${argTargetName}/opt/${argPacketName})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${src_dir} ${dst_dir})
message(STATUS "[wxTools-packing] copy ${src_dir} to ${dst_dir}")

# Rename argTargetName to argPacketName
set(src_name ${argTargetName})
set(dst_name ${argPacketName})
set(tmp_dir ${argWorkingDir}/${argTargetName}/opt/${argPacketName})
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${src_name} ${dst_name} WORKING_DIRECTORY ${tmp_dir})
message(STATUS "[wxTools-packing] Renamed ${src_name} to ${dst_name} in ${tmp_dir}")

# Copy argIconFile to opt/argPacketName
set(icon_src ${argIconFile})
set(icon_dst ${argWorkingDir}/${argTargetName}/opt/${argPacketName}/icon.png)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${icon_src} ${icon_dst})
message(STATUS "[wxTools-packing] cmake -E copy_if_different ${icon_src} to ${icon_dst}")

# Rename argWorkingDir/argTargetName/usr/share/applications/app.desktop to argWorkingDir/argTargetName/usr/share/applications/argPacketName.desktop
# usr/share/applications/app.desktop -> usr/share/applications/wx-tools.desktop
set(old_desktop_file ${argWorkingDir}/${argTargetName}/usr/share/applications/app.desktop)
set(new_desktop_file ${argWorkingDir}/${argTargetName}/usr/share/applications/${argPacketName}.desktop)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${old_desktop_file} ${new_desktop_file})
message(STATUS "[wxTools-packing] Renamed ${old_desktop_file} to ${new_desktop_file}")

# Update control file
set(control_file ${argWorkingDir}/${argTargetName}/DEBIAN/control)
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})
message(STATUS "[wxTools-packing] Updated control file: ${control_file}: set argPacketName to ${new_text}")
set(old_text argVersion)
set(new_text ${argVersion})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})

# Update desktop file
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${new_desktop_file})
set(old_text argTargetName)
set(new_text ${argTargetName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${new_desktop_file})

# cmake-format: off
execute_process(COMMAND ${argTool} ${argWorkingDir}/${argTargetName}/opt/${argPacketName}/${argPacketName} -always-overwrite -bundle-non-qt-libs)
execute_process(COMMAND ${CMAKE_COMMAND} -E rm ${argWorkingDir}/${argTargetName}/opt/${argPacketName}/AppRun -f)
execute_process(COMMAND dpkg -b ./${argTargetName} ${argTargetName}-linux-${argVersion}-amd64.deb WORKING_DIRECTORY ${argWorkingDir})
# cmake-format: on
