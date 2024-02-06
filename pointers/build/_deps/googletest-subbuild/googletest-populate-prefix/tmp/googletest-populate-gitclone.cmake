
if(NOT "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitinfo.txt" IS_NEWER_THAN "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout "https://github.com/google/googletest.git" "googletest-src"
    WORKING_DIRECTORY "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/google/googletest.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout f8d7d77c06936315286eb55f8de22cd23c188571 --
  WORKING_DIRECTORY "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'f8d7d77c06936315286eb55f8de22cd23c188571'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitinfo.txt"
    "/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/juliengauthier410/l3/s6/programmation_Multi-Paradigme/projet 1/pointers/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/googletest-populate-gitclone-lastrun.txt'")
endif()
