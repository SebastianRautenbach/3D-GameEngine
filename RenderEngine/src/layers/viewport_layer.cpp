#include "layers/viewport_layer.h"
#include "other utils/matrix_math.h"
#include "input.h"
#include "other utils/strconvr.h"
#include "system/mouse_picking.h"
#include "IconsFontAwesome5.h"
#include "system/camera_3d.h"
#include "other utils/common.h"
#include "system/scene_manager.h"
#include "system/input_manager.h"
#include "filetypes.h"
#include "system/assets/entity_asset.h"


wizm::viewport_layer::viewport_layer(core_framebuffer* fbo,
    std::shared_ptr<camera_manager> camera_manager, gl_renderer* renderer,
    asset_manager* manager, wizm::core_scene* scene)
    : core_layer("viewport_layer"), m_framebuffer(fbo), m_camera_manager(camera_manager),  
    m_renderer(renderer), m_asset_manager(manager), global_scene(scene)
{
 
}

wizm::viewport_layer::~viewport_layer()
{
}

void wizm::viewport_layer::on_attach()
{
    
}

void wizm::viewport_layer::on_detach()
{
}

void wizm::viewport_layer::update(float delta_time)
{
    

    ImGui::Begin("Viewport");
     
    
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    mSize = { viewportPanelSize.x, viewportPanelSize.y };
    m_camera_manager->m_viewport_camera->set_window_size(mSize.x, mSize.y);

    ImGui::Image(reinterpret_cast<void*>(m_framebuffer->get_color_attachment_render_id()), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    if (ImGui::BeginDragDropTarget() && engine_status == EDITOR_STATUS) {

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {

            const wchar_t* id = (const wchar_t*)payload->Data;
            auto temp = m_asset_manager->get_all_assets()[wstring_to_string(id)];
            
            if(is_map_file(wstring_to_string(id)))
            {
                global_scene->read_map_data(wstring_to_string(id));
                global_scene->m_reloaded = true;
                m_camera_manager->load_save_viewport_camera(wstring_to_string(id));
                m_camera_manager->update_crnt_camera(false);
            }
            else if (is_entity_file(temp->file_name)) {
                global_scene->load_entity(std::dynamic_pointer_cast<entity_asset>(temp)->data);
                global_scene->m_reloaded = true;
            }
            else if (is_mesh_file(temp->file_name)) {
                core_entity* new_ent = new core_entity("new ent");
                
                staticmesh_component* mesh = new staticmesh_component();
                mesh->m_model = std::dynamic_pointer_cast<staticmesh_asset>(temp);
                
                new_ent->add_component(mesh);                    
                global_scene->add_entity(new_ent);

                global_scene->m_reloaded = true;
            }

            
            
        }

        ImGui::EndDragDropTarget();
    }

    ImVec2 button_size = ImVec2(ImGui::CalcTextSize(" [T] ").x, 25);
    ImVec2 button_pos = ImVec2((0 + button_size.x), (33));
    
    ImGui::SetCursorPos(button_pos);
    if (ImGui::Button(ICON_FA_ARROWS_ALT "", button_size))
    {
        guizmo_type = ImGuizmo::OPERATION::TRANSLATE;
        m_snap_value = 0.5f;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_REDO "", button_size))
    {
        guizmo_type = ImGuizmo::OPERATION::ROTATE;
        m_snap_value = 45.0f;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_EXPAND "", button_size))
    {
        guizmo_type = ImGuizmo::OPERATION::SCALE;
        m_snap_value = 0.5f;
    }
    ImGui::SameLine();
    ImVec4 global_color = is_global_gizmo ? ImVec4(173 / 255.f, 55 / 255.f, 65 / 255.f, 1.f) : ImGui::GetStyle().Colors[ImGuiCol_Button];
    ImGui::PushStyleColor(ImGuiCol_Button, global_color);
    if (ImGui::Button(ICON_FA_GLOBE "", button_size))
    {
        is_global_gizmo = true;
    }
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImVec4 local_color = is_global_gizmo ?  ImGui::GetStyle().Colors[ImGuiCol_Button] : ImVec4(173 / 255.f, 55 / 255.f, 65 / 255.f, 1.f);
    ImGui::PushStyleColor(ImGuiCol_Button, local_color);
    if (ImGui::Button(ICON_FA_CUBE "", button_size))
    {
        is_global_gizmo = false;
    }
    ImGui::PopStyleColor();
    
    ImGui::SameLine(); 
    ImVec4 magnet_color = m_should_snap ? ImVec4(173 / 255.f, 55 / 255.f, 65 / 255.f,  1.f) : ImGui::GetStyle().Colors[ImGuiCol_Button];
    ImGui::PushStyleColor(ImGuiCol_Button, magnet_color);
    if (ImGui::Button(ICON_FA_MAGNET "", button_size))
    {
        if (m_should_snap)
            m_should_snap = false;
        else
            m_should_snap = true;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImVec4 screen_shot = ImGui::GetStyle().Colors[ImGuiCol_Button];
    ImGui::PushStyleColor(ImGuiCol_Button, screen_shot);
    if (ImGui::Button(ICON_FA_PHOTO_VIDEO "", button_size))
    {
        
    }
    ImGui::PopStyleColor();


    
    if(engine_status == EDITOR_STATUS)
        scene_viewport_func(delta_time);

    ImGui::End();
}

