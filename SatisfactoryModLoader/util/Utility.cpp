#define WIN32_LEAN_AND_MEAN

#include <stdafx.h>
#include <SatisfactoryModLoader.h>
#include "DetoursFwd.h"
#include "Utility.h"
#include <game/Utility.h>
#include <windows.h>

namespace SML {
	namespace Utility {
		std::ofstream logFile;

		void log() {}

		void log(LogType type) {}

		void writeHeader(std::string header, LogType type) {
			std::string logType;

			switch (type) {
			case Info:
			case ModInfo:
				logType = "Info]    ";
				break;
			case Warning:
			case ModWarning:
				logType = "Warning] ";
				break;
			case Error:
			case ModError:
				logType = "Error]   ";
				break;
			}

			// log line
			logFile << "[" + header + "::" + logType;
			logFile.flush();

			// cout line
			setConsoleColor(type > 2 && type <= 5 ? ConsoleColor::Cyan : ConsoleColor::White);
			std::cout << "[" << header << "] ";
		}

		void setConsoleColor(ConsoleColor color) {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			// sets the console color
			SetConsoleTextAttribute(hConsole, color + 1);
		}

		//target[0] = normal CL, target [1] = experimental CL
		void checkVersion(const int target) {
			std::wstring satisVersion{ call<&Objects::BuildSettings::GetBuildVersion>() };
			std::string str(satisVersion.begin(), satisVersion.end());
			int version = std::atoi(str.substr(str.length() - 6, str.length()).c_str());
			if (version >= target) {
				info("Version check passed!");
			}
			else if (version < target){
				error("WARNING: Version check failed");
				if (!supressErrors) {
					int ret = MessageBoxA(NULL, "The version of Satisfactory that you are running is too old for the current version of SML! Please update Satisfactory otherwise SML may run into errors. \nPress Ok to continue at your own discresion or cancel to exit.", "SatisfactoryModLoader Warning", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONEXCLAMATION);
					if (ret == IDCANCEL) {
						abort();
					}
				}
				else {
					warning("SupressErrors set to true, continuing...");
				}
			}
		}

		void displayCrash(std::string crashText) {
			MessageBoxA(NULL, (crashText + "\nClick OK to exit.").c_str(), "SatisfactoryModLoader has crashed!", MB_ICONERROR);
			abort();
		}
		void displayCrash(std::string header, std::string crashText) {
			MessageBoxA(NULL, (crashText + "\nClick OK to exit.").c_str(), header.c_str(), MB_ICONERROR);
			abort();
		}
	}
}