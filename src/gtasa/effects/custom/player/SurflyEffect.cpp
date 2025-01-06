/*
#include "util/EffectBase.h"
#include "util/GameUtil.h"

#include <CCamera.h>
#include <extensions/ScriptCommands.h>

using namespace plugin;

class WarpPlayerIntoRandomVehicleEffect : public EffectBase
{
    static inline float m_fSurflyTime = 0.0;

public:
    void
    OnStart (EffectInstance *inst) override
    {
        m_fSurflyTime = 0.0f;
    }

    // void
    // OnTick (EffectInstance *inst) override
    // {
    //     UpdateSurfly();
    // }

    // void UpdateSurfly() 
    // {
    //     CPlayerPed* playerPed = FindPlayerPed();  // Mendapatkan pointer ke karakter pemain
    //     if (!playerPed) return;

    //     // Mengecek apakah tombol untuk terbang atau melakukan aksi tertentu ditekan
    //     if (IsKeyPressed(VK_X) && IsKeyPressed(VK_CONTROL)) {
    //         playerPed->SetAnimation("FALL_SKYDIVE", true);

    //         // Mendapatkan kecepatan karakter (velocity) saat ini
    //         float velocity = playerPed->GetSpeed();
            
    //         // Meningkatkan kecepatan dengan faktor tertentu (seperti di CLEO)
    //         velocity.x *= 1.001f;
    //         velocity.y *= 1.001f;
    //         velocity.z *= 1.001f;

    //         // Menerapkan perubahan kecepatan karakter
    //         playerPed->SetVelocity(velocity);

    //         // Periksa tabrakan atau perubahan posisi karakter
    //         if (playerPed->IsInAir()) {
    //             // Jika karakter berada di udara, tambah kecepatan vertikal (z)
    //             velocity.z += 0.4f;
    //             playerPed->SetVelocity(velocity);
    //         }
    //     }

    //     // Mengatur pergerakan berdasarkan arah input
    //     if (IsKeyPressed(VK_W)) {
    //         CVector currentPos = playerPed->GetPosition();
    //         CVector offset = CVector(0.0f, 1.0f, 0.0f);  // Gerakan ke depan
    //         CVector newVelocity = playerPed->GetVelocity() + offset;
    //         playerPed->CalculateNewVelocity()
    //     }

    //     if (IsKeyPressed(VK_A)) {
    //         CVector currentPos = playerPed->GetPosition();
    //         CVector offset = CVector(-1.0f, 0.0f, 0.0f);  // Gerakan ke kiri
    //         CVector newVelocity = playerPed->GetVelocity() + offset;
    //         playerPed->SetVelocity(newVelocity);
    //     }

    //     if (IsKeyPressed(VK_D)) {
    //         CVector currentPos = playerPed->GetPosition();
    //         CVector offset = CVector(1.0f, 0.0f, 0.0f);  // Gerakan ke kanan
    //         CVector newVelocity = playerPed->GetVelocity() + offset;
    //         playerPed->SetVelocity(newVelocity);
    //     }

    //     if (IsKeyPressed(VK_S)) {
    //         CVector currentPos = playerPed->GetPosition();
    //         CVector offset = CVector(0.0f, -1.0f, 0.0f);  // Gerakan mundur
    //         CVector newVelocity = playerPed->GetVelocity() + offset;
    //         playerPed->SetVelocity(newVelocity);
    //     }

    //     // Pengaturan kondisi khusus ketika karakter terbang atau melompat
    //     if (IsKeyPressed(VK_SPACE)) {
    //         if (playerPed->IsInAir()) {
    //             CVector velocity = playerPed->GetVelocity();
    //             velocity.z += 0.2f;  // Menambah kecepatan vertikal
    //             playerPed->SetVelocity(velocity);
    //         }
    //     }
    // }

};
*/