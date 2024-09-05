#include "util/EffectBase.h"
#include "util/GlobalRenderer.h"

#include <deque>

#include <CEntity.h>
#include <CFont.h>
#include <CPed.h>
#include <CPlayerPed.h>
#include <CSprite.h>
#include <CWorld.h>
#include <common.h>

using namespace plugin;

class MidasTouchEffect : public EffectBase
{
    /* Vehicle Section */
    static inline std::vector<CVehicle *> touchedCars = {};

    static inline std::deque<std::pair<RwRGBA *, RwRGBA>> resetCarEntries = {};

    static inline ThiscallEvent<AddressList<0x5532A9, H_CALL>, PRIORITY_BEFORE,
                                ArgPickN<CVehicle *, 0>, void (CVehicle *)>
        setupRenderEvent;
    static inline ThiscallEvent<AddressList<0x55332A, H_CALL>, PRIORITY_AFTER,
                                ArgPickN<CVehicle *, 0>, void (CVehicle *)>
        resetAfterRenderEvent;

    /* Ped Section */

    static inline std::vector<CPed *>                     touchedPeds     = {};
    static inline std::deque<std::pair<RwRGBA *, RwRGBA>> resetPedEntries = {};

    /* Object Section */

    static inline std::deque<std::pair<RwRGBA *, RwRGBA>> resetObjectEntries
        = {};
    static inline std::vector<CObject *> touchedObjects = {};

    static inline ThiscallEvent<
        AddressList<0x54BDB2, H_CALL, 0x54BF78, H_CALL, 0x54C23A, H_CALL,
                    0x54C435, H_CALL, 0x54D17E, H_CALL, 0x54D27E, H_CALL,
                    0x54D3FE, H_CALL, 0x54D4D2, H_CALL>,
        PRIORITY_AFTER,
        ArgPick<ArgTypes<CPhysical *, CPhysical *, int, float *, int>, 0, 1, 2,
                3, 4>,
        void (CPhysical *, CPhysical *, int, float *, int)>
        applyCollisionEvent;

public:
    bool
    CanActivate () override
    {
        return FindPlayerPed () != nullptr;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        /* Vehicle Section */

        setupRenderEvent += RenderCarBefore;
        resetAfterRenderEvent += RenderCarAfter;

        Events::pedRenderEvent.before += RenderPedBefore;
        Events::pedRenderEvent.after += RenderPedAfter;

        // if(IsPluginInstalled("SilentPatchSA.asi") && patch::GetUChar(0x59F180) == 0xE9) {
        //     static ThiscallEvent<AddressList<0x59F180, H_JUMP>, PRIORITY_BEFORE, ArgPickN<CObject *, 0>, void (CObject *)> silentPatchRenderObjectEvent;
        //     silentPatchRenderObjectEvent.before += RenderObjectBefore;
        //     silentPatchRenderObjectEvent.after += RenderObjectBefore;
        // } else {
        //     Events::objectRenderEvent.before += RenderObjectBefore;
        //     Events::objectRenderEvent.after += RenderObjectAfter;
        // }

        GlobalRenderer::RenderObjectEvent += RenderObject;

        // Events::drawingEvent += DebugObjects;

        applyCollisionEvent += ApplyCollision;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        // Restore all the colors
        for (auto const &[color, backupColor] : resetCarEntries)
            *color = backupColor;
        
        for (auto const &[color, backupColor] : resetPedEntries)
            *color = backupColor;

        for (auto const &[color, backupColor] : resetObjectEntries)
            *color = backupColor;

        resetCarEntries.clear ();
        resetPedEntries.clear ();
        resetObjectEntries.clear ();

        touchedCars.clear ();
        touchedPeds.clear ();
        touchedObjects.clear ();

        Events::pedRenderEvent.before -= RenderPedBefore;
        Events::pedRenderEvent.after -= RenderPedAfter;

        setupRenderEvent -= RenderCarBefore;
        resetAfterRenderEvent -= RenderCarAfter;

        // if(IsPluginInstalled("SilentPatchSA.asi") && patch::GetUChar(0x59F180) == 0xE9) {
        //     static ThiscallEvent<AddressList<0x59F180, H_JUMP>, PRIORITY_BEFORE, ArgPickN<CObject *, 0>, void (CObject *)> silentPatchRenderObjectEvent;
        //     silentPatchRenderObjectEvent.before -= RenderObjectBefore;
        //     silentPatchRenderObjectEvent.after -= RenderObjectAfter;
        // } else {
        //     Events::objectRenderEvent.before -= RenderObjectBefore;
        //     Events::objectRenderEvent.after -= RenderObjectAfter;
        // }

        GlobalRenderer::RenderObjectEvent -= RenderObject;

        // Events::drawingEvent -= DebugObjects;

        applyCollisionEvent -= ApplyCollision;
    }

