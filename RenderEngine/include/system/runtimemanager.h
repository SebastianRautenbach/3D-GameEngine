#pragma once
#include "system/batcher.h"


namespace wizm {
	class runtime_manager {
	public:
		
		runtime_manager();
		void render_runtime(float delta_time, std::shared_ptr<core_gl_shader> m_shader);
		void init_runtime();
		void exit_runtime();


	private:
		std::unique_ptr<batcher> m_batcher_renderer;
		bool initiated_runtime = false;
	};
}
