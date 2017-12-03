#include <d3d11.h>
#include <mutex>

#include <MinHook.h>

#include "d3d11hook.h"

#pragma comment(lib, "d3d11.lib")

typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4]);

D3D11PresentHook phookD3D11Present = 0;
D3D11DrawIndexedHook phookD3D11DrawIndexed = 0;
D3D11ClearRenderTargetViewHook phookD3D11ClearRenderTargetViewHook = 0;

DWORD* pSwapChainVTable = 0;
DWORD* pDeviceContextVTable = 0;

static ID3D11Device *g_pd3dDevice = 0;
static ID3D11DeviceContext *g_pd3dContext = 0;
static IDXGISwapChain *g_pSwapChain = 0;

static std::once_flag g_isInitialized;

HRESULT __stdcall PresentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	std::call_once(g_isInitialized, [&]() {
		pSwapChain->GetDevice(__uuidof(ID3D11Device*), reinterpret_cast<void**>(g_pd3dDevice));
		g_pd3dDevice->GetImmediateContext(&g_pd3dContext);
	});

	ImplHookDX11_Present(g_pd3dDevice, g_pd3dContext, g_pSwapChain);

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}

void __stdcall DrawIndexedHook(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

void __stdcall ClearRenderTargetViewHook(ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4])
{
	return phookD3D11ClearRenderTargetViewHook(pContext, pRenderTargetView, ColorRGBA);
}

D3D11_HOOK_API DWORD __stdcall ImplHookDX11_Init(LPVOID)
{
	HWND hwnd = GetForegroundWindow(); // or FindWindow(0, WINDOWNAME);

	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &level, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, 0, &g_pd3dContext);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed to create device and swap chain.", L"Fatal Error", MB_ICONERROR);
		return E_FAIL;
	}

	pSwapChainVTable = reinterpret_cast<DWORD*>(g_pSwapChain);
	pSwapChainVTable = reinterpret_cast<DWORD*>(pSwapChainVTable[0]);

	pDeviceContextVTable = reinterpret_cast<DWORD*>(g_pd3dContext);
	pDeviceContextVTable = reinterpret_cast<DWORD*>(pDeviceContextVTable[0]);

	if (MH_Initialize() != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD*)pSwapChainVTable[8], PresentHook, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD*)pSwapChainVTable[8]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD*)pSwapChainVTable[12], DrawIndexedHook, reinterpret_cast<void**>(&phookD3D11DrawIndexed)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD*)pSwapChainVTable[12]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD*)pSwapChainVTable[50], ClearRenderTargetViewHook, reinterpret_cast<void**>(&phookD3D11ClearRenderTargetViewHook)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD*)pSwapChainVTable[50]) != MH_OK) { return 1; }

	DWORD old_protect;
	VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &old_protect);

	return S_OK;
}

D3D11_HOOK_API void ImplHookDX11_Shutdown()
{
	if (MH_DisableHook(MH_ALL_HOOKS)) { return; };
	if (MH_Uninitialize()) { return; }
}