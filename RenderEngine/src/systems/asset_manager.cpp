#include "system/asset_manager.h"
#include "system/scene_manager.h"
#include "system/assets/texture_asset.h"
#include "system/assets/script_asset.h"
#include "system/assets/sound_asset.h"
#include "system/assets/material_asset.h"
#include "system/assets/entity_asset.h"
#include "scene.h"

wizm::asset_manager::asset_manager(audio_manager* audio_manager, core_scene* scene)
	: m_auio_manager(audio_manager), global_scene(scene)
{
	load_assets_db();
}

wizm::asset_manager::~asset_manager()
{
	m_assets.clear();
}

/*
	We basicly check if the component has an asset ID and if it does apply that asset 
	to the neccasary variable.
*/

void wizm::asset_manager::assign_assets()
{
	if(total_entities < global_scene->m_entities.size() || global_scene->m_reloaded)
	{
		for (auto& ent : global_scene->m_entities) {
			for (auto& comp : ent->m_components_list)
			{
				auto mesh_comps = dynamic_cast<staticmesh_component*>(comp);		//------------------------------------ STATIC MESH ASSET
				if (mesh_comps)
				{
					if (m_assets[mesh_comps->m_mesh_asset_id])
					{
						mesh_comps->m_model = load<staticmesh_asset>(mesh_comps->m_mesh_asset_id, "");						
					}



					for (int i = 0; i < mesh_comps->m_material_asset_ids.size(); i++) {			//------------------------------------ MATERIAL ASSIGNMENT
						auto& mat_id = mesh_comps->m_material_asset_ids[i];
						
						if ((i + 1) > mesh_comps->m_materials.size())
							mesh_comps->m_materials.emplace_back();
						
						auto& mat = mesh_comps->m_materials[i];
						
						mat = load<material_asset>(mat_id, "");
					}

				}				

				auto script_comp = dynamic_cast<scripting_component*>(comp);		//------------------------------------ SCRIPTING ASSET ASSIGNMENT
				if (script_comp) {
					script_comp->m_script_asset = load<script_asset>(script_comp->script_asset_id, ""); 
				}

				auto sound_comp = dynamic_cast<sound_component*>(comp);				//------------------------------------ SOUND ASSET ASSIGNMENT		
				if (sound_comp) {
					sound_comp->m_sound_asset = load<sound_asset>(sound_comp->asset_id, "");
				}
			}
		}

		total_entities = global_scene->m_entities.size();
	}


}

void wizm::asset_manager::load_assets_db()
{
	auto assets = asset_import.retrieve_all_assets();

	for (const auto& asset : assets) {
		if (asset.type == tMESH) {
			load<staticmesh_asset>(asset.id, asset.path);
		}
		else if (asset.type == tTEXTURE) {
			load<texture_asset>(asset.id, asset.path);
		}
		else if (asset.type == tSCRIPT) {
			auto _asset = load<script_asset>(asset.id, asset.path);
			_asset->sc->init_script(global_scene, asset.path);
		}
		else if (asset.type == tSOUND) {
			auto _asset = load<sound_asset>(asset.id, asset.path);
			_asset->m_system = m_auio_manager->system;
			_asset->setup();			
		}
		else if (asset.type == tMATERIAL) {
			auto _asset = load<material_asset>(asset.id, asset.path);
			_asset->m_diffuse_texture = load<texture_asset>(_asset->diffuse_asset_id, "");
			_asset->m_specular_texture = load<texture_asset>(_asset->specular_asset_id, "");
		}
		else if (asset.type == tENTITY) {
			load<entity_asset>(asset.id, asset.path);
		}
	}
}
