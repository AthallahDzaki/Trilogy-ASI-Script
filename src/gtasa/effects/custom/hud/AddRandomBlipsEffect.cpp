#include "effects/OneTimeEffect.h"

#include <extensions/ScriptCommands.h>

using namespace plugin;

enum class RandomBlipType
{
    SPRITE,
    DESTINATION,
    BIG_DESTINATION
};

class AddRandomBlipsEffect : public OneTimeEffect
{
    RandomBlipType type;

public:

    AddRandomBlipsEffect () = delete;
    AddRandomBlipsEffect (RandomBlipType blipType) : type (blipType) {}

    void
    OnStart (EffectInstance *inst) override
    {
        for (int i = 0; i < inst->Random (1, 3); i++)
        {
            float x      = inst->Random (-3000.0f, 3000.0f);
            float y      = inst->Random (-3000.0f, 3000.0f);
            int theBlip  = -1;
            if (type == RandomBlipType::SPRITE) {
                int   sprite = inst->Random (2, 63);
                Command<Commands::ADD_SPRITE_BLIP_FOR_COORD> (
                    x, y, 0.0f, sprite);
            } else if (type == RandomBlipType::DESTINATION) {
                Command<Commands::ADD_BLIP_FOR_COORD> (x, y, 0.0f);
            } else if (type == RandomBlipType::BIG_DESTINATION) {
                Command<Commands::ADD_BLIP_FOR_COORD> (x, y, 0.0f, &theBlip);
                Command<Commands::CHANGE_BLIP_SCALE> (theBlip, 10);
            }
        }
    }
};

DEFINE_EFFECT (AddRandomBlipsEffect, "effect_add_random_sprite", 0, RandomBlipType::SPRITE);
DEFINE_EFFECT (AddRandomBlipsEffect, "effect_add_random_destination", 0, RandomBlipType::DESTINATION);
DEFINE_EFFECT (AddRandomBlipsEffect, "effect_add_random_big_destination", 0, RandomBlipType::BIG_DESTINATION);