    static void
    ApplyCollision (CPhysical *thisEntity, CPhysical *otherEntity, int a3,
                    float *a4, int a5)
    {
        CPlayerPed *player        = FindPlayerPed ();
        CVehicle   *playerVehicle = FindPlayerVehicle (-1, false);

        bool AreWeUseVehicle = playerVehicle != nullptr;

        if ((otherEntity->m_nType != ENTITY_TYPE_VEHICLE
             && otherEntity->m_nType != ENTITY_TYPE_OBJECT
             && otherEntity->m_nType != ENTITY_TYPE_PED)
            || (thisEntity->m_nType != ENTITY_TYPE_VEHICLE
                && thisEntity->m_nType != ENTITY_TYPE_OBJECT
                && thisEntity->m_nType != ENTITY_TYPE_PED))
        {
            // We Skip if the entity is not a vehicle, object or ped
            return;
        }

        if (AreWeUseVehicle)
        {
            if (playerVehicle == thisEntity || playerVehicle == otherEntity)
            {
                if(playerVehicle == otherEntity)
                {
                    switch(thisEntity->m_nType) {
                        case ENTITY_TYPE_VEHICLE: {
                            touchedCars.push_back(reinterpret_cast<CVehicle *>(thisEntity));
                            break;
                        }
                        case ENTITY_TYPE_PED: {
                            touchedPeds.push_back(reinterpret_cast<CPed *>(thisEntity));
                            break;
                        }
                        // case ENTITY_TYPE_OBJECT: {
                        //     touchedObjects.push_back(reinterpret_cast<CObject *>(thisEntity));
                        //     break;
                        // }
                    }
                } else if(playerVehicle == thisEntity) {
                    switch(otherEntity->m_nType) {
                        case ENTITY_TYPE_VEHICLE: {
                            touchedCars.push_back(reinterpret_cast<CVehicle *>(otherEntity));
                            break;
                        }
                        case ENTITY_TYPE_PED: {
                            touchedPeds.push_back(reinterpret_cast<CPed *>(otherEntity));
                            break;
                        }
                        // case ENTITY_TYPE_OBJECT: {
                        //     touchedObjects.push_back(reinterpret_cast<CObject *>(otherEntity));
                        //     break;
                        // }
                    }
                }
            }
        }
        else
        {
            if (player == thisEntity || player == otherEntity)
            {
                if(player == otherEntity)
                {
                    switch(thisEntity->m_nType) {
                        case ENTITY_TYPE_VEHICLE: {
                            touchedCars.push_back(reinterpret_cast<CVehicle *>(thisEntity));
                            break;
                        }
                        case ENTITY_TYPE_PED: {
                            touchedPeds.push_back(reinterpret_cast<CPed *>(thisEntity));
                            break;
                        }
                        // case ENTITY_TYPE_OBJECT: {
                        //     touchedObjects.push_back(reinterpret_cast<CObject *>(thisEntity));
                        //     break;
                        // }
                    }
                } else if(player == thisEntity) {
                    switch(otherEntity->m_nType) {
                        case ENTITY_TYPE_VEHICLE: {
                            touchedCars.push_back(reinterpret_cast<CVehicle *>(otherEntity));
                            break;
                        }
                        case ENTITY_TYPE_PED: {
                            touchedPeds.push_back(reinterpret_cast<CPed *>(otherEntity));
                            break;
                        }
                        // case ENTITY_TYPE_OBJECT: {
                        //     touchedObjects.push_back(reinterpret_cast<CObject *>(otherEntity));
                        //     break;
                        // }
                    }
                }
            }
        }
    }

