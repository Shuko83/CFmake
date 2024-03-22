# FILE_LIST : List of the files to copy
# DESTINATION : Directory where files will be copy, if the directory doesn't exist it creates it

message(STATUS "========================= Script CopyIfExist.cmake =========================")

if (NOT EXISTS ${DESTINATION})
    message("Destination directory is not existing.\nCreating ${DESTINATION}")
    file(MAKE_DIRECTORY ${DESTINATION})
endif()

set(LIST_SEPARATED ${FILE_LIST})
separate_arguments(LIST_SEPARATED)

foreach(file ${LIST_SEPARATED})
    if (EXISTS ${file})
        message("Adding files : ${file} for copy")
        list(APPEND EXISTING_FILES ${file})       
    else()
        message("Unable to copy ${file}, the file doesn't exist")
    endif()
endforeach()
message("")
message(STATUS "Copying files to ${DESTINATION}\n")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXISTING_FILES} ${DESTINATION})