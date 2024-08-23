#pragma once
#include <windows.h>
#include <string>
#include <format>
#include <string_view>
#include <fstream>

class ConsoleLogger {
public:
    static ConsoleLogger& Instance() {
        static ConsoleLogger instance;
        return instance;
    }

    enum class Mode {
        CONSOLE,
        FILE
    };

    // Method to set the logging mode
    void switchMode(Mode mode, const std::string& fileName = "ue_dump.txt") {
        logToFile = mode == Mode::FILE;

        if (logToFile) {
            logFile.open(fileName, std::ios::out | std::ios::app);
            if (!logFile.is_open()) {
                Log("Failed to open log file: {}\n", fileName);
                logToFile = false;
            }
        }
        else if (logFile.is_open()) {
            logFile.close();
        }
    }

    template<typename... Args>
    void Log(std::string_view format_string, Args&&... args) {
        std::string message = std::vformat(format_string, std::make_format_args(std::forward<Args>(args)...));

        if (logToFile && logFile.is_open()) {
            logFile << message;
            logFile.flush();
        }
        else {
            DWORD charsWritten;
            WriteConsoleA(
                hConsole,
                message.c_str(),
                static_cast<DWORD>(message.length()),
                &charsWritten,
                nullptr
            );
        }
    }

private:
    HANDLE hConsole;
    bool logToFile = false;
    std::ofstream logFile;

    ConsoleLogger() {
        AllocConsole();
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    ~ConsoleLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
        CloseHandle(hConsole);
    }

    // Delete copy constructor and assignment operator to enforce singleton pattern
    ConsoleLogger(const ConsoleLogger&) = delete;
    ConsoleLogger& operator=(const ConsoleLogger&) = delete;
};
