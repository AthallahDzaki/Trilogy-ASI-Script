#include "effects/OneTimeEffect.h"

#include <CPlayerPed.h>
#include "util/HoboManager.h"

using namespace plugin;

class RemoveAngryHobo : public OneTimeEffect
{
    int removeType = 0; // 0 - Random, 1 - Nearest, 2 - Farthest, 3 - All
public:
    RemoveAngryHobo (int type) : removeType (type) {}

    bool
    CanActivate () override
    {
        return HoboManager::angryHobo.size () > 0;
    }

    CVector
    FindSuitableTeleportPosition (EffectInstance *inst)
    {
        CVector randomPosition
            = CVector (inst->Random (-3000.0f, 3000.0f),
                       inst->Random (-3000.0f, 3000.0f), 0.0f);

        Command<Commands::REQUEST_COLLISION> (randomPosition.x,
                                                             randomPosition.y);
        CStreaming::StreamZoneModels (&randomPosition);
        CStreaming::LoadAllRequestedModels (false);
        CStreaming::LoadScene (&randomPosition);

        bool groundResult = false;
        randomPosition.z
            = CWorld::FindGroundZFor3DCoord (randomPosition.x, randomPosition.y,
                                             randomPosition.z + 250.0f,
                                             &groundResult, 0)
              + 3.0f;

        if (!groundResult) return FindSuitableTeleportPosition (inst);

        float waterLevel = 0.0f;
        Command<Commands::GET_WATER_HEIGHT_AT_COORDS> (
            randomPosition.x, randomPosition.y, false, &waterLevel);

        if (randomPosition.z <= waterLevel)
            return FindSuitableTeleportPosition (inst);

        return randomPosition;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        switch (removeType) {
            case 0: HoboManager::RemoveRandomAngryHobo (); break;
            case 1: {
                CPed *player = FindPlayerPed ();
                if (player) {
                    CPed *nearest = nullptr;
                    float nearestDist = 999999.0f;
                    //static inline std::vector<std::tuple<CPed *, int, eWeaponType>> angryHobo = {};
                    for(int i = 0; i < HoboManager::angryHobo.size (); i++) {
                        CPed *ped = std::get<CPed *> (HoboManager::angryHobo.at (i));
                        if (ped) {
                            float dist = DistanceBetweenPoints (player->GetPosition (), ped->GetPosition ());
                            if (dist < nearestDist) {
                                nearest = ped;
                                nearestDist = dist;
                            }
                        }
                    }
                    if (nearest) {
                        HoboManager::RemoveAngryHobo (nearest);
                    }
                }
                break;
            }
            case 2: {
                CPed *player = FindPlayerPed ();
                if (player) {
                    CPed *farthest = nullptr;
                    float farthestDist = 0.0f;
                    for(int i = 0; i < HoboManager::angryHobo.size (); i++) {
                        CPed *ped = std::get<CPed *> (HoboManager::angryHobo.at (i));
                        if (ped) {
                            float dist = DistanceBetweenPoints (player->GetPosition (), ped->GetPosition ());
                            if (dist > farthestDist) {
                                farthest = ped;
                                farthestDist = dist;
                            }
                        }
                    }
                    if (farthest) {
                        HoboManager::RemoveAngryHobo (farthest);
                    }
                }
                break;
            }
            case 3: HoboManager::RemoveAllAngryHobo (); break;
            case 4: {
                CPed *player = FindPlayerPed ();
                if (player) {
                    for(int i = 0; i < HoboManager::angryHobo.size (); i++) {
                        CPed *ped = std::get<CPed *> (HoboManager::angryHobo.at (i));
                        if (ped) {
                            CVector position = player->GetPosition ();
                            position.z += 3.0f;
                            ped->SetPosn (position);
                        }
                    }
                }
                break;
            }
            case 5: {
                for(int i = 0; i < HoboManager::angryHobo.size (); i++) {
                    CPed *ped = std::get<CPed *> (HoboManager::angryHobo.at (i));
                    if (ped) {
                        CVector position = FindSuitableTeleportPosition (inst);
                        ped->SetPosn (position);
                    }
                }
                break;
            }
        }
    }
};

DEFINE_EFFECT (RemoveAngryHobo, "effect_remove_random_angry_hobo", 0, 0);
DEFINE_EFFECT (RemoveAngryHobo, "effect_remove_nearest_angry_hobo", 0, 1);
DEFINE_EFFECT (RemoveAngryHobo, "effect_remove_farthest_angry_hobo", 0, 2);
DEFINE_EFFECT (RemoveAngryHobo, "effect_remove_all_angry_hobo", 0, 3);
DEFINE_EFFECT (RemoveAngryHobo, "effect_move_all_angry_hobo_to_you", 0, 4);
DEFINE_EFFECT (RemoveAngryHobo, "effect_move_all_angry_hobo_to_random", 0, 5);
