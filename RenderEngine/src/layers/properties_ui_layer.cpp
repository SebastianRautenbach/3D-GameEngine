#include "layers/properties_ui_layer.h"
#include "other utils/strconvr.h"
#include "system/scene_manager.h"

wizm::properties_ui_layer::properties_ui_layer( gl_renderer* renderer, asset_manager* manager)
	:core_layer("properties_ui_layer"), m_renderer(renderer), m_asset_manager(manager)
{
}

wizm::properties_ui_layer::~properties_ui_layer()
{
}

void wizm::properties_ui_layer::on_attach()
{
}

void wizm::properties_ui_layer::on_detach()
{
}

void wizm::properties_ui_layer::update(float delta_time)
{
	ImGui::Begin("Properties");

	if (engine_status != EDITOR_STATUS) {
		ImGui::End();
		return;
	}
	
	if (global_scene->get_crnt_entity() != nullptr)
	{
		ImGui::Text(global_scene->get_crnt_entity()->m_ent_ID.c_str());


		std::string trans_id;


		
		

		ImGui::Text("Position:");
		
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			float pos[3] = { global_scene->get_crnt_entity()->get_position().x, global_scene->get_crnt_entity()->get_position().y, global_scene->get_crnt_entity()->get_position().z };
			ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize(" X ").x, ImGui::GetFrameHeight());
			float dragWidth = 100.0f;
			



			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "posX";
			ImGui::Button("X", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &pos[0], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_position(glm::vec3(pos[0], pos[1], pos[2]));
			ImGui::PopStyleColor(3);



			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "posY";
			ImGui::SameLine();
			ImGui::Button("Y", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &pos[1], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_position(glm::vec3(pos[0], pos[1], pos[2]));
			ImGui::PopStyleColor(3);




			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.5f, 0.9f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "posZ";
			ImGui::SameLine();
			ImGui::Button("Z", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &pos[2], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_position(glm::vec3(pos[0], pos[1], pos[2]));
			ImGui::PopStyleColor(3);

			ImGui::PopStyleVar();
		}


		ImGui::Text("Rotation:");
		{
			

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			float rot[3] = { global_scene->get_crnt_entity()->get_rotation().x, global_scene->get_crnt_entity()->get_rotation().y, global_scene->get_crnt_entity()->get_rotation().z };
			ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize(" X ").x, ImGui::GetFrameHeight());
			float dragWidth = 100.0f;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "rotX";
			ImGui::Button("P", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &rot[0], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_rotation(glm::vec3(rot[0], rot[1], rot[2]));
			ImGui::PopStyleColor(3);

			
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "rotY";
			ImGui::SameLine();
			ImGui::Button("Y", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &rot[1], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_rotation(glm::vec3(rot[0], rot[1], rot[2]));
			ImGui::PopStyleColor(3);


			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.5f, 0.9f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "rotZ";
			ImGui::SameLine();
			ImGui::Button("R", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &rot[2], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_rotation(glm::vec3(rot[0], rot[1], rot[2]));
			ImGui::PopStyleColor(3);

			
			ImGui::PopStyleVar();
		}

		ImGui::Text("Scale:");

		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			float sca[3] = { global_scene->get_crnt_entity()->get_scale().x, global_scene->get_crnt_entity()->get_scale().y, global_scene->get_crnt_entity()->get_scale().z };
			ImVec2 buttonSize = ImVec2(ImGui::CalcTextSize(" X ").x, ImGui::GetFrameHeight());
			float dragWidth = 100.0f;



			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "scaX";
			ImGui::Button("X", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &sca[0], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_scale(glm::vec3(sca[0], sca[1], sca[2]));
			ImGui::PopStyleColor(3);


			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "scaY";
			ImGui::SameLine();
			ImGui::Button("Y", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &sca[1], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_scale(glm::vec3(sca[0], sca[1], sca[2]));
			ImGui::PopStyleColor(3);



			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.5f, 0.9f, 1.0f }); ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.4f, 0.8f, 1.0f });
			trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "scaZ";
			ImGui::SameLine();
			ImGui::Button("Z", buttonSize);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragWidth);
			if (ImGui::DragFloat(trans_id.c_str(), &sca[2], .01f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat))
				global_scene->get_crnt_entity()->set_scale(glm::vec3(sca[0], sca[1], sca[2]));
			ImGui::PopStyleColor(3);

			ImGui::PopStyleVar();
		}



		

		ImGui::Text("Components");
		ImGui::Separator();

		for (int i = 0; i < global_scene->get_crnt_entity()->m_components_list.size(); i++)
		{
			auto& comps = global_scene->get_crnt_entity()->m_components_list[i];


			std::string comp_type;

			switch (comps->m_component_type)
			{
			case ePointLight:
				comp_type = "PointLight";
				break;
			case eStaticMesh:
				comp_type = "StaticMesh";
				break;
			case eDirectionalLight:
				comp_type = "DirectionalLight";
				break;
			case eSpotLight:
				comp_type = "SpotLight";
				break;
			case eCamera:
				comp_type = "CameraComponent";
				break;
			case eScripting:
				comp_type = "ScriptingComponent";
				break;
			case eSound3D:
				comp_type = "SoundComponent";
				break;

			default:
				break;
			}


			trans_id = comp_type + "##" + global_scene->get_crnt_entity()->m_ent_ID + std::to_string(comps->m_component_type) + std::to_string(i);
			if (ImGui::CollapsingHeader(trans_id.c_str()))
			{
				trans_id = "transfrom ##" + global_scene->get_crnt_entity()->m_ent_ID + std::to_string(comps->m_component_type) + std::to_string(i);
				ImGui::Indent(20.0f);
				if (ImGui::CollapsingHeader("transform"))
				{

					ImGui::Text("Position:");
					float pos[3] = { comps->get_position().x, comps->get_position().y, comps->get_position().z };
					std::string trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "c pos" + std::to_string(comps->m_component_type) + std::to_string(i);
					if (ImGui::DragFloat3(trans_id.c_str(), pos, 0.01))
						comps->set_position(glm::vec3(pos[0], pos[1], pos[2]));

					ImGui::Text("Rotation:");
					float rot[3] = { comps->get_rotation().x, comps->get_rotation().y, comps->get_rotation().z };
					trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "c rot" + std::to_string(comps->m_component_type) + std::to_string(i);
					if (ImGui::DragFloat3(trans_id.c_str(), rot, 0.1))
						comps->set_rotation(glm::vec3(rot[0], rot[1], rot[2]));


					ImGui::Text("Scale:");
					float sca[3] = { comps->get_scale().x, comps->get_scale().y, comps->get_scale().z };
					trans_id = "##" + global_scene->get_crnt_entity()->m_ent_ID + "c sca" + std::to_string(comps->m_component_type) + std::to_string(i);
					if (ImGui::DragFloat3(trans_id.c_str(), sca, 0.01))
						comps->set_scale(glm::vec3(sca[0], sca[1], sca[2]));


				}



				ImGui::Unindent(20.0f);


				modify_component_attrib(comp_type, comps);
			}
			if (ImGui::IsItemClicked(1))
			{
				ImGui::OpenPopup("ModComp");
				global_scene->get_crnt_entity()->set_selected_comp(comps);
			}

		}

		

		if (ImGui::BeginPopup("ModComp")) {
			ImGui::Text("Modify Component");
			ImGui::Separator();

			if (ImGui::MenuItem("Delete")) {

				

				global_scene->get_crnt_entity()->m_components_list.erase(std::find(global_scene->get_crnt_entity()->m_components_list.begin(),
				global_scene->get_crnt_entity()->m_components_list.end(), global_scene->get_crnt_entity()->get_selected_comp()));
				global_scene->get_crnt_entity()->set_selected_comp(nullptr);
			}

			ImGui::EndPopup();
		}

		component_add_popup();

	}

	ImGui::End();
}

