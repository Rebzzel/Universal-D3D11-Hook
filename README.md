# Universal-D3D11-Hook
**Hook for DX11 based games written on C++**
## What you need?
[**DirectX SDK**](https://www.microsoft.com/en-us/download/details.aspx?id=6812)

[**MinHook**](https://github.com/TsudaKageyu/minhook) ([NuGet](https://www.nuget.org/packages/minhook/))

## Example
```C++
#include <windows.h>

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
      CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ImplHookDX11_Init), nullptr, 0, nullptr);
      break;
    case DLL_PROCESS_DETACH:
      ImplHookDX11_Shutdown();
      break;
  }

  return TRUE;
}
```

## Developers
[**Rebzzel**](https://github.com/Rebzzel)

## License
[**MIT License**](https://github.com/Rebzzel/Universal-D3D11-Hook/blob/master/LICENSE)
