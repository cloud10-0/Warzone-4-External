#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <iostream>
#include <d3d11.h>
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "../Cheat/features/esp.h"

#include <dwmapi.h>
#include "../Util/settings.h"

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d11.lib")

struct resources {
	HWND hwnd{ nullptr };
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
} static inline resources_t;

static inline bool setup_d3d() {
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc.Width = 0;
	swap_chain_desc.BufferDesc.Height = 0;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = resources_t.hwnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL selected_feature_level;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, _countof(feature_levels), D3D11_SDK_VERSION, &swap_chain_desc, &resources_t.swap_chain, &resources_t.device, &selected_feature_level, &resources_t.device_context)))return false;

	ID3D11Texture2D* back_buffer = nullptr;
	if (FAILED(resources_t.swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))) || !back_buffer) {
		return false;
	}

	if (FAILED(resources_t.device->CreateRenderTargetView(back_buffer, nullptr, &resources_t.render_target_view))) {
		return false;
	}

	back_buffer->Release();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();


	style->WindowTitleAlign.x = 0.50f;
	style->WindowRounding = 0;

	ImGui_ImplWin32_Init(resources_t.hwnd);
	ImGui_ImplDX11_Init(resources_t.device, resources_t.device_context);
	resources_t.device->Release();
	return true;
}

