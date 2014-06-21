#include <stdafx.h>
#include "CHUD.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CCmdlineParams.h"
#include "../Network/Network.h"

bool CGame::m_bGameLoaded;

void CGame::OnInitialize(IDirect3D9* pDirect3D, IDirect3DDevice9* pDevice, HWND hWindow)
{
	m_bGameLoaded = false;

	CGraphics::Initialize(pDirect3D, pDevice);
}

void CGame::OnLoad()
{
	if (!CCmdlineParams::ArgumentExists("d") && CCmdlineParams::ArgumentExists("c"))
	{
		CGame::UnprotectMemory();
		Network::Initialize();
		Network::Connect(CCmdlineParams::GetArgumentValue("h"), atoi(CCmdlineParams::GetArgumentValue("p").c_str())+1);
		CHUD::Initialize();
	}
}

void CGame::OnUnload()
{

}

int CGame::OnCursorMove(int iX, int iY)
{
	return false;
}

void CGame::PreDeviceReset()
{
	CGraphics::OnReset();
}

void CGame::PostDeviceReset()
{
	
}

void CGame::PreEndScene()
{
	if (IsLoaded())
	{
		//TODO: stuff???
	}

	CGraphics::PreEndScene();
}

void CGame::PostEndScene()
{
	if (!InMainMenu() && !IsLoaded())
	{
		OnLoad();
		m_bGameLoaded = true;
	}
}

bool CGame::IsLoaded()
{
	return m_bGameLoaded;
}

bool CGame::InMainMenu()
{
	return CMem::Get<bool>(0xBA6831);
}

bool CGame::Paused()
{
	return CMem::Get<bool>(0xB7CB49);
}

bool CGame::InMenu()
{
	return CMem::Get<bool>(0xBA67A4);
}

bool CGame::Playing()
{
	return IsLoaded() && !InMainMenu() && !Paused() && !InMenu();
}

bool CGame::HUDVisible()
{
	return CMem::Get<bool>(0xA444A0);
}

void CGame::SetHUDVisible(bool iVisible)
{
	CMem::PutSingle<bool>(0xA444A0, iVisible);
}

void CGame::SetRadioStation(unsigned long ulStation)
{
	((int(__thiscall*)(int, int, char, int, int))0x4EB3C0)(0x8CB6F8, ulStation, 0, 0, 0);
}

void CGame::SetWaveHeight(float fHeight)
{
	unsigned short ulOpcodes = 0xD8DD;
	CMem::Set(0x0072C667, 0x90, 0x04);
	CMem::Put<unsigned char>(0x72C665, &ulOpcodes, 2);
	CMem::Set(0x0072C659, 0x90, 0x0A);
	CMem::PutSingle<float>(0x00C812E8, fHeight);
}

void CGame::UnprotectMemory()
{
	CMem::Unprotect(0x0072C659, 0x12);
	CMem::Unprotect(0x00C812E8, sizeof(float));
}