void wizm::properties_ui_layer::component_add_popup()
{
	if (ImGui::Button("Add Component")) {
		ImGui::OpenPopup("AddComponentPopup");
	}

	if (ImGui::BeginPopup("AddComponentPopup")) {
		ImGui::Text("Select Component to Add");
		ImGui::Separator();

		if (ImGui::MenuItem("Static Mesh")) {
			global_scene->get_crnt_entity()->add_component(new staticmesh_component());
		}
		if (ImGui::MenuItem("Point Light")) {
			global_scene->get_crnt_entity()->add_component(new pointlight_component());
			
		}
		if (ImGui::MenuItem("Spot Light")) {
			global_scene->get_crnt_entity()->add_component(new spotlight_component());
		}
		if (ImGui::MenuItem("Directional Light")) {
			global_scene->get_crnt_entity()->add_component(new directionallight_component());
		}
		if (ImGui::MenuItem("Sound")) {
			global_scene->get_crnt_entity()->add_component(new sound_component());
		}
		if (ImGui::MenuItem("Camera")) {
			global_scene->get_crnt_entity()->add_component(new camera_component());
		}
		if (ImGui::MenuItem("Scripting")) {
			global_scene->get_crnt_entity()->add_component(new scripting_component());
		}

		ImGui::EndPopup();
	}
}

