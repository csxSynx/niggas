#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <cstdint>

std::string GenerateRandomserials(int length, bool excludeVolume);
std::string HexStringGenerator(int length);
std::string HddSerialGenerator();
std::string GuidSerialGenerator();
std::string newUUID();

void SpoofHWID();
void ChangeRegEdit();
void spoofing();