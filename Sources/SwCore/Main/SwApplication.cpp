/*!
\file SwApplication.cpp
\date 12/04/2006
\brief Classe d'une application StreamWork
\author  Big
\version 1.0
*/

#include <QApplication>
#include <QDir>
#include <QTextCodec>
#include <QXmlReader>
#include <QXmlStreamWriter>

#include "SwApplication.h"
#include "_SwPluginsBank_Class.h"
#include "_SwComplexeTypeAdaptersFactoriesBankImpl.h"
#include "SwLoader_Class.h"
#include "SwEnum.h"
#include "SwInteger.h"
#include "SwString.h"
#include "SwIntegerEnum.h"
#include "SwDouble.h"
#include "SwUUID.h"
#include "SwMacros.h"
#include "SwFileDescriptor.h"
#include "SwIconDescriptor.h"
#include "SwIpV4Address.h"
#include "ISwCheckService.h"
#include "cryptlib.h"
#include "rsa.h"
#include "hex.h"
#include "ProductLicense.h"

using namespace StreamWork::SwCore;
using namespace std;

static const std::string publicKey = "30819D300D06092A864886F70D010101050003818B0030818702818100A8B3838DAE1FC7F9F33C643BBF5A3B5B2D3E1A7C94319BD00353B8538CE6F38503B5AD74EBAF5D6BB80870ECD1D1C79BD1E735E70BD02B76BBB06184D3CA4024D87433C49006E1D9EA568F08468F990E8E9D66D3E875D9711B6A30C7EA311871DF77FD503335EDFDB1CF9B58BB8BE8855BA63162B4EDCC3EDD5CBCA5904B0F47020111";
static const QString START_SIGNATURE = QStringLiteral("<Signature>");
static const QString END_SIGNATURE = QStringLiteral("</Signature>");

//-----------------------------------------------------------------------
SwApplication::SwApplication()
	: SwServicesManager_Class()
	, _is_launch(false)
	, _isCheck(false)
	, _logTime(false)
	, _autoStart(false)
	, _developerMode(false)
	, _runtimeMode(false)
	, _productLicense (nullptr)
{
#ifndef NO_LICENSE
	_productLicense = new ProductLicense(licenseId::ProductId::Product_STREAMWORK);
	if (!_productLicense->takeCore())
	{
		LAUNCH_SWEXCEPTION("SwCore", "No licence for StreamWork could be found.");
	}
	if (_productLicense->takeFeature(licenseId::FunctionId::Function_DEVELOPPER))
	{
		_developerMode = true;
	}
	else if (_productLicense->takeFeature(licenseId::FunctionId::Function_RUNTIME))
	{
		_runtimeMode = true;
	}
	else
	{
		LAUNCH_SWEXCEPTION("SwCore", "No feature runtime or developper could be found.");
	}
#endif

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	_startPath = QDir::currentPath();

	if (dynamic_cast<QApplication *>(qApp) != NULL)
		_isGuiApp = true;
	else
		_isGuiApp = false;

	//Initialisation des types
	Initialize_Types();
	//Pas d'affichage
	_isVerboseMode = false;
	//Pas d'executeur
	_executor = NULL;
	//Finalisation de l'initialisation
	_initialisationFinalized = false;
	//Creation de la banque de plugin
	_bank = new _SwPluginsBank_Class(_productLicense);
	//Creation de la banque des fabriques des adatpateurs de type complexe
	_ctadaptersbank = new _SwComplexeTypeAdaptersFactoriesBankImpl;
	//Pas de stream
	_streams.clear();
	_current_stream = _streams.begin();
	//Compteur d'historique a 1
	_historic_counter = Q_UINT64_C(1);
}

//-----------------------------------------------------------------------
SwApplication::~SwApplication()
{
	delete _bank;
	delete _ctadaptersbank;
	delete _productLicense;
}

//-----------------------------------------------------------------------
SwApplication * SwApplication::GetInstance()
{
	static SwApplication singleton;
	return &singleton;
}

//-----------------------------------------------------------------------
const QString & SwApplication::GetApplicationDirPath()
{
	if(_applicationDirPath.isEmpty())
		SetApplicationDirPath(qApp->applicationDirPath()); // Valeur par defaut
	return _applicationDirPath;
}

