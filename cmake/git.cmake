# * Get the last tag.
# * git tag -d v1.0.0
# * git push github :refs/tags/v1.0.0

function(wxt_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git tag
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE git_tags
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(NOT git_tags)
    # Such as 25.2.5
    string(TIMESTAMP current_year "%Y")
    string(TIMESTAMP current_month "%m")
    string(TIMESTAMP current_day "%d")
    math(EXPR current_month "${current_month} + 0")
    math(EXPR current_day "${current_day} + 0")
    set(target_version "v${current_year}.${current_month}.${current_day}")
    # string(TIMESTAMP target_version "%y.%m.%d")
    set(GIT_LATEST_TAG ${target_version})
    set(${prefix}_GIT_TAG
        "${GIT_LATEST_TAG}"
        CACHE STRING "Latest git tag" FORCE)
    return()
  endif()

  if(WIN32)
    string(REPLACE "\r" "" git_tags ${git_tags})
  endif()
  string(REPLACE "\n" ";" git_tags ${git_tags})
  message("Tags found: ${git_tags}")
  list(LENGTH git_tags git_tags_count)
  if(WXT_DAILY_BUILD)
    list(GET git_tags 0 GIT_LATEST_TAG)
  else()
    list(GET git_tags -1 GIT_LATEST_TAG)
  endif()
  if(${GIT_LATEST_TAG} STREQUAL "" OR ${GIT_LATEST_TAG} STREQUAL "continuous")
    # Such as 25.2.5
    string(TIMESTAMP current_year "%Y")
    string(TIMESTAMP current_month "%m")
    string(TIMESTAMP current_day "%d")
    math(EXPR current_month "${current_month} + 0")
    math(EXPR current_day "${current_day} + 0")
    set(target_version "v${current_year}.${current_month}.${current_day}")
    # string(TIMESTAMP target_version "%y.%m.%d")
    set(GIT_LATEST_TAG ${target_version})
  endif()
  message("Latest git tag: ${GIT_LATEST_TAG}")
  set(${prefix}_GIT_TAG
      "${GIT_LATEST_TAG}"
      CACHE STRING "Latest git tag" FORCE)
  set(GIT_TAG
      "${GIT_LATEST_TAG}"
      PARENT_SCOPE)
  string(SUBSTRING ${WXT_GIT_TAG} 1 -1 wxtools_version)
  add_compile_definitions(${prefix}_GIT_TAG="${wxtools_version}")
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
