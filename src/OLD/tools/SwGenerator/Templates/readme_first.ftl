<#assign project_name= doc.SwFactoryEntry.@Name>

Pour generer directement le plugin:
faire a la racine
qmake ${project_name}.pro
puis:
nmake all
nmake install

si nmake n'est pas trouvé, faire:
"C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

si vous souhaitez editer le projet dans visual,
il faut créer un fichier projet pour visual. pour ce faire:
qmake -tp vc -o ${project_name}.vcproj ${project_name}.pro


!!!ATTENTION!!!:
la variable d'environnement SW_DIR doit etre defini:
SW_DIR=C:\Projects\Libraries\StreamWork\Dev
ou
SW_DIR=C:\Projects\Libraries\StreamWork\Stable
mettre dans le path système :
$(SW_DIR)\bin\

