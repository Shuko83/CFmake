package diginext.freemarker.utils;

import java.util.List;
import java.util.UUID;
import freemarker.template.SimpleScalar;
import freemarker.template.TemplateMethodModel;
import freemarker.template.TemplateModel;
import freemarker.template.TemplateModelException;
/**
 * <p>Gives FreeMarker the ability to generate an uuid</p>
 *
 * <p>Usage:<br />
 * From java:</p>
 * <pre>
 * SimpleHash root = new SimpleHash();
 *
 * root.put("uuid", new diginext.freemarker.utils.GuuidMethodModel()); 
 *
 * ...
 * </pre>
 *
 * <p>From your FreeMarker template:</p>
 * <pre>
 *
 * The following uuid is generated:
 * {${uuid()}}
 *
 * ...
 * </pre>
 *
 */

public class GuuidMethodModel implements TemplateMethodModel {
	
    public TemplateModel exec(List args) throws TemplateModelException {
        return new SimpleScalar(UUID.randomUUID().toString().toUpperCase());
    }
}
