# https://developer.apple.com/documentation/xcode/packaging-mac-software-for-distribution
function(wxt_make_pkg target dev_id_app dev_id_installer)
  # cmake-format: off
  add_custom_target(${target}_pkg VERBATIM
    COMMENT "Making pkg..."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/assets/${target}
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Application: ${dev_id_app}"
    COMMAND ${CMAKE_COMMAND} -E echo "Developer ID Installer: ${dev_id_installer}"
    COMMAND codesign -f -o runtime -s "${dev_id_app}" -v  ${target}.app --deep
    COMMAND productbuild --component ${target}.app /Applications --product ${target}.app/Contents/Info.plist ${target}.pkg
    COMMAND productsign --sign "${dev_id_installer}" ${target}.pkg ${target}-${WXT_VERSION}-${WXT_BUNDLE_VERSION}-signed.pkg
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
    COMMAND codesign -f -o runtime -s "${dev_id_app}" -v  ${target}.app --deep
    COMMAND hdiutil create -volname "${target}" -srcfolder "${target}.app" -ov -format UDZO "${WXT_ASSETS_NAME}.dmg"
  )
  # cmake-format: on
endfunction()