//-----------------------------------------------------------------------
void SwApplication::SetApplicationDirPath(const QString & path)
{
	qApp->setProperty("appDirPath", path);
	//enregistrement du path
	_applicationDirPath = path;
	//ajout des paths pour la banque de plugins
	if (QDir::isRelativePath(_applicationDirPath))
	{
		_bank->AddPath(QDir::current().absolutePath() + "/" + _applicationDirPath);
	}
	else
	{
		_bank->AddPath(_applicationDirPath);
	}
	//_bank->AddPath(_applicationDirPath + "/sw_plugins");
}

//-----------------------------------------------------------------------
QString SwApplication::GetWorkingPath()
{
	return _startPath;
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwApplication::isValidSignature(QString stream_desc) const
{
	QString minifiedStream;
	QString signature;

	QXmlStreamReader reader(stream_desc);
	QXmlStreamWriter writer(&minifiedStream);

	bool inStreamElement = false;
	while (!reader.atEnd()) {
		reader.readNext();
		if (reader.tokenType() == QXmlStreamReader::StartElement && reader.name() == CG_SW_XML_STREAM_NODE)
		{
			inStreamElement = true;
		}
		else if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == CG_SW_XML_STREAM_NODE)
		{
			writer.writeCurrentToken(reader);
			inStreamElement = false;
		}
		if (inStreamElement && !reader.isWhitespace())
		{
			writer.writeCurrentToken(reader);
		}
		if (reader.tokenType() == QXmlStreamReader::StartElement && reader.name() == CG_SW_XML_STREAMSIGNATURE_NODE)
		{
			signature += reader.readElementText();
		}
	}

	return isValidSignature(minifiedStream, signature);
}

//-----------------------------------------------------------------------
int SwApplication::Launch(QString stream_desc) throw(SwException)
{
	int result = 0;
	QString xml_error;
	int error_line, error_column;
	SwComponent_ClassPtr root_component;
	FinalizeInitialisation();
	if (_is_launch)
	{
		LAUNCH_SWEXCEPTION("SwCore", "Only one launch can be done simultaneously");
	}
	if (_runtimeMode && !isValidSignature(stream_desc))
	{
		LAUNCH_SWEXCEPTION("SwCore", "Signature not valid");
	}
	//Lancement effectuée
	_is_launch = true;
	//creation du document xml
	QDomDocument doc;
	//parsing xml du document
	if (!doc.setContent(stream_desc, &xml_error, &error_line, &error_column))
	{
		QString msg = QString("XML Parsing:%1 at position %2,%3\n%4").arg(xml_error).arg(error_line).arg(error_column).arg(stream_desc);
		LAUNCH_SWEXCEPTION("SwCore", msg)
	}
	//Construction du stream
	SwLoader_Class loader;
	if (IsVerbose())
		Logger().Log(LogLvl_Info, QString("Loading stream\n"));
	root_component = loader.Load(doc);
	if (!root_component)
	{
		LAUNCH_SWEXCEPTION("SwCore", "Unable to build stream or stream is empty");
	}
	//Enregistrement du composant
	if (IsVerbose())
		Logger().Log(LogLvl_Info, QString("Registering stream\n"));
	AddNewStream(root_component);
	//Si c'est une application GUI lancer la boucle d'execution
	if (IsVerbose())
		Logger().Log(LogLvl_Info, QString("Launching application execution\n"));
	//Check service
	ISwCheckService * cservice = dynamic_cast<ISwCheckService *>(QueryService(CG_SW_CHECK_SERVICE));
	if (cservice != 0)
	{
		cservice->check();
	}
	//Indication des mises a jours
	if (_bank->updateHaveDone())
	{
		_bank->displayUpdate();
	}
	//Lancement
	if (_isGuiApp)
	{
		if (_executor)
			result = _executor->StreamExecute();
		result = qApp->exec();
	}
	else
	{
		//Sinon execution du stream
		if (_executor)
		{
			result = _executor->StreamExecute();
		}
		else
		{
			result = -1;
			if (IsVerbose())
				Logger().Log(LogLvl_Warning, QString("No Executor found! \n"));
		}
	}
	if (_executor)
	{
		_executor->StreamStop();
	}
	//Fin
	if (IsVerbose())
		Logger().Log(LogLvl_Info, QString("Destroying all streams\n"));
	//Destruction des streams
	_streams.clear();
	//Destruction du singleton
	if (IsVerbose())
		Logger().Log(LogLvl_Info, QString("End of application execution\n"));
	root_component = nullptr;

	_is_launch = false;
	//Retour
	return result;
}

