/*!
 \file SwGenerator
 \brief Main entry
 \version 1.0
 \date 27/04/07
 \author F.Bighelli
*/

import java.io.File;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import diginext.freemarker.utils.CopyFileMethodModel;
import diginext.freemarker.utils.DirectoryTransformModel;
import diginext.freemarker.utils.FileTransformModel;
import diginext.freemarker.utils.GuuidMethodModel;
import diginext.freemarker.utils.NowMethodModel;
import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
import freemarker.template.TemplateException;

/*!
\class SwGenerator 
\brief Main entry
*/
public class SwGenerator {
	
	/*! \brief FreeMarker configuration */
	public static Configuration cfg;
	
	/*! \brief Main FreeMarker Template */
	public static Template main_template;
	
	/*! \brief Affichage de l'aide */
	public static void DisplayHelp() {
		System.out.println ("Usage:");
		System.out.println ("SwGenerator plugindesc.xml");
	}

	/*! \brief initialisation et configuration de FreeMarker */
	public static void Configure() throws Exception{
		//Creation
		cfg = new Configuration();
        //Ajout des add-ons diginext
		cfg.setSharedVariable("file", new FileTransformModel());	    
        cfg.setSharedVariable("folder", new DirectoryTransformModel());	    
        cfg.setSharedVariable("uuid", new GuuidMethodModel()); 
        cfg.setSharedVariable("copyfile", new CopyFileMethodModel()); 
        cfg.setSharedVariable("now", new NowMethodModel());
        //Check de l'existence du template dans les ressources
        URL url=SwGenerator.class.getClass().getResource("/Templates/main.ftl");
		if (url!=null) {
			//Si ok -> acces ressources
			cfg.setClassForTemplateLoading(SwGenerator.class.getClass(), "/Templates");
		} else {
			//Si nok -> acces fichier
			cfg.setDirectoryForTemplateLoading(new File("Templates"));
		}
		//A faire dans tous les cas
		cfg.setObjectWrapper(new DefaultObjectWrapper());
        /* Get or create a template */
		main_template = cfg.getTemplate("main.ftl");
	}
	
	
	/*! \brief Generation */
	public static void Generate(String filename) throws Exception{
		//Chargement des données
		Map root = new HashMap();
		root.put(
		        "doc",
		        freemarker.ext.dom.NodeModel.parse(new File(filename)));		
        Writer out = new OutputStreamWriter(System.out);
        //Generate
        main_template.process(root, out);
        out.flush();
	}
	
	/*! \brief Main */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println ("StreamWork Generator");
		if (args.length!=1 || args[0]=="?") {
			DisplayHelp();
			return;
		}
		//Check si le fichier xml est visible
		File xml_acces=new File(args[0]);
		if (!xml_acces.exists()) {
			System.out.println ("Unable to find "+args[0]);
			System.out.println ("Try to give an absolut path");
			return;
		}
		try {
			
			//Configuration de FreeMarker
			Configure();
		
			//Generation
			Generate(args[0]);
			
		} catch (Exception e) {
			//Montrer le probleme
			System.out.println ("Exception: "+e.getLocalizedMessage());
		}
		System.out.println ("End!");
		
	}

}
