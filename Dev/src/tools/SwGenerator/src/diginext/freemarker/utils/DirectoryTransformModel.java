package diginext.freemarker.utils;

import java.io.File;
import java.io.IOException;
import java.io.Writer;
import java.util.Map;
import java.util.Deque;
import java.util.ArrayDeque;

import freemarker.template.SimpleScalar;
import freemarker.template.TemplateModelException;
import freemarker.template.TemplateTransformModel;

/**
 * Change current directory used par <@file> transform.
 * If selected directory doesn't exist it'll be created.
 * <b>Warning</b>: this model isn't thread-safe 
 * 
 *
 * <p>Usage:<br />
 * From java:</p>
 * <pre>
 * SimpleHash root = new SimpleHash();
 *
 * root.put( "folder", new diginext.freemarker.utils.DirectoryTransformModel() );
 *
 * ...
 * </pre>
 *
 * <p>From your FreeMarker template:</p>
 * <pre>
 *  
 * this part of text is on current output 
 * the current dir is . 
 * <@folder name="tmp">
 * <@file name="foo.html">
 * this part of text is on file ./tmp/foo.html 
 * &lt;/@file>
 * &lt;/@folder>
 * </pre>
 * 
 *
 *@see diginext.freemarker.utils.FileTransformModel
 */
public class DirectoryTransformModel implements TemplateTransformModel {
	
	private static Deque<File> stack_path;
	
	
	public static File GetCurrentPath() {
		if (stack_path==null) {
			stack_path=new ArrayDeque<File>();
			stack_path.push(new File("."));
		}
		return stack_path.peek();
	}
	public static void AddSubPath(String subpath) throws TemplateModelException{
		if (stack_path==null) {
			stack_path=new ArrayDeque<File>();
			stack_path.push(new File("."));
		}
		File new_path=new File(subpath);
		if (new_path.isAbsolute()) {
			stack_path.push(new_path);
		} else {
			stack_path.push(new File(GetCurrentPath().getAbsolutePath()+File.separatorChar+subpath));
		}
		if (!GetCurrentPath().exists()) {
			System.out.println("Creating "+GetCurrentPath().getAbsolutePath());
			if (!GetCurrentPath().mkdirs()) {
				throw new TemplateModelException( "Fail to create "+ GetCurrentPath().getAbsolutePath() +" directory");
			}
		}
		System.out.println("Entering in directory: "+GetCurrentPath().getAbsolutePath());
	}
	public static void RemoveSubPath() {
		System.out.println("Leaving in directory: "+GetCurrentPath().getAbsolutePath());
		stack_path.pop();
		System.out.println("Return to directory: "+GetCurrentPath().getAbsolutePath());
	}

	public Writer getWriter(final Writer out, Map args) throws TemplateModelException
    {
        if (stack_path==null) {
        	new ArrayDeque<String>();
        }
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
    	AddSubPath(value);
		return new DirectoryWriter(out);
    }
	
	private class DirectoryWriter extends Writer {
	       
	       
        private Writer out;
           
        DirectoryWriter (Writer out) {
            this.out = out;
        }

        public void write(char[] cbuf, int off, int len)
                throws IOException {
            out.write(new String(cbuf, off, len).toUpperCase());
        }

        public void flush() throws IOException {
            out.flush();
        }

        public void close() {
        	RemoveSubPath();
        }
	}

}
