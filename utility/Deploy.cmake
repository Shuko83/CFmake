# cstoolkit_deploy_with_target(<target> FILES <file>... [DESTINATION <dir>])
# cstoolkit_deploy_with_target(<target> DIRECTORY <dir>... [DESTINATION <dir>])
# The last component of each directory name is appended to the destination directory
# but a trailing slash may be used to avoid this because it leaves the last component empty.
function(cstoolkit_deploy_with_target target mode)
    set(DEPLOY_OPTIONS)
    set(DEPLOY_UNIQUE DESTINATION)
    set(DEPLOY_MULTIPLE ${mode})
    cmake_parse_arguments(PARSE_ARGV 1 DEPLOY "${DEPLOY_OPTIONS}" "${DEPLOY_UNIQUE}" "${DEPLOY_MULTIPLE}")

    if(NOT TARGET ${target})
        message(SEND_ERROR "CSToolkit: cstoolkit_deploy_with_target(${target}): \"${target}\" is not a target")
        return()
    endif()

    if(NOT "${mode}" STREQUAL "FILES" AND NOT "${mode}" STREQUAL "DIRECTORY")
        message(SEND_ERROR "CSToolkit: cstoolkit_deploy_with_target(${target}): Invalid sub-command \"${mode}\"")
        return()
    endif()

    if(DEFINED DEPLOY_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "CSToolkit: cstoolkit_deploy_with_target(${target}): Unkown arguments \"${DEPLOY_UNPARSED_ARGUMENTS}\"")
        return()
    endif()

    set(INSTALL_BINDIR "$<TARGET_GENEX_EVAL:${target},$<TARGET_PROPERTY:${target},INSTALL_BINDIR>>")
    set(INSTALL_BINDIR "$<IF:$<BOOL:${INSTALL_BINDIR}>,${INSTALL_BINDIR},${CMAKE_INSTALL_BINDIR}>")

    get_target_property(INSTALL_COMPONENT ${target} INSTALL_COMPONENT)
    if(INSTALL_COMPONENT)
        set(INSTALL_COMPONENT COMPONENT ${INSTALL_COMPONENT})
    else()
        set(INSTALL_COMPONENT)
    endif()
    
    get_target_property(INSTALL_EXCLUDE_FROM_ALL ${target} INSTALL_EXCLUDE_FROM_ALL)
    if(NOT INSTALL_EXCLUDE_FROM_ALL)
        set(INSTALL_EXCLUDE_FROM_ALL)
    endif()

    set(DEPLOY_SOURCES_NORM "")
    cstoolkit_genex_extract("${DEPLOY_DIRECTORY}${DEPLOY_FILES}" DEPLOY_SOURCES DEPLOY_SOURCES_GENEX_LIST)
    foreach(elem ${DEPLOY_SOURCES_GENEX_LIST})
        list(APPEND DEPLOY_SOURCES_GENEX "${${elem}}")
    endforeach()

    foreach(_file IN LISTS DEPLOY_SOURCES)
        cmake_path(ABSOLUTE_PATH _file NORMALIZE OUTPUT_VARIABLE _file)
        list(APPEND DEPLOY_SOURCES_NORM "${_file}")
    endforeach()

    if(DEPLOY_SOURCES_GENEX)
        list(APPEND DEPLOY_SOURCES_NORM "${DEPLOY_SOURCES_GENEX}")
    endif()

    if(DEPLOY_FILES)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e
                "${DEPLOY_SOURCES_NORM}"
                "$<TARGET_FILE_DIR:${target}>/${DEPLOY_DESTINATION}"
                COMMAND_EXPAND_LISTS
        )
        install(FILES "${DEPLOY_SOURCES_NORM}" DESTINATION ${INSTALL_BINDIR}/${DEPLOY_DESTINATION} ${INSTALL_COMPONENT} ${INSTALL_EXCLUDE_FROM_ALL})
    else()
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CSTOOLKIT_COPY} -e -d
                "${DEPLOY_SOURCES_NORM}"
                "$<TARGET_FILE_DIR:${target}>/${DEPLOY_DESTINATION}"
                COMMAND_EXPAND_LISTS
        )
        install(DIRECTORY "${DEPLOY_SOURCES_NORM}" DESTINATION ${INSTALL_BINDIR}/${DEPLOY_DESTINATION} ${INSTALL_COMPONENT} ${INSTALL_EXCLUDE_FROM_ALL})
    endif()
endfunction()

function(cstoolkit_add_legacy_deploy FILE)
    message(DEBUG "-------------- Deploying file ${FILE} --------------")
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
        message(DEBUG "Unable to open : ${FILE}")
    else()
        get_target_property(target_binary_dir ${TARGET_NAME} RUNTIME_OUTPUT_DIRECTORY)

        file(READ ${FILE} jsonContent)
        string(JSON deployList ERROR_VARIABLE deployListError GET ${jsonContent} deploy)
          
        if(deployListError)
            message(DEBUG "Deploy Error : No parameter deploy")
        else()
            string(JSON deployListSize LENGTH ${deployList})
            math(EXPR deployListSize "${deployListSize}-1")

            foreach(index RANGE ${deployListSize})
                string(JSON deployElement GET ${deployList} ${index})
                string(JSON sourceDir ERROR_VARIABLE sourceDirError GET ${deployElement} sourceDir)
                string(JSON destDir ERROR_VARIABLE destDirError GET ${deployElement} destDir)
                string(JSON files ERROR_VARIABLE filesError GET ${deployElement} files)
                set(DESTINATION "${target_binary_dir}/${destDir}")

                string(JSON filesListSize LENGTH ${files})
                math(EXPR filesListSize "${filesListSize}-1")

                unset(filesFind)
                foreach(fileIndex RANGE ${filesListSize})

                    string(JSON file GET ${files} ${fileIndex})
                    string(FIND ${file} "*." IS_EXTENSION)
                    
                    if(${file} STREQUAL "*")
                        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CSTOOLKIT_COPY} -d 
                        "${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}" ${DESTINATION} COMMAND_EXPAND_LISTS)

                    else()
                        if(NOT IS_EXTENSION EQUAL -1)
                            file(GLOB_RECURSE extansionFiles ${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}/${file})
                            list(APPEND filesFind ${extansionFiles})
                        else()
                            list(APPEND filesFind "${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}/${file}")
                        endif()

                        list(LENGTH filesFind sizeFiles)
                        math(EXPR sizeFiles "${sizeFiles}-1")
                        foreach(indexSplit RANGE 0 ${sizeFiles} 300)
                            list(SUBLIST filesFind ${indexSplit} 300 splittedList)
                            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                                COMMAND ${CSTOOLKIT_COPY} ${splittedList} ${DESTINATION} COMMAND_EXPAND_LISTS
                            )
                        endforeach()  
                    endif()
                endforeach()  
            endforeach()            
        endif()
    endif() 
endfunction()