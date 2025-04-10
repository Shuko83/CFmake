function(cstoolkit_parse_repetitive_arguments prefix repetitive_keyword ignore_keywords)
    if(repetitive_keyword IN_LIST ignore_keywords)
        message(SEND_ERROR "CSToolkit: cstoolkit_parse_repetitive_arguments(): <ignore_keywords> contains <repetitive_keyword>")
        return()
    endif()
    
    set(${prefix}_${repetitive_keyword}C 0)
    set(${prefix}_${repetitive_keyword}V)

    set(insideValues FALSE)
    set(ARGI 0)
    set(currentArgName)
    
    # now iterate over all arguments and fill the result variables
    foreach(currentArg ${ARGN})
        if(currentArg STREQUAL repetitive_keyword)
            set(currentArgName ${prefix}_${repetitive_keyword}V${ARGI})
            set(${currentArgName})
            list(APPEND ${prefix}_${repetitive_keyword}V ${currentArgName})
            list(APPEND ${currentArgName} ${currentArg})
            set(insideValues TRUE)
            math(EXPR ARGI "${ARGI}+1")
            set(${prefix}_${repetitive_keyword}C ${ARGI})
        elseif(currentArg IN_LIST ignore_keywords)
            set(insideValues FALSE)
        elseif(insideValues)
            list(APPEND ${currentArgName} ${currentArg})
        endif()

    endforeach()

    # propagate the result variables to the caller:
    foreach(arg_name ${${prefix}_${repetitive_keyword}V})
        set(${arg_name} ${${arg_name}} PARENT_SCOPE)
    endforeach()

    set(${prefix}_${repetitive_keyword}C ${${prefix}_${repetitive_keyword}C} PARENT_SCOPE)
    set(${prefix}_${repetitive_keyword}V ${${prefix}_${repetitive_keyword}V} PARENT_SCOPE)

endfunction()
