#include "util/BoneHelper.h"
#include "util/EffectBase.h"
#include "util/GenericUtil.h"
#include <CCutsceneMgr.h>
#include <ePedBones.h>

#include "util/GlobalRenderer.h"

class CrazyScale : public EffectBase
{
    static inline RwV3d scale = {1.0f, 1.0f, 1.0f};
    static inline bool isOnTruckingMission = false;
    static inline bool isOnForkliftMission = false;	
public:
    void
    OnStart (EffectInstance *inst) override
    {
        GlobalRenderer::RenderBuildingEvent += RenderBuilding;
        GlobalRenderer::RenderObjectEvent += RenderObject;
		BoneHelper::RenderEvent += RenderPed;
		
		isOnTruckingMission = false;
        isOnForkliftMission = false;
        GlobalRenderer::RenderVehicleEvent += RenderVehicle;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        GlobalRenderer::RenderBuildingEvent -= RenderBuilding;
        GlobalRenderer::RenderObjectEvent -= RenderObject;
		BoneHelper::RenderEvent -= RenderPed;
		
		GlobalRenderer::RenderVehicleEvent -= RenderVehicle;
    }

    void
    OnTick (EffectInstance *inst) override
    {
        scale.x = inst->Random(0.01f, 2.0f);
        scale.y = inst->Random(0.01f, 2.0f);
        scale.z = inst->Random(0.01f, 2.0f);
		
		UpdateMissionChecks ();
    }

    static void
    RenderBuilding (CBuilding *building, RwFrame *frame)
    {
        RwFrameScale (frame, &scale, rwCOMBINEPRECONCAT);
    }

    static void
    RenderObject (CObject *object, RwFrame *frame)
    {
        RwFrameScale (frame, &scale, rwCOMBINEPRECONCAT);
    }
	
	static void
    RenderPed (CPed *ped)
    {
        // Scale every possible bone
        for (int i = 0; i < 500; i++)
            BoneHelper::ScaleBone (ped, i, scale);

        for (int i = 5000; i < 5026; i++)
            BoneHelper::ScaleBone (ped, i, scale);
    }
	
	void
    UpdateMissionChecks ()
    {
        isOnTruckingMission = false;
        isOnForkliftMission = false;

        for (auto i = CTheScripts::pActiveScripts; i; i = i->m_pNext)
        {
            if (!i->m_bIsMission || !i->m_bIsActive) continue;

            std::string missionName
                = GenericUtil::ToUpper (std::string (i->m_szName));

            if (missionName == "CAT3" || missionName == "TRUCK"
                || missionName == "TORENO2")
            {
                isOnTruckingMission = true;
            }

            if (missionName == "RYDER2")
            {
                isOnForkliftMission = true;
            }
        }
    }

    static bool
    IsVehicleModelValid (int modelId)
    {
        if (isOnTruckingMission)
        {
            switch (modelId)
            {
                case 403: // Linerunner
                case 514: // Tanker
                case 515: // Roadtrain
                    return false;

                default: break;
            }
        }

        if (isOnForkliftMission)
        {
            switch (modelId)
            {
                case 530: // Forklift
                    return false;

                default: break;
            }
        }

        return true;
    }

    static void
    RenderVehicle (CVehicle *vehicle, RwFrame *frame)
    {
        if (!IsVehicleModelValid (vehicle->m_nModelIndex)) return;

        RwFrameTranslate (frame, &scale, rwCOMBINEPRECONCAT);

        RwFrameScale (frame, &scale, rwCOMBINEPRECONCAT);
    }
};

// clang-format off
DEFINE_EFFECT (CrazyScale, "effect_crazy_scale", 0);
// clang-format on