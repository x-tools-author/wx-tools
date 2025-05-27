# https://github.com/cesanta/mongoose
set(packet_name "mongoose-7.18")

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${packet_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/3rd/${packet_name}.zip
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
endif()

include_directories(${CMAKE_SOURCE_DIR}/3rd/${packet_name})
add_library(mongoose STATIC ${CMAKE_SOURCE_DIR}/3rd/${packet_name}/mongoose.h
                            ${CMAKE_SOURCE_DIR}/3rd/${packet_name}/mongoose.c)
set_target_properties(mongoose PROPERTIES FOLDER "3rd")
