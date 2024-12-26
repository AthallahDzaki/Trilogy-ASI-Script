#include "CrashHandler.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <dbghelp.h>

// Inisialisasi statis untuk hVEH
PVOID CrashDumpHandler::hVEH = nullptr;

void CrashDumpHandler::Log(const std::string& message) {
    std::cout << "[LOG]: " << message << std::endl;
}

// Fungsi untuk mendapatkan nama file dump yang dinamis
std::string CrashDumpHandler::GetDynamicDumpFileName() {
    // Dapatkan waktu saat ini
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Dapatkan nama proses
    char processName[MAX_PATH];
    if (GetModuleFileNameA(nullptr, processName, MAX_PATH) == 0) {
        strcpy_s(processName, "UnknownProcess");
    }

    // Format nama file
    std::ostringstream oss;
    oss << "crash_dump_"
        << processName << "_"
        << st.wYear << std::setfill('0') << std::setw(2) << st.wMonth
        << std::setw(2) << st.wDay << "_"
        << std::setw(2) << st.wHour
        << std::setw(2) << st.wMinute
        << std::setw(2) << st.wSecond << ".dmp";

    return oss.str();
}

// Fungsi untuk membuat crash dump
bool CrashDumpHandler::GenerateCrashDump(EXCEPTION_POINTERS* pExceptionPointers) {
    std::string dumpFileName = GetDynamicDumpFileName();
	
	Log("Generated File Name " + dumpFileName);

    HANDLE hFile = CreateFileA(dumpFileName.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);


    if (hFile == INVALID_HANDLE_VALUE) {
        Log("Failed to create dump file.");
        return false;
    }

    MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
    exceptionInfo.ThreadId = GetCurrentThreadId();
    exceptionInfo.ExceptionPointers = pExceptionPointers;
    exceptionInfo.ClientPointers = FALSE;

    // Menggunakan opsi dump yang lebih lengkap
    BOOL success = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpWithFullMemory, // Dump lengkap dengan semua data proses
        &exceptionInfo,
        nullptr,
        nullptr
    );

    CloseHandle(hFile);

    if (!success) {
        Log("Failed to write dump file.");
        return false;
    }

    Log("Crash dump created successfully: " + dumpFileName);
    return true;
}

// Handler untuk menangkap exception menggunakan VEH
LONG CALLBACK CrashDumpHandler::VectoredExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo) {
    Log("Exception caught by Vectored Exception Handler!");

    DWORD exceptionCode = pExceptionInfo->ExceptionRecord->ExceptionCode;
    
    // Cek apakah exception dapat dilanjutkan atau tidak, dan hanya tangani yang menyebabkan crash
    switch (exceptionCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            Log("Access Violation exception detected.");
            break;
        
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_FLT_INVALID_OPERATION:
        case EXCEPTION_FLT_OVERFLOW:
        case EXCEPTION_FLT_UNDERFLOW:
            Log("Floating Point Exception detected.");
            break;
        
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            Log("Illegal Instruction exception detected.");
            break;

        case EXCEPTION_STACK_OVERFLOW:
            Log("Stack Overflow exception detected.");
            break;

        case EXCEPTION_INT_DIVIDE_BY_ZERO:
        case EXCEPTION_INT_OVERFLOW:
            Log("Integer exception detected.");
            break;

        case EXCEPTION_IN_PAGE_ERROR:
            Log("In-Page Error exception detected.");
            break;

        default:
            // Abaikan pengecualian yang tidak menyebabkan crash kritis
            Log("Non-crash exception, continuing...");
            return EXCEPTION_CONTINUE_SEARCH; // Teruskan pencarian dan biarkan sistem menangani exception ini.
    }

    // Hanya buat crash dump jika exception-nya kritis
    GenerateCrashDump(pExceptionInfo);

    return EXCEPTION_CONTINUE_SEARCH; // Biarkan Exception lainnya mendapatkan crash handler juga...
}


// Fungsi untuk menginisialisasi Vectored Exception Handler
void CrashDumpHandler::InitializeHandler() {
    Log("Setting up Vectored Exception Handler...");
    // Daftarkan VEH untuk menangkap exception
    hVEH = AddVectoredExceptionHandler(0, VectoredExceptionHandler);
    if (!hVEH) {
        Log("Failed to set up Vectored Exception Handler!");
    }
}
