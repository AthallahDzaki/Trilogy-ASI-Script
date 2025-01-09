#include "effects/OneTimeEffect.h"
#include "util/Globals.h"

#include "CPlayerPed.h"

#include "util/HoboManager.h"

using namespace plugin;

class SpawnAngryHobo : public OneTimeEffect
{
    int hoboType = 0; // 0 - DE, 1 - Rocket, 2 - Homing Rocket, 3 - Minigun, 4 - Spawn Hydra
    eWeaponType weaponType = WEAPONTYPE_BRASSKNUCKLE;

public:
    SpawnAngryHobo (int type) : hoboType (type) {
        switch (type) {
            case 0: weaponType = WEAPONTYPE_DESERT_EAGLE; break;
            case 1: weaponType = WEAPONTYPE_RLAUNCHER; break;
            case 2: weaponType = WEAPONTYPE_RLAUNCHER_HS; break;
            case 3: weaponType = WEAPONTYPE_MINIGUN; break;
        }
    }

    void
    OnStart (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CVector position
            = player->TransformFromObjectSpace (CVector (0.0f, -5.0f, 3.0f));

        HoboManager::AddAngryHobo(hoboType, weaponType, position);
    }
};

enum {
    ANGRY_HOBO = 0,
    CRAZY_HOBO,
    ULTRA_CRAZY_HOBO,
    GIGACHAD_HOBO,
    PILOT_HOBO // Spawn Angry Hobo with Hydra :)
};

DEFINE_EFFECT (SpawnAngryHobo, "effect_spawn_angry_hobo", 0, ANGRY_HOBO);
DEFINE_EFFECT (SpawnAngryHobo, "effect_spawn_crazy_hobo", 0, CRAZY_HOBO);
DEFINE_EFFECT (SpawnAngryHobo, "effect_spawn_ultra_crazy_hobo", 0, ULTRA_CRAZY_HOBO);
DEFINE_EFFECT (SpawnAngryHobo, "effect_spawn_gigachad_hobo", 0, GIGACHAD_HOBO);