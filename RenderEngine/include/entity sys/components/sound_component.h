#pragma once
#include "entity sys/component.h"
#include "system/audio_manager.h"
#include "system/assets/sound_asset.h"


using namespace lowlevelsys;

namespace wizm {

	class sound_component : public core_component, public core_renderable {
	public:

		sound_component();
		~sound_component();


		void component_preupdate() override;
		void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader) override;
		void component_postupdate() override;
		core_component* _copy() const;


		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override {
			set_position(glm::vec3(
				save_t[index]["transform"].get_float("position")[0],
				save_t[index]["transform"].get_float("position")[1],
				save_t[index]["transform"].get_float("position")[2]
			));
			set_rotation(glm::vec3(
				save_t[index]["transform"].get_float("rotation")[0],
				save_t[index]["transform"].get_float("rotation")[1],
				save_t[index]["transform"].get_float("rotation")[2]
			));
			set_scale(glm::vec3(
				save_t[index]["transform"].get_float("scale")[0],
				save_t[index]["transform"].get_float("scale")[1],
				save_t[index]["transform"].get_float("scale")[2]
			));

			asset_id = save_t[index].get_string("asset id")[0];
			m_loop = save_t[index].get_int("m_loop")[0];
			m_is3d = save_t[index].get_int("m_is3d")[0];
			m_play_on_start = save_t[index].get_int("m_play_on_start")[0];
			
		}

		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {
			(void)parent_name;
			save_t["sound_component" + index]["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
			save_t["sound_component" + index]["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
			save_t["sound_component" + index]["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });

			save_t["sound_component" + index].set_string("asset id", { asset_id });
			save_t["sound_component" + index].set_int("m_loop", { m_loop });
			save_t["sound_component" + index].set_int("m_is3d", { m_is3d });
			save_t["sound_component" + index].set_int("m_play_on_start", { m_play_on_start });
		}

		void on_start();
		void on_update();
		void play();
		void stop();

	public:
		bool m_loop = false;
		bool m_is3d = false;
		bool m_play_on_start = true;
		std::shared_ptr<sound_asset> m_sound_asset;
		std::string asset_id;


	private:
		bool on_runtime = false;
		bool stopped = false;
		FMOD::Channel* m_channel = nullptr;
	};
}