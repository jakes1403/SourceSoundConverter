/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// Copyright 2019-2020 jakers1403

// SourceSoundConverter.cpp : Converts a sounds folder into a vsndevts file

#include <iostream>
#include <fstream>
#include <filesystem>

#include "LuaIntegration\LuaIntegration.h"

namespace fs = std::filesystem;

#include <algorithm>
#include <vector>
#include <string>

using namespace std;


bool hasNumberAtEnd(string toCheck)
{
	int lengthOfString = toCheck.length();
	return isdigit(toCheck.at(lengthOfString - 1));
}

string removeNumberAtEnd(string withNum)
{
	while (hasNumberAtEnd(withNum))
	{
		withNum = withNum.substr(0, withNum.size() - 1);
	}
	return withNum;
}

bool isNameVariation(string name)
{
	string noNumAtEnd = removeNumberAtEnd(name);
	return noNumAtEnd.back() != '_' && hasNumberAtEnd(name);
}


std::string FMgt_RemoveExtension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

int main(int argc, char* argv[])
{
	cout << "Source Sound Converter By Jakes1403/Jakers1403\n";
	LI_Init();
	LI_ExecuteLua("config.lua");

	bool useVariations = LI_GetBoolVarValue("useVariations");
	bool usePerFileFunction = LI_GetBoolVarValue("perFileFunction");
	string soundType = LI_GetStringVarValue("soundType");
	string outputFileName = LI_GetStringVarValue("outputFileName");
	string inputPath = LI_GetStringVarValue("inputPath");
	float soundVolume = LI_GetFloatVarValue("volume");
	float soundHRTF = LI_GetFloatVarValue("hrtf");

	if (inputPath == "path_to_sounds_folder")
	{
		string throwaway;
		cout << "Please edit your config.lua file to point to your sounds folder.\nPress any key to continue.";
		getline(cin, throwaway);
		return 0;
	}

	ofstream fileStream;
	
	fileStream.open(outputFileName);

	const std::filesystem::path path = inputPath;

	fileStream << "{\n";

	vector<string> doneVariations;

	bool lastWasVariation = false;

	bool lastWasNewVariation = false;

	for (auto& p : fs::recursive_directory_iterator(inputPath))
	{
		string inGameName = FMgt_RemoveExtension(fs::relative(p.path(), inputPath).u8string());
		string nameOfFile = inGameName;

		bool isFolder = fs::is_directory(p.path());

		std::replace(inGameName.begin(), inGameName.end(), '\\', '.');
		std::replace(nameOfFile.begin(), nameOfFile.end(), '\\', '/');
		bool foundDash = false;
		bool foundSpace = false;
		if (inGameName.find('-') != std::string::npos)
			foundDash = true;
		if (inGameName.find(' ') != std::string::npos)
			foundSpace = true;
		if (foundDash || foundSpace)
		{
			cout << nameOfFile << " has spaces or dashes. Skipping!\n";
		}
		string overrideSoundType = soundType;
		float overideVolume = soundVolume;
		float overideHRTF = soundHRTF;
		if (usePerFileFunction)
		{
			LI_SetStringVarValue("currentFilePathNoExt", nameOfFile);
			LI_SetStringVarValue("currentInGameName", inGameName);
			LI_RunFunction("perFile");
			overideVolume = LI_GetFloatVarValue("volume");
			overideHRTF = LI_GetFloatVarValue("hrtf");
			if (LI_GetBoolVarValue("overideInGameName"))
			{
				inGameName = LI_GetStringVarValue("inGameName");
			}
		}
		if (useVariations)
		{
			bool isVariation = isNameVariation(inGameName);

			fileStream << isVariation << lastWasVariation << lastWasNewVariation << "\n";
			if (!isFolder && !foundDash && !foundSpace && isVariation)
			{
				lastWasVariation = true;
				if (!(std::count(doneVariations.begin(), doneVariations.end(), removeNumberAtEnd(inGameName))))
				{
					lastWasNewVariation = true;
					doneVariations.push_back(removeNumberAtEnd(inGameName));
					fileStream << "\t" << removeNumberAtEnd(inGameName) << " = \n\t{\n\t\ttype = \"hlvr_default_3d\"\n\t\tvolume = 1.000000\n\t\tuse_hrtf = 1.000000\n\t\tvsnd_files =\n\t\t[\n";
					fileStream << "\t\t\t\"sounds/" << nameOfFile << ".vsnd\",\n";
				}
				else
				{
					lastWasNewVariation = false;
					fileStream << "\t\t\t\"sounds/" << nameOfFile << ".vsnd\",\n";
				}
			}
			if (!isVariation && lastWasVariation && lastWasNewVariation)
			{
				fileStream << "\t\t]\n\t}\n";
				lastWasVariation = false;
			}
			if (!isFolder && !foundDash && !foundSpace && !isVariation)
			{
				fileStream << "\t" << inGameName << " = \n\t{\n\t\ttype = \"" << overrideSoundType << "\"\n\t\tvolume = " << overideVolume << "\n\t\tuse_hrtf = " << overideHRTF << "\n\t\tvsnd_files =\n\t\t[\n\t\t\t\"sounds/" << nameOfFile << ".vsnd\"\n\t\t]\n\t}\n";
			}
			else
			{
				cout << "Generating for folder: " << nameOfFile << endl;
			}
		}
		else
		{
			if (!isFolder && !foundDash && !foundSpace)
			{
				fileStream << "\t" << inGameName << " = \n\t{\n\t\ttype = \"" << overrideSoundType << "\"\n\t\tvolume = " << overideVolume << "\n\t\tuse_hrtf = " << overideHRTF << "\n\t\tvsnd_files =\n\t\t[\n\t\t\t\"sounds/" << nameOfFile << ".vsnd\"\n\t\t]\n\t}\n";
			}
			else
			{
				cout << "Generating for folder: " << nameOfFile << endl;
			}
		}
	}

	fileStream << "}\n";
	
	fileStream.close();

}
