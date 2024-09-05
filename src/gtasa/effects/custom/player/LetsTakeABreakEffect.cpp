#include "util/EffectBase.h"
#include "util/hooks/HookMacros.h"

class LetsTakeABreakEffect : public EffectBase
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        HOOK_METHOD_ARGS (inst, Hooked_GetPositionOfAnalogueSticks,
                          void (CRunningScript *, __int16), 0x48AF1F);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CPad *pad = player->GetPadFromPlayer ();
        if (!pad) return;

        pad->DisablePlayerControls = false;
    }

    void
    OnTick (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CPad *pad = player->GetPadFromPlayer ();
        if (!pad) return;

        pad->DisablePlayerControls = true;
    }

    static void
    Hooked_GetPositionOfAnalogueSticks (auto &&cb, CRunningScript *script,
                                        __int16 count)
    {
        ScriptParams[0] = 0;
        ScriptParams[1] = 0;
        ScriptParams[2] = 0;
        ScriptParams[3] = 0;

        cb ();
    }
};

DEFINE_EFFECT (LetsTakeABreakEffect, "effect_lets_take_a_break",
               GROUP_CONTROLS);