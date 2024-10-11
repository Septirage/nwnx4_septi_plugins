/***************************************************************************


 ***************************************************************************/

#include "httplib.h"
#include "srvadmin.h"
#include <cassert>
#include <random>
#include "crypto.h"
#include <iomanip>
#include <ctime>
#include <iostream>

#define LOGUPDATE_FILE					"SEPTPlugin_UPDATE.log"
#define VERSIONTEST_MAXSUCCESSIVEFAIL	5	


/*
 * Control identifiers for the server GUI admin window.
 */

#define IDC_SERVER_NAME			0x3F8



TstVersionPlg::TstVersionPlg(std::string nwnxH, std::string vers, std::string plug)
: mainGuiWindow( NULL )
{
	nwnxHome = nwnxH;
	version = vers;
	plugin = plug;
}

TstVersionPlg::~TstVersionPlg()
{
}

/*
 * Window message interface functions.
 */

BOOL
CALLBACK
TstVersionPlg::FindServerGuiWindowEnumProc(
	__in HWND hwnd,
	__in LPARAM lParam
	)
{
	DWORD      Pid;
	WCHAR      ClassName[ 256 ];

	GetWindowThreadProcessId( hwnd, &Pid );

	if (Pid != GetCurrentProcessId())
		return TRUE;

	if (GetClassNameW( hwnd, ClassName, 256 ))
	{
		if (!wcscmp( ClassName, L"Exo - BioWare Corp., (c) 1999 - Generic Blank Application"))
		{
			*(HWND *)lParam = hwnd;
			return FALSE;
		}
	}

	return TRUE;
}

HWND
TstVersionPlg::FindServerGuiWindow(
	)
{
	HWND hwnd;

	if (mainGuiWindow)
		return mainGuiWindow;

	hwnd = 0;

	EnumWindows( FindServerGuiWindowEnumProc, (LPARAM)&hwnd );

	if (hwnd)
		mainGuiWindow = hwnd;

	return hwnd;
}


//Really ugly.... Need to rework the whole thing
void TstVersionPlg::prepareMessage(std::vector<char> *message)
{
	//Prepare the message. Grab some data (server name) to allow help on debug.

	HWND SrvWnd = FindServerGuiWindow(); //Tested with service mode too. It work, no problem here.
	int iReserved = 0;

	char a[50];
	//Yes, i know.. I use it only for the server name for now. Probably need to do it in another way
	HWND ServerNameBox = GetDlgItem(SrvWnd, IDC_SERVER_NAME); 
	GetWindowTextA(ServerNameBox, a, 50);

	std::string sMessage2 = "";
	std::string srvName(a);
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(0, 255);
	sMessage2 += (char)uniform_dist(e1);
	sMessage2 += (char)uniform_dist(e1);
	sMessage2 += std::to_string(iReserved); //reserved for latter usage
	sMessage2 += "|";
	sMessage2 += version; //Current version of the plugin
	sMessage2 += "|";
	sMessage2 += std::to_string(srvName.size()); 
	sMessage2 += "|";
	sMessage2 += srvName; // Allow to help on debug
	sMessage2 += "|";
	sMessage2 += std::to_string(plugin.size());
	sMessage2 += "|";
	sMessage2 += plugin; //The plugin  to test.

	int paddedMessageLen = sMessage2.size();

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}
	unsigned char* encryptedMessage = new unsigned char[paddedMessageLen];

	AESEncryptB(sMessage2, key, encryptedMessage);
	for (int ip = 0; ip < paddedMessageLen; ip++)
	{
		message->push_back(encryptedMessage[ip]);
	}
	delete[] encryptedMessage;
}

