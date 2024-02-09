from lxml import etree as XML
import argparse, json, os, platform, sys

# Common types definitions

class Properties:

    version = ''
    product = ''
    description = ''
    company = ''
    year = ''
    copyright = ''


# package.json file parser
    
def ParseJson( filepath ):
    
    # package.json path check
    
    assert( filepath )

    filepath = os.path.join( os.getcwd(), filepath )
    filepath = os.path.normpath( os.path.join( filepath, 'package.json' ) )
    filename = os.path.basename( filepath )
    
    if ( not os.path.isfile( filepath ) ):
        sys.exit( 'Error: \'' + filepath + '\' is not a valid package.json file' )

    # target properties resolution
        
    properties = Properties()
        
    try:
        with open( filepath ) as package:
        
            data = json.load( package )
            
            properties.product = data['artifact']
            properties.version = data['version']
            properties.year = data['year']
            properties.company = data['organization']['name']
                
    except: 
        sys.exit( 'Error: cannot read \'' + filepath + '\' content' )
    
    # target properties check
    
    if ( not properties.product ) or ( properties.product == '' ): 
        sys.exit( 'Error: \'artifact\' property not found in \'' + filepath + '\'' )
        
    if ( not properties.version ) or ( properties.version == '' ): 
        sys.exit( 'Error: \'version\' property not found in \'' + filepath + '\'' )
        
    if ( not properties.company ) or ( properties.company == '' ): 
        sys.exit( 'Error: \'organization.name\' property not found in \'' + filepath + '\'' )
        
    if ( not properties.year ) or ( properties.year == '' ): 
        sys.exit( 'Error: \'year\' property not found in \'' + filepath + '\'' )
        
    return properties
    
    
# Target properties generation

def GenerateInfo( filepath, properties ):

    # cbproj path check
    
    assert( filepath )
    
    filepath = os.path.normpath( os.path.join( os.getcwd(), filepath ) )
    filename = os.path.basename( filepath )
    
    if ( not os.path.isfile( filepath ) ) or ( os.path.splitext( filepath )[1] != '.cbproj' ):
        sys.exit( 'Error: \'' + filepath + '\' is not a valid Borland project file' )
                
    # target properties set

    assert( properties )

    args = '--version ' + properties.version
    args = args + ' --product "' + properties.product + '"'

    if properties.description and ( properties.description != '' ):
        args = args + ' --description "' + properties.description + '"'
                
    args = args + ' --company "' + properties.company + '"'
    args = args + ' --copyright "Copyright (C) ' + properties.year + ' ' + properties.company + '"'
                
    generate_target_info = os.path.dirname( os.path.abspath( __file__ ) )
    generate_target_info = os.path.normpath( generate_target_info + '/../../borland/generate_target_info.py' )
    
    os.system( generate_target_info + ' ' +  args + ' ' + filepath )
    
                   
# Main function
    
def main():

    if ( platform.system() != 'Windows' ): 
        sys.exit( 'Error: script not applicable on ' + platform.system() + ' platforms' )

    parser = argparse.ArgumentParser()
    parser.add_argument( 'path', metavar = '<project>', help = 'Borland project filepath' )
    parser.add_argument( '--package', metavar = '<path>', help = 'package.json directory path (required)', required = True )
    parser.add_argument( '--description', metavar = '<text>', help = 'project target description' )
    
    args = parser.parse_args()
    properties = ParseJson( args.package )
    properties.description = args.description
    
    GenerateInfo( args.path, properties )
    

if __name__ == '__main__':
    main()
    