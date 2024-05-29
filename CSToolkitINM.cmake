include(${CMAKE_CURRENT_LIST_DIR}/CSToolkit.cmake)

if(WIN32)
    add_compile_definitions(QT_LARGEFILE_SUPPORT _CRT_SECURE_NO_WARNINGS Q_WS_WIN)
endif()
