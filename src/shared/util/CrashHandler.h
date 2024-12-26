#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <dbghelp.h>
#include <string>

// Namespace untuk menangani crash dump
class CrashDumpHandler {
public:
    // Fungsi untuk menginisialisasi Vectored Exception Handler
    static void InitializeHandler();

    // Fungsi untuk membuat crash dump
    static bool GenerateCrashDump(EXCEPTION_POINTERS* pExceptionPointers);

private:
    // Variabel untuk menyimpan handler VEH
    static PVOID hVEH;

    // Handler untuk menangkap exception menggunakan VEH
    static LONG CALLBACK VectoredExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo);

    // Fungsi untuk mendapatkan nama file dump yang dinamis
    static std::string GetDynamicDumpFileName();

    // Fungsi untuk log pesan
    static void Log(const std::string& message);
};

#endif // CRASH_HANDLER_H
