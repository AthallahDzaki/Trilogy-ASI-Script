#include "util/EffectBase.h"

#include <CStreaming.h>
#include <CTimer.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class SkibidiToiletEffect : public EffectBase
{
public:
    static inline CObject *toiletObject;

    void
    OnStart (EffectInstance *inst) override
    {
        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        CVector position
            = player->TransformFromObjectSpace (CVector (0.0f, 10.0f, 0.0f));

        bool  groundResult = false;
        float groundPos = CWorld::FindGroundZFor3DCoord (position.x, position.y,
                                                         position.z + 5.0f,
                                                         &groundResult, 0)
                          + 0.5f;

        if (groundResult) position.z = groundPos;

        position.z -= 0.3f;

        int model = 2514; // Toilet
        CStreaming::RequestModel (model, 2);
        CStreaming::LoadAllRequestedModels (false);
        Command<Commands::CREATE_OBJECT> (model, position.x,
                                                         position.y, position.z,
                                                         &toiletObject);
        CStreaming::SetModelIsDeletable (model);

        Command<Commands::SET_OBJECT_COLLISION> (toiletObject, false);

        CMatrix *matrix;

        CVehicle *vehicle = FindPlayerVehicle (-1, false);
        if (vehicle)
            matrix = vehicle->GetMatrix ();
        else
            matrix = player->GetMatrix ();

        toiletObject->SetMatrix (*matrix);
        toiletObject->SetPosn (position);
        toiletObject->m_nPhysicalFlags.bCollisionProof = false;
        toiletObject->m_nObjectType                    = OBJECT_TEMPORARY;
        toiletObject->m_dwRemovalTime
            = CTimer::m_snTimeInMilliseconds + inst->GetEffectDuration() + 5000;

        Command<Commands::ATTACH_OBJECT_TO_CHAR> (toiletObject, player, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 180.0f);
    }

    void
    OnEnd(EffectInstance *inst) override
    {
        if (toiletObject)
        {
            Command<Commands::DELETE_OBJECT> (&toiletObject);
            toiletObject = nullptr;
        }
    }
};

DEFINE_EFFECT (SkibidiToiletEffect, "effect_skibidi_toilet", 0);