// Change the Overlay 
void CreateOverlay() {
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),0,DefWindowProcA,0,0,nullptr,LoadIcon(nullptr, IDI_APPLICATION),LoadCursor(nullptr, IDC_ARROW),nullptr,nullptr,("Timo"),LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);
	resources_t.hwnd = CreateWindowExA(NULL, ("Timo"), ("Feets"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);

	SetWindowLong(resources_t.hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(resources_t.hwnd, &margin);
	ShowWindow(resources_t.hwnd, SW_SHOW);
	SetWindowPos(resources_t.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetLayeredWindowAttributes(resources_t.hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	UpdateWindow(resources_t.hwnd);
}

void Menu() {
	const DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

	if (GetAsyncKeyState(VK_INSERT) & 1) settings::show_menu = !settings::show_menu;
	if (settings::show_menu)
	{
		ImGui::SetNextWindowSize({ 620, 420 });
		ImGui::Begin("##MainWindow", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		{
			ImGui::Text("Render Base");
			ImGui::Separator();

			if (ImGui::Button("Aim", ImVec2(151, 25))) settings::current_tab = 0;
			ImGui::SameLine(0, 0);
			if (ImGui::Button("Visuals", ImVec2(151, 25))) settings::current_tab = 1;
			ImGui::SameLine(0, 0);
			if (ImGui::Button("Exploits", ImVec2(151, 25))) settings::current_tab = 2;
			ImGui::SameLine(0, 0);
			if (ImGui::Button("Settings", ImVec2(151, 25))) settings::current_tab = 3;

			ImVec2 window_size = ImGui::GetContentRegionAvail();
			float window_width = window_size.x;
			float window_height = window_size.y;

			switch (settings::current_tab)
			{
			case 0:
			{
				// Top Left - Aim Configuration
				ImGui::BeginChild("AimConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					ImGui::Checkbox("Enable Aimbot", &settings::aim::mouse_aim);
					ImGui::Checkbox("Prediction", &settings::aim::prediction);
					ImGui::Checkbox("Visible Only", &settings::aim::visible_only);
					ImGui::Checkbox("FOV Circle", &settings::aim::show_fov);
					//ImGui::Checkbox("Crosshair", &settings::aim::show_crosshair);
				}
				ImGui::EndChild();

				ImGui::SameLine();

				// Top Right - Extra Aim Configuration
				ImGui::BeginChild("ExtraAimConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					ImGui::SliderFloat("FOV", &settings::aim::fov, 0, 500, "%.0f");
					ImGui::SliderFloat("Smoothness", &settings::aim::smoothness, 2, 50, "%.0f");
					ImGui::Spacing();
					ImGui::Text("Aim Key: %d", settings::aim::aim_key);
					ImGui::SameLine();
					if (ImGui::Button("Change##AimKey", ImVec2(60, 20))) {
						keystatus = !keystatus;
					}
					if (keystatus) {
						for (int i = 0; i < 256; i++) {
							if (GetAsyncKeyState(i) & 1) {
								settings::aim::aim_key = i;
								keystatus = false;
								break;
							}
						}
					}
				}
				ImGui::EndChild();

				// Bottom Left - Triggerbot Configuration
				ImGui::BeginChild("TriggerbotConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					//ImGui::Checkbox("Enable Triggerbot", &settings::aim::triggerbot);
					//ImGui::Checkbox("Shotgun Only", &settings::aim::shotgun_only);
				}
				ImGui::EndChild();

				ImGui::SameLine();

				// Bottom Right - Extra Triggerbot Configuration
				ImGui::BeginChild("ExtraTriggerbotConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					//ImGui::SliderFloat("Speed", &settings::aim::triggerbot_speed, 1, 50, "%.0f");
					ImGui::Spacing();
					//ImGui::Text("Trig Key: %d", settings::aim::trig_key);
					ImGui::SameLine();
					if (ImGui::Button("Change##TrigKey", ImVec2(60, 20))) {
						keystatus2 = !keystatus2;
					}
					if (keystatus2) {
						for (int i = 0; i < 256; i++) {
							if (GetAsyncKeyState(i) & 1) {
								//settings::aim::trig_key = i;
								keystatus2 = false;
								break;
							}
						}
					}
				}
				ImGui::EndChild();

				break;
			}
			case 1:
			{
				// Top Left - Visuals Configuration
				ImGui::BeginChild("VisualsConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					ImGui::Checkbox("Box", &settings::visuals::box);
					ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
					ImGui::Checkbox("Snaplines", &settings::visuals::snaplines);
					ImGui::Checkbox("Distance", &settings::visuals::distance);
					
					
					
				}
				ImGui::EndChild();

				ImGui::SameLine();

				
				ImGui::BeginChild("ExtraVisualsConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					ImGui::Text("Color Configuration");
					ImGui::Spacing();
					ImGui::ColorEdit4("Visible Color", reinterpret_cast<float*>(&settings::visuals::visible_color), picker_flags);
				}
				ImGui::EndChild();

				
				ImGui::BeginChild("BoxConfig", ImVec2(window_width / 2 - 5, window_height / 2 - 5), true);
				{
					ImGui::Checkbox("Filled Box", &settings::visuals::filled);
					ImGui::Checkbox("Corner Box", &settings::visuals::corner);
					ImGui::SliderFloat("Box Thickness", &settings::visuals::thickness, 1.0f, 5.0f, "%.1f");
				}
				ImGui::EndChild();

				break;
			}

			case 2:
			{
				
				ImGui::BeginChild("SettingsConfig", ImVec2(window_width / 2 - 5, window_height - 10), true);
				{
					ImGui::Checkbox("Stream Proof", &settings::misc::stream_proof);
					ImGui::Spacing();
					if (ImGui::Button("Unload", ImVec2(100, 25))) exit(0);
				}
				ImGui::EndChild();

				ImGui::SameLine();

				// Right - Info
				ImGui::BeginChild("Info", ImVec2(window_width / 2 - 5, window_height - 10), true);
				{
					ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
					ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
				}
				ImGui::EndChild();

				break;
			}
			}
		}
		ImGui::End();
	}
}

static inline void loop() {
	constexpr float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	MSG msg{};

	while (msg.message != WM_QUIT) {
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			settings::Menu = !settings::Menu;
		}

		if (settings::Menu) {
			POINT cursor_pos;
			GetCursorPos(&cursor_pos);
			io.MousePos = ImVec2(static_cast<float>(cursor_pos.x), static_cast<float>(cursor_pos.y));
			io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame(); {
			ActorLoop();

			if (settings::Menu) {
				Menu();
			}
		}
		ImGui::Render();

		resources_t.device_context->OMSetRenderTargets(1, &resources_t.render_target_view, nullptr);
		resources_t.device_context->ClearRenderTargetView(resources_t.render_target_view, clear_color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		resources_t.swap_chain->Present(settings::visuals::VSync, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

static inline void setup() {
	CreateOverlay();
	setup_d3d();
	loop();
}