void TstVersionPlg::TestVersionPlugin()
{
	HWND SrvWnd = 0;
	while (!SrvWnd)
	{
		Sleep(1000 * 10 * 6);
		SrvWnd = FindServerGuiWindow();
	}

	bool retVal = false;

	bool forced = false;
	unsigned char decryptedMessage[1024];

	int iNumberOfSuccessiveFail = 0;
	/*
	*/
	int iTime = 0;
	while(true)
	{
		bool msgReceived = true;
        int nWait = 30;
		{
			//On fait la clef

			std::random_device r;
			std::default_random_engine e1(r());
			std::uniform_int_distribution<int> uniform_dist(0, 255);
			for (int ik = 0; ik < 16; ik++)
			{
				int valK = uniform_dist(e1);
				key[ik] = (unsigned char)valK;
			}

			std::vector<char> message;
			long long* encrypted = new long long[16];
			//encrypted = rsa_encrypt(key, 16, &pub);
			encrypted = (long long *) &key[0];
			for (int ik = 0; ik < 16; ik++)
			{
				message.push_back(key[ik]);
			}
			prepareMessage(&message);

			httplib::Client cli("http://nwn2.septirage.com");
			auto res = cli.Post("/version.php", message.data(), message.size(), "application/octet-stream");

			if (res.error() != httplib::Error::Success)
			{
				msgReceived = false;
				nWait = 1;
			}
			else
			{
				iNumberOfSuccessiveFail = 0;
				size_t responseLenght = res->body.size();
				//int length = message.size();
				int length = res->body.size();
				char * buffer = new char [length];

			
				std::memcpy ( buffer, res->body.data(), responseLenght);

				if (length > 0)
				{
					AESDecryptB(key, (unsigned char*) buffer, decryptedMessage);


					/////////////////////////////////////////////////////////////////////////////////////////////
					// 
					// First two bytes are reserved for now
					// 
					/////////////////////////////////////////////////////////////////////////////////////////////



					int nSizeDat = 0;
					int npos = 2;

					/////////////////////////////////////////////////////////////////////////////////////////////
					// 
					// Grab the current up to date version
					// 
					/////////////////////////////////////////////////////////////////////////////////////////////

					// Size of versionString
					while (decryptedMessage[npos] != '|')
					{
						nSizeDat = (nSizeDat*10)+(decryptedMessage[npos] - '0');
						npos++;
					}
					/////////////////////////////////////////////////////////////////////////////////////////////////
					//Pass the |
					npos++;

					// Grab The versionString
					std::string sVersionMaj = "";
					//Size
					for (int ij = 0; ij < nSizeDat && (npos +ij) < length; ij++)
					{
						sVersionMaj += decryptedMessage[npos + ij];
					}
					npos += nSizeDat;
					//As the webservice is always up to date, if sended version is different than actual, you need to update.
					if (sVersionMaj != version)
					{

						std::ofstream myfile;
						std::string logfile(nwnxHome);
						logfile.append("\\");
						logfile.append(LOGUPDATE_FILE);
						myfile.open (logfile, std::ios::out | std::ios::app);
						auto t = std::time(nullptr);
						auto tm = *std::localtime(&t);
						myfile << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "]" << " Plugin " << plugin << " is out of date. You have v" << version << ". The current up to date version : v" << sVersionMaj << "\n";
						myfile.close();
					}



					nSizeDat = 0;

					/////////////////////////////////////////////////////////////////////////////////////////////
					// 
					// Grab option value
					// 
					/////////////////////////////////////////////////////////////////////////////////////////////


					while (npos < length && decryptedMessage[npos] != '|')
					{
						nSizeDat = (nSizeDat*10)+(decryptedMessage[npos] - '0');
						npos++;
					}
					//Pass the |
					npos++;
					//Used for now to reduce the looping time after first successfull request. Can be used
					//To allow special conf by server.
					//Can also be used if I know that an update will came soon.
					nWait = 0;
					for (int ij = 0; ij < nSizeDat; ij++)
					{
						nWait = (nWait*10)+(decryptedMessage[npos + ij] - '0');
					}
					iTime = nWait;
					delete[] buffer;

				}
			}
		}


		if (msgReceived == false)
		{

			std::ofstream myfile;
			std::string logfile(nwnxHome);
			logfile.append("\\");
			logfile.append(LOGUPDATE_FILE);
			myfile.open (logfile, std::ios::out | std::ios::app);
			auto t = std::time(nullptr);
			auto tm = *std::localtime(&t);

			if (iNumberOfSuccessiveFail >= VERSIONTEST_MAXSUCCESSIVEFAIL)
			{
				myfile << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "]" << " Plugin " << plugin << " Too many failed attempts to contact the webservice. No more try for this run.\n";
				myfile.close();
				return;
			}
			else
			{
				myfile << "[" << std::put_time(&tm, "%d-%m-%Y %H-%M-%S") << "]" << " Plugin " << plugin << " Can't access the webservice.\n";
				myfile.close();
				iNumberOfSuccessiveFail++;
			}
		}

		//Wait before next try
		Sleep(1000 * 60 * nWait);
	}
	return;
}


