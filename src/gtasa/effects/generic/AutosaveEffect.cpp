#include "util/Config.h"
#include "util/EffectBase.h"
#include "util/GameUtil.h"

#include <CGangWars.h>
#include <CTheScripts.h>

class AutosaveEffect : public EffectBase
{
    bool didSave        = false;
    int  missionsPassed = 0;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        didSave        = false;
        missionsPassed = inst->GetCustomData ().value ("missionsPassed", 0);

        inst->OverrideName ("Autosaving...");
    }

    void
    OnTick (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (player && !didSave && !CTheScripts::IsPlayerOnAMission ()
            && !CGangWars::State)
        {
            CRunningScript *script = CTheScripts::pActiveScripts->m_pNext;
            while (script != nullptr)
            {
                if (script->m_bIsMission && script->m_bIsActive) { return; }

                script = script->m_pNext;
            }

            bool wasInVehicle              = player->m_nPedFlags.bInVehicle;
            player->m_nPedFlags.bInVehicle = false;

            if (Config::GetOrDefault ("Chaos.SaveToSlot8", false))
            {
                GameUtil::SaveToFile ("GTASAsf8.b");
            }

            GameUtil::SaveToFile ("chaos_mod\\chaos_autosave.b");

            std::string missionSave;
            missionSave.append ("chaos_mod\\chaos_autosave.mission_")
                .append (std::to_string (missionsPassed))
                .append (".b");
            GameUtil::SaveToFile (missionSave);

            player->m_nPedFlags.bInVehicle = wasInVehicle;

            inst->OverrideName ("Autosave Completed");

            didSave = true;

            inst->SetDuration (inst->GetEffectDuration ());
            inst->Disable ();
        }
    }
};

DEFINE_EFFECT (AutosaveEffect, "effect_autosave", 0);