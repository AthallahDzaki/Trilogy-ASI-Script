#include "effects/OneTimeEffect.h"

#include <extensions/ScriptCommands.h>

using namespace plugin;

class DisarmAllNPCsEffect : public OneTimeEffect
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        for (CPed *ped : CPools::ms_pPedPool)
        {
            if (ped == player) continue;

            Command<Commands::REMOVE_ALL_CHAR_WEAPONS> (ped);
        }
    }
};

DEFINE_EFFECT (DisarmAllNPCsEffect, "effect_disarm_all_npcs", 0);