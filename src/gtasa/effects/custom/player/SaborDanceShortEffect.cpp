#include "util/EffectBase.h"
#include <extensions/ScriptCommands.h>

using namespace plugin;

class SadborDanceShortEffect : public EffectBase
{
    static inline bool hasParachute = false;
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

        Command<Commands::CLEAR_CHAR_TASKS_IMMEDIATELY> (playerPed);

        playerPed->m_pIntelligence->ClearTasks (true, false);

        CPad *pad = playerPed->GetPadFromPlayer ();
        if (pad) pad->DisablePlayerControls |= 1 << 1;

        if(playerPed->DoWeHaveWeaponAvailable(WEAPON_PARACHUTE)) // Has Parachute?
        {
            hasParachute = true;
            playerPed->ClearWeapon(WEAPON_PARACHUTE);
        }
              

        Command<Commands::SET_PLAYER_FIRE_BUTTON>(playerPed, false);

        Command<Commands::SET_PLAYER_FIRE_BUTTON>(playerPed, false);

        Command<Commands::REQUEST_ANIMATION>("DANCING");

        Command<Commands::LOAD_ALL_MODELS_NOW>();

        Command<Commands::TASK_PLAY_ANIM_NON_INTERRUPTABLE> (
                playerPed, "DAN_Down_A", "DANCING", 4.0, false, true, true, true, (13 * 1000) + 270);

        inst->SetDuration((13 * 1000) + 270);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        CPlayerPed* playerPed = FindPlayerPed ();
        CPad *pad = playerPed->GetPadFromPlayer ();
        if (pad) pad->DisablePlayerControls &= ~(1 << 1);

        if(hasParachute) // Has Parachute?
        {
            hasParachute = false;
            playerPed->GiveWeapon(WEAPON_PARACHUTE, 1, true);
        }
        Command<Commands::CLEAR_CHAR_TASKS_IMMEDIATELY> (playerPed);
        Command<Commands::SET_PLAYER_FIRE_BUTTON>(playerPed, true);
        Command<Commands::SET_PLAYER_FIRE_BUTTON>(playerPed, true);
    }
};

DEFINE_EFFECT (SadborDanceShortEffect, "effect_sadbor_dance_short", 0);