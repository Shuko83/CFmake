from lxml import etree as XML
import argparse, os, platform, re, sys

# Common types definitions

BuildMode = { 'Base':     '',
              'Débogage': 'debug', 
              'Release':  'release' }

class PropertyGroup:

    config_id = ''
    includepath = list()
    librarypath = list()

class Project:

    filepath = ''
    template = ''
    
    headers = list()
    sources = list()
    libraries = list()

    properties = { 'Base':     PropertyGroup(), 
                   'Débogage': PropertyGroup(), 
                   'Release':  PropertyGroup() }

                   
# Borland project file parser
                   
def ParseCbproj( filepath ):

    # internal definitions
                      
    def normalize_path( path ): 
        path = os.path.normpath( path )
        return re.sub( '\\\\', '/', path )
        
    # cbproj path check
    
    assert( filepath )
    
    filepath = os.path.normpath( os.path.join( os.getcwd(), filepath ) )
    filename = os.path.basename( filepath )
    
    if ( not os.path.isfile( filepath ) ) or ( os.path.splitext( filepath )[1] != '.cbproj' ):
        sys.exit( 'Error: \'' + filepath + '\' is not a valid Borland project file' )
                
    # cbproj xml file parsing
    
    nsmap = { 'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003',
              'regex':   'http://exslt.org/regular-expressions' }
        
    try: root = XML.parse( filepath ).getroot()
    except: sys.exit( 'Error: cannot read \'' + filepath + '\' content' )
    
    project = Project()
    project.filepath = filepath
    
    # build configurations resolution

    nb_properties = len( root.xpath( 'msbuild:PropertyGroup[regex:test(@Condition, "^\'\$\(Config\)\'==.+$")]', namespaces = nsmap ) )
    
    if ( nb_properties < 2 ) or ( nb_properties > 3 ):
        sys.exit( 'Error: invalid build configurations detected in \'' + filename + '\'' )
    
    configurations = list()
    
    for config in project.properties.keys():
    
        configurations.append( config )
        configurations.append( BuildMode[config] )
        
    configurations = list( filter( None, configurations ) )
    
    for config in configurations:
        for group in root.xpath( 'msbuild:PropertyGroup[regex:test(@Condition, "^\'\$\(Config\)\'==\'' + config + '\'.+$")]', namespaces = nsmap ):
        
            cfg_ = group.xpath( 'msbuild:*[regex:test(local-name(), "^Cfg_\d+$")]', namespaces = nsmap )
            
            nb_properties = nb_properties - 1
            assert( nb_properties >= 0 )
            
            if ( not config in project.properties.keys() ):
            
                for key, value in BuildMode.items():
                    if ( config == value ): config = key
                
            if project.properties[config].config_id:
                sys.exit( 'Error: multiple ' + config + ' configurations detected in \'' + filename + '\'' )
                
            if ( not cfg_ ) and ( config == 'Base' ):           project.properties[config].config_id = 'Base'                
            elif ( len( cfg_ ) == 1 ) and ( config != 'Base' ): project.properties[config].config_id = str.replace( cfg_[0].tag, '{' + nsmap['msbuild'] + '}', '' )
            else:                                               sys.exit( 'Error: invalid build configurations detected in \'' + filename + '\'' )
            
    if not project.properties['Base'].config_id:
        sys.exit( 'Error: Base configuration not found in \'' + filename + '\'' )
        
    if ( nb_properties > 0 ):
        sys.exit( 'Error: Débogage or Release configuration not found in \'' + filename + '\'' )
    
    # project type resolution
    
    template = root.find( 'msbuild:ProjectExtensions/msbuild:Borland.ProjectType', nsmap )
    
    if template is None: 
        sys.exit( 'Error: project type not defined in \'' + filename + '\'' )

    if ( template.text != "CppVCLApplication" ) and \
       ( template.text != "CppStaticLibrary" ) and \
       ( template.text != "CppDynamicLibrary" ):
        sys.exit( 'Error: project type not supported in \'' + filename + '\'' )
        
    project.template = template.text
    
    # project items resolution
        
    items = root.find( 'msbuild:ItemGroup', nsmap )
    
    if items is None:    
        sys.exit( 'Error: items definition not found in \'' + filename + '\'' )
    
    for source in items.findall( 'msbuild:CppCompile[@Include]', nsmap ):
    
        depends = source.find( 'msbuild:DependentOn', nsmap )
        
        if ( depends is not None ):
            project.headers.append( depends.text )
            
        project.sources.append( source.get( 'Include' ) )
        
    for library in items.findall( 'msbuild:LibFiles[@Include]', nsmap ):
    
        ignore = library.find( 'msbuild:IgnorePath', nsmap )
        library = library.get( 'Include' )

        if ( ignore is None ) or ( ignore.text != 'true' ):
            project.properties['Base'].librarypath.append( os.path.dirname( library ) )
        
        library = re.sub( '\.[^\.]+', '', os.path.basename( library ) )
        project.libraries.append( library )
        
    project.headers = list( filter( None, project.headers ) )
    project.headers = list( set( [ normalize_path( path ) for path in project.headers] ) )
    project.headers.sort()
    
    project.sources = list( filter( None, project.sources ) )
    project.sources = list( set( [ normalize_path( path ) for path in project.sources] ) )
    project.sources.sort()
    
    project.libraries = list( filter( None, project.libraries ) )
    project.libraries = list( set( [ normalize_path( path ) for path in project.libraries] ) )
    project.libraries.sort()
        
    # include/library paths resolution
            
    for config in project.properties.keys():
    
        group = root.xpath( 'msbuild:PropertyGroup[regex:test(@Condition, "^\'\$\(' + project.properties[config].config_id + '\)\'!=\'\'$")]', namespaces = nsmap )
        
        if ( len( group ) != 1 ):
            sys.exit( 'Error: ' + config + ' configuration properties not found in \'' + filename + '\'' )
        
        for path in group[0].xpath( 'msbuild:*[regex:test(local-name(), "^(?:\w+_)?IncludePath$")]', namespaces = nsmap ):
        
            path.text = re.sub( '\$\(\w+\)[^;]*(?:;)?', '', path.text )
            project.properties[config].includepath = project.properties[config].includepath + path.text.split( ';' )

        project.properties[config].includepath = list( filter( None, project.properties[config].includepath ) )
        project.properties[config].includepath = list( set( [ normalize_path( path ) for path in project.properties[config].includepath] ) )
        
        if ( config != 'Base' ):
            project.properties[config].includepath = list( set( project.properties[config].includepath ) - \
                                                           set( project.properties['Base'].includepath ) )
            
        project.properties[config].includepath.sort()
        
        for path in group[0].xpath( 'msbuild:*[local-name()="DCC_UnitSearchPath" or \
                                               regex:test(local-name(), "^ILINK_(?:\w+)?LibraryPath$")]', namespaces = nsmap ):
            path.text = re.sub( '\$\(\w+\)[^;]*(?:;)?', '', path.text )
            project.properties[config].librarypath = project.properties[config].librarypath + path.text.split( ';' )
                        
        project.properties[config].librarypath = list( filter( None, project.properties[config].librarypath ) )
        project.properties[config].librarypath = list( set( [ normalize_path( path ) for path in project.properties[config].librarypath] ) )
        
        if ( config != 'Base' ):
            project.properties[config].librarypath = list( set( project.properties[config].librarypath ) - \
                                                           set( project.properties['Base'].librarypath ) )
            
        project.properties[config].librarypath.sort()
    
    return project
        
    
