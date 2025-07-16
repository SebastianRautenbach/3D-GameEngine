#include "layers/script_debug_layer.h"
#include "other utils/common.h"

wizm::script_debug_layer::script_debug_layer()
    :   core_layer("script_debug_layer")
{
}

wizm::script_debug_layer::~script_debug_layer()
{
}

void wizm::script_debug_layer::on_attach()
{
}

void wizm::script_debug_layer::on_detach()
{
}

void wizm::script_debug_layer::update(float delta_time)
{
    (void)delta_time;
	ImGui::Begin("Debug");
    ImVec4 bg_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    ImVec4 warning_text_color = ImVec4(1, 1, 0, 1);
    ImVec4 error_text_color = ImVec4(1, 0, 0, 1);
    ImVec4 success_text_color = ImVec4(0.3f, 0.686f, 0.314f, 1.f);
    ImVec4 crnt_color = ImVec4(1, 1, 1, 1);

    if (ImGui::Button("clear console")) {
        global_console_out.clear();
    }
    
    ImGui::BeginChild("Console", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = ImGui::GetContentRegionAvail(); 
    float scroll_x = ImGui::GetScrollX(), scroll_y = ImGui::GetScrollY(); 
    

    
    draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x + scroll_x, pos.y + size.y + scroll_y), ImGui::GetColorU32(bg_color));

    
    for (const auto& msg : global_console_out) {
    
        if (msg.log_type == CONSOLE_ERROR_LOG)
            crnt_color = error_text_color;
        else if (msg.log_type == CONSOLE_WARNING_LOG)
            crnt_color = warning_text_color;
        else if (msg.log_type == CONSOLE_SUCCESS_LOG)
            crnt_color = success_text_color;
        else
            crnt_color = ImVec4(1.0, 1.0, 1.0, 1.0);

        ImGui::PushStyleColor(ImGuiCol_Text, crnt_color);
        ImGui::TextColored(crnt_color, msg.log.c_str());
        ImGui::PopStyleColor();
    
    }
    
   

    ImGui::EndChild();
	ImGui::End();
}
