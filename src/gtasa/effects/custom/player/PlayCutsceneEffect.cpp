#include "util/EffectBase.h"
#include "util/GenericUtil.h"
#include "util/Teleportation.h"
#include "util/hooks/HookMacros.h"
#include "util/GameUtil.h"

#include <CMessages.h>
#include <CPad.h>
#include <CPlayerPed.h>
#include <CCutsceneMgr.h>
#include <CCamera.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

char cutsceneArray[148][64]
    = {"BCESA4W",  "BCESA5W",  "BCESAR2", "BCESAR4", "BCESAR5", "BCRAS1",
       "BCRAS2",   "BHILL1",   "BHILL2",  "BHILL3A", "BHILL3B", "BHILL3C",
       "BHILL5A",  "BHILL5B",  "CAS6B_1", "CAS6B_2", "CAS_11A", "CAS_1A",
       "CAS_2",    "CAS_3",    "CAS_4A",  "CAS_4B",  "CAS_4C",  "CAS_5A",
       "CAS_6A",   "CAS_7B",   "CAS_9A1", "CAS_9A2", "CAT_1",   "CAT_2",
       "CAT_3",    "CAT_4",    "CESAR1A", "CRASH1A", "CRASH3A", "CRASHV1",
       "CRASHV2A", "CRASHV2B", "CUTTEST", "D10_ALT", "D8_ALT",  "DATE1A",
       "DATE1B",   "DATE2A",   "DATE2B",  "DATE3A",  "DATE3B",  "DATE4A",
       "DATE4B",   "DATE5A",   "DATE5B",  "DATE6A",  "DATE6B",  "DESERT1",
       "DESERT2",  "DESERT3",  "DESERT4", "DESERT6", "DESERT8", "DESERT9",
       "DES_10A",  "DES_10B",  "DOC_2",   "EPILOG",  "FARL_2A", "FARL_3A",
       "FARL_3B",  "FARL_4A",  "FARL_5A", "FINAL1A", "FINAL2A", "FINAL2B",
       "GARAG1B",  "GARAG1C",  "GARAG3A", "GROVE1A", "GROVE1B", "GROVE1C",
       "GROVE2",   "HEIST1A",  "HEIST2A", "HEIST4A", "HEIST5A", "HEIST6A",
       "HEIST8A",  "INTRO1A",  "INTRO1B", "INTRO2A", "PROLOG1", "PROLOG3",
       "RIOT4E1",  "RIOT4E2",  "RIOT_1A", "RIOT_1B", "RIOT_2",  "RIOT_4A",
       "RIOT_4B",  "RIOT_4C",  "RIOT_4D", "RYDER1A", "RYDER2A", "RYDER3A",
       "SCRASH1",  "SCRASH2",  "SMOKE1A", "SMOKE1B", "SMOKE2A", "SMOKE2B",
       "SMOKE3A",  "SMOKE4A",  "STEAL_1", "STEAL_2", "STEAL_4", "STEAL_5",
       "STRAP1A",  "STRAP2A",  "STRAP3A", "STRAP4A", "STRP4B1", "STRP4B2",
       "SWEET1A",  "SWEET1B",  "SWEET1C", "SWEET2A", "SWEET2B", "SWEET3A",
       "SWEET3B",  "SWEET4A",  "SWEET5A", "SWEET6A", "SWEET6B", "SWEET7A",
       "SYND_2A",  "SYND_2B",  "SYND_3A", "SYND_4A", "SYND_4B", "SYND_7",
       "TRUTH_1",  "TRUTH_2",  "W2_ALT",  "WOOZI1A", "WOOZI1B", "WOOZIE2",
       "WOOZIE4",  "ZERO_1",   "ZERO_2",  "ZERO_4"};

class PlayRandomCutscene : public EffectBase
{
    CVector oldPosition;
    int previousInterior = 0;
    bool cutsceneLoaded = false;
    bool ended = false;

public:
    bool CanActivate() override
    {
        return FindPlayerPed() != nullptr;
    }

    void OnStart(EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed();
        if (!player) return;

        oldPosition = player->GetPosition();
        previousInterior = player->m_nAreaCode;

        int index = inst->Random(0, 10000) % 148;
        const char* cutsceneName = cutsceneArray[index];

        //CMessages::AddMessageJumpQ(cutsceneName, 5000, 0, 0);
        CCutsceneMgr::LoadCutsceneData(cutsceneName);

        HOOK (inst, Hooked_IsCutsceneSkipButtonBeingPressed, bool (), 0x469F0E, 0x5B1947);
    }

    static bool
    Hooked_IsCutsceneSkipButtonBeingPressed (auto &&cb)
    {
        return false; // Ignore Skip Cutscene
    }

    void OnEnd(EffectInstance *inst) override
    {
        
    }

    void OnTick(EffectInstance *inst) override
    {

        static char msg[256];
        snprintf(msg, 256, "Cutscene Playing : %d | Cutscene Status %d", GameUtil::IsCutsceneProcessing (), CCutsceneMgr::ms_cutsceneLoadStatus);

        if (!cutsceneLoaded)
        {
            if (Command<Commands::HAS_CUTSCENE_LOADED>())  // Ensure it's fully loaded
            {
                CCutsceneMgr::StartCutscene();
                cutsceneLoaded = true;
            }
        }
        else
        {
            CMessages::AddMessageJumpQ(msg, 1000, 0, 0);
            if (CCutsceneMgr::HasCutsceneFinished())
            {
                static char message[256];
                snprintf(message, 256, "Cutscene Done");
                CMessages::AddMessageJumpQ(message, 1000, 0, 0);
                CCutsceneMgr::FinishCutscene();
                TheCamera.FinishCutscene();
            }
        }
    }
};

DEFINE_EFFECT (PlayRandomCutscene, "effect_random_cutscene", 0);