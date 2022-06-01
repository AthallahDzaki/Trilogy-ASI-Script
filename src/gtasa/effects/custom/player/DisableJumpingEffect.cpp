#include "util/EffectBase.h"

#include <CMenuSystem.h>

class DisableJumpingEffect : public EffectBase
{
public:
    void
    OnProcessScripts (EffectInstance *inst) override
    {
        if (CMenuSystem::num_menus_in_use) return;

        CPlayerPed *player = FindPlayerPed ();
        if (player && !FindPlayerVehicle (-1, false))
        {
            CPad *pad = player->GetPadFromPlayer ();
            if (pad) pad->NewState.ButtonSquare = 0;
        }
    }
};

DEFINE_EFFECT (DisableJumpingEffect, "effect_disable_jumping", 0);