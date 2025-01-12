#pragma once

#include <filesystem>

#include <CAutomobile.h>
#include <CBike.h>
#include <CBmx.h>
#include <CBoat.h>
#include <CCamera.h>
#include <CCarCtrl.h>
#include <CCheat.h>
#include <CClothes.h>
#include <CCutsceneMgr.h>
#include <CEntryExitManager.h>
#include <CGame.h>
#include <CGenericGameStorage.h>
#include <CHeli.h>
#include <CMenuManager.h>
#include <CModelInfo.h>
#include <CMonsterTruck.h>
#include <CPlane.h>
#include <CQuadBike.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CTrailer.h>
#include <CWorld.h>

class GameUtil
{
    static inline CPedAcquaintance backupAcquaintances[32];

    static inline std::string loadFilePath;
    static inline char       *gamePath = reinterpret_cast<char *> (0xC92368);

public:
    static CPedAcquaintance *GetPedTypeAcquaintances (int pedType);
    static void              BackupAcquaintances ();
    static void              RestoreSavedAcquaintances ();
    static void SaveAcquaintances (CPedAcquaintance *acquaintances);
    static void LoadAcquaintances (CPedAcquaintance *acquaintances);

    static void        DeleteAutoSave ();
    static void        SaveToFile (std::string fileName);
    static bool        LoadFromFile (std::string fileName);
    static std::string GetLoadFileName ();

    static void SetVehiclesToRealPhysics ();
    static int  GetRealMissionsPassed ();

    static void RebuildPlayer ();
    static void RemovePlayerBalaclava ();

    static CVehicle *CreateVehicle (int vehicleID, CVector position,
                                    float orientation, bool clearSpace);

    static void ClearWeapons (CPed *ped);

    static bool IsCutsceneProcessing ();
    static bool IsPlayerSafe ();
	
	static inline bool IsRunRandomMission = false;
	static inline int OurMissionPassed;

    template <typename T>
    static T &
    GetGlobalVariable (uint32_t index)
    {
        return *reinterpret_cast<T *> (CTheScripts::ScriptSpace + 4 * index);
    }
};