#include "layers/gui_cntx.h"
#include "scene.h"
#include "IconsFontAwesome5.h"
#include "scene.h"
#include "other utils/common.h"

wizm::gui_layer::gui_layer(GLFWwindow* window, std::shared_ptr<camera_manager> camera_manager, asset_manager* p_asset_manager, core_scene* scene)
    : core_layer("gui_layer"), m_camera_manager(camera_manager), m_asset_manager(p_asset_manager), global_scene(scene)
{
    m_window = window;
}

wizm::gui_layer::~gui_layer()
{
   // OnDetach();
}

void wizm::gui_layer::on_attach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    

   

    io.Fonts->AddFontDefault();

    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 }; 
    ImFontConfig icons_config;
    icons_config.MergeMode = true; 
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("resources/fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);

   
    ImGui::StyleColorsDark();
   

   
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.Colors[ImGuiCol_WindowBg] = winCol;
        style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
        style.Colors[ImGuiCol_Button] = bgCol;
        style.Colors[ImGuiCol_ButtonActive] = btnActiveCol;
        style.Colors[ImGuiCol_ButtonHovered] = btnHoverCol;
        style.Colors[ImGuiCol_FrameBg] = bgCol;
        style.Colors[ImGuiCol_FrameBgActive] = frameCol;
        style.Colors[ImGuiCol_FrameBgHovered] = hoverCol;
        style.Colors[ImGuiCol_Text] = textCol;
        style.Colors[ImGuiCol_ChildBg] = childCol;
        style.Colors[ImGuiCol_CheckMark] = itemActiveCol;
        style.Colors[ImGuiCol_SliderGrab] = itemCol;
        style.Colors[ImGuiCol_SliderGrabActive] = itemActiveCol;
        style.Colors[ImGuiCol_Header] = itemActiveCol;
        style.Colors[ImGuiCol_HeaderHovered] = itemCol;
        style.Colors[ImGuiCol_HeaderActive] = itemActiveCol;
        style.Colors[ImGuiCol_ResizeGrip] = resizeGripCol;
        style.Colors[ImGuiCol_ResizeGripHovered] = resizeGripHoverCol;
        style.Colors[ImGuiCol_ResizeGripActive] = itemActiveCol;
        style.Colors[ImGuiCol_SeparatorHovered] = resizeGripHoverCol;
        style.Colors[ImGuiCol_SeparatorActive] = itemActiveCol;
        style.Colors[ImGuiCol_TitleBgActive] = itemActiveCol;

        style.WindowRounding = 6;
        style.ChildRounding = 6;
        style.FrameRounding = 2;
        style.GrabRounding = 2;
        style.PopupRounding = 2;

        style.ScrollbarSize = 9;
        style.FramePadding = ImVec2(6, 3);
        style.ItemSpacing = ImVec2(4, 4);


        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void wizm::gui_layer::on_detach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void wizm::gui_layer::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 12.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    static bool show_save_popup = false;
    static bool show_cam_error = false;
    static char map_name[128] = "GAME/new_save.zer";


    if (ImGui::BeginMenuBar())
    {
   
        if (ImGui::BeginMenu("File") )
        {
            if(engine_status == EDITOR_STATUS)
            {
                if (ImGui::MenuItem("Save-as"))
                {
                    show_save_popup = true;
                }
                if (ImGui::MenuItem("Save"))
                {
                    global_scene->save_map_data("");
                    m_camera_manager->save_viewport_camera(global_scene->current_scene);
                }
                if (ImGui::MenuItem("Open")) {
                    // open blah
                }
            }
            ImGui::EndMenu();
        }
        ImGui::SameLine(0, 20); 
       
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        ImGui::SameLine(0, 20);
        
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::EndMenu();
        }
        ImGui::SameLine(0, 20);
        
        if (ImGui::BeginMenu("Window"))
        {
            if (ImGui::MenuItem("Reset layout")) {
               
            }
            ImGui::EndMenu();
        }

        
       
        float menu_bar_width = ImGui::GetContentRegionAvail().x;
        float button_width = ImGui::CalcTextSize("play").x + ImGui::GetStyle().FramePadding.x * 2.0f;
        ImVec2 button_pos = ImVec2((menu_bar_width - button_width) / 2.0f, 0.0f);

        ImGui::SetCursorPosX(button_pos.x);

        /*
            ------------------------------------------------------------------------------------------START RUNTIME
            -------------------------------------------------------------------------------------------------------
        */
        
        if (ImGui::Button(ICON_FA_PLAY "")) {
            if (!m_camera_manager->update_crnt_camera(true)) {
                show_cam_error = true;
            }
            else if(engine_status != RUNTIME_STATUS)
            {
                engine_status = RUNTIME_STATUS;
                           
                global_scene->save_map_data("");
            }
        }

        /*
            -------------------------------------------------------------------------------------------STOP RUNTIME
            -------------------------------------------------------------------------------------------------------
        */

        if (ImGui::Button(ICON_FA_SQUARE "")) {
            m_camera_manager->update_crnt_camera(false);
            
            
            if(engine_status != EDITOR_STATUS) {
                global_scene->read_map_data(global_scene->current_scene);
                engine_status = EDITOR_STATUS;
            }
        }

        /*
            ---------------------------------------------------------------------------------------------DEBUG CODE
            -------------------------------------------------------------------------------------------------------
        */

        if (ImGui::Button(ICON_FA_BUG "") && engine_status == EDITOR_STATUS) {
            bool build_success;
            wizm::add_console_line("------------------------- BUILDING", CONSOLE_GENERAL_LOG);
            auto assets = m_asset_manager->get_all_assets();
            for (const auto& asset : assets) {
                auto script = std::dynamic_pointer_cast<script_asset>(asset.second);
                if (script) {
                    std::string temp_path = m_asset_manager->get_asset_details_from_id(asset.first).path;
                    build_success = script->sc->reload_script(temp_path);
                }
            }

            if (build_success)
                wizm::add_console_line("------------------------- BUILDING SUCCESS", CONSOLE_GENERAL_LOG);            
            else
                wizm::add_console_line("------------------------- BUILDING FAILED", CONSOLE_GENERAL_LOG);

        }
        ImGui::EndMenuBar();
    }
    ImGui::PopStyleVar();

    if (show_save_popup) {
        ImGui::OpenPopup("Save Map As");
    }

    if (show_cam_error) {
        ImGui::OpenPopup("No camera found!");
    }

    if (ImGui::BeginPopupModal("No camera found!", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("No main camera found in scene!");
        auto btnsize = ImGui::GetItemRectSize();
        if (ImGui::Button("OK", ImVec2(btnsize.x, 0))) {
            show_cam_error = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }


    if (ImGui::BeginPopupModal("Save Map As", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter the map name:");
        ImGui::InputText("##map_name", map_name, IM_ARRAYSIZE(map_name));

        if (ImGui::Button("OK", ImVec2(120, 0))) {


            global_scene->save_map_data(map_name);

            show_save_popup = false;


            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            show_save_popup = false;
            ImGui::CloseCurrentPopup();
        }


        ImGui::EndPopup();
    }

    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    ImGui::End();

}



void wizm::gui_layer::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


}

ImVec4 wizm::gui_layer::rgbaToVec4(float r, float g, float b, float a)
{
    return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}
