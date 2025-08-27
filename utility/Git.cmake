find_package(Git QUIET)

if(GIT_EXECUTABLE)
    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --all --dirty --broken --long --always
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_COMMIT
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_SHA
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" describe --exact-match --tags
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_TAG
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_BRANCH
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" log -n 1 --pretty=%cD
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_DATE
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" remote get-url origin
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_URL
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --show-toplevel
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    OUTPUT_VARIABLE CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(CSTOOLKIT_GIT_BRANCH STREQUAL "HEAD")
        set(CSTOOLKIT_GIT_BRANCH "")
    endif()

    if(CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL)
        cmake_path(RELATIVE_PATH CMAKE_SOURCE_DIR BASE_DIRECTORY "${CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL}" OUTPUT_VARIABLE CSTOOLKIT_GIT_RELATIVE_TOP_LEVEL)
    endif()
endif()

function(cstoolkit_git_safe_delete git_src_dir)
    # Check if directory exists and is not empty
    if(EXISTS "${git_src_dir}")
        if(NOT IS_DIRECTORY "${git_src_dir}")
            message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), is not a directory")
            return()
        endif()

        file(GLOB DIR_CONTENTS "${git_src_dir}/*")
        list(LENGTH DIR_CONTENTS DIR_CONTENT_COUNT)

        if(DIR_CONTENT_COUNT EQUAL 0)
            # Directory exists but is empty
            file(REMOVE_RECURSE "${git_src_dir}")
        endif()
    else()
        # Already deleted
        return()
    endif()

    # Check if we are in a git repository
    execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --show-toplevel
        WORKING_DIRECTORY "${git_src_dir}"
        OUTPUT_VARIABLE _git_safe_delete_toplevel
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _git_safe_delete_toplevel STREQUAL git_src_dir)
        message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), not a git repository")
        return()
    endif()

    # Check any untracked/uncommitted changes
    execute_process(COMMAND "${GIT_EXECUTABLE}" status --porcelain=v1
        WORKING_DIRECTORY "${git_src_dir}"
        OUTPUT_VARIABLE _git_safe_delete_status
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _git_safe_delete_status STREQUAL "")
        message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), uncommitted changes")
        return()
    endif()

    # Check detached HEAD
    execute_process(COMMAND "${GIT_EXECUTABLE}" symbolic-ref -q HEAD
        WORKING_DIRECTORY "${git_src_dir}"
        RESULT_VARIABLE _git_safe_delete_detached
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(NOT _git_safe_delete_detached EQUAL 0)
        # Current commit is detached HEAD
        # git  branch -r --contains HEAD will not work on shallow clone
        # Shallow clones are blocked by a pre commit hook
        execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --is-shallow-repository
            WORKING_DIRECTORY "${git_src_dir}"
            OUTPUT_VARIABLE _git_safe_delete_shallow
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(_git_safe_delete_shallow STREQUAL "false") # not a shallow repository
            execute_process(COMMAND "${GIT_EXECUTABLE}" branch -r --contains HEAD
                WORKING_DIRECTORY "${git_src_dir}"
                OUTPUT_VARIABLE _git_safe_delete_contains
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(_git_safe_delete_contains STREQUAL "")
                message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), detached HEAD commit is not pushed")
                return()
            endif()
        endif()
    endif()

    # Check all branches
    execute_process(COMMAND "${GIT_EXECUTABLE}" for-each-ref "--format=%(refname:short)" refs/heads
        WORKING_DIRECTORY "${git_src_dir}"
        OUTPUT_VARIABLE _git_safe_delete_branches
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REPLACE "\r\n" "\n" _git_safe_delete_branches "${_git_safe_delete_branches}")
    string(REPLACE "\n" ";" _git_safe_delete_branches "${_git_safe_delete_branches}")

    foreach(branch ${_git_safe_delete_branches})
        # Get the symbolic name of the upstream branch (e.g., origin/main)
        execute_process(COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref "${branch}@{u}"
            WORKING_DIRECTORY "${git_src_dir}"
            OUTPUT_VARIABLE upstream_symbolic_name
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(upstream_symbolic_name STREQUAL "")
            message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), branch '${branch}' is untracked")
            return()
        endif()

        execute_process(COMMAND "${GIT_EXECUTABLE}" config branch.${branch}.remote
            WORKING_DIRECTORY "${git_src_dir}"
            OUTPUT_VARIABLE remote_name
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        execute_process(COMMAND "${GIT_EXECUTABLE}" config branch.${branch}.merge
            WORKING_DIRECTORY "${git_src_dir}"
            OUTPUT_VARIABLE remote_branch_name
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        string(REGEX REPLACE "^refs/heads/" "" remote_branch_name "${remote_branch_name}")

        execute_process(COMMAND "${GIT_EXECUTABLE}" show-ref --verify --quiet "refs/remotes/${remote_name}/${remote_branch_name}"
            WORKING_DIRECTORY "${git_src_dir}"
            RESULT_VARIABLE _git_safe_delete_gone
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if(NOT _git_safe_delete_gone EQUAL 0)
            message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), branch '${branch}' tracks a gone remote branch")
            return()
        endif()

        execute_process(COMMAND "${GIT_EXECUTABLE}" rev-list --count "refs/remotes/${remote_name}/${remote_branch_name}..${branch}"
            WORKING_DIRECTORY "${git_src_dir}"
            OUTPUT_VARIABLE _git_safe_delete_unpushed
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(_git_safe_delete_unpushed GREATER 0)
            message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), branch '${branch}' has unpushed commit(s)")
            return()
        endif()
    endforeach()

    # Check stashed changes
    execute_process(COMMAND "${GIT_EXECUTABLE}" stash list
        WORKING_DIRECTORY "${git_src_dir}"
        OUTPUT_VARIABLE _git_safe_delete_stash
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT _git_safe_delete_stash STREQUAL "")
        message(FATAL_ERROR "CSToolkit: Delete git repository failed (${git_src_dir}), stashed changes found")
        return()
    endif()

    file(REMOVE_RECURSE "${git_src_dir}")
endfunction()
