######################
## Deprecated Rules ##
######################

# -- TO REMOVE: remove deprecated rules -- #
CONFIG -= deprecated_rules
# -- TO REMOVE: remove deprecated rules -- #


#################
## Qt Settings ##
#################

# -- Build modes configuration -- #
#    by default: - debug_and_release option is enabled for any toolkit (if debug or release CONFIG option not set in qmake arguments)
#                - build_all option is enabled for any toolkit (if debug_and_release option is enabled)
#                - debug and release options are not enabled for auxiliary targets

debug_or_release: \
    CONFIG -= debug_and_release

else: !TEMPLATE( aux ): \
    CONFIG *= debug_and_release build_all

# -- Compiler miscellaneous supports -- #
#    by default: enables precompiled headers support

CONFIG *= precompile_header

# -- qmake features required -- #

assert( !isEmpty( QSMAKEPATH ) )


#################
## Qt Linguist ##
#################

# -- lupdate tool precessing -- #
#    by default: - disables project and target informations generation
#                - removes target custom extension

lupdate_run {

    CONFIG *= no_generate_project
    CONFIG *= no_generated_target_info
    clear( TARGET_EXT )
}


#######################
## Builtin Variables ##
#######################

## PACKAGE      = absolute path to package containing this project
## DELIVERY     = absolute path to Delivery package directory: $${PACKAGE}/Delivery
## BUILD_MKSPEC = output subdirectory path based on toolkit mkspec: $${TOOLKIT.mkspec}[-QT$$[QT_VERSION]] or for QT >= 5.10 $${TOOLKIT.mkspec}[-QT$${QT_VERSION_MM}]

PACKAGE = $$search_parent_until( _PRO_FILE_PWD_, Delivery )
isEmpty( PACKAGE ): qserror( "Unable to resolve package path" )

DELIVERY = $$clean_path( $${PACKAGE}/Delivery )

qt {
    qtGreaterThan( 5.9.9 ) : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$${QT_VERSION_MM}
    else : BUILD_MKSPEC = $${TOOLKIT.mkspec}-QT$$[QT_VERSION]
}
else: BUILD_MKSPEC = $${TOOLKIT.mkspec}

add_pseudo_var( PACKAGE )
add_pseudo_var( DELIVERY )
add_pseudo_var( BUILD_MKSPEC )


########################
## Target Definitions ##
########################

# -- Target name definition -- #

_target = $$remove( TARGET, $$re_escape( [] ) )

!find( _target, \\[.+\\] ) {                    # TARGET doesn't contain debug modifiers or ends with [] modifier:

    !endsWith( TARGET, $$re_escape( [] ) ) {    # if TARGET does not end with [] modifier
                                                # add target debug suffix [d] for binary target project
        TEMPLATE( binary ): \
            _target = $${_target}[d]
    }
}

TARGET = $${_target}
unset( _target )


# -- Target properties definition -- #

TEMPLATE( binary ) {

    ## product and version properties resolution from Jenkins
    ### VARIABLES JENKINS A VERIFIER

    _jenkins_product = $$(PRODUCT)
    _jenkins_version = $$(VERSION)
    _jenkins_build = $$(BUILDNUMBER)

    !isEmpty( _jenkins.product ): \
        PROPERTIES.product = $${_jenkins_product}

    !isEmpty( _jenkins_version ) {

        PROPERTIES.version = $${_jenkins_version}

        !isEmpty( _jenkins_build ) {
		
			_jenkins_build = $$remove( _jenkins_build, ^[b] )
            PROPERTIES.version = $${PROPERTIES.version}.$${_jenkins_build}
		}        
    }

    unset( _jenkins_version )
    unset( _jenkins_product )
    unset( _jenkins_build )

    ## company and copyright properties definitions

    PROPERTIES.company = Diginext
    PROPERTIES.domain = diginext.fr
    PROPERTIES.copyright = Copyright (C) %y %c
}


# -- Target install definitions -- #

## INSTALLDIR = installation directory absolute path: points to Delivery package directory or one of its subdirectories

isEmpty( INSTALLDIR ): \
    INSTALLDIR = $${DELIVERY}

else {

    # resolves pseudo-variables in INSTALLDIR for path handling functions
    resolve_pseudo_vars( INSTALLDIR )

    _delivery_path = $$absolute_path( $${INSTALLDIR}, $${_PRO_FILE_PWD_} )

    !startsWith( _delivery_path, $${DELIVERY} ): \
        qserror( "INSTALLDIR variable does not point to Delivery package directory or one of its subdirectories" )

    unset( _delivery_path )
}

## INSTALLS target.path = target installation path: $${BUILD_MKSPEC}/${LIBRARY_MODE}/${BUILD_MODE} if not defined

contains( INSTALLS, target ) {

    isEmpty( target.path ): \
        target.path = $${BUILD_MKSPEC}/${LIBRARY_MODE}/${BUILD_MODE}
}


############################
## Project Tree Structure ##
############################

TEMPLATE( binary ) {

    # Binary project tree must not contain projects other than current one

    rglob( _projects, $${_PRO_FILE_PWD_}/*/*.pro \
                      $${_PRO_FILE_PWD_}/*/*.cbproj )

    !isEmpty( _projects ): \
        qserror( "Unexpected project file(s) detected in '$$system_path( $${_PRO_FILE_PWD_} )' subdirectories" )

    unset( _projects )
}


##########################
## Metadata Generations ##
##########################

# -- Dependencies project generator -- #

depends.template = $${PWD}/generators/depends.prg
depends.target   = $${_PRO_FILE_PWD_}/$${_PRO_FILE_NAME_}.deps
depends.CONFIG   = no_target


# -- LDTools meta-project generator -- #

ldtools.template = $${PWD}/generators/ldtools.prg
ldtools.target   = $${_PRO_FILE_PWD_}/$${_PRO_FILE_NAME_}.ldtools
ldtools.CONFIG   = no_target release


## QMAKE_METADATA_GENERATORS = list of additional metadata generators

QMAKE_METADATA_GENERATORS += depends ldtools


#######################
## Compiler Settings ##
#######################

build_pass( binary ) {

    # -- C++ preprocessor macros definition -- #

    win32:       DEFINES *= _WINDOWS _WIN32_PLATEFORM_              # custom Windows platform preprocessor macros
    else: linux: DEFINES *= _LINUX_PLATEFORM_                       # custom Linux platform preprocessor macro

    TEMPLATE( shared ): \
        DEFINES *= $$upper( $$TARGET()_LIB )                        # defines symbols import/export control macro (shared library only)


    # -- MSVC compiler settings -- #

    msvc {

        QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings                 # enables string literal type conversion in release and profile modes
        QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings

        QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
        QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
    }
}
