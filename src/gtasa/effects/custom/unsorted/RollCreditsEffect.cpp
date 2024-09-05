#include "effects/OneTimeEffect.h"
#include "util/hooks/HookMacros.h"

#include <RenderWare.h>
#include <CCredits.h>
#include <CTimer.h>
#include <CFont.h>
#include <CCamera.h>

class RollCreditsEffect : public OneTimeEffect
{
    // int creditsStartTime = 0;
    static CRGBA  dropColor;
    static float  currentPosition;
    static float  currentOffset;
    static float  centerSize;
    static float  endOffset;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        HOOK (inst, Hooked_CCredits_Render, void (), 0x53EBC8, 0x53EBDF, 0x53E862);

        CCredits::Start ();
    }

    static void
    Hooked_CCredits_Render (auto &&cb)
    {
        DefinedState2d();
        currentPosition = 0.0;
        currentOffset = (CTimer::m_snTimeInMilliseconds - CCredits::CreditsStartTime) * 0.043478262;
        CFont::SetBackground(0, 0);
        centerSize = RsGlobal.maximumWidth * 0.92000002;
        CFont::SetCentreSize(centerSize);
        CFont::SetOrientation (ALIGN_CENTER);
        CFont::SetProportional (true);
        CFont::SetFontStyle(FONT_SUBTITLES);
        CFont::SetEdge(0);
        dropColor = CRGBA::CRGBA(0, 0, 0, 0xFF);
        CFont::SetDropColor(dropColor);

        float _static_currentOffset = currentOffset;

        for(int i = 0; i < 30; i++) {
            static char text[256];
            snprintf(text, 256, "%s", "Test");
            unsigned short theText = *(unsigned short*)text;
            unsigned int currentPosNP = (unsigned int) (signed int) &currentPosition;
            CCredits::PrintCreditText(1.1, 1.1, &theText, &currentPosNP, _static_currentOffset, 0);
            currentPosition += 37.5;
        }

        CFont::DrawFonts();
        if ( TheCamera.m_bWideScreenOn )
            TheCamera.DrawBordersForWideScreen();

        endOffset = ((currentPosition) + RsGlobal.maximumHeight) - currentOffset;
        if ( endOffset < -10.0 )
            CCredits::bCreditsGoing = 0;

    }

    // void
    // OnEnd (EffectInstance *inst) override
    // {
    //     if (CCredits::CreditsStartTime == this->creditsStartTime)
    //     {
    //         CCredits::Stop ();
    //     }
    // }

    // void
    // OnTick (EffectInstance *inst) override
    // {
    //     if (!CCredits::bCreditsGoing)
    //     {
    //         CCredits::Start ();
    //         this->creditsStartTime = CCredits::CreditsStartTime;
    //     }
    // }
};

DEFINE_EFFECT (RollCreditsEffect, "effect_roll_credits", 0);