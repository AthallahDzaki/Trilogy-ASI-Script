#include "util/EffectBase.h"
#include "util/hooks/HookMacros.h"

#include <CMenuSystem.h>

class DisableOneMovementKeyEffect : public EffectBase
{
    enum eMovementKeyType
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    static inline eMovementKeyType mode = eMovementKeyType::UP;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        mode = (eMovementKeyType) inst->Random (0, 3);

        HOOK_METHOD_ARGS (inst, Hooked_GetPositionOfAnalogueSticks,
                          void (CRunningScript *, __int16), 0x48AF1F);
    }

    void
    OnProcessScripts (EffectInstance *inst) override
    {
        if (CMenuSystem::num_menus_in_use) return;

        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CPad *pad = player->GetPadFromPlayer ();
        if (!pad) return;

        switch (mode)
        {
            case eMovementKeyType::UP:
            {
                pad->NewState.LeftStickY
                    = std::max ((short) 0u, pad->NewState.LeftStickY);

                if (FindPlayerVehicle (-1, false))
                {
                    pad->NewState.ButtonCross = 0;
                }
                break;
            }
            case eMovementKeyType::DOWN:
            {
                pad->NewState.LeftStickY
                    = std::min (pad->NewState.LeftStickY, (short) 0u);

                if (FindPlayerVehicle (-1, false))
                {
                    pad->NewState.ButtonSquare = 0;
                }
                break;
            }
            case eMovementKeyType::LEFT:
            {
                pad->NewState.LeftStickX
                    = std::max ((short) 0u, pad->NewState.LeftStickX);
                break;
            }
            case eMovementKeyType::RIGHT:
            {
                pad->NewState.LeftStickX
                    = std::min (pad->NewState.LeftStickX, (short) 0u);
                break;
            }
        }
    }

    static void
    Hooked_GetPositionOfAnalogueSticks (auto &&cb, CRunningScript *script,
                                        __int16 count)
    {
        switch (mode)
        {
            case UP:
            {
                ScriptParams[1]
                    = std::max (0, ScriptParams[1]);
                break;
            }
            case DOWN:
            {
                ScriptParams[1]
                    = std::min (0, ScriptParams[1]);
                break;
            }
            case LEFT:
            {
                ScriptParams[0]
                    = std::max (0, ScriptParams[0]);
                break;
            }
            case RIGHT:
            {
                ScriptParams[0]
                    = std::min (0, ScriptParams[0]);
                break;
            }
            default: break;
        }

        cb ();
    }
};

DEFINE_EFFECT (DisableOneMovementKeyEffect, "effect_disable_one_movement_key",
               GROUP_CONTROLS);