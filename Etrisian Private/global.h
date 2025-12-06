#pragma once
#define NOMINMAX
#include <Windows.h>
#include <tlhelp32.h>

#include <print>
#include <format>
#include <fstream>
#include <iostream>

#include <string>
#include <string_view>

#include <chrono>
#include <mutex>
#include <thread>

#include <queue>
#include <array>
#include <span>

#include <cmath>
#include <random>
#include <limits>

#include <expected>
#include <functional>

#include <tchar.h>
#include <Dwmapi.h>

//#include <imgui.h>
//#include <imgui_impl_win32.h>
//#include <imgui_impl_dx11.h>

#include <d3d11.h>
#include <dxgi1_2.h>

#pragma comment(lib, "winmm.lib")

// project includes, etc.

struct HandleRAII {
	void operator()(HANDLE handle) const {
		if (handle != NULL && handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
		}
	}
}; using unique_handle = std::unique_ptr<void, HandleRAII>;

namespace globals {
	inline ULONG64 g_peb = 0;
	inline ULONG64 g_baseAddress = 0;
}



//#include "Config/config.h"
//
//#include "GUI/Menu/Resource/Font/MuseoSans.h"
//#include "GUI/Menu/Resource/Font/IconsFontAwesome6.h"
//#include "GUI/Menu/Resource/Font/IconsFontAwesome6Brands.h"

#include "render/render.h"

//#include "sdk/sdk.h
//#include "Game/SDK/Engine/structs.h"
//#include "sdk/offsets.hpp"
//#include "Game/SDK/Engine/engine.h"

//#include "esp.h"
//#include "Game/Aimbot/Aimbot.h"

