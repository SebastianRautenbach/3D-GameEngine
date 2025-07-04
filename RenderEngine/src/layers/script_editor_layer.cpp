#include "layers/script_editor_layer.h"
#include "other utils/common.h"
#include "other utils/strconvr.h"
#include "system/asset_manager.h"
#include "system/assets/script_asset.h"

wizm::script_editor_layer::script_editor_layer(asset_manager* passet_manager)
	:core_layer("script_editor_layer"), m_asset_manager(passet_manager)
{
}

wizm::script_editor_layer::~script_editor_layer()
{
}

void wizm::script_editor_layer::on_attach()
{
}

void wizm::script_editor_layer::on_detach()
{
}

void wizm::script_editor_layer::update(float delta_time)
{



	ImGui::Begin("script editor");

	
	if (engine_status == EDITOR_STATUS)
	{
		static char char_buffer[20000];

		if (ImGui::Button("BUILD")) {
			if (!crnt_script_asset_path.empty())
			{
				write_script(crnt_script_asset_path, char_buffer);
				auto asset = m_asset_manager->load<script_asset>(crnt_script_asset_id, "");
				asset->sc->reload_script(crnt_script_asset_path);
			}
		}

		float window_hieght = ImGui::GetWindowHeight() - 75;



		ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
		ImGui::InputTextMultiline("##crnt_script_work", char_buffer, sizeof(char_buffer), ImVec2(-FLT_MIN, window_hieght), flags);
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {

				const wchar_t* id = (const wchar_t*)payload->Data;
				crnt_script_asset_path = m_asset_manager->get_asset_details_from_id(wstring_to_string(id)).path;
				crnt_script_asset_id = wstring_to_string(id);
				if (crnt_script_asset_path.find(".wizs") != -1)
				{
					load_script(crnt_script_asset_path, char_buffer, 20000);
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
	else
	{
		ImGui::Text("CAN'T EDIT DURING RUNTIME");
	}
	


	ImGui::End();
}

bool wizm::script_editor_layer::write_script(const std::string& filepath, const char* buffer)
{
	std::ofstream file(filepath, std::ios::binary);

	if (!file.is_open()) {
		return false;
	}

	file.write(buffer, std::strlen(buffer));

	if (!file) {
		return false;
	}

	file.close();
	return true;
}

bool wizm::script_editor_layer::load_script(const std::string& filepath, char* buffer, size_t bufferSize)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file.is_open()) {
		return false;
	}
	file.read(buffer, bufferSize);

	if (!file) {
		return false;
	}

	buffer[file.gcount()] = '\0';

	file.close();
	return true;


}


