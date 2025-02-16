# * argTarget: cmake target
# * argPacketName: packet name of deb packet
# * argFriendlyName: desktop entry file name
# * argVersion: the application version
# * argWorkingDir: working dir

set(control_file ${argWorkingDir}/DEBIAN/control)
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})
set(old_text argVersion)
set(new_text ${argVersion})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})

set(desktop_file_name_tmp ${argWorkingDir}/usr/share/applications/app.desktop)
set(desktop_file_name ${argWorkingDir}/usr/share/applications/${argPacketName}.desktop)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${desktop_file_name_tmp} ${desktop_file_name})

set(old_text argFriendlyName)
set(new_text ${argFriendlyName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})
set(old_text argTarget)
set(new_text ${argTarget})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})

# icon
set(icon_file_name_tmp ${argWorkingDir}/usr/share/icons/icon.png)
set(icon_file_name ${argWorkingDir}/usr/share/icons/${argPacketName}-icon.png)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${icon_file_name_tmp} ${icon_file_name})

set(old_text icon.png)
set(new_text ${argPacketName}-icon.png)
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})

execute_process(COMMAND dpkg -b ./ ${argPacketName}-${argVersion}-amd64.deb
                WORKING_DIRECTORY ${argWorkingDir})
