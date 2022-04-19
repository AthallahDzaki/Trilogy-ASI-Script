#include <effects/OneTimeEffect.h>
#include <util/GameUtil.h>
#include <util/MathHelper.h>

enum eVehicleMomentumType
{
    BOOST,
    INVERT_MOMENTUM,
    INVERT_MOMENTUM_AND_MATRIX
};

class VehicleMomentumEffect : public OneTimeEffect
{
    eVehicleMomentumType type = eVehicleMomentumType::BOOST;

public:
    VehicleMomentumEffect (eVehicleMomentumType type) : OneTimeEffect ()
    {
        this->type = type;
    }

    void
    OnStart (EffectInstance *inst) override
    {
        GameUtil::SetVehiclesToRealPhysics ();

        for (CVehicle *vehicle : CPools::ms_pVehiclePool)
        {
            CMatrixLink *matrix = vehicle->GetMatrix ();
            if (this->type == eVehicleMomentumType::INVERT_MOMENTUM_AND_MATRIX)
            {
                InvertVehicle (matrix);
            }

            if (this->type == eVehicleMomentumType::BOOST)
            {
                vehicle->m_vecMoveSpeed.x = vehicle->m_vecMoveSpeed.x * 5.0f;
                vehicle->m_vecMoveSpeed.y = vehicle->m_vecMoveSpeed.y * 5.0f;
                vehicle->m_vecMoveSpeed.z = vehicle->m_vecMoveSpeed.z * 5.0f;
            }
            else
            {
                vehicle->m_vecMoveSpeed.x = -vehicle->m_vecMoveSpeed.x;
                vehicle->m_vecMoveSpeed.y = -vehicle->m_vecMoveSpeed.y;
                vehicle->m_vecMoveSpeed.z = -vehicle->m_vecMoveSpeed.z;
            }
        }
    }

    void
    InvertVehicle (CMatrix *matrix)
    {
        matrix->right.x = -matrix->right.x;
        matrix->right.y = -matrix->right.y;
        matrix->right.z = -matrix->right.z;

        matrix->up.x = -matrix->up.x;
        matrix->up.y = -matrix->up.y;
        matrix->up.z = -matrix->up.z;
    }
};

// clang-format off
DEFINE_EFFECT (VehicleMomentumEffect, "effect_vehicle_boost",           0, eVehicleMomentumType::BOOST);
DEFINE_EFFECT (VehicleMomentumEffect, "effect_invert_vehicle_speed",    0, eVehicleMomentumType::INVERT_MOMENTUM);
DEFINE_EFFECT (VehicleMomentumEffect, "effect_turn_vehicles_around",    0, eVehicleMomentumType::INVERT_MOMENTUM_AND_MATRIX);
// clang-format on