#include "util/EffectBase.h"

#include <extensions/ScriptCommands.h>

using namespace plugin;

class SadborDanceShortEffect : public EffectBase
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        CVehicle* vehicle = FindPlayerVehicle (-1, false);
        CPlayerPed* playerPed = FindPlayerPed ();
        if (vehicle) 
        {
            if (vehicle->m_pDriver == playerPed)
            {
                Command<Commands::REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION> (
                    vehicle->m_pDriver, vehicle);
            }

            for (CPed *ped : vehicle->m_apPassengers)
            {
                if (ped != playerPed || !ped) continue;

                Command<Commands::REMOVE_CHAR_FROM_CAR_MAINTAIN_POSITION> (
                    ped, vehicle);
            }
        }

        Command<Commands::CLEAR_CHAR_TASKS> (playerPed);

        Command<Commands::REQUEST_ANIMATION>("DANCING");

        Command<Commands::LOAD_ALL_MODELS_NOW>();

        Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE> (
                playerPed, "DAN_Down_A", "DANCING", 4.0, false, false, false, false, (13 * 1000) + 270);

        CPad *pad = playerPed->GetPadFromPlayer ();
        if (pad) pad->DisablePlayerControls = true;

        inst->SetDuration((13 * 1000) + 270);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        CPlayerPed* playerPed = FindPlayerPed ();
        CPad *pad = playerPed->GetPadFromPlayer ();
        if (pad) pad->DisablePlayerControls = false;
        Command<Commands::CLEAR_CHAR_TASKS> (playerPed);
    }
};

DEFINE_EFFECT (SadborDanceShortEffect, "effect_sadbor_dance_short", 0);