# https://developer.apple.com/documentation/xcode/packaging-mac-software-for-distribution
function(wxt_make_pkg target dev_id_app dev_id_installer)
  # cmake-format: off
  add_custom_target(${target}_pkg VERBATIM
    COMMENT "Making pkg..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/assets/${target}
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Application: ${dev_id_app}"
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Installer: ${dev_id_installer}"
    COMMAND echo "codesign --deep --force --verbose --sign ${dev_id_app} ${target}.app"
    COMMAND codesign --deep --force --verbose --sign "${dev_id_app}" "${target}.app"
    COMMAND pkgbuild --root ${target}.app --identifier "${WXT_APP_ID}" --version "${WXT_VERSION}" --install-location /Applications/${target}.app ${WXT_ASSETS_NAME}.pkg
  )
  # cmake-format: on
endfunction()

function(wxt_make_dmg target dev_id_app dev_id_installer)
  # cmake-format: off
  add_custom_target(${target}_dmg VERBATIM
    COMMENT "Making dmg..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/assets/${target}
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Application: ${dev_id_app}"
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Installer: ${dev_id_installer}"
    COMMAND ${CMAKE_COMMAND} -E echo "codesign --deep --force --verbose --sign ${dev_id_app} ${target}.app"
    COMMAND codesign --deep --force --verbose --sign "${dev_id_app}" "${target}.app"
    COMMAND hdiutil create -volname "${target}" -srcfolder "${target}.app" -ov -format UDZO "${WXT_ASSETS_NAME}.dmg"
    COMMAND codesign --force --sign "${dev_id_installer}" "${target}.dmg"
  )
  # cmake-format: on
endfunction()
