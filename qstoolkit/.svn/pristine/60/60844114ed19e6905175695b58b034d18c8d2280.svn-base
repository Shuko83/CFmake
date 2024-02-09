from lxml import etree as XML
import argparse, os, platform, re, sys

# Common types definitions
    
class Version:

    def __init__( self, text='' ):
    
        result = re.findall( '^(\d+)(?:\.(\d+))?(?:\.(\d+))?(?:\.(\d+))?$', text )

        if result:
            if result[0][0]: self.major = result[0][0]
            if result[0][1]: self.minor = result[0][1]
            if result[0][2]: self.patch = result[0][2]
            if result[0][3]: self.build = result[0][3]

    major = '0'
    minor = '0'
    patch = '0'
    build = '0'
    
class Properties:

    version = Version()
    product = ''
    description = ''
    company = ''
    copyright = ''
                   
        
# Target properties generation
        
def GenerateInfo( filepath, properties ):
    
    # internal definitions
    
    def normalize_path( path, root ): 
        path = os.path.normpath( os.path.join( root, path ) )
        return re.sub( '\\\\', '/', path )
        
    # cbproj path check
    
    assert( filepath )
    
    filepath = os.path.normpath( os.path.join( os.getcwd(), filepath ) )
    filename = os.path.basename( filepath )

    if ( not os.path.isfile( filepath ) ) or ( os.path.splitext( filepath )[1] != '.cbproj' ):
        sys.exit( 'Error: \'' + filepath + '\' is not a valid Borland project file' )

    # target properties check
    
    assert ( properties )
    
    if not properties.product: properties.product = ''
    if not properties.description: properties.description = ''
    if not properties.company: properties.company = ''
    if not properties.copyright: properties.copyright = ''
        
    # cbproj xml file parsing
    
    nsmap = { 'msbuild': 'http://schemas.microsoft.com/developer/msbuild/2003',
              'regex':   'http://exslt.org/regular-expressions' }
        
    try: root = XML.parse( filepath ).getroot()
    except: sys.exit( 'Error: cannot read \'' + filepath + '\' content' )
    
    # project type resolution
    
    template = root.find( 'msbuild:ProjectExtensions/msbuild:Borland.ProjectType', nsmap )
    
    if template is None: 
        sys.exit( 'Error: project type not defined in \'' + filename + '\'' )

    if ( template.text != "CppVCLApplication" ) and \
       ( template.text != "CppDynamicLibrary" ):
        sys.exit( 'Error: project type not supported in \'' + filename + '\'' )

    # target extension resolution
            
    extension = root.xpath( 'msbuild:PropertyGroup[@Condition]/msbuild:OutputExt', namespaces = nsmap )
    
    if ( extension is None ) or ( len( extension ) != 1 ):
        sys.exit( 'Error: invalid target extension detected in \'' + filename + '\'' )

    extension = extension[0].text
        
    # target outputs resolution

    outputs = list()
    
    for group in root.xpath( 'msbuild:PropertyGroup[@Condition]/msbuild:FinalOutputDir | \
                              msbuild:PropertyGroup[@Condition]/msbuild:BPILibOutputDir', namespaces = nsmap ):
        outputs.append( group.text + '/' + str.replace( filename, os.path.splitext( filepath )[1], '' ) + '.' + extension )
        
    if ( len( extension ) < 1 ):
        sys.exit( 'Error: target output paths not found in \'' + filename + '\'' )
        
    outputs = list( filter( None, outputs ) )
    outputs = list( set( [ normalize_path( path, os.path.dirname( filepath ) ) for path in outputs] ) )        

    # target properties set
    
    verpatch = os.path.dirname( os.path.abspath( __file__ ) )
    verpatch = os.path.normpath( verpatch + '/../tools/verpatch.exe' )

    args = '/va ' + properties.version.major + '.' + properties.version.minor + '.' + properties.version.patch + '.' + properties.version.build
    args = args + ' /langid 0x409'
    args = args + ' /pv ' + properties.version.major + '.' + properties.version.minor + '.' + properties.version.patch + '.' + properties.version.build
    args = args + ' /s CompanyName "' + properties.company + '"'
    args = args + ' /s FileDescription "' + properties.description + '"'
    args = args + ' /s LegalCopyright "' + str.replace( properties.copyright, '(C)', '\251' ) + '"'
    args = args + ' /s ProductName "' + properties.product + '"'
            
    for output in outputs:
    
        if not os.path.isfile( output ):
            sys.exit( 'Error: cannot apply properties to \'' + output + '\' (file not found)' )
    
        if ( os.system( verpatch + ' ' + output + ' ' + args ) != 0 ):
            sys.exit( 'Error: cannot apply properties to \'' + output + '\' (verpatch error)' )
      
    
# Main function
    
def main():

    if ( platform.system() != 'Windows' ): 
        sys.exit( 'Error: script not applicable on ' + platform.system() + ' platforms' )

    parser = argparse.ArgumentParser()
    parser.add_argument( 'path', metavar = '<project>', help = 'Borland project filepath' )
    parser.add_argument( '--version', metavar = '<number>', help = 'target version number' )
    parser.add_argument( '--product', metavar = '<name>', help = 'product name for the project target' )
    parser.add_argument( '--description', metavar = '<text>', help = 'target description' )
    parser.add_argument( '--company', metavar = '<name>', help = 'name of the organization publishing the project target' )
    parser.add_argument( '--copyright', metavar = '<text>', help = 'copyright information for the project target' )
    
    args = parser.parse_args()
    properties = Properties()
    
    properties.version = Version( args.version )
    properties.product = args.product
    properties.description = args.description
    properties.company = args.company
    properties.copyright = args.copyright
    
    GenerateInfo( args.path, properties )


if __name__ == '__main__':
    main()
    