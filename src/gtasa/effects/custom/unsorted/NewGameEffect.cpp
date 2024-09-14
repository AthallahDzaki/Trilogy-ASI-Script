#include "effects/OneTimeEffect.h"

#include <CMenuManager.h>
#include <CGame.h>

class NewGameEffect : public OneTimeEffect
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        FrontEndMenuManager.m_bMenuActive  = false;

        byte gameState
            = injector::ReadMemory<byte> (0xC8D4C0, true); // GameState

        if (gameState == 9)
        {
            CGame::ShutDownForRestart ();
            CGame::InitialiseWhenRestarting ();
        }

        CGame::bMissionPackGame = 0;
        FrontEndMenuManager.DoSettingsBeforeStartingAGame();
        FrontEndMenuManager.m_bShutDownFrontEndRequested = 1;
    }
};

DEFINE_EFFECT (NewGameEffect, "effect_newgame", 0);