//-----------------------------------------------------------------------
void SwApplication::FinalizeInitialisation()
{
	if (_initialisationFinalized)
		return;

	if (_applicationDirPath.isEmpty())
		SetApplicationDirPath(qApp->applicationDirPath()); // Valeur par defaut

	QMap<QString, SwPluginFactory_Class *> * plugins = _bank->GetAllPlugins();
	QMap<QString, SwPluginFactory_Class *>::iterator it = plugins->begin();
	while (it != plugins->end())
	{
		it.value()->FinalizeInitialisation();
		++it;
	}
	//Fin
	_initialisationFinalized = true;
}

//-----------------------------------------------------------------------
void SwApplication::RegisterExecutor(ISwExecutor * executor)
{
	_executor = executor;
}

//-----------------------------------------------------------------------
ISwPluginsBank & SwApplication::ComponentsBank()
{
	return *_bank;
}

//-----------------------------------------------------------------------
ISwComplexeTypeAdaptersFactoriesBank & SwApplication::CTFactoriesBank()
{
	return *_ctadaptersbank;
}

//-----------------------------------------------------------------------
ISwLogger & SwApplication::Logger()
{
	return _logger;
}

//-----------------------------------------------------------------------
ISwAlerter & SwApplication::Alerter()
{
	return _alerter;
}

//-----------------------------------------------------------------------
void SwApplication::Verbose()
{
	_isVerboseMode = true;
}

//-----------------------------------------------------------------------
bool SwApplication::IsVerbose() const
{
	return _isVerboseMode;
}

//-----------------------------------------------------------------------
void SwApplication::enableLogTime()
{
	_logTime = true;
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwApplication::logTime() const
{
	return _logTime;
}

//-----------------------------------------------------------------------
void SwApplication::enableDeveloperMode()
{
	_developerMode = true;
}

//-----------------------------------------------------------------------
bool StreamWork::SwCore::SwApplication::developerMode() const
{
	return _developerMode;
}

//-------------------------------------------------------------------------
bool StreamWork::SwCore::SwApplication::isValidSignature(QString message, QString signature) const
{
	std::string publicKeyBin;
	CryptoPP::StringSource stringSourcePublicKey(publicKey, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(publicKeyBin)));
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::PublicKey pubKey;
	pubKey.BERDecode(CryptoPP::StringStore(publicKeyBin).Ref());
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Verifier verifier(pubKey);

	byte *signatureDecoded = new byte[verifier.MaxSignatureLength()];
	CryptoPP::StringSource stringSource(signature.toStdString(), true, new CryptoPP::HexDecoder(new CryptoPP::ArraySink(signatureDecoded, verifier.MaxSignatureLength())));

	return verifier.VerifyMessage(reinterpret_cast<const byte*>(message.toUtf8().constData()), message.toUtf8().size(), signatureDecoded, verifier.MaxSignatureLength());
}

//-----------------------------------------------------------------------
void StreamWork::SwCore::SwApplication::enableAutoStart()
{
	_autoStart = true;
}

//-----------------------------------------------------------------------
void SwApplication::AddNewStream(SwComponent_Class * stream_root)
{
	_streams.insert(SwComponent_ClassPtr(stream_root));
	/*QString val=""; QBN ? pourquoi le service parameter ?
	ISwServiceParameters * serviceParameters = dynamic_cast<ISwServiceParameters*>(QueryService(CG_SW_SERVICE_PARAMETER_MANAGER));
	if(serviceParameters && serviceParameters->getParameter("Check",val))
	{
	if(!_isCheck)
	{
	//Check service
	ISwCheckService * cservice=dynamic_cast<ISwCheckService *>(QueryService(CG_SW_CHECK_SERVICE));
	if (cservice!=0) {
	cservice->check();
	}
	_isCheck = true;
	}

	}*/
}

//-----------------------------------------------------------------------
SwComponent_Class * SwApplication::CreateNewStream(QString name_of_stream)
{
	SwComponent_ClassPtr component = _bank->CreateComponent(QString(), QString());
	component->SetName(name_of_stream);
	_streams.insert(component);
	return component;
}

//-----------------------------------------------------------------------
SwComponent_Class * SwApplication::CreateNewStream(QString name_of_stream, QString plugin_name, QString component_type)
{
	SwComponent_ClassPtr component = _bank->CreateComponent(plugin_name, component_type);
	component->SetName(name_of_stream);
	_streams.insert(component);
	return component;
}

//-----------------------------------------------------------------------
void SwApplication::DestroyStream(SwComponent_Class * stream_root)  throw(SwException)
{
	QSet<SwComponent_ClassPtr>::iterator it;

	it = _streams.find(SwComponent_ClassPtr(stream_root));
	if (it == _streams.end())
	{
		//LAUNCH_SWEXCEPTION("SwCore","Trying to destroy undefined stream");            
	}
	else
	{
		if (_current_stream == it) _current_stream++;
		//(*it)->Release();
		_streams.erase(it);
	}
}