void wizm::viewport_layer::scene_viewport_func(float delta_time)
{
    
    if(global_scene->get_crnt_entity())
    {
        auto pivot = global_scene->get_crnt_entity();
        glm::mat4 mat;

        if (is_global_gizmo) {
            glm::mat4 rotation = glm::mat4_cast(glm::quat(glm::vec3(0.0)));

            mat = glm::translate(glm::mat4(1.0f), pivot->get_position()) * rotation *
                glm::scale(glm::mat4(1.0f), pivot->get_scale());
        }
        else {
            mat = pivot->get_world_transform();
        }

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);



        glm::mat4 viewMatrix = m_camera_manager->m_viewport_camera->get_view_matrix();

        glm::mat4 projectionMatrix = m_camera_manager->m_viewport_camera->get_projection_matrix();


        float snapvals[3] = { m_snap_value, m_snap_value , m_snap_value };


        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
            (ImGuizmo::OPERATION)guizmo_type,
            ImGuizmo::LOCAL, glm::value_ptr(mat), nullptr, m_should_snap ? snapvals : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 position = glm::vec3(0);
            glm::vec3 rotation = glm::vec3(0);
            glm::vec3 scale = glm::vec3(0);



            lowlevelsys::decompose_transform(mat, position, rotation, scale);

            glm::vec3 diff = - pivot->get_position() + position;

            for (auto e : global_scene->get_selected_entities()) {
                if (e != nullptr) {
                    glm::mat4 currentWorldTransform = e->get_world_transform();

                    if (e->get_parent()) {
                        glm::mat4 parentWorldTransform = e->get_parent()->get_world_transform();
                        glm::mat4 newLocalTransform = glm::inverse(parentWorldTransform) * mat;

                        glm::vec3 localPosition, localRotation, localScale;
                        lowlevelsys::decompose_transform(newLocalTransform, localPosition, localRotation, localScale);

                        e->set_position(localPosition);
                        e->set_rotation(localRotation);
                        e->set_scale(localScale);
                    }
                    else {
                        glm::vec3 diff = position - pivot->get_position();
                        glm::vec3 rotationDelta = rotation - pivot->get_rotation();
                        glm::vec3 scaleDelta = scale / pivot->get_scale();

                        e->add_position(diff);
                        e->add_rotation(rotationDelta);
                        e->set_scale(e->get_scale() * scaleDelta);
                    }
                }
            }

        }


       
    }

    

    if (!ImGuizmo::IsUsing() && ImGui::IsWindowHovered())
    {
        static ImVec2 rectStart(-1, -1);
        static ImVec2 rectEnd(-1, -1);


        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && global_input_manager->has_key_been_pressed(GLFW_KEY_LEFT_CONTROL)) {
            get_mouse_pick(true);
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {



            get_mouse_pick(false);

            rectStart = ImGui::GetMousePos();
            rectEnd = rectStart;


        }

        

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
            properties_mouse_pick(false);
        }

        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            rectEnd = ImGui::GetMousePos();
        }


        if (rectStart.x >= 0 && rectStart.y >= 0 && rectEnd.x >= 0 && rectEnd.y >= 0) {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(rectStart, rectEnd, IM_COL32(0, 0, 255, 100));
        }


        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            rectStart = ImVec2(-1, -1);
            rectEnd = ImVec2(-1, -1);
        }
    }




    if (ImGui::BeginPopup("ModEnt") && global_scene->get_crnt_entity() != nullptr) {

        ImGui::Text("Modify Entity");
        ImGui::Separator();

        if (ImGui::MenuItem("Delete")) {         
            global_scene->delete_enity(global_scene->get_crnt_entity());
            global_scene->clear_selected_entities();
            m_renderer->update_draw_data();
        }
        if (ImGui::MenuItem("Duplicate")) {

            auto name = global_scene->get_crnt_entity()->m_ent_name;

            while (global_scene->does_ent_name_exist(name)) {
                name += "(1)";
            }


            auto crnt = global_scene->get_crnt_entity()->copy_(name);
            global_scene->add_entity(crnt);
            global_scene->clear_selected_entities();
            global_scene->add_selected_entity(crnt);       
        }

        ImGui::EndPopup();
    }
}

