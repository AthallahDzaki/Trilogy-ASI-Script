#pragma once

#include <filesystem>
#include <variant>

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
        // Load Hobo
        const std::filesystem::path managerPath = GetHoboManagerPath ();

        if (!std::filesystem::exists (managerPath)) // Skip if file not exists
            return;

        mINI::INIFile file (managerPath.string ());

        // next, create a structure that will hold data
        mINI::INIStructure ini;

        // now we can read the file
        file.read (ini);

        int   Type, Alive, Weapon;
        float Health, X, Y, Z;
        for (auto &section : ini)
        {
            for (auto &item : section.second)
            {
                if (item.first == "Type")
                {
                    Type = std::stoi (item.second);
                }
                else if (item.first == "Alive")
                {
                    Alive = std::stoi (item.second);
                }
                else if (item.first == "Weapon")
                {
                    Weapon = std::stoi (item.second);
                }
                else if (item.first == "Health")
                {
                    Health = std::stof (item.second);
                }
                else if (item.first == "X")
                {
                    X = std::stof (item.second);
                }
                else if (item.first == "Y")
                {
                    Y = std::stof (item.second);
                }
                else if (item.first == "Z")
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

            ini["Hobo" + std::to_string (i)]["Type"] = Type;
            ini["Hobo" + std::to_string (i)]["Alive"]
                = ped->m_nPedState != PEDSTATE_DIE;
            ini["Hobo" + std::to_string (i)]["Weapon"] = weaponType;
            ini["Hobo" + std::to_string (i)]["Health"] = ped->m_fHealth;
            ini["Hobo" + std::to_string (i)]["X"]      = position.x;
            ini["Hobo" + std::to_string (i)]["Y"]      = position.y;
            ini["Hobo" + std::to_string (i)]["Z"]      = position.z;
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