    // void
    // OnTick (EffectInstance *inst) override
    // {
    //     CPlayerPed *player = FindPlayerPed ();
    //     for (CVehicle *vehicle : CPools::ms_pVehiclePool)
    //     {
    //         if (player->GetIsTouching (reinterpret_cast<CEntity *>
    //         (vehicle)))
    //         {
    //             touchedCars.push_back (vehicle);
    //         }
    //     }

    //     for (CPed *ped : CPools::ms_pPedPool)
    //     {
    //         if (ped == player) continue;
    //         if (player->GetIsTouching (reinterpret_cast<CEntity *> (ped)))
    //         {
    //             touchedPeds.push_back (ped);
    //         }
    //     }

    //     for (CObject *object : CPools::ms_pObjectPool)
    //     {
    //         if (player->GetIsTouching (reinterpret_cast<CEntity *> (object)))
    //         {
    //             touchedObjects.push_back (object);
    //         }
    //     }
    // }

    static void
    DebugObjects ()
    {
        CPlayerPed *player = FindPlayerPed ();
        for (auto object : CPools::ms_pObjectPool)
        {
            CVector &posn   = object->GetPosition ();
            RwV3d    rwPosn = {posn.x, posn.y, posn.z};
            RwV3d    screenCoors;
            float    w, h;
            if (CSprite::CalcScreenCoors (rwPosn, &screenCoors, &w, &h, true,
                                          true))
            {
                CFont::SetOrientation (ALIGN_CENTER);
                CFont::SetColor (CRGBA (255, 255, 255, 255));
                CFont::SetDropShadowPosition (1);
                CFont::SetBackground (false, false);
                CFont::SetWrapx (500.0);
                CFont::SetScale (0.5, 1.0);
                CFont::SetFontStyle (FONT_SUBTITLES);
                CFont::SetProportional (true);
                char text[256];
                sprintf (text, "Object: %d | Touched : %d", object->m_nModelIndex, player->GetIsTouching (object));
                CFont::PrintString (screenCoors.x, screenCoors.y, text);
            }
        }
    }

    /* PED Section */

    static void
    RenderPedBefore (CPed *ped)
    {
        for (auto touched : touchedPeds)
        {
            if (ped == touched)
            {
                if (!ped->m_pRwObject) continue;
                if (ped->m_pRwClump->object.type == rpCLUMP)
                {
                    RpClumpForAllAtomics (
                        reinterpret_cast<RpClump *> (ped->m_pRwClump),
                        [] (RpAtomic *atomic, void *data)
                        {
                            if (atomic->geometry)
                            {
                                atomic->geometry->flags
                                    |= rpGEOMETRYMODULATEMATERIALCOLOR;
                                RpGeometryForAllMaterials (
                                    atomic->geometry,
                                    [] (RpMaterial *material, void *data)
                                    {
                                        resetPedEntries.push_front (
                                            std::make_pair (&material->color,
                                                            material->color));
                                        material->color.red   = 255;
                                        material->color.green = 215;
                                        material->color.blue  = 0;
                                        material->color.alpha = 255;
                                        return material;
                                    },
                                    data);
                            }
                            return atomic;
                        },
                        ped);
                }
            }
        }
    }

    static void
    RenderPedAfter (CPed *ped)
    {
        // Check if ped already in tupple list
        for (auto const &[color, backupColor] : resetPedEntries)
            *color = backupColor;
        resetPedEntries.clear ();

        // auto it = std::find (touchedPeds.begin (), touchedPeds.end (), ped);
        // if (it != touchedPeds.end ()) touchedPeds.erase (it);
    }

    /* Vehicle Section */

