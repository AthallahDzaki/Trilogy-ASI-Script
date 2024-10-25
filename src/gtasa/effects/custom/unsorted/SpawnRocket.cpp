#include "effects/OneTimeEffect.h"

#include <CStreaming.h>
#include <CTimer.h>
#include <CWorld.h>
#include <CProjectileInfo.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class SpawnRocket : public OneTimeEffect
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CVector position
            = player->TransformFromObjectSpace (CVector (0.0f, 0.0f, 100.0f));

        CVector a = player->TransformFromObjectSpace (CVector (10.0f, 10.0f, 0.0f));
        for(int i = 0; i < inst->Random(1,5); i++)
            CProjectileInfo::AddProjectile(player, WEAPON_ROCKET_HS, player->TransformFromObjectSpace(CVector(0.0f, 0.0f, 100.0f)), 5.0f, &a, player);
    }
};

DEFINE_EFFECT (SpawnRocket, "effect_spawn_rocket", 0);