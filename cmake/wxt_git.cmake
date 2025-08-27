# * Get the last tag.
# * git tag -d v1.0.0
# * git push github :refs/tags/v1.0.0
function(wxt_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git tag
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE git_tags
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # date_version: such as 2025.04.27
  # cmake-format: off
  string(TIMESTAMP current_year "%Y")
  string(TIMESTAMP current_month "%m")
  string(TIMESTAMP current_day "%d")
  math(EXPR current_month "${current_month} + 0")
  math(EXPR current_day "${current_day} + 0")
  set(date_version "${current_year}.${current_month}.${current_day}")
  set(${prefix}_GIT_TAG "${date_version}" CACHE STRING "Latest git tag" FORCE)
  # cmake-format: on

  if(NOT git_tags)
    add_compile_definitions(${prefix}_GIT_TAG="${date_version}")
    return()
  endif()

  if(WXT_DAILY_BUILD)
    add_compile_definitions(${prefix}_GIT_TAG="${date_version}")
    return()
  endif()

  string(REPLACE "\r" "" git_tags ${git_tags})
  string(REPLACE "\n" ";" git_tags ${git_tags})
  message("Tags found: ${git_tags}")
  list(LENGTH git_tags git_tags_count)
  list(GET git_tags -1 git_latest_tag)
  if(${git_latest_tag} STREQUAL "" OR ${git_latest_tag} STREQUAL "continuous")
    set(git_latest_tag ${date_version})
  endif()

  # Remove 'v' and 'V' from git_latest_tag
  string(REGEX REPLACE "^[vV]" "" git_latest_tag ${git_latest_tag})

  # cmake-format: off
  message("Latest git tag: ${git_latest_tag}")
  set(${prefix}_GIT_TAG ${git_latest_tag} CACHE STRING "Latest git tag" FORCE)
  add_compile_definitions(${prefix}_GIT_TAG="${git_latest_tag}")
  # cmake-format: on
endfunction()

# Get the last commit.
function(wxt_git_get_latest_commit working_dir prefix)
  execute_process(
    COMMAND git log -1 --pretty=%H
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message("Latest git commit(${prefix}_GIT_COMMIT): ${GIT_COMMIT}")
  add_compile_definitions(${prefix}_GIT_COMMIT="${GIT_COMMIT}")
endfunction()

# Get last commit time.
function(wxt_git_get_latest_commit_time working_dir prefix)
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message("Latest git commit time: ${GIT_COMMIT_TIME}")
  add_compile_definitions(${prefix}_GIT_COMMIT_TIME="${GIT_COMMIT_TIME}")
endfunction()
