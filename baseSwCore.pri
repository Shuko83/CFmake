#Pour Generation en relatif PROJECTS_PATH doit être définit
isEmpty( PROJECTS_PATH ) {
	PROJECTS_PATH = C:/Projects
	warning(PROJECTS_PATH is empty !!!)
}

SWPATH = $$PROJECTS_PATH/Libraries/StreamWork/src/SwCore
SWSERVICEPATH = $$PROJECTS_PATH/Libraries/StreamWork/src/SwService

INCLUDEPATH += ./ \
				$$SWPATH/ \
				$$SWPATH/Component/\
				$$SWPATH/Component/Base/\
				$$SWPATH/Component/Interfaces/\
				$$SWPATH/Component/Pin/\
				$$SWPATH/Component/Services/\
				$$SWPATH/Component/Services/ServiceImpl/\
				$$SWPATH/Extensions/ \
				$$SWPATH/Main/ \
				$$SWPATH/Main/Connexion/ \
				$$SWPATH/Main/Plugin/ \
				$$SWPATH/Main/Serialization \
				$$SWPATH/Main/Services/ \
				$$SWPATH/Main/Services/Management/ \
				$$SWPATH/Properties/ \
				$$SWPATH/SwRef/ \
				$$SWPATH/Tools/ \
				$$SWPATH/Tools/Exception/ \
				$$SWPATH/Tools/Log/ \
				$$SWPATH/Tools/Signal/ \
				$$SWPATH/Types/ \
				$$SWPATH/Types/UserType/ \
				$$SWSERVICEPATH/ServiceAlert/ \
				$$SWSERVICEPATH/ServiceConfiguration/ \
				$$SWSERVICEPATH/ServiceImpl/ \
				$$SWSERVICEPATH/ServiceShortcut/ \
				$$SWSERVICEPATH/ServiceUnitSI/ \
