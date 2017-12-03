/*
 *      ____ _____ ____  _ _ _   _             _
 *     |  _ \___ /|  _ \/ / | | | | ___   ___ | | __
 *     | | | ||_ \| | | | | | |_| |/ _ \ / _ \| |/ /
 *     | |_| |__) | |_| | | |  _  | (_) | (_) |   <
 *     |____/____/|____/|_|_|_| |_|\___/ \___/|_|\_\
 *
 *     D3D11Hook by Rebzzel
 *     For compile hook you need:
 *      - DirectX SDK (https://www.microsoft.com/en-us/download/details.aspx?id=6812)
 *      - Minhook (you can download in NuGet or github repository https://github.com/TsudaKageyu/minhook)
 *     License: https://github.com/Rebzzel/Universal-D3D11-Hook#license
 */


#ifndef D3D11_HOOK_H_INCLUDED_
#define D3D11_HOOK_H_INCLUDED_

#define D3D11_HOOK_API

typedef void *LPVOID; // from windef.h
typedef unsigned long DWORD; // from windef.h

struct ID3D11Device; // from d3d11.h
struct ID3D11DeviceContext; // from d3d11.h
struct IDXGISwapChain; // from d3d11.h

// Use for rendering graphical user interfaces (for example: ImGui)
extern D3D11_HOOK_API void ImplHookDX11_Present(ID3D11Device *device, ID3D11DeviceContext *ctx, IDXGISwapChain *swap_chain);

// Main function in Hook. 
// Using for initialize hook.
// Initialize: CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ImplHookDX11_Init), nullptr, 0, nullptr);
D3D11_HOOK_API DWORD __stdcall ImplHookDX11_Init(LPVOID);

// Using for unintialize hook.
D3D11_HOOK_API void ImplHookDX11_Shutdown();

#endif