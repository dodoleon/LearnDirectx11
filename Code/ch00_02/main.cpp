#include <iostream>
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_syswm.h>

#include <cstdio>
#include <sstream>
#include <chrono>
#include <vector>
#include <array>


#include <Windows.h>
#include <atlcomcli.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>

using namespace std;


#if _DEBUG
class ComErrorDesc
{
public:
    ComErrorDesc(HRESULT hr)
    {
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            m_Msg,
            _countof(m_Msg),
            NULL);
        auto nLen = strlen(m_Msg);
        if (nLen > 1 && m_Msg[nLen - 1] == '\n')
        {
            m_Msg[nLen - 1] = 0;
            if (m_Msg[nLen - 2] == '\r')
            {
                m_Msg[nLen - 2] = 0;
            }
        }
    }

    const char* Get() { return m_Msg; }

private:
    char m_Msg[4096];
};

#define CheckD3DError(hr)                            \
    {                                                \
        if (FAILED(hr))                              \
        {                                            \
            ComErrorDesc ErrDesc(hr);                \
            throw std::runtime_error(ErrDesc.Get()); \
        }                                            \
    }

#else
#define CheckD3DError
#endif

void CreateDevice(ID3D11Device** device, ID3D11DeviceContext** context)
{
    UINT creation_flags = 0;
#if _DEBUG
    creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    std::array feature_levels = {
        D3D_FEATURE_LEVEL_11_0,
    };
    D3D_FEATURE_LEVEL d3d_feature_level = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDevice(
        nullptr,                     // Specify nullptr to use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,    // If no adapter specified, request hardware graphics driver.
        0,                           // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        creation_flags,              // Set debug and Direct2D compatibility flags.
        feature_levels.data(),       // List of feature levels this app can support.
        UINT(feature_levels.size()), // Size of the list above.
        D3D11_SDK_VERSION,           // Always set this to D3D11_SDK_VERSION for Windows Store apps.
        device,                      // Returns the Direct3D device created.
        &d3d_feature_level,          // Returns feature level of device created.
        context                      // Returns the device immediate context.
    );

    if (FAILED(hr))
    {
        printf("the initialization fails, fall back to the WARP device");

        CheckD3DError(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP,
            0,
            creation_flags,
            feature_levels.data(),
            UINT(feature_levels.size()),
            D3D11_SDK_VERSION,
            device,
            &d3d_feature_level,
            context));
    }
}

CComPtr<IDXGISwapChain> CreateSwapChain(HWND window_handle, CComPtr<ID3D11Device> device, bool b_full_screen)
{
    CComPtr<IDXGISwapChain> swap_chain;
    CComPtr<IDXGISwapChain1> swap_chain1;
    CComPtr<IDXGIFactory2> factory;
    CheckD3DError(CreateDXGIFactory(__uuidof(factory), (void**)&factory));

    RECT rc;
    if (b_full_screen)
    {
        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &rc);
    }
    else
    {
        GetClientRect(window_handle, &rc);
    }

    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
    swap_chain_desc.Width = rc.right - rc.left;
    swap_chain_desc.Height = rc.bottom - rc.top;
    swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.Stereo = false;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 2;
    swap_chain_desc.Scaling = DXGI_SCALING_NONE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC full_screen_desc = {};
    full_screen_desc.Windowed = b_full_screen ? false : true;
    full_screen_desc.RefreshRate.Numerator = 60;
    full_screen_desc.RefreshRate.Denominator = 1;
    full_screen_desc.Scaling = DXGI_MODE_SCALING_CENTERED;
    full_screen_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    CheckD3DError(factory->CreateSwapChainForHwnd(device, window_handle, &swap_chain_desc, &full_screen_desc, nullptr, &swap_chain1));
    CheckD3DError(swap_chain1->QueryInterface(__uuidof(swap_chain1), reinterpret_cast<void**>(static_cast<IDXGISwapChain**>(&swap_chain))));
    return swap_chain;
}

struct SwapChainView
{
    CComPtr<ID3D11RenderTargetView> m_rtv;
};

SwapChainView CreateSwapChainTextureView(CComPtr<ID3D11Device> device, CComPtr<IDXGISwapChain> swap_chain)
{
    SwapChainView swap_chain_view;
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    RTVDesc.Texture2D.MipSlice = 0;

    CComPtr<ID3D11Texture2D> back_buffer;
    CheckD3DError(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(static_cast<ID3D11Texture2D**>(&back_buffer))));
    CheckD3DError(device->CreateRenderTargetView(back_buffer, &RTVDesc, &swap_chain_view.m_rtv));

    return swap_chain_view;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    int window_width = 1600;
    int window_height = 900;
    std::string title = "ch00_02_hello_window";
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        window_flags);
    if (window == nullptr)
    {
        printf("SDL Create window failed");
        return -1;
    }
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    CComPtr<ID3D11Device> device;
    CComPtr<ID3D11DeviceContext> context;
    CreateDevice(&device, &context);
    CComPtr<IDXGISwapChain> swap_chain = CreateSwapChain(wmInfo.info.win.window, device, false);
    auto swap_chain_view = CreateSwapChainTextureView(device, swap_chain);

    bool close_window = false;

    while (!close_window)
    {
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
                close_window = true;
            if (sdl_event.type == SDL_WINDOWEVENT && sdl_event.window.event == SDL_WINDOWEVENT_CLOSE && sdl_event.window.windowID == SDL_GetWindowID(window))
                close_window = true;
            if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                close_window = true;
        }

        ID3D11RenderTargetView* rtvs[] = {swap_chain_view.m_rtv};
        context->OMSetRenderTargets(1, rtvs, nullptr);

        float color[4] = {0, 1, 1, 1};
        context->ClearRenderTargetView(swap_chain_view.m_rtv, color);
        swap_chain->Present(0, 0);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}