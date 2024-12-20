#include "util/EffectBase.h"

#include <d3d9.h>
#include <CFont.h>
#include <CMenuManager.h>
#include <CSprite.h>

using namespace plugin;

class PedWallhackEffect : public EffectBase
{
    static inline std::vector<std::string> weaponNames = {"None",
                                                          "Brassknuckle",
                                                          "Golfclub",
                                                          "Nightstick",
                                                          "Knife",
                                                          "Bat",
                                                          "Shovel",
                                                          "Poolstick",
                                                          "Katana",
                                                          "Chainsaw",
                                                          "Fun Toy", // DILDO1
                                                          "Fun Toy", // DILDO2
                                                          "Fun Toy", // VIBE1
                                                          "Fun Toy", // VIBE2
                                                          "Flowers",
                                                          "Cane",
                                                          "Grenade",
                                                          "Teargas",
                                                          "Molotov",
                                                          "Rocket Launcher",
                                                          "HS Rocket Launcher",
                                                          "Unused",
                                                          "Pistol",
                                                          "Silenced Pistol",
                                                          "Desert Eagle",
                                                          "Shotgun",
                                                          "Sawn-off Shotgun",
                                                          "Combat Shotgun",
                                                          "Micro Uzi",
                                                          "MP5",
                                                          "AK-47",
                                                          "M4",
                                                          "Tec-9",
                                                          "Country Rifle",
                                                          "Sniper",
                                                          "Rocket Launcher",
                                                          "HS Rocket Launcher",
                                                          "Flamethrower",
                                                          "Minigun",
                                                          "Satchel",
                                                          "Satchel Detonator",
                                                          "Spraycan",
                                                          "Fire Extinguisher",
                                                          "Camera",
                                                          "Nightvision Goggles",
                                                          "Infrared Goggles",
                                                          "Parachute",
                                                          "Unused",
                                                          "Armor",
                                                          "Flare"};

public:
    void
    OnStart (EffectInstance *inst) override
    {
        Events::drawHudEvent.before += OnDraw;
    }

    void
    OnEnd (EffectInstance *inst) override
    {
        Events::drawHudEvent.before -= OnDraw;
    }

    static void
    OnDraw ()
    {
        if (FrontEndMenuManager.m_bMenuActive) return;

        CPlayerPed *player = FindPlayerPed ();
        if (!player) return;

        for (CPed *ped : CPools::ms_pPedPool)
        {
            if (!ped || ped == player || !ped->IsAlive ()) continue;

            Wallhack (ped);
        }
    }

    static std::string
    GetActivePedWeapon (CPed *ped)
    {
        int slot = ped->m_nActiveWeaponSlot;
        if (slot == 0) return "None";

        CWeapon weapon = ped->m_aWeapons[slot];
        return weaponNames[weapon.m_eWeaponType];
    }

    static std::string
    IsPedInCar (CPed *ped)
    {
        CVehicle *vehicle = ped->m_pVehicle;
        if (vehicle && (vehicle->IsDriver (ped) || vehicle->IsPassenger (ped)))
            return "true";

        return "false";
    }

