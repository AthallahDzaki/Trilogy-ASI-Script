#include "effects/OneTimeEffect.h"
#include "util/GameUtil.h"

#include <CStats.h>

class BuffPlayerEffect : public OneTimeEffect
{
public:

    void
    OnStart (EffectInstance *inst) override
    {
        CStats::SetStatValue (STAT_MUSCLE, 1000.0f);

        GameUtil::RebuildPlayer ();
    }
};

DEFINE_EFFECT (BuffPlayerEffect, "effect_buff_player", GROUP_STATS);