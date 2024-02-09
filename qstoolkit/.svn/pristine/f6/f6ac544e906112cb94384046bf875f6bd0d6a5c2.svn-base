# Definition du repertoire de sortie en fonction du compilateur visual sous windows ou gcc sous linux

win32 {

    COMPILO = vc

    equals( TOOLKIT.compiler, msvc2013 ) {

        equals( TOOLKIT.host.arch, x86 ): \
            COMPILO = vc12

        else: equals( TOOLKIT.host.arch, x64 ): \
            COMPILO = vc12-x64

        else: error( "Windows architecture cannot be resolved" )
    }

    else: equals( TOOLKIT.compiler, msvc2015 ) {

        equals( TOOLKIT.host.arch, x86 ): \
            COMPILO = vc14

        else: equals( TOOLKIT.host.arch, x64 ): \
            COMPILO = vc14-x64

        else: error( "Windows architecture cannot be resolved" )
    }

    DEPLOY = $${TOOLKIT.mkspec}
}

else: unix {

    COMPILO = li
    DEPLOY = $${TOOLKIT.mkspec}
}

build_mode( debug ) {

    DEBUG_PATH = debug
    LIB_PATH = /lib/$${COMPILO}/$${DEBUG_PATH}
    BIN_PATH = /bin/$${COMPILO}/$${DEBUG_PATH}
    DELIVERY_PATH = /Delivery/$${DEPLOY}/$${DEBUG_PATH}
}

else: build_mode( release ) {

    RELEASE_PATH = release
    LIB_PATH = /lib/$${COMPILO}/$${RELEASE_PATH}
    BIN_PATH = /bin/$${COMPILO}/$${RELEASE_PATH}
    DELIVERY_PATH = /Delivery/$${DEPLOY}/$${RELEASE_PATH}
}


win32 {

    DEFINES *= QT_LARGEFILE_SUPPORT _CRT_SECURE_NO_WARNINGS
    qtGreaterThan( 5.0, true ): DEFINES *= Q_WS_WIN
}