    static void
    Wallhack (CPed *ped)
    {
        CVector position = ped->GetPosition ();
        RwV3d   pos      = {position.x, position.y, position.z};

        RwV3d coords;
        float w, h;

        if (!CSprite::CalcScreenCoors (pos, &coords, &w, &h, true, true))
            return;
        if (!ped->GetIsOnScreen()) 
            return; // Skip Unrendered Ped

        DrawCornerBox(static_cast<IDirect3DDevice9*>(RwD3D9GetCurrentD3DDevice()), (int)coords.x - 3, (int)coords.y + 25, 30, 60, 1, D3DCOLOR_ARGB(255, 0, 255, 255));

        RwV3d worldPos1 = { 0, 0, 0 };
        RwV3d worldPos2 = { 0, 0, 0 };
        RwV3d screenPos1 = { 0, 0 };
        RwV3d screenPos2 = { 0, 0 };

        int t[14] = { 3, 4, 5, 51, 52, 41, 42, 31, 32, 33, 21, 22, 23, 2 };
        for (int i = 0; i < 14; i++)
        {
            ped->GetBonePosition(worldPos1, t[i], false);
            ped->GetBonePosition(worldPos2, t[i] + 1, false);
            
            CSprite::CalcScreenCoors (pos, &screenPos1, &w, &h, true, true);
            CSprite::CalcScreenCoors (pos, &screenPos2, &w, &h, true, true);

            DrawLine(static_cast<IDirect3DDevice9*>(RwD3D9GetCurrentD3DDevice()), screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y, D3DCOLOR_ARGB(255, 255, 255, 255));

        }
        for (int i = 4; i < 6; i++)
        {
            ped->GetBonePosition(worldPos2, i * 10 + 1, false);
            CSprite::CalcScreenCoors (pos, &screenPos2, &w, &h, true, true);

            DrawLine(static_cast<IDirect3DDevice9*>(RwD3D9GetCurrentD3DDevice()), screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y, D3DCOLOR_ARGB(255, 255, 255, 255));
        }
/*
        CFont::SetOrientation (ALIGN_LEFT);
        CFont::SetColor (color::White);
        CFont::SetDropShadowPosition (1);
        CFont::SetBackground (false, false);
        CFont::SetWrapx (SCREEN_WIDTH);
        CFont::SetScale (0.5, 1.0);
        CFont::SetFontStyle (FONT_SUBTITLES);
        CFont::SetProportional (true);

        std::string text = std::format ("Skin ID: {0:}~n~", ped->m_nModelIndex);
        text.append (std::format ("Position: {:.2f}, {:.2f}, {:.2f}~n~", pos.x,
                                  pos.y, pos.z));
        text.append (std::format ("Health: {:.2f}~n~", ped->m_fHealth));
        text.append (std::format ("Armor: {:.2f}~n~", ped->m_fArmour));
        text.append (std::format ("Weapon: {}~n~", GetActivePedWeapon (ped)));
        text.append (std::format ("In Vehicle: {}~n~", IsPedInCar (ped)));

        CFont::PrintString (coords.x, coords.y, (char *) text.c_str ());
*/
    }

    static void DrawP(IDirect3DDevice9* Device, int baseX, int baseY, int baseW, int baseH, D3DCOLOR Cor)
    {
        D3DRECT BarRect = { baseX, baseY, baseX + baseW, baseY + baseH };
        Device->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Cor, 0, 0);
    }

    static void DrawCornerBox(IDirect3DDevice9* Device, int x, int y, int w, int h, int borderPx, DWORD borderColor)
    {
        DrawP(Device, x - (w / 2), (y - h + borderPx), w / 3, borderPx, borderColor); //bottom w
        DrawP(Device, x - (w / 2) + w - w / 3, (y - h + borderPx), w / 3, borderPx, borderColor); //bottom 
        DrawP(Device, x - (w / 2), (y - h + borderPx), borderPx, w / 3, borderColor); //left 
        DrawP(Device, x - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, borderColor); //left 
        DrawP(Device, x - (w / 2), y, w / 3, borderPx, borderColor); //top 
        DrawP(Device, x - (w / 2) + w - w / 3, y, w / 3, borderPx, borderColor); //top 
        DrawP(Device, (x + w - borderPx) - (w / 2), (y - h + borderPx), borderPx, w / 3, borderColor);//right 
        DrawP(Device, (x + w - borderPx) - (w / 2), (y - h + borderPx) + h - w / 3, borderPx, w / 3, borderColor);//right 
    }

    static void DrawLine(IDirect3DDevice9* m_pDevice, int x1, int y1, int x2, int y2, DWORD Color, bool _AntiAliased = false)
{
	struct CVtx
	{
		float m_Pos[4];
		DWORD m_Color;
	};
	CVtx p[2];

	p[0].m_Pos[0] = (float)x1;
	p[0].m_Pos[1] = (float)y1;
	p[0].m_Pos[2] = 0;
	p[0].m_Pos[3] = 0;
	p[0].m_Color = Color;

	p[1].m_Pos[0] = (float)x2;
	p[1].m_Pos[1] = (float)y2;
	p[1].m_Pos[2] = 0;
	p[1].m_Pos[3] = 0;
	p[1].m_Color = Color;

	//if( m_State->m_Caps.LineCaps & D3DLINECAPS_ANTIALIAS )
	m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, (_AntiAliased ? TRUE : FALSE));
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, p, sizeof(CVtx));
	//if( m_State->m_Caps.LineCaps & D3DLINECAPS_ANTIALIAS )
	//m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

};

DEFINE_EFFECT (PedWallhackEffect, "effect_ped_wallhack", 0);