# Internal configuration

# URLs
set(CFMAKE_ARTIFACTORY_URL "http://artifactory.divst:8081/artifactory" CACHE STRING "Url of artifactory")
set(CFMAKE_NEXUS_DR_URL "https://dr-aix-nexus-01v.sccoa.si.c-s.fr/repository" CACHE STRING "Url of Nexus DR")
set(CFMAKE_NEXUS_ITAR_URL "https://itar-aix-nexus-01v.sccoa.si.c-s.fr/repository" CACHE STRING "Url of Nexus ITAR")

# License
if(NOT $ENV{ADLINK_LICENSE} STREQUAL "")
    set(CFMAKE_ADLINK_LICENSE $ENV{ADLINK_LICENSE} CACHE STRING "Path to license file or license server for OpenSplice")
elseif(NOT $ENV{prismtech_LICENSE} STREQUAL "")
    set(CFMAKE_ADLINK_LICENSE $ENV{prismtech_LICENSE} CACHE STRING "Path to license file or license server for OpenSplice")
else()
    set(CFMAKE_ADLINK_LICENSE "7201@fraixems1.diginext.local" CACHE STRING "Path to license file or license server for OpenSplice")
endif()