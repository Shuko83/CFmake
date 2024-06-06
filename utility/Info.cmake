function(cstoolkit_generate_target_info)

    set(options QT_EXECUTABLE)
    set(one_value_keywords
        TARGET
        VERSION
        PRODUCT
        ORGANIZATION
        COPYRIGHT)
    set(multi_value_keywords)
    
    cmake_parse_arguments(PARAMS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    message(DEBUG "cstoolkit_generate_target_info: Valeur Target : " ${PARAMS_TARGET})

    set(INFO_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated/info)
    
    set(cmake_target_cpp_info ${INFO_DIRECTORY}/info_${PARAMS_TARGET}.cpp)
    set(cmake_target_h_info ${INFO_DIRECTORY}/info_${PARAMS_TARGET}.h)      

    set(PARAMS_INCLUDE info_${PARAMS_TARGET}.h)

    string(REPLACE "-" "_" PARAMS_NAMESPACE "${PARAMS_TARGET}")
    string(TOUPPER "${PARAMS_NAMESPACE}" PARAMS_INCGUARD)

    if(QT_EXECUTABLE)
        file(READ ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/info_target_qt.in _info_target_qt_content)
        string(CONFIGURE _info_target_qt_content PARAMS_QT_SECTION @ONLY)
        set(PARAMS_ABOUT_QT_VERSION "\"Qt version: ${QT_VERSION}\\n\"")
    endif()

    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/info_target.cpp.in ${cmake_target_cpp_info} @ONLY)
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/info_target.h.in ${cmake_target_h_info} @ONLY)

    target_sources(${PARAMS_TARGET} PRIVATE ${cmake_target_cpp_info} )
    target_sources(${PARAMS_TARGET}
        PRIVATE FILE_SET "info" TYPE HEADERS BASE_DIRS ${INFO_DIRECTORY} FILES ${cmake_target_h_info}
    )
    source_group(TREE ${INFO_DIRECTORY}/.. PREFIX "Generated Files" FILES ${cmake_target_cpp_info} ${cmake_target_h_info})

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
