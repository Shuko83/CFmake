# Note: VortexOpenSplice is tree special case: suitable behavior required

find_vos_package_libraries(
  "VortexOpenSplice"
  "cmsoap;cmxml;dcpsisocpp2;dds_security_core;ddshts;ddsi2;ddskernel;ddsos;ddsosnet;ddsrrstorage;durability;spliced;sqlite3-ospl"
# "cmjni;cmsoap;cmxml;dbmsconnect;dcpsc99;dcpsgapi;dcpsisocpp2;dcpssac;dcpssacpp;dcpssaj;dds_labview;dds_security_ac;dds_security_auth;dds_security_core;dds_security_crypto;ddsconf;ddsconfparser;ddsconfvalidator;ddsdatabase;ddshts;ddsi2;ddsi2e;ddskernel;ddsos;ddsosnet;ddsrmi;ddsrrstorage;ddsserialization;ddsuser;ddsutil;dlite;durability;durability_migrate;facecpp;lzf-ospl;networking;nwbridge;rnr;service_logger;snappy-ospl;snetworking;spliced;sqlite3-ospl;streamssacpp"
  "win32/include/os_os_if.h"
  "lib"
  "bin")

find_file(VortexOpenSplice_CONFIG_FILE
  NAMES ospl_metaconfig.xml
  PATHS ${VortexOpenSplice_ROOT}/etc)
