#include "util/EffectBase.h"
#include "util/GlobalRenderer.h"

class ObjectRandomSizeEffect : public EffectBase
{
public:
    static inline RwV3d scale = {1.0f, 1.0f, 1.0f};
    void
    OnStart (EffectInstance *inst) override
    {
        GlobalRenderer::RenderBuildingEvent += RenderBuilding;
        GlobalRenderer::RenderObjectEvent += RenderObject;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        GlobalRenderer::RenderBuildingEvent -= RenderBuilding;
        GlobalRenderer::RenderObjectEvent -= RenderObject;
    }

    void
    OnTick (EffectInstance *inst) override
    {
        scale.x = inst->Random(0.01f, 2.0f);
        scale.y = inst->Random(0.01f, 2.0f);
        scale.z = inst->Random(0.01f, 2.0f);
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
};

// clang-format off
DEFINE_EFFECT (ObjectRandomSizeEffect, "effect_object_size_random", 0);
// clang-format on