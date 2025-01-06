#pragma once

#include <string>
#include <fstream>

class EffectTracer {
public:
    static EffectTracer& GetInstance(); // Singleton accessor

    void Initialize(const std::string& logDirectory);
    void LogEffect(const std::string& effectName);

private:
    EffectTracer() = default; // Private constructor
    ~EffectTracer();

    std::string logDirectory;
    std::ofstream logFile;

    void InitializeLog();
    std::string GetTimestamp() const;
};