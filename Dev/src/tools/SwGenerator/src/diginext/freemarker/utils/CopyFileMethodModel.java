package diginext.freemarker.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.List;

import freemarker.template.SimpleScalar;
import freemarker.template.TemplateMethodModel;
import freemarker.template.TemplateModel;
import freemarker.template.TemplateModelException;
/**
 * <p>Gives FreeMarker the ability to copy a file</p>
 *
 * <p>Usage:<br />
 * From java:</p>
 * <pre>
 * SimpleHash root = new SimpleHash();
 *
 * root.put("copyfile", new diginext.freemarker.utils.CopyFileMethodModel()); 
 *
 * ...
 * </pre>
 *
 * <p>From your FreeMarker template:</p>
 * <pre>
 *
 * The following uuid is generated:
 * {${copyfile("C:\\tmp\\foobar.html","foobar.html")}}
 *
 * ...
 * </pre>
 *
 */

public class CopyFileMethodModel implements TemplateMethodModel {
	
    public TemplateModel exec(List args) throws TemplateModelException {
        if( args.size() != 2 ) {
            throw new TemplateModelException( "Need two filenames to make copy" );
        }
    	File source=new File ((String)(args.get(0)));
    	File target=new File ((String)(args.get(1)));
    	if(!source.isAbsolute()) {
    		source=new File (DirectoryTransformModel.GetCurrentPath().getAbsolutePath()+File.separatorChar+(String)(args.get(0)));
    	}
    	if(!target.isAbsolute()) {
    		target=new File (DirectoryTransformModel.GetCurrentPath().getAbsolutePath()+File.separatorChar+(String)(args.get(1)));
    	}
    	if (!source.exists()) {
    		throw new TemplateModelException( "Copyfile::Source file doesn't exist" );
    	}
    	
    	if (target.exists()) {
    		target.delete();
    	}
    	try {
	        FileInputStream fis  = new FileInputStream(source);
	        FileOutputStream fos = new FileOutputStream(target);
	        byte[] buf = new byte[1024];
	        int i = 0;
	        while((i=fis.read(buf))!=-1) {
	          fos.write(buf, 0, i);
	          }
	        fis.close();
	        fos.close();   	
    	} catch (Exception e) {
    		throw new TemplateModelException( "Copyfile::Error on copy:"+e.getMessage());
    	}
        return new SimpleScalar((String)"");
    }
}
