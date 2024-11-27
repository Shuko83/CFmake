#pragma once

// Copied fromm boost/core/demagle.hpp
// core::demangle
//
// Copyright 2014 Peter Dimov
// Copyright 2014 Andrey Semashev
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <QString>
#include <QtGlobal>

#ifdef Q_OS_LINUX
#include <cxxabi.h>
#endif
namespace StreamWork
{

namespace SwCore
{

#ifdef Q_OS_LINUX
inline char const * demangle_alloc( char const * name );
inline void demangle_free( char const * name );

class scoped_demangled_name
{
private:
    char const * m_p;

public:

    explicit scoped_demangled_name( char const * name ) :
        m_p( demangle_alloc( name ) )
    {
    }

    ~scoped_demangled_name()
    {
        demangle_free( m_p );
    }
    char const * get() const
    {
        return m_p;
    }
};

inline char const * demangle_alloc( char const * name )
{
    int status = 0;
    std::size_t size = 0;
    return abi::__cxa_demangle( name, NULL, &size, &status );
}

inline void demangle_free( char const * name )
{
    std::free( const_cast< char* >( name ) );
}


inline QString demangle( const QString& name )
{
    scoped_demangled_name demangled_name( name.toLatin1() );
    char const * p = demangled_name.get();
    if( !p )
        return name;
    return QString::fromLatin1(p);
}
#else
inline QString demangle( const QString& name )
{
    QString nameDemangled = name;
    if(nameDemangled.startsWith(QLatin1String("class ")))
    {
        nameDemangled.remove(0,6);
    }
    if(nameDemangled.startsWith(QLatin1String("struct ")))
    {
        nameDemangled.remove(0,7);
    }
#ifdef Q_OS_WIN64
    nameDemangled.replace(" * __ptr64", "*");
#else
    nameDemangled.replace(" * __ptr32", "*");
#endif

    return nameDemangled;
}
#endif
}
}