void wizm::viewport_layer::get_mouse_pick(bool multi_select)
{ 
    ImVec2 crnt_mouse_pos = ImGui::GetMousePos();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();


    glm::vec2 rel_mouse_pos(crnt_mouse_pos.x - window_pos.x, crnt_mouse_pos.y - window_pos.y);
    glm::vec2 norm_mouse_pos(rel_mouse_pos.x / window_size.x, rel_mouse_pos.y / window_size.y);


    glm::vec3 ray_dir = ray::ray_cast(norm_mouse_pos, glm::vec2(1.0f, 1.0f), m_camera_manager->m_viewport_camera->get_projection_matrix(), m_camera_manager->m_viewport_camera->get_view_matrix());
    glm::vec3 ray_pos = ray::ray_origin(m_camera_manager->m_viewport_camera->get_view_matrix());

    if (!multi_select)
        global_scene->clear_selected_entities();

    global_scene->add_selected_entity(get_ent_pick(ray_dir, ray_pos));
}

void wizm::viewport_layer::properties_mouse_pick(bool multi_select)
{
    ImVec2 crnt_mouse_pos = ImGui::GetMousePos();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();


    glm::vec2 rel_mouse_pos(crnt_mouse_pos.x - window_pos.x, crnt_mouse_pos.y - window_pos.y);
    glm::vec2 norm_mouse_pos(rel_mouse_pos.x / window_size.x, rel_mouse_pos.y / window_size.y);


    glm::vec3 ray_dir = ray::ray_cast(norm_mouse_pos, glm::vec2(1.0f, 1.0f), m_camera_manager->m_viewport_camera->get_projection_matrix(), m_camera_manager->m_viewport_camera->get_view_matrix());
    glm::vec3 ray_pos = ray::ray_origin(m_camera_manager->m_viewport_camera->get_view_matrix());

    if(!multi_select)
        global_scene->clear_selected_entities();
    
    
    global_scene->add_selected_entity(get_ent_pick(ray_dir, ray_pos));

    if (global_scene->get_crnt_entity() != nullptr) { ImGui::OpenPopup("ModEnt"); }

   
}





core_entity* wizm::viewport_layer::get_ent_pick(glm::vec3 ray_dir, glm::vec3 ray_pos)
{
    std::vector<std::pair<float, core_entity*>> touched_entities;

    for (const auto& ent : global_scene->m_entities) {
        for (const auto& comp : ent->m_components_list) {
            /*
                Only reason we doing the static mesh component different from the rest is because
                its bounding volume is determined by its asset and not as the component itself
            */
            auto sm_comp = dynamic_cast<staticmesh_component*>(comp);
            auto renderable = dynamic_cast<core_renderable*>(comp);

            glm::vec3 intersection_point;

            if (sm_comp && sm_comp->m_model) {
                
                sm_comp->m_model->update_boundingvolume(
                    ent->get_world_position(),
                    glm::eulerAngles(ent->get_world_rotation_quat()), 
                    ent->get_world_scale()
                );

                if (sm_comp->m_model->ray_intersect(ray_dir, ray_pos, intersection_point)) {
                    float distance = glm::length(intersection_point - m_camera_manager->m_viewport_camera->get_position());
                    touched_entities.emplace_back(distance, ent);
                }
            }
            else if (renderable) {
                renderable->update_boundingvolume(
                    comp->get_world_position(),
                    glm::vec3(0.0f), 
                    glm::vec3(1.0f)
                );

                if (renderable->ray_intersect(ray_dir, ray_pos, intersection_point)) {
                    float distance = glm::length(intersection_point - m_camera_manager->m_viewport_camera->get_position());
                    touched_entities.emplace_back(distance, ent);
                }
            }
        }
    }

    if (touched_entities.empty())
        return nullptr;

    auto closest_entity_it = std::min_element(
        touched_entities.begin(), touched_entities.end(),
        [](const auto& a, const auto& b) {
            return a.first < b.first;
        }
    );

    if (closest_entity_it != touched_entities.end()) {
        return closest_entity_it->second;
    }

    return nullptr;


}

