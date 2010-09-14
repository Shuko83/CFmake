package diginext.freemarker.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Map;

import freemarker.template.SimpleScalar;
import freemarker.template.TemplateModelException;
import freemarker.template.TemplateTransformModel;


/**
 * Set text output targeted on named file created on current directory.
 * 
 *
 * <p>Usage:<br />
 * From java:</p>
 * <pre>
 * SimpleHash root = new SimpleHash();
 *
 * root.put( "file", new diginext.freemarker.utils.FileTransformModel() );
 *
 * ...
 * </pre>
 *
 * <p>From your FreeMarker template:</p>
 * <pre>
 *  
 * this part of text is on current output
 * <@file name="foo.html">
 * this part of text is on file foo.html 
 * &lt;/@file>
 * </pre>
 *
 * To change current directory
 * @see diginext.freemarker.utils.DirectoryTransformModel
 */
public class FileTransformModel implements TemplateTransformModel {
	
	public Writer getWriter(final Writer out, Map args) throws TemplateModelException
    {
        if (args.size()!=1) {
        	throw new TemplateModelException( "Need an argument to execute" );
        }
        if (!args.containsKey((String)"name")) {
        	throw new TemplateModelException( "Need a name parameter" );
        }
        SimpleScalar svalue=(SimpleScalar)args.get((String)"name");
        String value=svalue.getAsString();
    	if (value.isEmpty()) {
        	throw new TemplateModelException( "Invalid value for name" );
    	}   	
		return new FileWriter(out,DirectoryTransformModel.GetCurrentPath().getAbsolutePath()+File.separatorChar+value);
    }
	
	private class FileWriter extends Writer {
	       
        //private Writer prec_out;
        private Writer out;
        private FileOutputStream output_stream;
        
        FileWriter (Writer out,String filename) {
        	//la ligne suivante est utilisé pour le test
        	System.out.println("Generating file "+filename);
        	File current_file=new File(filename);
        	if (current_file.exists()) {
        		current_file.delete();
        	}
        	try {
            	//this.prec_out = out;
        		this.output_stream=new FileOutputStream(current_file);
        		this.out=new OutputStreamWriter(output_stream);
        	} catch (IOException e) {
            	this.out = out;        		
        	}
        }

        public void write(char[] cbuf, int off, int len)
                throws IOException {
            out.write(new String(cbuf, off, len));
            //la ligne suivante est utilisé pour le test
            //prec_out.write(new String(cbuf, off, len));
        }

        public void flush() throws IOException {
            out.flush();
        }

        public void close(){
           	//la ligne suivante est utilisé pour le test
           	//System.out.println("Leaving file ");
           	try {
                out.flush();
           		output_stream.flush();
           		output_stream.close();
           	} catch(IOException e) {
           		System.out.println("Error when closing output stream");   		
        	}
        }
    }	
}
