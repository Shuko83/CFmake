from lxml import etree as XML
from datetime import date
import argparse, json, os, platform, sys

# Common types definitions

class Properties:

    version = ''
    product = ''
    description = ''


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

    if ( not properties.version ) or ( properties.version == '' ):
    
        properties.version = 0
        
        if os.environ.has_key( 'VERSION' ):
        
            properties.version = os.environ.get( 'VERSION' )
            
            if os.environ.has_key( 'BUILDNUMBER' ):
                properties.version = properties.version + '.' + os.environ.get( 'BUILDNUMBER' )
            
    if ( not properties.product ) or ( properties.product == '' ):
    
        properties.product = str.replace( filename, os.path.splitext( filepath )[1], '' )
        
        if os.environ.has_key( 'PRODUCT' ):
            properties.product = os.environ.get( 'PRODUCT' )
        
    args = '--version "' + properties.version + '"'        
    args = args + ' --product "' + properties.product + '"'

    if properties.description and ( properties.description != '' ):
        args = args + ' --description "' + properties.description + '"'
                
    args = args + ' --company "Diginext"'
    args = args + ' --copyright "Copyright (C) ' + str( date.today().year ) + ' Diginext"'
    
    generate_target_info = os.path.dirname( os.path.abspath( __file__ ) )
    generate_target_info = os.path.normpath( generate_target_info + '/../../borland/generate_target_info.py' )
    
    os.system( generate_target_info + ' ' + args + ' ' + filepath )
    
                   
# Main function
    
def main():

    if ( platform.system() != 'Windows' ): 
        sys.exit( 'Error: script not applicable on ' + platform.system() + ' platforms' )

    parser = argparse.ArgumentParser()
    parser.add_argument( 'path', metavar = '<project>', help = 'Borland project filepath' )
    parser.add_argument( '--version', metavar = '<number>', help = 'target version number' )
    parser.add_argument( '--product', metavar = '<name>', help = 'product name for the project target' )
    parser.add_argument( '--description', metavar = '<text>', help = 'target description' )
    
    args = parser.parse_args()
    properties = Properties()
    
    properties.version = Version( args.version )
    properties.product = args.product
    properties.description = args.description
    
    GenerateInfo( args.path, properties )
    

if __name__ == '__main__':
    main()
    