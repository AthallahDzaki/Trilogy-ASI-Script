#include "effects/OneTimeEffect.h"

#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

int exceptions[] = {
    40, // First Date
    35, // Race Tournament / 8-track / Dirt Track
    83, // Learning to Fly
    71, // Back To School
};

using namespace plugin;

class PlayRandomMission : public OneTimeEffect
{
public:
    bool CanActivate() override
    {
        return FindPlayerPed() != nullptr;
    }

    void OnStart(EffectInstance *inst) override
    {
        if (CTheScripts::IsPlayerOnAMission ()) {
			inst->Disable (); // Disable if On Mission
			return;
		}

regenerate:
		int MissionID = inst->Random(11, 112);
		
		if (std::find (std::begin (exceptions), std::end (exceptions),
                   MissionID)
            != std::end (exceptions))
		{
			goto regenerate;
		}
		
		Command<Commands::LOAD_AND_LAUNCH_MISSION_INTERNAL> (MissionID);
    }
};

DEFINE_EFFECT (PlayRandomMission, "effect_random_mission", 0);