# Regex utility functions for CMake

#[[
    cstoolkit_regex_make_case_insensitive

    Converts a string into a case-insensitive regex pattern by replacing each letter
    with a character class containing both lowercase and uppercase variants.

    Example:
        cstoolkit_regex_make_case_insensitive("MyLib" result)
        # result will be: [Mm][Yy][Ll][Ii][Bb]

    Arguments:
        INPUT_STRING    - The input string to convert
        OUTPUT_VAR      - The name of the variable to store the result

    Usage:
        cstoolkit_regex_make_case_insensitive("MyTarget" pattern)
        string(REGEX REPLACE "${pattern}" "replacement" result "${input}")
]]
function(cstoolkit_regex_make_case_insensitive INPUT_STRING OUTPUT_VAR)
    # Convert to lowercase first, then only need 26 replacements instead of 52 - fastest approach
    string(TOLOWER "${INPUT_STRING}" _p)
    string(REGEX REPLACE "a" "[Aa]" _p "${_p}")
    string(REGEX REPLACE "b" "[Bb]" _p "${_p}")
    string(REGEX REPLACE "c" "[Cc]" _p "${_p}")
    string(REGEX REPLACE "d" "[Dd]" _p "${_p}")
    string(REGEX REPLACE "e" "[Ee]" _p "${_p}")
    string(REGEX REPLACE "f" "[Ff]" _p "${_p}")
    string(REGEX REPLACE "g" "[Gg]" _p "${_p}")
    string(REGEX REPLACE "h" "[Hh]" _p "${_p}")
    string(REGEX REPLACE "i" "[Ii]" _p "${_p}")
    string(REGEX REPLACE "j" "[Jj]" _p "${_p}")
    string(REGEX REPLACE "k" "[Kk]" _p "${_p}")
    string(REGEX REPLACE "l" "[Ll]" _p "${_p}")
    string(REGEX REPLACE "m" "[Mm]" _p "${_p}")
    string(REGEX REPLACE "n" "[Nn]" _p "${_p}")
    string(REGEX REPLACE "o" "[Oo]" _p "${_p}")
    string(REGEX REPLACE "p" "[Pp]" _p "${_p}")
    string(REGEX REPLACE "q" "[Qq]" _p "${_p}")
    string(REGEX REPLACE "r" "[Rr]" _p "${_p}")
    string(REGEX REPLACE "s" "[Ss]" _p "${_p}")
    string(REGEX REPLACE "t" "[Tt]" _p "${_p}")
    string(REGEX REPLACE "u" "[Uu]" _p "${_p}")
    string(REGEX REPLACE "v" "[Vv]" _p "${_p}")
    string(REGEX REPLACE "w" "[Ww]" _p "${_p}")
    string(REGEX REPLACE "x" "[Xx]" _p "${_p}")
    string(REGEX REPLACE "y" "[Yy]" _p "${_p}")
    string(REGEX REPLACE "z" "[Zz]" _p "${_p}")
    set(${OUTPUT_VAR} "${_p}" PARENT_SCOPE)
endfunction()
