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

        CVector Angle = player->TransformFromObjectSpace (CVector (10.0f, 10.0f, 0.0f));
        int SpawnProjectile = inst->Random(1,15);
        for(int i = 0; i < SpawnProjectile; i++)
            CProjectileInfo::AddProjectile(player, WEAPON_ROCKET_HS, player->TransformFromObjectSpace(CVector(0.0f, 0.0f, 100.0f + ((float)i * 50.0f))), 5.0f, &Angle, player);
        inst->OverrideName("Spawn " + std::to_string(SpawnProjectile) + " Rocket");
    }
};

DEFINE_EFFECT (SpawnRocket, "effect_spawn_rocket", 0);