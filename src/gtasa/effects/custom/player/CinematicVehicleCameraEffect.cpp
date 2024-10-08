#include "util/EffectBase.h"

#include <extensions/ScriptCommands.h>

using namespace plugin;

class CinematicVehicleCameraEffect : public EffectBase
{
public:
    bool
    CanTickDown (EffectInstance *instance) override
    {
        return !CONFIG_CC_ENABLED || FindPlayerVehicle (-1, false);
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        Command<Commands::SET_CINEMA_CAMERA> (false);
    }

    void
    OnTick (EffectInstance *inst) override
    {
        Command<Commands::SET_CINEMA_CAMERA> (
            FindPlayerVehicle (-1, false) != nullptr);
    }
};

DEFINE_EFFECT (CinematicVehicleCameraEffect, "effect_cinematic_vehicle_camera",
               GROUP_CAMERA);