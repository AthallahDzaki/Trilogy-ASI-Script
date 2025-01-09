// Inspiration from Zolika's Chaos Mod - "Sacrificial Circle"

#include "util/EffectBase.h"
#include "util/MathHelper.h"

#include <CStreaming.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class TheFiringCircusEffect : public EffectBase
{
    std::vector<CPed *> createdPeds = {};

public:
    bool
    CanActivate () override
    {
        return FindPlayerPed () != nullptr
               && FindPlayerVehicle (-1, false) != nullptr;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        createdPeds.clear ();

        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CVehicle *vehicle = FindPlayerVehicle (-1, false);

        for (int x = 0; x < 6; x++)
        {
            float angle     = 360.0f / 6 * x;
            float angle_rad = MathHelper::ToRadians (angle);

            CVector position      = player->GetPosition ();
            CVector spawnPosition = player->TransformFromObjectSpace (
                CVector (5.0f * sin (angle_rad), 5.0f * cos (angle_rad),
                         0.25f));

            int modelId = 264;

            CStreaming::RequestModel (modelId, 2);
            CStreaming::LoadAllRequestedModels (false);
            CStreaming::SetModelIsDeletable (modelId);

            CPed *createdPed;
            Command<Commands::CREATE_CHAR> (
                ePedType::PED_TYPE_CIVMALE, modelId, spawnPosition.x,
                spawnPosition.y, spawnPosition.z, &createdPed);

            Command<Commands::SET_CHAR_RELATIONSHIP> (
                createdPed, 4, ePedType::PED_TYPE_PLAYER1);

            CStreaming::RequestModel (MODEL_AK47, 2);
            CStreaming::LoadAllRequestedModels (false);
            CStreaming::SetModelIsDeletable (MODEL_AK47);

            Command<Commands::GIVE_WEAPON_TO_CHAR> (createdPed,
                                                                   WEAPONTYPE_AK47,
                                                                   9999);
            Command<Commands::SET_CURRENT_CHAR_WEAPON> (
                createdPed, WEAPONTYPE_AK47);

            Command<Commands::SET_CHAR_SHOOT_RATE> (createdPed,
                                                                   100);
            Command<Commands::SET_CHAR_ACCURACY> (createdPed,
                                                                 100);

            if (vehicle)
            {
                Command<Commands::TASK_DESTROY_CAR> (createdPed,
                                                                    vehicle);
            }
            else
            {
                Command<Commands::TASK_KILL_CHAR_ON_FOOT> (
                    createdPed, player);
            }

            createdPeds.push_back (createdPed);
        }
    }

    void
    OnEnd (EffectInstance *inst)
    {
        for (CPed *ped : createdPeds)
            Command<Commands::REMOVE_CHAR_ELEGANTLY> (ped);
    }
};

DEFINE_EFFECT (TheFiringCircusEffect, "effect_the_firing_circus", 0);
