# FILE_LIST : List of the files to copy
# DIRECTORY_LIST : List of the directories to copy
# DESTINATION : Directory where files will be copy, if the directory doesn't exist it creates it
# STOP_ON_ERROR : Copy stop if file / directory specified doesn't exist
# DIRECTORY_MODE : Allow to give list of directories instead of files for the copy 

# Parsing arguments to retieve options
set(index 0)
math(EXPR argumentsListSize "${CMAKE_ARGC}-1")
while(NOT "${CMAKE_ARGV${index}}" STREQUAL "--" AND ${index} LESS ${argumentsListSize})
    math(EXPR index "${index}+1")
endwhile()

math(EXPR index "${index}+1")

# Settings variable depending on options
while(${index} LESS ${argumentsListSize})
    string(REGEX MATCH "^(-.)$" IS_OPTION "${CMAKE_ARGV${index}}")    
    if (IS_OPTION)
        if (${CMAKE_MATCH_1} STREQUAL "-d")
            set(DIRECTORY_MODE true)
        elseif(${CMAKE_MATCH_1} STREQUAL "-e") 
            set(STOP_ON_ERROR true)
        else()
            break()
        endif()
    else()
        break()   
    endif()

    math(EXPR index "${index}+1")
endwhile()

# Parsing files or directories
math(EXPR fileListSize "${CMAKE_ARGC}-2")
foreach(indexFile RANGE ${index} ${fileListSize})
    if (DIRECTORY_MODE)
        list(APPEND DIRECTORY_LIST "${CMAKE_ARGV${indexFile}}")
    else()
        list(APPEND FILE_LIST "${CMAKE_ARGV${indexFile}}")
    endif()
endforeach()

# Checking if last argument is a directory
if (IS_DIRECTORY "${CMAKE_ARGV${argumentsListSize}}")
    set(DESTINATION "${CMAKE_ARGV${argumentsListSize}}")
endif()

# Checking if file/directories are specified
if (NOT FILE_LIST AND NOT DIRECTORY_LIST)
    message(DEBUG "No file or directory specified to copy")
    return()
endif()

# Checking if destination is specified
if (NOT DESTINATION)
    message(DEBUG "No destination specified")
    return()
endif()

# Creating destination if it doesn't exist
if (NOT EXISTS ${DESTINATION})
    message(DEBUG "Destination directory is not existing.\nCreating ${DESTINATION}")
    file(MAKE_DIRECTORY ${DESTINATION})
endif()

# Checking if file exist, if error mode enabled script stops
if (FILE_LIST)
    message(DEBUG "========= Mode File =========")
    foreach(file ${FILE_LIST})
        if (EXISTS ${file} AND NOT IS_DIRECTORY ${file})
            message(DEBUG "Adding files : ${file} for copy")
            list(APPEND EXISTING_FILES ${file})       
        elseif(STOP_ON_ERROR)
            message(DEBUG "Error copying ${file}, file doesn't exist")
            return()
        endif()
    endforeach()

    # Copying files
    if (EXISTING_FILES)
        message(DEBUG "\nCopying files to ${DESTINATION}\n")
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXISTING_FILES} ${DESTINATION})
    endif()
endif()

# Checking if directory exist, if error mode enabled script stops
if(DIRECTORY_LIST)
    message(DEBUG "========= Mode Directory =========")
    foreach(directory ${DIRECTORY_LIST})
        if (IS_DIRECTORY ${directory})        
            list(APPEND EXISTING_DIRECTORIES ${directory})
        elseif(STOP_ON_ERROR)
            message(DEBUG "Directory mode specified, but ${directory} is not a directory")
            return()
        endif()
    endforeach()

    # Copying directories
    if (EXISTING_DIRECTORIES)
        message(DEBUG "\nCopying directories to ${DESTINATION}\n")
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${EXISTING_DIRECTORIES} ${DESTINATION})
    endif()
endif()
