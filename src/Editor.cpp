#include "Editor.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_sdl.h"
#include "imgui_stdlib.h"
#include "imgui_impl_sdl.h"
#include "Builder.h"
#include "Map.h"
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include "Paths.h"
#include "Objects_m.h"

using namespace ImGui;

void Editor::init() {
    CreateContext();
    int w, h;
    SDL_GetWindowSize(Renderer::g_Window, &w, &h);
    ImGuiSDL::Initialize(Renderer::g_Renderer, w, h); //Needs actual dimensions of window
    ImGui_ImplSDL2_Init(Renderer::g_Window);
}

static void addField(std::string field, bool& enabled) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(field.c_str());

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    ImGui::Checkbox((field + "##value").c_str(), &enabled);
    ImGui::SetNextItemWidth(-FLT_MIN);

    ImGui::TableNextRow();
    ImGui::NextColumn();
}

static void addField(std::string field, std::string& str, std::string button = "", std::function<void(std::string)> f = NULL) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(field.c_str());

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    if (!button.empty()) {
        if (ImGui::BeginTable(std::string(field + "##value").c_str(), 2)) {
            TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            // ImGui::InputText(field.c_str(), &str);


            std::string items = "";
            std::vector<std::string> levels;
            for (const auto& entry : std::filesystem::directory_iterator(Paths::levelPath)) {
                if (!std::filesystem::is_regular_file(entry))
                    continue;

                items += entry.path().filename().c_str();
                items += '\0';
                levels.push_back(entry.path().filename());
            }
            static int item_current = 0;

            ImGui::Combo("levels##value", &item_current, items.c_str());
            Builder::door = levels[item_current];
            // ImGui::SameLine();
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button(button.c_str()))
                f(levels[item_current]);

            ImGui::SetNextItemWidth(-FLT_MIN);
            EndTable();
        }
    }
    else
        ImGui::InputText(field.c_str(), &str);

    ImGui::TableNextRow();
    ImGui::NextColumn();
}

static void addField(std::string field, int& value) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();

    ImGui::Text(field.c_str());
    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-FLT_MIN);

    ImGui::InputInt(field.c_str(), &value);

    ImGui::TableNextRow();
    ImGui::NextColumn();
}

static void addField(std::string field, std::vector<std::string>& vec) {
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (ImGui::BeginTable(std::string(field + "Main").c_str(), 2)) {
        TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text(field.c_str());

        ImGui::TableSetColumnIndex(1);
        if (ImGui::Button("+"))
            vec.insert(vec.begin(), "");

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

    // // bool useSpritesHandler;  checkbox
    addField("ID:", cur->ID);
    addField("target:", cur->target);
    addField("type:", cur->type);
    //addField("texture:", cur->textureName); // add button to the side UPDATE
    {   // It should be a function
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("texture:");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text(cur->textures.resource.c_str());
        ImGui::SetNextItemWidth(-FLT_MIN);

        if (ImGui::BeginTable(std::string("texty##value").c_str(), 2)) {
            TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            static std::string meta;
            ImGui::InputText("textyee##value", &meta);
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button("UPDATE")) {
                Objects_m::fillObject(*cur, "texture: " + meta);
            }
            ImGui::SetNextItemWidth(-FLT_MIN);
            EndTable();
            ImGui::TableNextRow();
            ImGui::NextColumn();
        }
    }
    addField("meta:", cur->meta);
    addField("flags:", cur->flags);
    addField("targetnames:", cur->targetnames);
    // addField("x:", cur->x);
    // addField("y:", cur->y);
    addField("Enabled", cur->enabled);
    addField("Default enabled", cur->default_enabled);

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

    ImGui::PopStyleVar();
    ImGui::End();
}

std::string toLoad;
void showActions() {

    if (!Begin("Actions")) {
        End();
        return;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

    if (ImGui::BeginTable("Actions##value", 2,
        ImGuiTableFlags_Resizable
        //| ImGuiTableFlags_NoBordersInBody
        | ImGuiTableFlags_BordersInner
        | ImGuiTableFlags_RowBg)) {

        ImGui::TableNextRow();
        addField("Current plan:", Builder::currentPlan);
        std::string dummy = Map::levelname;
        addField("Current level:", dummy);
        addField("Load level:", toLoad, "LOAD", Builder::loadLevel);

        {   // It should be a function
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Set ent:");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::BeginTable(std::string("enty##value").c_str(), 2)) {
                TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                static std::string entLoad;
                ImGui::InputText("yee##value", &entLoad);
                ImGui::TableSetColumnIndex(1);
                if (ImGui::Button("LOAD")) {
                    Builder::currentObject = &Builder::fetchObject(entLoad);
                    Builder::lastIdToPlace = Builder::idToPlace;
                    Builder::idToPlace = Builder::currentObject->ID;
                }
                ImGui::SetNextItemWidth(-FLT_MIN);
                EndTable();
            }
        }

        ImGui::TableNextRow();
        ImGui::NextColumn();


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

    // ShowDemoWindow();
    showCurrentStuff();
    showActions();

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());

    SDL_RenderSetLogicalSize(Renderer::g_Renderer, Renderer::SCREEN_W, Renderer::SCREEN_H);
}