//-----------------------------------------------------------------------
SwComponent_Class * SwApplication::GetFirstStream()
{
	SwComponent_Class * stream;
	_current_stream = _streams.begin();
	//Sommes nous la fin
	if (_current_stream == _streams.end())
	{
		return NULL;//Oui
	}
	//Non, recuperation de l'enfant
	stream = *_current_stream;
	//Incrementation
	++_current_stream;
	//Renvoie de l'enfant
	return stream;
}

//-----------------------------------------------------------------------
SwComponent_Class * SwApplication::GetNextStream()
{
	SwComponent_Class * stream;
	//Sommes nous la fin
	if (_current_stream == _streams.end())
	{
		return NULL;//Oui
	}
	//Non, recuperation de l'enfant
	stream = *_current_stream;
	//Incrementation
	++_current_stream;
	//Renvoie de l'enfant
	return stream;
}
//------------------------------------------------------------------------
// Demarrage externe
//------------------------------------------------------------------------

//-----------------------------------------------------------------------
bool SwApplication::LaunchAutoStart()
{
	if (_autoStart && _executor)
	{
		_executor->StreamExecute();
		_executor = 0;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------
void SwApplication::StopLaunch()
{
	if (_executor != 0)
	{
		_executor->StreamStop();
		_executor = 0;
	}
}

//-----------------------------------------------------------------------
quint64 SwApplication::GetHistoricCpt()
{
	return _historic_counter++;
}

//-----------------------------------------------------------------------
void SwApplication::SetHistoricCpt(quint64 value)
{
	_historic_counter = std::max(value + 1, _historic_counter);
}

//-----------------------------------------------------------------------
void SwApplication::Initialize_Types()
{
	//Enregistrement de SwEnum
	qRegisterMetaType<StreamWork::SwCore::SwEnum>("StreamWork::SwCore::SwEnum");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwEnum>("StreamWork::SwCore::SwEnum");
	//Enregistrement de SwIntegerEnum
	qRegisterMetaType<StreamWork::SwCore::SwIntegerEnum>("StreamWork::SwCore::SwIntegerEnum");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwIntegerEnum>("StreamWork::SwCore::SwIntegerEnum");
	//Enregistrement de SwString
	qRegisterMetaType<StreamWork::SwCore::SwString>("StreamWork::SwCore::SwString");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwString>("StreamWork::SwCore::SwString");
	//Enregistrement de SwInteger
	qRegisterMetaType<StreamWork::SwCore::SwInteger>("StreamWork::SwCore::SwInteger");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwInteger>("StreamWork::SwCore::SwInteger");
	//Enregistrement de SwDouble
	qRegisterMetaType<StreamWork::SwCore::SwDouble>("StreamWork::SwCore::SwDouble");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwDouble>("StreamWork::SwCore::SwDouble");
	//Enregistrement de SwUUID
	qRegisterMetaType<StreamWork::SwCore::SwUUID>("StreamWork::SwCore::SwUUID");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwUUID>("StreamWork::SwCore::SwUUID");
	//Enregistrement de SwFileDescriptor
	qRegisterMetaType<StreamWork::SwCore::SwFileDescriptor>("StreamWork::SwCore::SwFileDescriptor");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwFileDescriptor>("StreamWork::SwCore::SwFileDescriptor");
	//Enregistrement de SwIconDescriptor
	qRegisterMetaType<StreamWork::SwCore::SwIconDescriptor>("StreamWork::SwCore::SwIconDescriptor");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwIconDescriptor>("StreamWork::SwCore::SwIconDescriptor");
	//Enregistrement de SwEnum
	qRegisterMetaType<StreamWork::SwCore::SwIpV4Address>("StreamWork::SwCore::SwIpV4Address");
	//Et de ses methodes de serialisation
	qRegisterMetaTypeStreamOperators<StreamWork::SwCore::SwIpV4Address>("StreamWork::SwCore::SwIpV4Address");

}

//-----------------------------------------------------------------------
void SwApplication::raiseQueryError(QString serviceName)
{
	if (_missingServices.find(serviceName) == _missingServices.end())
	{
		if (SW_APP->IsVerbose()) SW_APP->Logger().Log(LogLvl_Debug, QString("the service " + serviceName + " hasn't been loaded, check the presence of the dll"));
		_missingServices.insert(serviceName);
	}
}
