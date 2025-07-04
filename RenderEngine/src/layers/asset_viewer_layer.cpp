#include "layers/asset_viewer_layer.h"
#include "other utils/copy_to_clipboard.h"

wizm::asset_viewer_layer::asset_viewer_layer(asset_manager* p_asset_manager)
	:m_asset_manager(p_asset_manager)
{
	auto x = m_asset_manager->get_all_assets();
	for (const auto& asset : x) {
		m_assets.emplace_back(m_asset_manager->get_asset_details_from_id(asset.first));
	}
}

wizm::asset_viewer_layer::~asset_viewer_layer()
{
}

void wizm::asset_viewer_layer::on_attach()
{
}

void wizm::asset_viewer_layer::on_detach()
{
}

void wizm::asset_viewer_layer::update(float delta_time)
{
	ImGui::Begin("Asset View");
	if (ImGui::Button("Refresh##assetrefresh")) {
		m_assets.clear();
		auto x = m_asset_manager->get_all_assets();
		for (const auto& asset : x) {
			m_assets.emplace_back(m_asset_manager->get_asset_details_from_id(asset.first));
		}
	}
	for (const auto& asset : m_assets) {
		std::string id_ = "copy id##" + asset.id;
		if (ImGui::Button(id_.c_str())) {
			copy_to_clipboard(asset.id);
		}
		ImGui::SameLine();
		ImGui::Text(" || ");
		ImGui::SameLine();
		
		
		ImVec2 text_pos = ImGui::GetCursorScreenPos();
		ImGui::Text(asset.path.c_str());

		
		ImVec2 text_end = ImGui::GetItemRectMax(); 
		auto* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddLine(
			ImVec2(text_pos.x, text_end.y + 2),       
			ImVec2(text_end.x, text_end.y + 2),       
			IM_COL32(255, 255, 255, 255),             
			1.0f                                      
		);

	}
	ImGui::End();
}
