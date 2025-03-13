#include "util/EffectBase.h"
#include "util/GameUtil.h"
#include "util/GenericUtil.h"
#include "util/MathHelper.h"

class StruckByCaddyEffect : public EffectBase
{
    CAutomobile *caddy = nullptr;
    int          wait       = 700;

public:
    bool
    CanActivate () override
    {
        CPlayerPed *player = FindPlayerPed ();

        return player && !player->m_nAreaCode && GameUtil::IsPlayerSafe ();
    }

    void
    OnStart (EffectInstance *inst) override
    {
        inst->SetIsOneTimeEffect ();

        caddy      = nullptr;
        wait       = 700;

        inst->SetTimerVisible (false);
    }

    void
    OnTick (EffectInstance *inst) override
    {
        if (!CanActivate ()) return;

        SpawnOrUpdateTruck ();

        wait -= (int) GenericUtil::CalculateTick ();
        if (wait > 0) return;

        caddy->PlaceOnRoadProperly ();

        CMatrix *matrix   = caddy->GetMatrix ();
        float    velocity = 1.0f;

        caddy->m_vecMoveSpeed.x = velocity * matrix->up.x;
        caddy->m_vecMoveSpeed.y = velocity * matrix->up.y;
        caddy->m_vecMoveSpeed.z = velocity * matrix->up.z;

        inst->Disable ();
    }

    void
    SpawnOrUpdateTruck ()
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CVector position
            = player->TransformFromObjectSpace (CVector (0.0f, 25.0f, 0.0f));

        float playerFacing = player->m_fCurrentRotation - M_PI;

        if (caddy)
        {
            caddy->SetPosn (position);
            caddy->SetOrientation (0.0f, 0.0f, playerFacing);
            return;
        }

        // caddy
        caddy
            = (CAutomobile *) GameUtil::CreateVehicle (457, position,
                                                       playerFacing, true);

        caddy->PlaceOnRoadProperly ();
    }
};

DEFINE_EFFECT (StruckByCaddyEffect, "effect_struck_by_caddy", 0);
