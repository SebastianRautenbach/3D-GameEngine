#pragma once
#include "system/assets.h"
#include "system/scripting/script_controller.h"
#include <filesystem>


namespace wizm {


	class script_asset : public core_asset {

	public:
		~script_asset() {
			delete sc;
		}

		void load(const std::string& path) override {
			std::filesystem::path file_path(path);
			file_name = file_path.filename().string();
			sc = new script_controller();
		}

		void on_start() {
			sc->on_start();
		}
		void on_update(float delta_time) {
			sc->on_update(delta_time);
		}

		void reset() {
			sc->reset();
		}


	public:
		bool m_initiated_script = false;
		std::string file_name;
		script_controller* sc;
	};
}