    static void
    RenderCarBefore (CVehicle *vehicle)
    {
        // Check if vehicle already in tupple list
        for (auto touched : touchedCars)
        {
            if (vehicle == touched)
            {
                if (vehicle->m_pRwObject)
                {
                    if (vehicle->m_pRwObject->type == rpCLUMP)
                    {
                        RpClumpForAllAtomics (
                            reinterpret_cast<RpClump *> (vehicle->m_pRwObject),
                            [] (RpAtomic *atomic, void *data)
                            {
                                if (atomic->geometry)
                                {
                                    atomic->geometry->flags
                                        |= rpGEOMETRYMODULATEMATERIALCOLOR;
                                    RpGeometryForAllMaterials (
                                        atomic->geometry,
                                        [] (RpMaterial *material, void *data)
                                        {
                                            resetCarEntries.push_front (
                                                std::make_pair (
                                                    &material->color,
                                                    material->color));
                                            material->color.red   = 255;
                                            material->color.green = 215;
                                            material->color.blue  = 0;
                                            material->color.alpha = 255;
                                            return material;
                                        },
                                        data);
                                }
                                return atomic;
                            },
                            vehicle);
                    }
                    else
                    {
                        // AtomicCallback (reinterpret_cast<RpAtomic *> (
                        //                     vehicle->m_pRwObject),
                        //                 vehicle);
                        RpAtomic *atomic = reinterpret_cast<RpAtomic *> (
                            vehicle->m_pRwObject);
                        if (atomic->geometry)
                        {
                            atomic->geometry->flags
                                |= rpGEOMETRYMODULATEMATERIALCOLOR;
                            RpGeometryForAllMaterials (
                                atomic->geometry,
                                [] (RpMaterial *material, void *data)
                                {
                                    resetCarEntries.push_back (
                                        std::make_pair (&material->color,
                                                        material->color));
                                    material->color.red   = 255;
                                    material->color.green = 215;
                                    material->color.blue  = 0;
                                    material->color.alpha = 255;
                                    return material;
                                },
                                vehicle);
                        }
                    }
                }
            }
        }
    }

    static void
    RenderCarAfter (CVehicle *vehicle)
    {
        // Check if vehicle already in tupple list
        for (auto const &[color, backupColor] : resetCarEntries)
            *color = backupColor;
        resetCarEntries.clear ();

        // auto it = std::find (touchedCars.begin (), touchedCars.end (),
        // vehicle); if (it != touchedCars.end ()) touchedCars.erase (it);
    }

    /* Object Section */

    static void
    RenderObject (CObject *object, RwFrame *frame)
    {
        for (auto touched : touchedObjects)
        {
            if (object == touched)
            {
                if (object->m_pRwObject)
                {
                    if (object->m_pRwObject->type == rpCLUMP)
                    {
                        RpClumpForAllAtomics (
                            reinterpret_cast<RpClump *> (object->m_pRwObject),
                            [] (RpAtomic *atomic, void *data)
                            {
                                if (atomic->geometry)
                                {
                                    atomic->geometry->flags
                                        |= rpGEOMETRYMODULATEMATERIALCOLOR;
                                    RpGeometryForAllMaterials (
                                        atomic->geometry,
                                        [] (RpMaterial *material, void *data)
                                        {
                                            resetObjectEntries.push_front (
                                                std::make_pair (
                                                    &material->color,
                                                    material->color));
                                            material->color.red   = 255;
                                            material->color.green = 215;
                                            material->color.blue  = 0;
                                            material->color.alpha = 255;
                                            return material;
                                        },
                                        data);
                                }
                                return atomic;
                            },
                            object);
                    }
                    else
                    {
                        // AtomicCallback (reinterpret_cast<RpAtomic *> (
                        //                     object->m_pRwObject),
                        //                 object);
                        RpAtomic *atomic = reinterpret_cast<RpAtomic *> (
                            object->m_pRwObject);
                        if (atomic->geometry)
                        {
                            atomic->geometry->flags
                                |= rpGEOMETRYMODULATEMATERIALCOLOR;
                            RpGeometryForAllMaterials (
                                atomic->geometry,
                                [] (RpMaterial *material, void *data)
                                {
                                    resetObjectEntries.push_back (
                                        std::make_pair (&material->color,
                                                        material->color));
                                    material->color.red   = 255;
                                    material->color.green = 215;
                                    material->color.blue  = 0;
                                    material->color.alpha = 255;
                                    return material;
                                },
                                object);
                        }
                    }
                }
            }
        }
    }
};

DEFINE_EFFECT (MidasTouchEffect, "effect_midas_touch", 0);