void wizm::properties_ui_layer::modify_component_attrib(std::string& type, core_component* component)
{
	std::string trans_id;
	if (type == "PointLight")
	{
		trans_id = "Radius ##" + global_scene->get_crnt_entity()->m_ent_ID + std::to_string(component->m_component_type);
		auto pointlight = dynamic_cast<pointlight_component*>(component);

		ImGui::InputFloat(trans_id.c_str(), &pointlight->m_radius);


		trans_id = "Intensity ##" + global_scene->get_crnt_entity()->m_ent_ID + std::to_string(component->m_component_type);
		ImGui::InputFloat(trans_id.c_str(), &pointlight->m_intensity);

		float test[3] = { pointlight->m_diffuse.x, pointlight->m_diffuse.y, pointlight->m_diffuse.z };

		ImGui::ColorPicker3("test", test);
		pointlight->m_diffuse = glm::vec3(test[0], test[1], test[2]);
	}


	if (type == "DirectionalLight")
	{
		auto directionallight = dynamic_cast<directionallight_component*>(component);

		float test[3] = { directionallight->m_ambient.x, directionallight->m_ambient.y, directionallight->m_ambient.z };

		ImGui::ColorPicker3("test", test);
		directionallight->m_ambient = glm::vec3(test[0], test[1], test[2]);
	}

	if (type == "SpotLight")
	{
		auto spotlight = dynamic_cast<spotlight_component*>(component);

		float ambient[3] = { spotlight->m_ambient.x, spotlight->m_ambient.y, spotlight->m_ambient.z };
		ImGui::ColorPicker3("Ambient", ambient);
		spotlight->m_ambient = glm::vec3(ambient[0], ambient[1], ambient[2]);

		float diffuse[3] = { spotlight->m_diffuse.x, spotlight->m_diffuse.y, spotlight->m_diffuse.z };
		ImGui::ColorPicker3("Diffuse", diffuse);
		spotlight->m_diffuse = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);


		float specular[3] = { spotlight->m_specular.x, spotlight->m_specular.y, spotlight->m_specular.z };
		ImGui::ColorPicker3("Specular", specular);
		spotlight->m_specular = glm::vec3(specular[0], specular[1], specular[2]);

		ImGui::SliderFloat("Constant", &spotlight->m_constant, 0.0f, 10.0f, "%.3f");
		ImGui::SliderFloat("Linear", &spotlight->m_linear, 0.0f, 10.0f, "%.3f");
		ImGui::SliderFloat("Quadratic", &spotlight->m_quadratic, 0.0f, 10.0f, "%.3f");
		ImGui::InputFloat("Distance", &spotlight->m_distance,0.1f);

		float cutOffDeg = glm::degrees(glm::acos(spotlight->m_cutOff));
		float outerCutOffDeg = glm::degrees(glm::acos(spotlight->m_outerCutOff));

		ImGui::SliderFloat("CutOff Angle", &cutOffDeg, 0.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Outer CutOff Angle", &outerCutOffDeg, 0.0f, 90.0f, "%.1f");

		spotlight->m_cutOff = glm::cos(glm::radians(cutOffDeg));
		spotlight->m_outerCutOff = glm::cos(glm::radians(outerCutOffDeg));
	}

	if (type == "CameraComponent") {
		auto camera = dynamic_cast<camera_component*>(component);
		ImGui::Separator();
		ImGui::Text("Main camera?");
		ImGui::SameLine();
		ImGui::Checkbox("##maincamera", &camera->is_main_camera);
	}


	if (type == "StaticMesh")
	{
		auto staticmesh = dynamic_cast<staticmesh_component*>(component);
		std::string staticmeshfilename, materialassetname;
		
		{
			if (staticmesh->m_model)
				staticmeshfilename = staticmesh->m_model->file_name;
			else
				staticmeshfilename = "Drag and Drop";
		}
		


		
		ImGui::Text("Change Mesh");
		ImGui::Button(staticmeshfilename.c_str(), ImVec2(125, 125));
		if (ImGui::BeginDragDropTarget()) {
		
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {

				const wchar_t* id = (const wchar_t*)payload->Data;

				if (m_asset_manager->get_asset_details_from_id(wstring_to_string(id)).type == tMESH)
				{
					staticmesh->m_mesh_asset_id = wstring_to_string(id);
					m_asset_manager->assign_assets();
					global_scene->m_reloaded = true;
				}
			}

			ImGui::EndDragDropTarget();
		}
		
		ImGui::Text("Change Material");
		
		for (int i = 0; i < staticmesh->m_materials.size(); i++) {
			
			if(i < staticmesh->m_materials.size())
			{

				materialassetname = staticmesh->m_materials[i] ? staticmesh->m_materials[i]->file_name : "Drag and Drop";
				materialassetname += "##";
				materialassetname += i;

				ImGui::Button(materialassetname.c_str(),
					ImVec2(125, 125));
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const wchar_t* id = (const wchar_t*)payload->Data;
						if (m_asset_manager->get_asset_details_from_id(wstring_to_string(id)).type == tMATERIAL) {
						
								while ((i + 1) > staticmesh->m_material_asset_ids.size()) {
									staticmesh->m_material_asset_ids.emplace_back();
								}

							staticmesh->m_material_asset_ids[i] = wstring_to_string(id);													
							global_scene->m_reloaded = true;
							m_asset_manager->assign_assets();
						}
					}

					ImGui::EndDragDropTarget();
				}
			}
		}	
		
		ImGui::Checkbox("is geometry static?", &staticmesh->is_static);
	}


	if (type == "ScriptingComponent") {

		std::string path;
		auto script_component = dynamic_cast<scripting_component*>(component);

		if (script_component) {

			if(script_component->m_script_asset)
				path = script_component->m_script_asset->file_name;

			if (path.empty())
				path = "Change script";

		}

		ImGui::Button(path.c_str(), ImVec2(125, 125));
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {

				const wchar_t* id = (const wchar_t*)payload->Data;

				if (m_asset_manager->get_asset_details_from_id(wstring_to_string(id)).type == tSCRIPT)
				{
					script_component->script_asset_id = wstring_to_string(id);
					global_scene->m_reloaded = true;
					m_asset_manager->assign_assets();
				}
			}

			ImGui::EndDragDropTarget();
		}
	}


	if (type == "SoundComponent") {
		std::string path;
		auto _sound_component = dynamic_cast<sound_component*>(component);
		if (_sound_component) {
			if (_sound_component->m_sound_asset)
				path = _sound_component->m_sound_asset->file_name;

			if (path.empty())
				path = "Change sound";
		}

		ImGui::Button(path.c_str(), ImVec2(125, 125));
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {

				const wchar_t* id = (const wchar_t*)payload->Data;

				if (m_asset_manager->get_asset_details_from_id(wstring_to_string(id)).type == tSOUND)
				{
					_sound_component->asset_id = wstring_to_string(id);
					global_scene->m_reloaded = true;
					m_asset_manager->assign_assets();
				}
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Text("Play from start:");
		ImGui::SameLine();
		ImGui::Checkbox("##playfromstart", &_sound_component->m_play_on_start);

		ImGui::Text("3D sound:");
		ImGui::SameLine();
		ImGui::Checkbox("##3dsound", &_sound_component->m_is3d);

		ImGui::Text("Looping?:");
		ImGui::SameLine();
		ImGui::Checkbox("##looping", &_sound_component->m_loop);
	}
}