# Borland qmake file generator
    
def GeneratePro( project, output ):

    # internal definitions

    def write_collection( file, variable, values ):
    
        item = values.pop(0)
        file.write( variable + ' += ' + item )
            
        for item in values:
            file.write( ' \\\n' + ' ' * ( len( variable ) + 4 ) + item )
        
        file.write( '\n' )
            
    # qmake file writing

    assert( project )
            
    if output: filepath = os.path.join( os.path.dirname( project.filepath ), output )
    else: filepath = os.path.splitext( project.filepath )[0] + '.pro'

    if ( os.path.splitext( filepath )[1] != '.pro' ):
        sys.exit( 'Error: \'' + filepath + '\' is not a valid Borland qmake file' )
        
    try:
        with open( filepath, 'w' ) as pro:

            # target definition section
        
            pro.write( '# Target Definition\n\n' )
            pro.write( 'TARGET = ' + os.path.basename( os.path.splitext( project.filepath )[0] ) + '\n' )
            
            if ( project.template == "CppVCLApplication" ):
                pro.write( 'TEMPLATE = app\n' )

            elif ( project.template == "CppStaticLibrary" ) or \
                 ( project.template == "CppDynamicLibrary" ):
                pro.write( 'TEMPLATE = lib\n' )

            # Qt configuration section
                
            pro.write( '\n# Qt Configuration\n\n' )
            
            if ( project.template == "CppStaticLibrary" ):
                pro.write( 'CONFIG += static\n' )
                
            if ( project.template == "CppDynamicLibrary" ):
                pro.write( 'CONFIG += shared\n' )
                
            pro.write( 'CONFIG -= qt\n' )
                
            # C++ compiler options section
                
            for config in project.properties.keys():
            
                if project.properties[config].librarypath: includepath = True
                if project.properties[config].librarypath: librarypath = True
                            
            if includepath or librarypath:
               
                pro.write( '\n# C++ Compiler Options\n' )
                
                for config in project.properties.keys():

                    spaces = 4 if BuildMode[config] else 0
                    
                    if project.properties[config].includepath:

                        pro.write( '\n' )                    
                        if spaces: pro.write( 'build_mode( ' + BuildMode[config] + ' ): \\\n' )                
                        write_collection( pro, ' ' * spaces + 'INCLUDEPATH', project.properties[config].includepath )
                    
                    if project.properties[config].librarypath:
                    
                        pro.write( '\n' )                    
                        if spaces: pro.write( 'build_mode( ' + BuildMode[config] + ' ): \\\n' )                
                        write_collection( pro, ' ' * spaces + 'LIBRARYPATH', project.properties[config].librarypath )
                        
            # sources definition section
                    
            if project.headers or project.sources:
               
                pro.write( '\n# Sources Definition\n' )
                
                if project.headers:
                
                    pro.write( '\n' )
                    write_collection( pro, 'HEADERS', project.headers )
                    
                if project.sources:
                
                    pro.write( '\n' )
                    write_collection( pro, 'SOURCES', project.sources )
                
            # dependencies definition section
                
            if project.libraries:
                
                pro.write( '\n# Dependencies Definition\n\n' )
                write_collection( pro, 'LIBS', project.libraries )
                
            # toolkit definition section
                
            pro.write( '\n# Toolkit Definition\n\n' )
            pro.write( 'TOOLKIT.host.arch = x86\n' )
            pro.write( 'TOOLKIT.compiler.name = bl\n' )
            pro.write( 'TOOLKIT.compiler.version = 2010\n\n' )
            pro.write( 'TOOLKIT.compiler = $${TOOLKIT.compiler.name}$${TOOLKIT.compiler.version}\n' )
            pro.write( 'TOOLKIT.mkspec = $${TOOLKIT.host.platform}_$${TOOLKIT.compiler}_$${TOOLKIT.host.arch}\n' )

            # ST rules section
            
            pro.write( '\n# ST Rules Include\n\n' )
            pro.write( 'include( $${QSMAKEPATH}/rules/standard_project.pri )\n' )
            
            pro.close()

    except:
        sys.exit( 'Error: \'' + filepath + '\' cannot be generated' )
        
    
# Main function
    
def main():

    if ( platform.system() != 'Windows' ): 
        sys.exit( 'Error: script not applicable on ' + platform.system() + ' platforms' )

    parser = argparse.ArgumentParser()
    parser.add_argument( 'path', metavar = '<project>', help = 'Borland project filepath' )
    parser.add_argument( '--output', metavar = '<name>', help = 'qmake project filename' )

    args = parser.parse_args()
    GeneratePro( ParseCbproj( args.path ), args.output )


if __name__ == '__main__':
    main()
    