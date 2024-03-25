function(deploy FILE)
    message(STATUS "-------------- Deploying file ${FILE} --------------")
    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
        message("Unable to open : ${FILE}")
    else()
        get_target_property(target_binary_dir ${TARGET_NAME} RUNTIME_OUTPUT_DIRECTORY)

        file(READ ${FILE} jsonContent)
        string(JSON deployList ERROR_VARIABLE deployListError GET ${jsonContent} deploy)
          
        if (deployListError)
            message("Deploy Error : No parameter deploy")
        else()
            string(JSON deployListSize LENGTH ${deployList})
            math(EXPR deployListSize "${deployListSize}-1")

            foreach(index RANGE ${deployListSize})
                string(JSON deployElement GET ${deployList} ${index})
                string(JSON sourceDir ERROR_VARIABLE sourceDirError GET ${deployElement} sourceDir)
                string(JSON destDir ERROR_VARIABLE destDirError GET ${deployElement} destDir)
                string(JSON files ERROR_VARIABLE filesError GET ${deployElement} files)
                set(DESTINATION "${target_binary_dir}/$<CONFIG>/${destDir}")

                string(JSON filesListSize LENGTH ${files})
                math(EXPR filesListSize "${filesListSize}-1")

                unset(filesFind)
                foreach(fileIndex RANGE ${filesListSize})

                    string(JSON file GET ${files} ${fileIndex})
                    string(REGEX MATCH "\\*\\.(.*)" IS_REGEX ${file})
                    
                    if(${file} STREQUAL "*")
                        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND}
                        -DDIRECTORY_LIST="${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}"
                        -DDESTINATION="${DESTINATION}"
                        -P C:/CSToolkit/Copy.cmake COMMAND_EXPAND_LISTS)

                    else()
                        if(IS_REGEX)
                            message("file(GLOB_RECURSE extansionFiles ${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}/*.${CMAKE_MATCH_1})")
                            file(GLOB_RECURSE extansionFiles ${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}/*.${CMAKE_MATCH_1})
                            message("File research ${extansionFiles}")
                            list(APPEND filesFind ${extansionFiles})
                        else()
                            message("No extansion mode")
                            list(APPEND filesFind "${CMAKE_CURRENT_SOURCE_DIR}/${sourceDir}/${file}")
                        endif()

                        list(LENGTH filesFind sizeFiles)
                        math(EXPR sizeFiles "${sizeFiles}-1")
                        foreach(indexSplit RANGE 0 ${sizeFiles} 300)
                            list(SUBLIST filesFind ${indexSplit} 300 splittedList)
                            add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                                COMMAND ${CMAKE_COMMAND}
                                -DFILE_LIST="${splittedList}"
                                -DDESTINATION=${DESTINATION}
                                -P C:/CSToolkit/Copy.cmake COMMAND_EXPAND_LISTS
                            )
                        endforeach()  
                    endif()
                endforeach()  
            endforeach()            
        endif()
    endif() 
endfunction()