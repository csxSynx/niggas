#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "spoofer.h"
#include <fstream>
#include <filesystem>
#include <random>
#include <psapi.h>
#include <sstream>
#include <thread>
#include <cstdlib>
#include <iphlpapi.h>
#include "dwmapi.h"
#include <wininet.h>
#include <direct.h>
#include <algorithm>
#include "xor.hpp"
#include <mutex>

std::mutex mtx;

#pragma comment(lib, "wininet.lib")
#pragma warning(disable : 4996)

std::string GenerateRandomserials(int length, bool excludeVolume) {
    std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";
    std::string randomStr;
    if (excludeVolume) {
        std::srand(std::time(nullptr));
        for (int i = 0; i < length; ++i) {
            randomStr += chars[std::rand() % chars.size()];
        }
    }
    return randomStr;
}


std::string HexStringGenerator(int length) {
    char hex_characters[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
    std::string Hex_String;
    for (int i = 0; i < length; i++) {
        Hex_String += hex_characters[rand() % 16];
    }
    return Hex_String;
}

std::string HddSerialGenerator() {
    return HexStringGenerator(4) + "-" + HexStringGenerator(4);
}

std::string GuidSerialGenerator() {
    return HexStringGenerator(8) + "-" + HexStringGenerator(4) + "-" + HexStringGenerator(4) + "-" + HexStringGenerator(4) + HexStringGenerator(12);
}

void SpoofHWID() {
    std::lock_guard<std::mutex> lock(mtx);  

    srand(time(0));

    std::string guidValue = GuidSerialGenerator();

    system(("REG ADD \"HKLM\\SOFTWARE\\Microsoft\\Cryptography\" /v MachineGuid /t REG_SZ /d \"" + guidValue + "\" /f").c_str());
    system(("REG ADD \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\IDconfigDB\\Hardware Profiles\\0001\" /v HwProfileGuid /t REG_SZ /d \"{" + guidValue + "}\" /f").c_str());
    system(("REG ADD \"HKLM\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001\" /v NetworkAddress /t REG_SZ /d \"" + HexStringGenerator(12) + "\" /f").c_str());
    system(("AMIDEWINx64.EXE /SU " + HexStringGenerator(32)).c_str());
}

std::string newUUID()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);
	static std::uniform_int_distribution<> dis2(8, 11);

	std::stringstream ss;
	ss << std::hex;
	for (int i = 0; i < 8; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (int i = 0; i < 4; i++) {
		ss << dis(gen);
	}
	ss << "-4";
	for (int i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	ss << dis2(gen);
	for (int i = 0; i < 3; i++) {
		ss << dis(gen);
	}
	ss << "-";
	for (int i = 0; i < 12; i++) {
		ss << dis(gen);
	}
	return ss.str();
}

void ChangeRegEdit()
{
    std::string value = newUUID();
    std::string regPath = "reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography\" /v MachineGuid /t REG_SZ /d \"" + value + "\" /f";
    std::thread([&] {
        system(regPath.c_str());
        }).detach();
};

void spoofing()
{
    system("AMIDEWINx64.EXE /IVN \"AMI\"");
    system("AMIDEWINx64.EXE /SP \"System product name\"");
    system("AMIDEWINx64.EXE /SV \"System product name\"");
    system(("AMIDEWINx64.EXE /SS" + GenerateRandomserials(10, false)).c_str());
    system("AMIDEWINx64.EXE /SU AUTO");
    system("AMIDEWINx64.EXE /SK \"To  Be Filled By O.E.M\"");
    system("AMIDEWINx64.EXE /SK \"To  Be Filled By O.E.M\"");
    system("AMIDEWINx64.EXE /BM \"AsRock\"");
    system("AMIDEWINx64.EXE /BP \"B560M-C\"");
    system(("AMIDEWINx64.EXE /BS" + GenerateRandomserials(14, false)).c_str());
    system("AMIDEWINx64.EXE  /BT \"Default String\"");
    system("AMIDEWINx64.EXE  /BLC \"Default String\"");
    system("AMIDEWINx64.EXE  /CM \"Default String\"");
    system("AMIDEWINx64.EXE  /CV \"Default String\"");
    system(("AMIDEWINx64.EXE /CS" + GenerateRandomserials(10, false)).c_str());
    system("AMIDEWINx64.EXE  /CA \"Default String\"");
    system("AMIDEWINx64.EXE /CSK \"SKU\"");
    system("AMIDEWINx64.EXE /PSN \"To Be Filled By O.E.M\"");
    system("AMIDEWINx64.exe /PAT \"To Be Filled By O.E.M\"");
    system("AMIDEWINx64.exe /PPn \"To Be Filled By O.E.M\"");
}