#include <util/EffectBase.h>

using namespace plugin;

template <int bias> class SteerBiasEffect : public EffectBase
{
public:
    void
    OnStart (EffectInstance *inst) override
    {
        Events::processScriptsEvent += OnProcessScripts;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        Events::processScriptsEvent -= OnProcessScripts;
    }

    static void
    OnProcessScripts ()
    {
        CPlayerPed *player = FindPlayerPed ();
        if (player)
        {
            CVehicle *vehicle = FindPlayerVehicle (-1, false);
            if (vehicle)
            {
                // TODO: This is only working when we are using
                // Events::processScriptsEvent, not Events::gameProcessEvent
                CPad *pad = player->GetPadFromPlayer ();
                if (pad) pad->NewState.LeftStickX += bias;
            }
        }
    }
};

// clang-format off
using SteerBiasLeftEffect = SteerBiasEffect<-64>;
DEFINE_EFFECT (SteerBiasLeftEffect, "effect_steer_bias_left", 0);

using SteerBiasRightEffect = SteerBiasEffect<64>;
DEFINE_EFFECT (SteerBiasRightEffect, "effect_steer_bias_right", 0);
// clang-format on