function(cstoolkit_streamwork_generate_devpaths target)
    if(NOT TARGET ${target})
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths: \"${target}\" is not a target.")
        return()
    endif()

    get_target_property(target_type ${target} TYPE)

    if(NOT target_type STREQUAL EXECUTABLE)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths: \"${target}\" is not an executable target.")
        return()
    endif()

    # Parse arguments
    set(DEVPATHS_OPTIONS)
    set(DEVPATHS_UNIQUE OUTPUT)
    set(DEVPATHS_MULTIPLE)
    cmake_parse_arguments(PARSE_ARGV 1 DEVPATHS "${DEVPATHS_OPTIONS}" "${DEVPATHS_UNIQUE}" "${DEVPATHS_MULTIPLE}")

    if(NOT DEVPATHS_OUTPUT)
        message(SEND_ERROR "CSToolkit: cstoolkit_streamwork_generate_devpaths: Missing OUTPUT argument.")
        return()
    endif()

    set(DEVPATHS_DIRS "$<TARGET_PROPERTY:${target},RUNTIME_DEPENDENCIES>;$<TARGET_PROPERTY:${target},PLUGINS_DEPENDENCIES>")
    set(DEVPATHS_DIRS "$<TARGET_GENEX_EVAL:${target},${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<LIST:REMOVE_DUPLICATES,${DEVPATHS_DIRS}>")
    set(DEVPATHS_DIRS "$<JOIN:${DEVPATHS_DIRS},;>") # remove empty elements
    set(DEVPATHS_DIRS "$<LIST:TRANSFORM,${DEVPATHS_DIRS},REPLACE,(.+),$<1:$><$<1:$><TARGET_EXISTS:\\0$<ANGLE-R>:$<1:$><TARGET_FILE_DIR:\\0$<ANGLE-R>$<ANGLE-R>>")
    set(DEVPATHS_DIRS "$<TARGET_GENEX_EVAL:${target},${DEVPATHS_DIRS}>")
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
