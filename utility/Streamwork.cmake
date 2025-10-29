function(cstoolkit_streamwork_generate_devpaths target)
    if(NOT TARGET ${target})
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths(${target}): \"${target}\" is not a target")
        return()
    endif()

    get_target_property(target_type ${target} TYPE)

    if(NOT target_type STREQUAL EXECUTABLE)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths(${target}): \"${target}\" is not an executable target")
        return()
    endif()

    # Parse arguments
    set(DEVPATHS_OPTIONS)
    set(DEVPATHS_UNIQUE OUTPUT)
    set(DEVPATHS_MULTIPLE ADDITIONAL_DIRS)
    cmake_parse_arguments(PARSE_ARGV 1 DEVPATHS "${DEVPATHS_OPTIONS}" "${DEVPATHS_UNIQUE}" "${DEVPATHS_MULTIPLE}")

    if(NOT DEVPATHS_OUTPUT)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths(${target}): Missing mandatory parameter OUTPUT")
        return()
    endif()

    set(DOLLAR "$<1:$>")
    set(REGEX_VALID_TARGET_NAME "^[a-zA-Z0-9:_.\\-]+${DOLLAR}")

    set(GENEX_TARGET_EXISTS "${DOLLAR}<TARGET_EXISTS:\\0$<ANGLE-R>")
    set(GENEX_TARGET_SHARED "${DOLLAR}<STREQUAL:${DOLLAR}<TARGET_PROPERTY:\\0$<COMMA>TYPE$<ANGLE-R>$<COMMA>SHARED_LIBRARY$<ANGLE-R>")
    set(GENEX_TARGET_MODULE "${DOLLAR}<STREQUAL:${DOLLAR}<TARGET_PROPERTY:\\0$<COMMA>TYPE$<ANGLE-R>$<COMMA>MODULE_LIBRARY$<ANGLE-R>")
    set(GENEX_TARGET_PLUGIN "${DOLLAR}<OR:${GENEX_TARGET_SHARED}$<COMMA>${GENEX_TARGET_MODULE}$<ANGLE-R>")

    set(DEVPATHS_DIRS "$<TARGET_PROPERTY:${target},RUNTIME_DEPENDENCIES>;$<TARGET_PROPERTY:${target},PLUGINS_DEPENDENCIES>")
    set(DEVPATHS_DIRS "$<TARGET_GENEX_EVAL:${target},${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<LIST:REMOVE_DUPLICATES,${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<LIST:FILTER,${DEVPATHS_DIRS},INCLUDE,${REGEX_VALID_TARGET_NAME}>")
    set(DEVPATHS_DIRS "$<LIST:TRANSFORM,${DEVPATHS_DIRS},REPLACE,(.+),${DOLLAR}<${GENEX_TARGET_EXISTS}:${DOLLAR}<${GENEX_TARGET_PLUGIN}:${DOLLAR}<TARGET_FILE_DIR:\\0$<ANGLE-R>$<ANGLE-R>$<ANGLE-R>>")
    set(DEVPATHS_DIRS "$<TARGET_GENEX_EVAL:${target},${DEVPATHS_ADDITIONAL_DIRS};${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<LIST:REMOVE_DUPLICATES,${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<JOIN:${DEVPATHS_DIRS},;>") # remove empty elements

    if(Qt5_INSTALL_PREFIX) #Filtering of Qt's dlls, necessary for development
        set(DEVPATHS_DIRS "$<FILTER:${DEVPATHS_DIRS},EXCLUDE,^${Qt5_INSTALL_PREFIX}>")
    endif()

    set(DEVPATHS_DIRS "$<LIST:TRANSFORM,${DEVPATHS_DIRS},PREPEND,<Path target=\">")
    set(DEVPATHS_DIRS "$<LIST:TRANSFORM,${DEVPATHS_DIRS},APPEND,\" use=\"$<LOWER_CASE:$<CONFIG>>\"/$<ANGLE-R>>")
    set(DEVPATHS_DIRS "$<LIST:JOIN,${DEVPATHS_DIRS},
    >")

    file(GENERATE OUTPUT ${DEVPATHS_OUTPUT}
        CONTENT "<Paths>
	${DEVPATHS_DIRS}
</Paths>
")
endfunction()

function(cstoolkit_streamwork_generate_launchers target)
    if(NOT TARGET ${target})
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_launchers(${target}): \"${target}\" is not a target")
        return()
    endif()

    # Parse arguments
    set(DEVPATHS_OPTIONS)
    set(DEVPATHS_UNIQUE DEVPATH APP_DIR_PATH MODELS MODELS_DIR_PATH STREAM)
    set(DEVPATHS_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 1 option "${DEVPATHS_OPTIONS}" "${DEVPATHS_UNIQUE}" "${DEVPATHS_MULTIPLE}")
  
    if(NOT option_APP_DIR_PATH)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_launchers(${target}): Missing mandatory parameter APP_DIR_PATH")
        return()
    endif()

    if(NOT option_DEVPATH)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_launchers(${target}): Missing mandatory parameter DEVPATH")
        return()
    endif()

    if(NOT option_STREAM)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_launchers(${target}): Missing mandatory parameter STREAM")
        return()
    endif()

    set(SW_COMMAND "$<TARGET_FILE:Streamwork::StreamWorkEditor>")
    set(SW_ARGUMENTS_COMMAND "-autostart -appDirPath ${option_APP_DIR_PATH} -pdesc ${option_DEVPATH}")
    
    if(option_MODELS)
        string(APPEND SW_ARGUMENTS_COMMAND " -models ${option_MODELS}")
    endif()

    if(option_MODELS_DIR_PATH)
        string(APPEND SW_ARGUMENTS_COMMAND " -modelsDirPath ${option_MODELS_DIR_PATH}")
    endif()

    file(GENERATE OUTPUT StreamworkEditorLauncher$<CONFIG>.bat
        CONTENT "${SW_COMMAND} ${SW_ARGUMENTS_COMMAND}")

    string(APPEND SW_ARGUMENTS_COMMAND " -stream ${option_STREAM}")
    file(GENERATE OUTPUT ${target}Launcher$<CONFIG>.bat
        CONTENT "${SW_COMMAND} ${SW_ARGUMENTS_COMMAND}")  

    if(MSVC)
        set_target_properties(${target} PROPERTIES VS_DEBUGGER_COMMAND ${SW_COMMAND})
        set_target_properties(${target} PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "${SW_ARGUMENTS_COMMAND}")
    endif()
endfunction()
