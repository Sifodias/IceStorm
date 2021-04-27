#include "Editor.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_stdlib.h"
#include "Builder.h"
#include <string>

using namespace ImGui;

void Editor::init() {
    CreateContext();
    int w, h;
    SDL_GetWindowSize(Renderer::g_Window, &w, &h);
    ImGuiSDL::Initialize(Renderer::g_Renderer, w, h); //Needs actual dimensions of window
}

static void ShowPlaceholderObject(const char* prefix, int uid) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    if (Builder::currentObject == NULL) {
        Text("Current ent: None");
        return;
    }
    auto cur = Builder::currentObject;
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(Builder::currentObject->ID);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Object", "%s", prefix);
    ImGui::TableSetColumnIndex(1);

    Text(std::to_string(Builder::currentObject->ID).c_str());


    // int ID;
    // std::string target;
    // std::vector<std::string> targetnames;
    // std::vector<std::string> flags;
    // std::string type;

    // bool useSpritesHandler;
    // std::string textureName;
    // int x;
    // int y;
    // std::string content;




    if (node_open) {
        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
        ImGui::TreeNodeEx("target", flags, "target:");
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);


        ImGui::InputText("target##current", &cur->target);

        ImGui::NextColumn();


        ImGui::TreePop();
    }
    ImGui::PopID();
}

void showCurrentStuff() {
    SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (!Begin("Current stuff")) {
        End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("currentStuff", 2,
        ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody)) {

        ShowPlaceholderObject("Current ent:", 0);
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

void Editor::routine(SDL_Event& e) {
    int w, h;
    SDL_GetWindowSize(Renderer::g_Window, &w, &h);

    ImGuiIO& io = GetIO();
    int wheel = 0;
    if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            io.DisplaySize.x = static_cast<float>(e.window.data1);
            io.DisplaySize.y = static_cast<float>(e.window.data2);
        }
    }
    else if (e.type == SDL_MOUSEWHEEL)
        wheel = e.wheel.y;

    else if (e.type == SDL_KEYDOWN) {
        io.AddInputCharacter(e.key.keysym.sym);
    }

    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

    io.MousePos = ImVec2(static_cast<float>(mouseX) / 2, static_cast<float>(mouseY) / 2);
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
    io.MouseWheel = static_cast<float>(wheel);

    SDL_RenderSetLogicalSize(Renderer::g_Renderer, w / 2, h / 2);

    NewFrame();

    ShowDemoWindow();
    showCurrentStuff();

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());

    SDL_RenderSetLogicalSize(Renderer::g_Renderer, Renderer::SCREEN_W, Renderer::SCREEN_H);
}