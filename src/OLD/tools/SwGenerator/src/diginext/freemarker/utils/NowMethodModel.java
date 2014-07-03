package diginext.freemarker.utils;

import java.util.Date;
import java.util.List;

import freemarker.template.SimpleScalar;
import freemarker.template.TemplateMethodModel;
import freemarker.template.TemplateModel;
import freemarker.template.TemplateModelException;
/**
 * <p>Gives FreeMarker the ability to generate current date</p>
 *
 * <p>Usage:<br />
 * From java:</p>
 * <pre>
 * SimpleHash root = new SimpleHash();
 *
 * root.put("now", new diginext.freemarker.utils.NowMethodModel()); 
 *
 * ...
 * </pre>
 *
 * <p>From your FreeMarker template:</p>
 * <pre>
 *
 * The following uuid is generated:
 * {${now()}}
 *
 * ...
 * </pre>
 *
 */

public class NowMethodModel implements TemplateMethodModel {
	
    public TemplateModel exec(List args) throws TemplateModelException {
        Date d=new Date();
    	return new SimpleScalar(d.toString());
    }
}
