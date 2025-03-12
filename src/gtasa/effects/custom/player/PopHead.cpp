#include "util/EffectBase.h"
#include "util/GameUtil.h"
#include "util/BoneHelper.h"
#include "util/GenericUtil.h"

#include <ePedBones.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class PopHeadEffect : public EffectBase
{
	int  wait  = 0;
	static inline RwV3d scale = {1.0f, 1.0f, 1.0f};
	
public:
    bool
    CanActivate () override
    {
        return GameUtil::IsPlayerSafe ();
    }

    void
    OnStart (EffectInstance *inst) override
    {
		wait = 0;
		BoneHelper::RenderEvent += RenderPed;
    }
	
	void
	OnEnd (EffectInstance *inst) override
	{
		BoneHelper::RenderEvent -= RenderPed;
		
		scale = {1.0f, 1.0f, 1.0f};
		
		CPlayerPed *pPed = FindPlayerPed ();
		
		for (int i = BONE_NECK; i <= BONE_RIGHTUPPERTORSO - 1; i++)
            BoneHelper::ScaleBone (pPed, i, scale, BONE_NECK);

        for (int i = 5000; i < 5026; i++)
            BoneHelper::ScaleBone (pPed, i, scale, BONE_NECK);

        // Cutscene related?
        BoneHelper::ScaleBone (pPed, 30, scale, BONE_NECK);
	}

    void
    OnTick (EffectInstance *inst) override
    {
		wait -= (int) GenericUtil::CalculateTick ();
        if (wait > 0) return;
		
        CPlayerPed *player = FindPlayerPed ();
		
		scale.x += 0.1f;
		scale.y += 0.1f;
		scale.z += 0.1f;
		
		if(scale.x > -0.5f && scale.x < -0.3f)
		{
			inst->Disable ();
		}
		
		if(scale.x > 5.0) {
			scale = { -1.1f, -1.1f, -1.1f };
			Command<Commands::EXPLODE_CHAR_HEAD> (player);
		}
		
		wait = 50;
    }
	
	static void
    RenderPed (CPed *ped)
    {
		CPlayerPed *pPed = FindPlayerPed ();
		if(ped != pPed) return;
		
		for (int i = BONE_NECK; i <= BONE_RIGHTUPPERTORSO - 1; i++)
            BoneHelper::ScaleBone (pPed, i, scale, BONE_NECK);

        for (int i = 5000; i < 5026; i++)
            BoneHelper::ScaleBone (pPed, i, scale, BONE_NECK);

        // Cutscene related?
        BoneHelper::ScaleBone (pPed, 30, scale, BONE_NECK);
    }
};

DEFINE_EFFECT (PopHeadEffect, "effect_pop_head", GROUP_HEALTH);