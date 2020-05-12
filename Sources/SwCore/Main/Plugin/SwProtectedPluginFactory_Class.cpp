#include <windows.h>
#include <psapi.h>
#include <QJsonObject>
#include <QJsonDocument>

#include "SwProtectedPluginFactory_Class.h"

#include "cryptlib.h"
#include "rsa.h"
#include "hex.h"

using namespace StreamWork::SwCore;

static const std::string publicKey = "30819D300D06092A864886F70D010101050003818B0030818702818100A8B3838DAE1FC7F9F33C643BBF5A3B5B2D3E1A7C94319BD00353B8538CE6F38503B5AD74EBAF5D6BB80870ECD1D1C79BD1E735E70BD02B76BBB06184D3CA4024D87433C49006E1D9EA568F08468F990E8E9D66D3E875D9711B6A30C7EA311871DF77FD503335EDFDB1CF9B58BB8BE8855BA63162B4EDCC3EDD5CBCA5904B0F47020111";

//---------------------------------------------------------------------------------
bool StreamWork::SwCore::SwProtectedPluginFactory_Class::unlock(const char * hash) const
{
	HANDLE	process = GetCurrentProcess();
	DWORD	size;

	// Process ID
	int		processId = GetCurrentProcessId();

	// Process Name
	char processName[4096];
	size = GetModuleBaseNameA(process, NULL, processName, 4095);
	processName[size] = '\0';

	// Process Times
	FILETIME lpCreationTime;
	FILETIME lpExitTime;
	FILETIME lpKernelTime;
	FILETIME lpUserTime;
	if (!GetProcessTimes(process, &lpCreationTime, &lpExitTime, &lpKernelTime, &lpUserTime))
		return false;

	// User name
	char userName[4096];
	size = 4096;
	GetUserNameA(userName, &size);
	userName[size] = '\0';

	// Computer name
	char computerName[4096];
	size = 4096;
	GetComputerNameA(computerName, &size);
	computerName[size] = 0;

	QJsonObject jsonInfo;
	jsonInfo.insert("ProcessID", processId);
	jsonInfo.insert("ProcessName", processName);
	jsonInfo.insert("ProcessLowDateTime", (int)lpCreationTime.dwLowDateTime);
	jsonInfo.insert("ProcessHighDateTime", (int)lpCreationTime.dwHighDateTime);
	jsonInfo.insert("UserName", userName);
	jsonInfo.insert("ComputerName", computerName);

	// json info to string
	QJsonDocument doc(jsonInfo);
	QString data(doc.toJson(QJsonDocument::Compact));


	std::string hashHexDecoded;
	CryptoPP::StringSource(hash, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(hashHexDecoded)));
	std::string publicKeyBin;
	CryptoPP::StringSource(publicKey, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(publicKeyBin)));
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::PublicKey pubKey;
	pubKey.BERDecode(CryptoPP::StringStore(publicKeyBin).Ref());
	CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA>::Verifier verifier(pubKey);

	if (hashHexDecoded.length() != verifier.SignatureLength())
		return false;

	return verifier.VerifyMessage((const byte*)data.toStdString().c_str(), data.length(), (const byte*)hashHexDecoded.c_str(), hashHexDecoded.size());
}
