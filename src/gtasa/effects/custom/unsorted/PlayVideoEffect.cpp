/***
WORK IN PROGRESS
***/

#include <cmath>

#include "effects/OneTimeEffect.h"
#include "util/hooks/HookMacros.h"

#include <RenderWare.h>
#include <CCredits.h>
#include <CTimer.h>
#include <CFont.h>
#include <CText.h>
#include <CCamera.h>

class PlayVideoEffect : public OneTimeEffect
{

public:

    void
    OnStart (EffectInstance *inst) override
    {
		//char path_s[] = "movies\\GTAtitles.mpg";
		//char* path = path_s;
		//plugin::Call<0x747660, int, char*>(0, path);
		gGameState = 3;
    }
};

DEFINE_EFFECT (PlayVideoEffect, "effect_play_video", 0);