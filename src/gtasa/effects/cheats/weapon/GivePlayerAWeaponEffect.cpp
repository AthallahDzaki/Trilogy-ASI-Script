#include "effects/OneTimeEffect.h"
#include "util/GameUtil.h"

#include <CPickups.h>
#include <CStreaming.h>
#include <CWeaponInfo.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;
namespace
{
enum eWeaponSlot : unsigned int
{
    UNARMED,
    MELEE,
    HANDGUN,
    SHOTGUN,
    SMG,
    RIFLE,
    SNIPER,
    HEAVY,
    THROWN,
    SPECIAL,
    GIFT,
    PARACHUTE,
    DETONATOR,
};
} // namespace

class GivePlayerAWeapon : public OneTimeEffect
{
private:
    eWeaponType weaponType = WEAPONTYPE_BRASSKNUCKLE;

public:
    GivePlayerAWeapon (eWeaponType weapon) : weaponType (weapon) {}

    bool
    CanActivate () override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return false;

        if (player->m_aWeapons[player->GetWeaponSlot (weaponType)].m_eWeaponType
            == WEAPONTYPE_PARACHUTE)
            return false;

        return GameUtil::IsPlayerSafe ();
    }

    void
    OnStart (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        auto m = CPickups::ModelForWeapon (weaponType);
        CStreaming::RequestModel (m, 2);
        CStreaming::LoadAllRequestedModels (false);

        const auto *info = CWeaponInfo::GetWeaponInfo (weaponType, 1);

        if (!info) return;

        unsigned int ammo = 0;

        switch (info->m_nSlot)
        {
            case eWeaponSlot::HANDGUN: ammo = 50; break;
            case eWeaponSlot::SHOTGUN: ammo = 35; break;
            case eWeaponSlot::SMG: ammo = 100; break;
            case eWeaponSlot::RIFLE: ammo = 50; break;
            case eWeaponSlot::SNIPER: ammo = 20; break;
            case eWeaponSlot::HEAVY: ammo = 10; break;
            case eWeaponSlot::THROWN: ammo = 10; break;
            case eWeaponSlot::SPECIAL: ammo = 500; break;
            default: break;
        }

        if (weaponType == WEAPONTYPE_MINIGUN)
        {
            ammo = 250;
        }

        Command<Commands::GIVE_WEAPON_TO_CHAR> (player,
                                                               weaponType,
                                                               ammo);

        CStreaming::SetModelIsDeletable (m);
    }
};

// clang-format off
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_ak47", GROUP_WEAPONS, WEAPONTYPE_AK47);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_uzi", GROUP_WEAPONS, WEAPONTYPE_MICRO_UZI);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_sniper", GROUP_WEAPONS, WEAPONTYPE_SNIPERRIFLE);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_shotgun", GROUP_WEAPONS, WEAPONTYPE_SPAS12);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_rocket_launcher", GROUP_WEAPONS, WEAPONTYPE_RLAUNCHER_HS);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_pistol", GROUP_WEAPONS, WEAPONTYPE_PISTOL_SILENCED);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_minigun", GROUP_WEAPONS, WEAPONTYPE_MINIGUN);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_weapon_set_4", GROUP_WEAPONS, WEAPONTYPE_MINIGUN);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_grenades", GROUP_WEAPONS, WEAPONTYPE_GRENADE);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_give_chainsaw", GROUP_WEAPONS, WEAPONTYPE_CHAINSAW);
DEFINE_EFFECT (GivePlayerAWeapon, "effect_get_parachute", GROUP_WEAPONS, WEAPONTYPE_PARACHUTE);
// clang-format on