#include "layers/content_browser_layer.h"
#include "other utils/strconvr.h"
#include "IconsFontAwesome5.h"
#include "filetypes.h"
#include "system/scripting/compose_script.h"
#include "system/compose_material.h"

#include "system/compose_level.h"
#include "other utils/copy_to_clipboard.h"
#include "other utils/common.h"


wizm::content_browser_layer::content_browser_layer(asset_manager* p_asset_manager)
    :m_asset_manager(p_asset_manager)
{
	watcher = new filewatcher();
	assets = asset_import.retrieve_all_assets();

    folder_file_texture = new core_gl_texture("resources/images/folder_icon.png");
	file_texture = new core_gl_texture("resources/images/file_icon.png");
    mesh_file_icon = new core_gl_texture("resources/images/mesh_file_icon.png");
    image_file_icon = new core_gl_texture("resources/images/image_file_icon.png");
    map_file_icon = new core_gl_texture("resources/images/map_file_icon.png");
    script_file_icon = new core_gl_texture("resources/images/script_icon.png");
    sound_file_icon = new core_gl_texture("resources/images/sound_file_icon.png");
    material_file_icon = new core_gl_texture("resources/images/material_file_icon.png");
    entity_file_icon = new core_gl_texture("resources/images/entity_file_icon.png");
}

wizm::content_browser_layer::~content_browser_layer()
{
	delete watcher;
	delete folder_file_texture;
    delete mesh_file_icon;
    delete image_file_icon;
    delete map_file_icon;
    delete script_file_icon;
    delete material_file_icon;
    delete entity_file_icon;
}

void wizm::content_browser_layer::on_attach()
{
    all_content = get_directory_content(current_directory.string());
}

void wizm::content_browser_layer::on_detach()
{
	
}




