#include "Editor.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_stdlib.h"
#include "imgui_impl_sdl.h"
#include "Builder.h"

#include <string>
#include <vector>

using namespace ImGui;

void Editor::init() {
    CreateContext();
    int w, h;
    SDL_GetWindowSize(Renderer::g_Window, &w, &h);
    ImGuiSDL::Initialize(Renderer::g_Renderer, w, h); //Needs actual dimensions of window
    ImGui_ImplSDL2_Init(Renderer::g_Window);
}
static void addField(std::string field, std::string& str) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    ImGui::TreeNodeEx(field.c_str(), flags, field.c_str());
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    ImGui::InputText(field.c_str(), &str);

    ImGui::TableNextRow();
    ImGui::NextColumn();
}
static void addField(std::string field, int ID) {
    std::string id = std::to_string(ID);
    addField(field, id);
}
static void addField(std::string field, std::vector<std::string>& vec) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    //ImGui::TreeNodeEx(field.c_str(), flags, field.c_str());

    if (ImGui::BeginTable(std::string(field + "Main").c_str(), 2)) {
        TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx(field.c_str(), flags, field.c_str());

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("+")) {
            vec.insert(vec.begin(), "");
        }

        ImGui::SetNextItemWidth(-FLT_MIN);
        EndTable();
    }

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    if (ImGui::BeginTable(field.c_str(), 2,
        ImGuiTableFlags_Resizable
        | ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_BordersInner
        | ImGuiTableFlags_RowBg)) {

        int i = 0;
        for (string& entry : vec) {
            ImGui::TableNextRow();
            PushID(i);
            ImGui::TableSetColumnIndex(0);
            // ImGui::AlignTextToFramePadding();
            ImGui::InputText(std::to_string(i++).c_str(), &entry);

            ImGui::TableSetColumnIndex(1);
            ImGui::AlignTextToFramePadding();
            if (ImGui::Button("-")) {
                vec.erase(vec.begin() + (i - 1));
            }


            ImGui::SetNextItemWidth(-FLT_MIN);
            PopID();
        }
        ImGui::EndTable();
    }

    ImGui::TableNextRow();
    ImGui::NextColumn();
}
static void showCurrentObj(const char* prefix) {
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    auto cur = Builder::currentObject;
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(Builder::currentObject->ID);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::AlignTextToFramePadding();

    // bool useSpritesHandler;  checkbox
    addField("ID:", cur->ID);
    addField("target:", cur->target);
    addField("type:", cur->type);
    addField("texture:", cur->textureName); // add button to the side UPDATE
    addField("content:", cur->content);
    addField("flags:", cur->flags);
    addField("targetnames:", cur->targetnames);
    addField("x:", cur->x);
    addField("y:", cur->y);

    ImGui::TreePop();
    PopID();
}

void showCurrentStuff() {
    //SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    if (Builder::currentObject == NULL) {
        return;
    }

    if (!Begin("Current ent")) {
        End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("currentStuff", 2,
        ImGuiTableFlags_Resizable
        //| ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_BordersInner
        | ImGuiTableFlags_RowBg)) {

        showCurrentObj("Current ent:");
        ImGui::EndTable();
    }
    if (ImGui::BeginTable("Actions", 2,
        ImGuiTableFlags_Resizable
        //| ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_BordersInner
        | ImGuiTableFlags_RowBg)) {
            

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
    ImGui::End();
}

void Editor::routine(SDL_Event& e) {
    int w, h;
    SDL_GetWindowSize(Renderer::g_Window, &w, &h);

    ImGuiIO& io = GetIO();

    ImGui_ImplSDL2_NewFrame(Renderer::g_Window);

    int wheel = 0;
    if (e.type == SDL_WINDOWEVENT) {
        if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            io.DisplaySize.x = static_cast<float>(e.window.data1);
            io.DisplaySize.y = static_cast<float>(e.window.data2);
        }
    }
    else if (e.type == SDL_MOUSEWHEEL)
        wheel = e.wheel.y;

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