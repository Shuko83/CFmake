set_property(GLOBAL PROPERTY CSTOOLKIT_IDLPP_TARGET_DEPENDENCIES "CSTOOLKIT")

function(cstoolkit_idlpp_generate)
    set(_options APPEND_PATH)
    set(_singleargs TARGET LANGUAGE MODE)
    set(_multiargs IDLS)
    cmake_parse_arguments(PARSE_ARGV 0 idlpp_generate "${_options}" "${_singleargs}" "${_multiargs}")

    if(NOT idlpp_generate_TARGET)
        message(SEND_ERROR "Error: idlpp_generate called without any target")
        return()
    endif()

    if(NOT idlpp_generate_LANGUAGE)
        message(SEND_ERROR "Error: idlpp_generate called without any language")
        return()
    endif()
    string(TOLOWER ${idlpp_generate_LANGUAGE} idlpp_generate_LANGUAGE)

    if(NOT idlpp_generate_IDLS)
        message(SEND_ERROR "Error: idlpp_generate could not find any .idl files")
        return()
    endif()

    find_program(OPENSPLICE_IDLPP idlpp "${OSPL_BIN}")
    if(NOT OPENSPLICE_IDLPP)
        message(FATAL_ERROR "-- Failed to find idlpp code generator")
    endif()

    if(WIN32)
        set(OpenSplice_IDLGEN_BINARY ${CMAKE_COMMAND} -E env ADLINK_LICENSE=${CSTOOLKIT_ADLINK_LICENSE} ${OPENSPLICE_IDLPP})
    else()
        set(OpenSplice_IDLGEN_BINARY ${CMAKE_COMMAND} -E env ADLINK_LICENSE=${CSTOOLKIT_ADLINK_LICENSE} LD_LIBRARY_PATH=${OSPL_LIB} ${OPENSPLICE_IDLPP})
    endif()

    string(TOLOWER "${idlpp_generate_MODE}" idlpp_generate_MODE)
    set(idlpp_mode)
    if("${idlpp_generate_MODE}" STREQUAL "face")
        set(idlpp_mode "-F")
    elseif("${idlpp_generate_MODE}" STREQUAL "streams")
        set(streams "1")
        set(mode "cpp")
    elseif("${idlpp_generate_MODE}" STREQUAL "sacpp")
        set(idlpp_mode "-P OS_API,cpp_dcps_if.h")
    endif()

    target_sources(${idlpp_generate_TARGET} PRIVATE ${idlpp_generate_IDLS})
    source_group("Idls Files" FILES ${idlpp_generate_IDLS})

    set(_out_dir "${CMAKE_CURRENT_BINARY_DIR}/generated/idl")
    set(MODEL_SOURCES)
    set(TYPE_SOURCES)
    set(MODEL_HEADERS)

    foreach(_idl IN LISTS idlpp_generate_IDLS)
        get_filename_component(_idlfile ${_idl} ABSOLUTE)
        get_filename_component(_idlname ${_idlfile} NAME_WE)
        cmake_path(RELATIVE_PATH _idlfile BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE _relpath)

        cstoolkit_define_OpenSplice_SOURCES("${_out_dir}" "${idlpp_generate_LANGUAGE}" "${idlpp_generate_MODE}" "${_idlname}")

        add_custom_command(OUTPUT ${_model_sources}
            COMMAND ${CSTOOLKIT_LOCK} ${CMAKE_BINARY_DIR}/idlpp.lock ${OpenSplice_IDLGEN_BINARY}
            -I "$<JOIN:${IDL_PATH};$<TARGET_PROPERTY:${idlpp_generate_TARGET},INCLUDE_DIRECTORIES>,;-I;>" -S -l ${idlpp_generate_LANGUAGE} ${idlpp_mode} -d ${_out_dir} ${_idlfile}
            COMMENT "IDLPP ${_relpath}"
            MAIN_DEPENDENCY ${_idlfile}
            COMMAND_EXPAND_LISTS
            USES_TERMINAL # To avoid parrallel execution of idlpp
        )
        if(streams)
        endif()
        target_sources(${idlpp_generate_TARGET} PRIVATE ${_model_sources})
        source_group(TREE ${CMAKE_CURRENT_BINARY_DIR}/generated PREFIX "Generated Files" FILES ${_model_sources})
    endforeach()

    set_property(TARGET ${idlpp_generate_TARGET} APPEND PROPERTY COMPILE_OPTIONS $<$<CXX_COMPILER_ID:MSVC>:/W1> $<$<CXX_COMPILER_ID:GNU>:-w>)
    get_target_property(_target_type ${idlpp_generate_TARGET} TYPE)
    if(_target_type STREQUAL "STATIC_LIBRARY")
        set_property(TARGET ${idlpp_generate_TARGET} APPEND PROPERTY STATIC_LIBRARY_OPTIONS $<$<CXX_COMPILER_ID:MSVC>:/ignore:4221>)
    endif()
    target_include_directories(${idlpp_generate_TARGET} PUBLIC $<BUILD_INTERFACE:${_out_dir}>)

    # Creating serialization between targets to avoid parrallel execution of idlpp
    get_property(IDLPP_TARGET_DEPENDENCIES GLOBAL PROPERTY CSTOOLKIT_IDLPP_TARGET_DEPENDENCIES)
    add_dependencies(${idlpp_generate_TARGET} "${IDLPP_TARGET_DEPENDENCIES}")
    set_property(GLOBAL PROPERTY CSTOOLKIT_IDLPP_TARGET_DEPENDENCIES "${idlpp_generate_TARGET}")
