<#-- macro pour la generation cpp -->
<#macro hheader name_of_file name_of_author description>
/**
 * @file ${name_of_file}.h
 * @brief ${description}
 * @version 1.0
 * @date ${now()}
 * @author ${name_of_author}
 */

#ifndef _${name_of_file}_H
#define _${name_of_file}_H
</#macro>

<#macro hfooter>
#endif
//------------------------------------------------------
//DO NOT WRITE ANY CODE AFTER THIS POINT
//------------------------------------------------------
</#macro>

<#macro cppheader name_of_file name_of_author description>
/**
 * @file ${name_of_file}.cpp
 * @brief ${description}
 * @version 1.0
 * @date ${now()}
 * @author ${name_of_author}
 */

</#macro>
