#include "util/EffectBase.h"

#include <CMenuSystem.h>
#include <CRadar.h>

using namespace plugin;

class DisableHUDEffect : public EffectBase
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        HOOK (inst, Hooked_CHud_Draw, void (), 0x53E4FF);
        // injector::MakeCALL (0x53E4FF, Hooked_CHud_Draw);
        HOOK (inst, Hooked_CRadar_DrawBlips, void (), 0x58AA2D);
        // injector::MakeJMP (0x58AA2D, Hooked_CRadar_DrawBlips);

        HOOK (inst, Hooked_CMenuSystem_DisplayGridMenu,
              tMenuPanel * (unsigned __int8, unsigned __int8), 0x58261E,
              0x582679, 0x5826CF, 0x58274D);
        // injector::MakeJMP (0x58261E, Hooked_CMenuSystem_DisplayGridMenu);
        // for (int address : {0x582679, 0x5826CF, 0x58274D})
        // {
        //     injector::MakeCALL (address, Hooked_CMenuSystem_DisplayGridMenu);
        // }

        HOOK (inst, Hooked_CMenuSystem_DisplayStandardMenu,
              void (unsigned __int8, bool), 0x582627, 0x582687, 0x5826D9,
              0x582757);
        // injector::MakeJMP (0x582627, Hooked_CMenuSystem_DisplayStandardMenu);
        // for (int address : {0x582687, 0x5826D9, 0x582757})
        // {
        //     injector::MakeCALL (address,
        //                         Hooked_CMenuSystem_DisplayStandardMenu);
        // }

        HOOK (inst, Hooked_CHud_DrawAreaName, void (), 0x58D542);
        // injector::MakeCALL (0x58D542, Hooked_CHud_DrawAreaName);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        // TODO: Unhook
    }

    static void
    Hooked_CHud_Draw (auto &&cb)
    {
        CRadar::Draw3dMarkers ();

        if (CMenuSystem::num_menus_in_use) CMenuSystem::Process (-99);
    }

    static void
    Hooked_CRadar_DrawBlips (auto &&cb)
    {
    }

    static tMenuPanel *
    Hooked_CMenuSystem_DisplayGridMenu (auto &&cb)
    {
        return nullptr;
    }

    static void
    Hooked_CMenuSystem_DisplayStandardMenu (auto &&cb)
    {
    }

    static void
    Hooked_CHud_DrawAreaName (auto &&cb)
    {
    }
};

DEFINE_EFFECT (DisableHUDEffect, "effect_disable_hud", GROUP_HUD);