endfunction()

macro(cstoolkit_define_OpenSplice_SOURCES dir lang mode idlfilename)
    set(_model_sources)
    set(_model_headers)

    if("${lang}" STREQUAL "c")
        #message(" lang matches \"c\"")
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SacDcps.c)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.c)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SacDcps.h)
    elseif("${lang}" STREQUAL "cpp")
        #message(" lang matches \"cpp\"")
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}.cpp)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps.cpp)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}Dcps.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps_impl.cpp)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}Dcps_impl.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.cpp)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}SplDcps.h)
        set(_model_headers ${_model_headers} ${dir}/ccpp_${idlfilename}.h)
        set(_model_sources ${_model_sources} ${_model_headers})
        if("${mode}" STREQUAL "streams")
            #message(" mode matches \"streams\"")
            set(_model_sources ${_model_sources} ${dir}/${idlfilename}_impl.cpp)
            set(_model_sources ${_model_sources} ${dir}/${idlfilename}_impl.h)
        endif()
    elseif("${lang}" STREQUAL "c99")
        #message(" lang matches \"c99\"")
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps.c)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SacDcps.c)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SacDcps.h)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.c)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.h)
    elseif("${lang}" STREQUAL "isocpp2")
        #message(" lang matches \"isocpp2\"")
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}.h)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}SplDcps.h)
        set(_model_headers ${_model_headers} ${dir}/${idlfilename}_DCPS.hpp)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.cpp)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}.cpp)
        set(_model_sources ${_model_sources} ${_model_headers})
        if("${mode}" STREQUAL "face")
            #message(" mode matches \"face\"")
            set(_model_sources ${_model_sources} ${dir}/${idlfilename}_FACE.hpp)
            set(_model_sources ${_model_sources} ${dir}/${idlfilename}_FACE.cpp)
        endif()
    elseif("${lang}" STREQUAL "csharp")
        #message(" lang matches \"csharp\"")
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}.cs)
        set(_model_sources ${_model_sources} ${dir}/I${idlfilename}Dcps.cs)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}Dcps.cs)
        set(_model_sources ${_model_sources} ${dir}/${idlfilename}SplDcps.cs)
    endif()
endmacro()
####################GENERATE REQUIRED FILES ##########################################

#################### DEPLOY SHARED LIBRARY ###########################################
function(deploy_opensplice)
    set(options)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs)
    cmake_parse_arguments(DEPLOY_ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    ### list of librairies ###
    # ddskernel: DDS kernel (used by all libraries)
    # dcpsisocpp2: ISO C++ 2 API
    # dcpssacs: C# API
    # ddsos and ddsosnet: ?
    # ddshts: ?
    # spliced: OpenSplice Domain service
    # ddsi2: DDSI2 networking service
    # dds_security_core: needed by ddsi2
    # durability: Durability service
    # sqlite3-ospl: needed by durability
    # cmsoap: CMSOAP service needed for the Tuner
    # cmxml: needed by cmsoap
    # ddsrrstorage: needed by cmxml

    set(DEPLOY_FILES         cmsoap.dll cmxml.dll ddshts.dll ddsi2.dll ddskernel.dll ddsos.dll ddsosnet.dll ddsrrstorage.dll dds_security_core.dll durability.dll spliced.dll sqlite3-ospl.dll)
    list(APPEND DEPLOY_FILES cmsoap.pdb cmxml.pdb ddshts.pdb ddsi2.pdb ddskernel.pdb ddsos.pdb ddsosnet.pdb ddsrrstorage.pdb dds_security_core.pdb durability.pdb spliced.pdb)
    get_property(_languages GLOBAL PROPERTY ENABLED_LANGUAGES)
    if("CXX" IN_LIST _languages)
        list(APPEND DEPLOY_FILES dcpsisocpp2.dll dcpsisocpp2.pdb)
    endif()
    if("CSharp" IN_LIST _languages)
        list(APPEND DEPLOY_FILES net46/dcpssacs.dll net46/dcpssacs.pdb)
    endif()
    list(TRANSFORM DEPLOY_FILES PREPEND "$<IF:$<CONFIG:Debug>,${OSPL_BIN_DEBUG},${OSPL_BIN}>/")
    # ospl_metaconfig.xml is needed at runtime
    list(APPEND DEPLOY_FILES "${OSPL_HOME}/etc/ospl_metaconfig.xml")

    install(FILES ${DEPLOY_FILES} DESTINATION ${DEPLOY_ARG_DESTINATION})
endfunction()
