#pragma once

#include <filesystem>
#include <variant>

#include "util/GlobalHooksInstance.h"
#include "util/CPedDamageResponseCalculator.h"

#include <CCivilianPed.h>
#include <CPed.h>
#include <CPickups.h>
#include <CPlayerPed.h>
#include <CPopulation.h>
#include <CStreaming.h>
#include <CVector.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class HoboManager
{
public:
    static inline std::vector<std::tuple<CPed *, int, eWeaponType>> angryHobo
        = {};

    static inline bool angryHoboLoaded = false;

    static std::filesystem::path
    GetHoboManagerPath ()
    {
        const std::string hoboManagerName = "ChaosMod/HoboManager.ini";

        const std::filesystem::path managerPath
            = GAME_PATH ((char *) hoboManagerName.c_str ());

        std::filesystem::create_directories (managerPath.parent_path ());

        return managerPath;
    }

    static void
    LoadAngryHobo ()
    {
        if (angryHoboLoaded) return;

        byte gameState
            = injector::ReadMemory<byte> (0xC8D4C0, true); // GameState

        if (gameState != 9) return;
        // Load Hobo
        const std::filesystem::path managerPath = GetHoboManagerPath ();

        if (!std::filesystem::exists (managerPath)) // Skip if file not exists
            return;

        mINI::INIFile file (managerPath.string ());

        // next, create a structure that will hold data
        mINI::INIStructure ini;

        // now we can read the file
        file.read (ini);

        int   Type = 0, Alive = 0, Weapon = 0;
        float Health = 0.0, X = 0.0, Y = 0.0, Z = 0.0;
        for (auto &section : ini)
        {
            for (auto &item : section.second)
            {
                std::cout << item.first << " = " << item.second << std::endl;
                if (item.first == "type")
                {
                    Type = std::stoi (item.second);
                }
                else if (item.first == "alive")
                {
                    Alive = std::stoi (item.second);
                }
                else if (item.first == "weapon")
                {
                    Weapon = std::stoi (item.second);
                }
                else if (item.first == "health")
                {
                    Health = std::stof (item.second);
                }
                else if (item.first == "x")
                {
                    X = std::stof (item.second);
                }
                else if (item.first == "y")
                {
                    Y = std::stof (item.second);
                }
                else if (item.first == "z")
                {
                    Z = std::stof (item.second);
                }
            }
            if (Alive)
            {
                CPed *ped = SpawnAngryHobo (Type, (eWeaponType) Weapon,
                                            CVector (X, Y, Z));
                angryHobo.push_back (
                    std::make_tuple (ped, Type, (eWeaponType) Weapon));
            }
        }

        HOOK_METHOD_ARGS (GlobalHooksInstance::Get (), Hooked_ComputeWillKillPed,
                          void (CPedDamageResponseCalculator *, CPed *, void *,
                                char),
                          0x4B5B27);

        angryHoboLoaded = true;
    }

    static void
    Hooked_ComputeWillKillPed (auto                        &&cb,
                               CPedDamageResponseCalculator *thisCalc,
                               CPed *ped, void *a3, char a4)
    {
        cb ();

        if (!ped) return;

        if(ped->m_fHealth > 0.0f) return; // Skip the hook if the ped is still alive

        auto index = std::find_if (
            angryHobo.begin (), angryHobo.end (),
            [ped] (std::tuple<CPed *, int, eWeaponType> &t)
            { return std::get<CPed *> (t) == ped; });
        
        if (index != angryHobo.end ())
        {
            _RemoveAngryHobo (ped);
        }
    }

    static void
    SaveAngryHobo ()
    {
        const std::filesystem::path managerPath = GetHoboManagerPath ();

        // We Create the file
        mINI::INIFile      file (managerPath.string ());
        mINI::INIStructure ini;

        for (int i = 0; i < angryHobo.size (); i++)
        {
            CPed       *ped        = std::get<CPed *> (angryHobo.at (i));
            int         Type       = std::get<int> (angryHobo.at (i));
            eWeaponType weaponType = std::get<eWeaponType> (angryHobo.at (i));
            CVector     position   = ped->GetPosition ();

            ini["Hobo" + std::to_string (i)]["Type"] = std::to_string (Type);
            ini["Hobo" + std::to_string (i)]["Alive"]
                = std::to_string (ped->m_nPedState != PEDSTATE_DIE);
            ini["Hobo" + std::to_string (i)]["Weapon"]
                = std::to_string (weaponType);
            ini["Hobo" + std::to_string (i)]["Health"]
                = std::to_string (ped->m_fHealth);
            ini["Hobo" + std::to_string (i)]["X"] = std::to_string (position.x);
            ini["Hobo" + std::to_string (i)]["Y"] = std::to_string (position.y);
            ini["Hobo" + std::to_string (i)]["Z"] = std::to_string (position.z);
        }

        file.generate (ini);
    }

    static void
    AddAngryHobo (int Type, eWeaponType weaponType, CVector position)
    {
        CPed *ped = SpawnAngryHobo (Type, weaponType, position);
        // Add to the list
        angryHobo.push_back (std::make_tuple (ped, Type, weaponType));
        SaveAngryHobo ();
    }

    static CPed *
    SpawnAngryHobo (int Type, eWeaponType weaponType, CVector position)
    {
        int hoboType = Type;
        int modelID  = 230; // hobo
        CStreaming::RequestModel (modelID, 0);
        CStreaming::LoadAllRequestedModels (false);
        CPed *ped = new CCivilianPed (PED_TYPE_CIVMALE, modelID);
        if (ped)
        {
            ped->SetCharCreatedBy (2); // SCM Ped cannot be Removed!
            ped->SetPosn (position);
            ped->SetOrientation (0.0f, 0.0f, 0.0f);
            CWorld::Add (ped);
            ped->m_nWeaponShootingRate = 99999;
            ped->m_nWeaponAccuracy     = 99999;
            ped->m_fHealth             = hoboType == 3 ? 5000.0f : 1000.0f;

            // Only Explosion and Ramp can kill normal hobo
            ped->m_nPhysicalFlags.bBulletProof = true;
            ped->m_nPhysicalFlags.bFireProof   = true;
            ped->m_nPhysicalFlags.bMeleeProof  = true;
            if (hoboType >= 1)
            {
                ped->m_nPhysicalFlags.bExplosionProof = true;
            }
            if (hoboType >= 2)
            {
                ped->m_nPhysicalFlags.bCollisionProof = true;
            }
            // How to Kill the Undead Hobo (Collsion proof and Explosion Proof)?
            // 1. Delete from the game, (Need Global Variable)
            // 2. Use Parachute and use Stinklikeabee cheat to kill the hobo

            auto model = CPickups::ModelForWeapon (weaponType);
            CStreaming::RequestModel (model, 2);
            CStreaming::LoadAllRequestedModels (false);
            ped->GiveWeapon (weaponType, 99999, false);
            ped->SetCurrentWeapon (weaponType);
            CStreaming::SetModelIsDeletable (model);

            ped->PositionAnyPedOutOfCollision ();
            Command<eScriptCommands::COMMAND_SET_CHAR_RELATIONSHIP> (
                ped, 4, ePedType::PED_TYPE_PLAYER1);
        }
        return ped;
    }

    static void
    RemoveAngryHobo (CPed *ped)
    {
        _RemoveAngryHobo (ped);
    }

    static void
    RemoveAllAngryHobo ()
    {
        for (int i = 0; i < angryHobo.size (); i++)
        {
            _RemoveAngryHobo (i);
        }
    }

    static void
    RemoveRandomAngryHobo ()
    {
        if (angryHobo.size () > 0)
        {
            _RemoveAngryHobo (rand () % angryHobo.size ());
        }
    }

private:
    static void
    _RemoveAngryHobo (CPed *ped)
    {
        if (ped)
        {
            Command<eScriptCommands::COMMAND_REMOVE_CHAR_ELEGANTLY> (ped);
            angryHobo.erase (
                std::remove_if (angryHobo.begin (), angryHobo.end (),
                                [ped] (std::tuple<CPed *, int, eWeaponType> &t)
                                { return std::get<CPed *> (t) == ped; }),
                angryHobo.end ());
            SaveAngryHobo ();
        }
    }

    static void
    _RemoveAngryHobo (int index)
    {
        if (index >= 0 && index < angryHobo.size ())
        {
            CPed *ped = std::get<CPed *> (angryHobo.at (index));
            _RemoveAngryHobo (ped);
        }
    }
};