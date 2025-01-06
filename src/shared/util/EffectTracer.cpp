#include "EffectTracer.h"
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

EffectTracer& EffectTracer::GetInstance() {
    static EffectTracer instance;
    return instance;
}

void EffectTracer::Initialize(const std::string& logDirectory) {
    this->logDirectory = logDirectory;
    InitializeLog();
}

void EffectTracer::LogEffect(const std::string& effectName) {
    if (logFile.is_open()) {
        logFile << "[" << GetTimestamp() << "] Effect: " << effectName << std::endl;
    }
}

void EffectTracer::InitializeLog() {
    namespace fs = std::filesystem;

    std::string oldLogPath = logDirectory + "/effect_log.txt";
    std::string newLogPath = logDirectory + "/effect_log_" + GetTimestamp() + ".txt";

    if (fs::exists(oldLogPath)) {
        fs::rename(oldLogPath, newLogPath);
    }

    logFile.open(logDirectory + "/effect_log.txt", std::ios::out | std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "Failed to create new log file in folder: " << logDirectory << std::endl;
    }
}

std::string EffectTracer::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    auto localTime = std::localtime(&timeT);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y%m%d_%H%M%S");
    return oss.str();
}

EffectTracer::~EffectTracer() {
    if (logFile.is_open()) {
        logFile.close();
    }
}
