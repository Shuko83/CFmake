# -- Metadata generator conditions -- #

!TEMPLATE( binary ): \
    qswarning( "'$${TEMPLATE}' target not supported. Dependencies generation skipped..." )
    
else: isEmpty( PACKAGE ): \
    qswarning( "Empty PACKAGE variable value detected. Dependencies generation skipped..." )
        
else: isEmpty( INSTALLDIR ): \
    qswarning( "Empty INSTALLDIR variable value detected. Dependencies generation skipped..." )
        
else {


    # -- function: project_root() -- #

    ## project_root = top-level project sources path:
    ##   - equals to $${PACKAGE} directory in case of single project package
    ##   - otherwise equals to top-level directory including the project in use only

    defineReplace( project_root ) {

        current = $${_PRO_FILE_PWD_}
        
        glob( excluded, $${_PRO_FILE_PWD_}/*.pro \
                        $${_PRO_FILE_PWD_}/*.cbproj )

        subdirs = $$split( _PRO_FILE_PWD_, / )
        iterator = $${_PRO_FILE_PWD_}
        
        for( subdir, subdirs ) {

            iterator = $$relative_path( $${iterator}, $${PACKAGE} )

            equals( iterator, . ) {
            
                current = $${PACKAGE}
                resolved = true
                break()
            }
            
            iterator = $$absolute_path( $${iterator}, $${PACKAGE} )
            
            rglob( projects, $${iterator}/*.pro \
                             $${iterator}/*.cbproj )
            projects -= $${excluded}
            
            !isEmpty( projects ) {
            
                resolved = true
                break()
            }
            
            current = $${iterator}
            iterator = $$dirname( iterator )
        }

        isEmpty( resolved ): \
            qserror( "Unable to resolve top-level project sources directory" )
            
        return( $${current} )
    }
    
    
    build_pass() {

        # -- function: add_depend( variablename, path ) -- #
                
        defineTest( add_depend.external ) {

            ## external dependency check sub-routine
            ## dependency out of $${PACKAGE} directory

            arg.path = $${1}
            arg.path = $$relative_path( $${arg.path}, $${PACKAGE} )

            is_absolute_path( $${arg.path} )| \
            startsWith( arg.path, $$re_escape( .. ) ): \
                return( true )
            
            return( false )
        }
        
        defineTest( add_depend.delivery ) {

            ## delivery dependency check sub-routine
            ## dependency in $${INSTALLDIR} directory

            arg.path = $${1}

            !add_depend.external( $${arg.path} ) {

                arg.path = $$relative_path( $${arg.path}, $${_PRO_FILE_PWD_}/$${INSTALLDIR} )

                is_relative_path( $${arg.path} ): \
                !startsWith( arg.path, $$re_escape( .. ) ): \
                    return( true )
            }
            
            return( false )
        }
        
        defineTest( add_depend.project ) {

            ## project in use dependency check sub-routine
            ## dependency in $${_project_root} directory

            arg.path = $${1}

            !add_depend.delivery( $${arg.path} ) {

                arg.path = $$relative_path( $${arg.path}, $${_project_root} )

                is_relative_path( $${arg.path} ): \
                !startsWith( arg.path, $$re_escape( .. ) ): \
                    return( true )
            }
            
            return( false )
        }
            
        defineTest( add_depend ) {

            arg.variable = $${1}
            arg.path = $${2}

            !isEmpty( arg.variable ): \
            !isEmpty( arg.path ) {
                
                ## external dependency registration (external to the package)
                
                arg.path = $$absolute_path( $${arg.path}, $${_PRO_FILE_PWD_} )
                
                add_depend.external( $${arg.path} ) {
                
                    arg.path = $$relative_path( $${arg.path}, $${PACKAGE} )

                    arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION}/ )
                    arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION_MM}/ )
                    arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}(?:/|$), /${TOOLKIT.mkspec}/ )
                    arg.path = $$replace( arg.path, /(?:debug|release)(?:/|$), /${BUILD_MODE}/ )
                                        
                    eval( _externals.$${arg.variable} *= $$clean_path( $${arg.path} ) )
                    export( _externals.$${arg.variable} )
                }

                ## delivery dependency registration (internal to the package)
                
                else { 
                
                    add_depend.delivery( $${arg.path} ) {
                    
                        arg.path = $$relative_path( $${arg.path}, $${_PRO_FILE_PWD_}/$${INSTALLDIR} )
                        arg.path = $$relative_path( $${_PRO_FILE_PWD_}/$${INSTALLDIR}/$${arg.path}, $${PACKAGE} )
                    
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION}/ )
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION_MM}/ )
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}(?:/|$), /${TOOLKIT.mkspec}/ )
                        arg.path = $$replace( arg.path, /(?:debug|release)(?:/|$), /${BUILD_MODE}/ )
                        
                        eval( _internals.$${arg.variable} *= $$clean_path( $${arg.path} ) )
                        export( _internals.$${arg.variable} )
                    }

                ## dependency outside project 'in use' registration (internal to the package)
                
                    else: !add_depend.project( $${arg.path} ) {
                        
                        arg.path = $$relative_path( $${arg.path}, $${_project_root} )
                        arg.path = $$relative_path( $${_project_root}/$${arg.path}, $${PACKAGE} )
                        
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION}/ )
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}(?:/|$), /${TOOLKIT.mkspec}-QT${QT_VERSION_MM}/ )
                        arg.path = $$replace( arg.path, /$${TOOLKIT.mkspec}(?:/|$), /${TOOLKIT.mkspec}/ )
                        arg.path = $$replace( arg.path, /(?:debug|release)(?:/|$), /${BUILD_MODE}/ )

                        eval( _internals.$${arg.variable} *= $$clean_path( $${arg.path} ) )
                        export( _internals.$${arg.variable} )
                    }
                }
                
                return( true )
            }

            return( false )
        }

        
        # -- function: write_depends( type, config ) -- #
        
        defineTest( write_depends ) {
        
            arg.type = $${1}
            arg.config = $${2}
            
            !isEmpty( arg.type ) {
        
                !isEmpty( _$${arg.type}.sources )| \
                !isEmpty( _$${arg.type}.libraries )| \
                !isEmpty( _$${arg.type}.plugins )| \
                !isEmpty( _$${arg.type}.others ) {
                
                    append_string( "," )
                    
                    append_line()
                    append_line(     "    \"$${arg.type}$${arg.config}\":" )
                    append_line(     "    {" )
                    
                    !isEmpty( _$${arg.type}.sources ) {
                    
                        append_line( "        \"sources\": [ $$join( _$${arg.type}.sources, "$${LITERAL_QUOTES}, $${LITERAL_QUOTES}", $${LITERAL_QUOTES}, $${LITERAL_QUOTES} ) ]" )
                        
                        !isEmpty( _$${arg.type}.libraries )| \
                        !isEmpty( _$${arg.type}.plugins )| \
                        !isEmpty( _$${arg.type}.others ): \
                            append_string( "," )
                    }
                    
                    !isEmpty( _$${arg.type}.libraries ) {

                        append_line( "        \"libraries\": [ $$join( _$${arg.type}.libraries, "$${LITERAL_QUOTES}, $${LITERAL_QUOTES}", $${LITERAL_QUOTES}, $${LITERAL_QUOTES} ) ]" )
                        
                        !isEmpty( _$${arg.type}.plugins )| \
                        !isEmpty( _$${arg.type}.others ): \
                            append_string( "," )
                    }

                    !isEmpty( _$${arg.type}.plugins ) {

                        append_line( "        \"plugins\": [ $$join( _$${arg.type}.plugins, "$${LITERAL_QUOTES}, $${LITERAL_QUOTES}", $${LITERAL_QUOTES}, $${LITERAL_QUOTES} ) ]" )
                        
                        !isEmpty( _$${arg.type}.others ): \
                            append_string( "," )
                    }

                    !isEmpty( _$${arg.type}.others ): \
                        append_line( "        \"others\": [ $$join( _$${arg.type}.others, "$${LITERAL_QUOTES}, $${LITERAL_QUOTES}", $${LITERAL_QUOTES}, $${LITERAL_QUOTES} ) ]" )
                    
                    append_line(     "    }" )
                }
            }
        }
        
        
        # -- Project dependencies registration -- #

        _project_root = $$project_root()
        
        for( file, SOURCES ):         add_depend( sources, $$dirname( file ) )
        for( file, HEADERS ):         add_depend( sources, $$dirname( file ) )
        for( path, IQUOTEPATH ):      add_depend( sources, $${path} )
        for( path, INCLUDEPATH ):     add_depend( sources, $${path} )

        for( path, LIBRARYPATH ):     add_depend( libraries, $${path} )
        for( path, PLUGINPATH ):      add_depend( plugins,   $${path} )
        
        for( file, OTHER_FILES ):     add_depend( others, $$dirname( file ) )
        for( file, DISTFILES ):       add_depend( others, $$dirname( file ) )
        
        unset( _project_root )
    }
    

    # -- Project dependencies file generation -- #
    #    all dependencies paths will be relative to package path
    
    # {
    #     "properties":
    #     {
    #         "root":     "<top-level path relative to project path>",
    #         "package":  "<package path relative to project path>",
    #         "install":  "<installation path relative to project path>"
    #     },
    #
    #     "toolkit":
    #     {
    #         "platform":         "<platform shortname>",
    #         "compiler.name":    "<compiler shortname>",
    #         "compiler.version": "<compiler version>",
    #         "architecture":     "<architecture shortname>"
    #     },
    #  
    #     "internals.<config>":                                             (optional)
    #     {
    #         "sources":   <array of internal source paths dependencies>,   (optional)
    #         "libraries": <array of internal library paths dependencies>,  (optional)
    #         "plugins":   <array of external plugin paths dependencies>,   (optional)
    #         "others":    <array of internal other paths dependencies>     (optional)
    #     },
    #  
    #     "externals.<config>":                                             (optional)
    #     {
    #         "sources":   <array of external source paths dependencies>,   (optional)
    #         "libraries": <array of external library paths dependencies>,  (optional)
    #         "plugins":   <array of external plugin paths dependencies>,   (optional)
    #         "others":    <array of external other paths dependencies>     (optional)
    #     }
    # }
    
    # with <config> equals to:
    #   - (debug|release) according to current build mode
    #   - (static|shared)-(debug|release) if static_and_shared option is enabled
    
    on_writer_start() {

        append_line( "$${LITERAL_OPEN_BRACE}" )
        append_line( "    \"description\": \"Dependencies file generated by qmake $$[QMAKE_VERSION] ($$[QT_VERSION]).\"," )
        append_line()
        append_line( "    \"properties\":" )
        append_line( "    {" )
        append_line( "        \"root\": \"$$relative_path( $$project_root(), $${_PRO_FILE_PWD_} )\"," )
        append_line( "        \"package\": \"$$relative_path( $${PACKAGE}, $${_PRO_FILE_PWD_} )\"," )
        append_line( "        \"install\": \"$${INSTALLDIR}\"" )
        append_line( "    }," )
        append_line()
        append_line( "    \"toolkit\":" )
        append_line( "    {" )
        append_line( "        \"platform\": \"$${TOOLKIT.host.platform}\"," )
        append_line( "        \"compiler.name\": \"$${TOOLKIT.compiler.name}\"," )
        append_line( "        \"compiler.version\": \"$${TOOLKIT.compiler.version}\"," )
        append_line( "        \"architecture\": \"$${TOOLKIT.host.arch}\"" )
        append_line( "    }" )
    }
    
    build_pass() {
    
        static_and_shared {
        
            TEMPLATE( shared ):       _depend_config += shared
            else: TEMPLATE( static ): _depend_config += static
        }

        build_mode( release ):        _depend_config += release
        else: build_mode( debug ):    _depend_config += debug

        _depend_config = .$$join( _depend_config, - )
        
        write_depends( internals, $${_depend_config} )
        write_depends( externals, $${_depend_config} )
        
        unset( _internals.sources )
        unset( _internals.libraries )
        unset( _internals.plugins )
        unset( _internals.others )

        unset( _externals.sources )
        unset( _externals.libraries )
        unset( _externals.plugins )
        unset( _externals.others )

        unset( _depend_config )
    }
    
    on_writer_end(): \
        append_line( "$${LITERAL_CLOSE_BRACE}" )
}