void wizm::content_browser_layer::update(float delta_time)
{
	ImGui::Begin("Content Browser");

    if (engine_status != EDITOR_STATUS) {
        ImGui::End();
        return;
    }
	
	
	if (ImGui::Button("refresh")) {
		
        refresh_assets();
        
	}

    ImGui::SameLine();

  
    
 
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ImGui::CalcTextSize("Add Asset").x + 30));
    if (ImGui::BeginMenu("Add Asset")) {
        
        if (ImGui::MenuItem("Create new script")) {
            std::string unique_name = generate_unique_name(all_content, "new_script", ".wizs");
            engine_scripting::create_script(current_directory.string(), unique_name);
            refresh_assets();
        }

        if (ImGui::MenuItem("Create new level")) {        
            std::string unique_name = generate_unique_name(all_content, "new_level", ".zer");
            create_level(current_directory.string(), unique_name);
            refresh_assets();
        }

        if (ImGui::MenuItem("Create new material")) {         
            std::string unique_name = generate_unique_name(all_content, "new_material", ".wmat");
            create_material(current_directory.string(), unique_name);
            refresh_assets();
        }

 
        ImGui::EndMenu();
    }
    ImGui::SetCursorPosX(0);

	if (ImGui::Button("...##back")) {
		if(current_directory != "GAME")
        {
            current_directory = current_directory.parent_path();
            all_content = get_directory_content(current_directory.string());
        }
	}


	float thumdnail_size = 256;
	float panel_width = ImGui::GetContentRegionAvail().x / thumdnail_size * 2;

    if (panel_width < 1) { panel_width = 1; }

	ImGui::Columns(static_cast<int>(panel_width), 0, false);
	
    static std::string new_file_name;
    static std::string selected_file_path;
	
    for (auto& entry : all_content) {

        auto file_name = entry.filename().string();
        ImGui::PushID(file_name.c_str());

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (std::filesystem::is_directory(entry)) {
            //---------------------------------------------------------------------------------------------------------------	FOLDER
            ImGui::ImageButton((ImTextureID)folder_file_texture->texture_id, { thumdnail_size / 2, thumdnail_size / 2 }, { 0,1 }, { 1,0 });
            
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("FilePopup");
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                current_directory = entry;
                all_content = get_directory_content(current_directory.string());

                ImGui::PopStyleColor();
                ImGui::NextColumn();
                ImGui::PopID();
                break;
            }
            ImGui::Text(file_name.c_str());
        }
        else {
            //---------------------------------------------------------------------------------------------------------------	FILE
            ImTextureID final_texture_icon = (ImTextureID)file_texture->texture_id;
            
            if (is_mesh_file(file_name))
                final_texture_icon = (ImTextureID)mesh_file_icon->texture_id;
            else if (is_texture_file(file_name))
                final_texture_icon = (ImTextureID)image_file_icon->texture_id;
            else if (is_map_file(file_name))
                final_texture_icon = (ImTextureID)map_file_icon->texture_id;
            else if (is_script_file(file_name))
                final_texture_icon = (ImTextureID)script_file_icon->texture_id;
            else if (is_sound_file(file_name))
                final_texture_icon = (ImTextureID)sound_file_icon->texture_id;
            else if (is_material_file(file_name))
                final_texture_icon = (ImTextureID)material_file_icon->texture_id;
            else if(is_entity_file(file_name))
                final_texture_icon = (ImTextureID)entity_file_icon->texture_id;

               

            ImGui::ImageButton(final_texture_icon, { thumdnail_size / 2, thumdnail_size / 2 }, { 0,1 }, { 1,0 });

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("FilePopup");
                //selected_file_path = entry.string();
                new_file_name = file_name;
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // this is for now

                if (is_material_file(file_name)) {


                    window_open.open = true;
                    window_open.window_type = MATERIAL_EDITOR_WINDOW;
                    
                    for (const auto& asset : assets) {
                        auto asset_path = std::filesystem::directory_entry(asset.path).path().string();
                        std::replace(asset_path.begin(), asset_path.end(), '/', '\\');
                        auto entry_path = entry.string();
                    
                        if (asset_path == entry_path) {
                            window_open.window_open_with_id = asset.id.c_str();
                        }
                    }
                }
                else if (is_map_file(file_name)) {
                    
                }
                else {
                    ShellExecute(NULL, L"open", entry.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }

            }

            if (ImGui::BeginPopup("FilePopup")) {
                if (ImGui::MenuItem("Delete")) {
                   
                    std::filesystem::remove(entry);
                    add_console_line("removed entity:" + entry.string(), CONSOLE_GENERAL_LOG);
                    refresh_assets();

                    ImGui::EndPopup();
                    ImGui::PopStyleColor();
                    ImGui::NextColumn();
                    ImGui::PopID();
                    break;
                }
                if (ImGui::MenuItem("Rename")) {
                    
                    ImGui::SetKeyboardFocusHere();
                }
                if (ImGui::MenuItem("Copy ID")) {
                    for (const auto& asset : assets) {
                        auto asset_path = std::filesystem::directory_entry(asset.path).path().string();
                        std::replace(asset_path.begin(), asset_path.end(), '/', '\\');
                        auto entry_path = entry.string();

                        if (asset_path == entry_path) {
                            copy_to_clipboard(asset.id);
                        }
                    }
                    refresh_assets();
                }

               
                ImGui::InputText("##RenameFile", &new_file_name[0], ImGuiInputTextFlags_EnterReturnsTrue);
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    std::string asset_id;
                    for (const auto& asset : assets) {
                        auto asset_path = std::filesystem::directory_entry(asset.path).path().string();
                        std::replace(asset_path.begin(), asset_path.end(), '/', '\\');
                        auto entry_path = entry.string();

                        if (asset_path == entry_path) {
                            asset_id = asset.id;
                        }
                    }

                    if (!new_file_name.empty()) {
                        std::filesystem::path parent_path = entry.parent_path();
                        std::filesystem::path original_extension = entry.extension();
                       
                        std::filesystem::path new_path = parent_path / new_file_name.c_str();
                        if (new_path.has_extension()) {
                            new_path.replace_extension(new_path.extension());
                        }
                        else {                      
                            new_path.replace_extension(original_extension);
                        }

                        try {
                            std::filesystem::rename(entry, new_path);
                            asset_import.edit_asset_path(asset_id, new_path.string());
                            refresh_assets();
                            std::cout << "File renamed to: " << new_path << std::endl;
                        }
                        catch (const std::filesystem::filesystem_error& e) {
                            std::cerr << "Error renaming file: " << e.what() << std::endl;
                        }
                    }
                    ImGui::CloseCurrentPopup();
                }



                ImGui::EndPopup();
            }

            
            if (ImGui::BeginDragDropSource()) {
                ImGui::SetTooltip("%s", entry.filename().string().c_str());

                std::wstring asset_id;
                const wchar_t* wstr = L"";
                
                if (entry.filename().string().find(".zer") != -1) {
                    
                    asset_id = string_to_wstring(entry.string());
                    wstr = asset_id.c_str();
                }


                for (const auto& asset : assets) {
                    auto asset_path = std::filesystem::directory_entry(asset.path).path().string();
                    std::replace(asset_path.begin(), asset_path.end(), '/', '\\');
                    auto entry_path = entry.string();

                    if (asset_path == entry_path) {
                        asset_id = string_to_wstring(asset.id);
                        wstr = asset_id.c_str();
                    }
                }

                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", wstr, (wcslen(wstr) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::Text(file_name.c_str());
        }





        ImGui::PopStyleColor();
        ImGui::NextColumn();
        ImGui::PopID();
    }

	ImGui::End();
}

void wizm::content_browser_layer::refresh_assets()
{
    watcher->refresh();
    m_asset_manager->load_assets_db();
    assets = asset_import.retrieve_all_assets();
    all_content = get_directory_content(current_directory.string());
}


std::vector<std::filesystem::path> wizm::content_browser_layer::get_directory_content(const std::filesystem::path& path)
{
	std::vector<std::filesystem::path> contents;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		contents.push_back(entry.path());
	}
	return contents;
}

std::string wizm::content_browser_layer::generate_unique_name(const std::vector<std::filesystem::path>& all_files_in_dir, const std::string& base_name, const std::string& extension)
{
    std::string unique_name = base_name;
    int counter = 0;

    auto is_name_taken = [&](const std::string& name) {
        return std::any_of(all_files_in_dir.begin(), all_files_in_dir.end(),
            [&](const std::filesystem::path& path) {
                return path.filename().string() == name;
            });
        };

    while (is_name_taken(unique_name + extension)) {
        counter++;
        unique_name = base_name + std::to_string(counter);
    }

    return unique_name + extension;
}
