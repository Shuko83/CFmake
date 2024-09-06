#ifndef SNWIDGETS_H
#define SNWIDGETS_H

#include <QtGlobal>

#if !defined( SN_WIDGETS_EXPORT )
    #if defined( MYQTLIB_LIB )
        #define SN_WIDGETS_EXPORT Q_DECL_EXPORT
    #else
        #define SN_WIDGETS_EXPORT Q_DECL_IMPORT
    #endif
#endif


namespace sunset {
    namespace widgets {
    
        void snRegisterWidgets();
    }
}


#endif
