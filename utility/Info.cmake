function(cstoolkit_generate_target_info)

    set(options QT EXECUTABLE)
    set(one_value_keywords
        TARGET
        VERSION
        PRODUCT
        ORGANIZATION
        COPYRIGHT)
    set(multi_value_keywords)
    
    cmake_parse_arguments(PARSE_ARGV 0 PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}")

    message(DEBUG "cstoolkit_generate_target_info: Valeur Target : " ${PARAMS_TARGET})

    set(INFO_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated/info)
    
    set(target_info_cpp_filename ${INFO_DIRECTORY}/target_info.cpp)
    set(target_info_h_filename ${INFO_DIRECTORY}/target_info.h)

    if (MSVC)
        set(PARAMS_COMPILER "Visual Studio ${CSTOOLKIT_COMPILER_VERSION}, ${CMAKE_CXX_COMPILER_ID} ${CMAKE_VS_PLATFORM_TOOLSET} (${CMAKE_CXX_COMPILER_VERSION})")
    else()
        set(PARAMS_COMPILER "${CSTOOLKIT_COMPILER_NAME} ${CMAKE_CXX_COMPILER_VERSION}")
    endif()

    if(PARAMS_QT)
        set(PARAMS_ABOUT_QT_VERSION "\"Qt version: ${QT_VERSION}\\n\"")
        if(PARAMS_EXECUTABLE)
            file(READ ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/target_info_qapp.cpp.in _target_info_qapp_content)
            string(CONFIGURE "${_target_info_qapp_content}" PARAMS_QAPP_SECTION @ONLY)
        endif()
    endif()

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/target_info.cpp.in ${target_info_cpp_filename} @ONLY)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/target_info.h.in ${target_info_h_filename} @ONLY)

    target_sources(${PARAMS_TARGET} PRIVATE ${target_info_cpp_filename} )
    target_sources(${PARAMS_TARGET}
        PRIVATE FILE_SET "target_info" TYPE HEADERS BASE_DIRS ${INFO_DIRECTORY} FILES ${target_info_h_filename}
    )
    source_group(TREE ${INFO_DIRECTORY}/.. PREFIX "Generated Files" FILES ${target_info_cpp_filename} ${target_info_h_filename})

    message(DEBUG "Target informations generation success !!")

    # verpatch for windows
    #isEmpty( RC_LANG ): RC_LANG = 0x0409                                                                                # set language to English(USA) if not defined

    #_verpatch_args =  $$quote( "/va $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}.$${VER_BLD}" )                                  # specifies the version for the project target
    #_verpatch_args += $$quote( "/langid $${RC_LANG}" )                                                                  # specifies the language for the project target
    #_verpatch_args += $$quote( "/pv $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}.$${VER_BLD}" )
    #_verpatch_args += $$quote( "/s CompanyName \"$${PROPERTIES.company}\"" )                                            # specifies the company for the project target
    #_verpatch_args += $$quote( "/s FileDescription \"$${PROPERTIES.description}\"" )                                    # specifies the description for the project target
    #_verpatch_args += $$quote( "/s LegalCopyright \"$$replace( PROPERTIES.copyright, $$re_escape( "(C)" ), "©" )\"" )   # specifies the copyright information for the project target
    #_verpatch_args += $$quote( "/s ProductName \"$${PROPERTIES.product}\"" )                                            # specifies the product for the project target

    #QMAKE_POST_LINK = $$quote( "$${QSMAKEPATH}/tools/verpatch.exe $${DESTDIR}/$${TARGET}$${TARGET_EXT} $${_verpatch_args}" ) $${QMAKE_POST_LINK}
    
endfunction()
