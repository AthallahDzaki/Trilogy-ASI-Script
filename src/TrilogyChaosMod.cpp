#pragma comment ( lib, "Crypt32.lib" )
#pragma comment ( lib, "Wldap32.lib" )
#pragma comment ( lib, "Normaliz.lib" )

#include "util/Config.h"
#include "util/DrawHelper.h"
#include "util/EffectHandler.h"
#include "util/GameHandler.h"
#include "util/Websocket.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define CURL_STATICLIB
#include <curl.h>

#ifdef GTASA
CdeclEvent<AddressList<0x53E550, H_CALL, 0x53E83C, H_CALL, 0x53EBA2, H_CALL>,
           PRIORITY_AFTER, ArgPickNone, void ()>
    drawAfterFadeEvent;
#elif GTAVC
CdeclEvent<AddressList<0x4A5D3D, H_CALL, 0x4A6138, H_CALL>, PRIORITY_AFTER,
           ArgPickNone, void ()>
    drawAfterFadeEvent;
#elif GTA3
CdeclEvent<AddressList<0x48E470, H_CALL>, PRIORITY_AFTER, ArgPickNone, void ()>
    drawAfterFadeEvent;
#endif

// Thanks to Parik for this!
static void
UnProtectInstance ()
{
    auto              hExecutableInstance = (size_t) GetModuleHandle (NULL);
    IMAGE_NT_HEADERS *ntHeader
        = (IMAGE_NT_HEADERS *) (hExecutableInstance
                                + ((IMAGE_DOS_HEADER *) hExecutableInstance)
                                      ->e_lfanew);
    SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
    DWORD  oldProtect;
    VirtualProtect ((VOID *) hExecutableInstance, size, PAGE_EXECUTE_READWRITE,
                    &oldProtect);
}

class TrilogyChaosMod
{
public:
    TrilogyChaosMod ()
    {

        UnProtectInstance ();

        curl_global_init(CURL_GLOBAL_ALL);

        Config::Init ();
        Websocket::Setup ();

        Events::initRwEvent += GameHandler::Initialise;

        Events::gameProcessEvent += EffectHandler::Tick;
        Events::gameProcessEvent += GameHandler::ProcessGame;
        Events::processScriptsEvent += EffectHandler::ProcessScripts;

        Events::gameProcessEvent += DrawHelper::Tick;
        drawAfterFadeEvent += DrawHelper::DrawEffects;
        drawAfterFadeEvent += DrawHelper::Draw;
    }
} trilogyChaosMod;
