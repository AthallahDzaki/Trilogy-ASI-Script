#include "util/EffectBase.h"

#include <CMenuManager.h>

class FramerateEffect : public EffectBase
{
    int oldFramerate = 25;
    int framerate    = 25;

    bool oldVSync = false;

public:
    FramerateEffect (int framerate) : EffectBase ()
    {
        this->framerate = framerate;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        inst->WriteMemory<byte> (0x53E94C, 0);

        this->oldFramerate    = RsGlobal.maxFPS;
        this->oldVSync        = FrontEndMenuManager.m_bPrefsVsync;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        RsGlobal.maxFPS                       = this->oldFramerate;
        FrontEndMenuManager.m_bPrefsVsync     = this->oldVSync;
    }

    void
    OnTick (EffectInstance *inst) override
    {
        RsGlobal.maxFPS                       = this->framerate;
        FrontEndMenuManager.m_bPrefsVsync     = true;
    }
};

DEFINE_EFFECT (FramerateEffect, "effect_fps_15", GROUP_FRAMERATE, 15);
DEFINE_EFFECT (FramerateEffect, "effect_fps_60", GROUP_FRAMERATE, 60);