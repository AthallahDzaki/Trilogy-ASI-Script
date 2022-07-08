#include "util/EffectBase.h"

#include <CPlayerInfo.h>

class BankruptEffect : public EffectBase
{
public:
    void
    OnTick (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CPlayerInfo *playerInfo = player->GetPlayerInfoForThisPlayerPed ();
        if (!playerInfo) return;

        playerInfo->m_nMoney = 0;
        inst->Disable ();
    }
};

DEFINE_EFFECT (BankruptEffect, "effect_bankrupt", 0);