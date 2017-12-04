# Universal-D3D11-Hook
[![GitHub release](https://img.shields.io/github/release/rebzzel/Universal-D3D11-Hook.svg)](https://github.com/Rebzzel/Universal-D3D11-Hook/releases)

**Hook for DX11 based games written on C++**
## What you need?
[**DirectX SDK**](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

[**MinHook**](https://github.com/TsudaKageyu/minhook) ([NuGet](https://www.nuget.org/packages/minhook/))

## Example
```C++
#include <windows.h>
#include <d3d11.h>

#include "d3d11hook.h"

D3D11_HOOK_API void ImplHookDX11_Present(ID3D11Device *device, ID3D11DeviceContext *ctx, IDXGISwapChain *swap_chain)
{
  // here your magic
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
      ImplHookDX11_Init(FindWindow(0, L"WINDOWNAME"));
      break;
    case DLL_PROCESS_DETACH:
      ImplHookDX11_Shutdown();
      break;
  }

  return TRUE;
}

```
## Changelog
**GO TO [RELEASES FOR](https://github.com/Rebzzel/Universal-D3D11-Hook/releases) SEE LATEST CHANGELOGS**

## Developers
[**Rebzzel**](https://github.com/Rebzzel)

## License
[**MIT License**](https://github.com/Rebzzel/Universal-D3D11-Hook/blob/master/LICENSE)
