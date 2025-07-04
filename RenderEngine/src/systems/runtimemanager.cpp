#include "system/runtimemanager.h"
#include "other utils/ZER.h"


wizm::runtime_manager::runtime_manager()
{
	filedata::ZER get_batchmax;
	get_batchmax.read_file_cntx("batchsettings");

	int max_vert = 65001;
	if(get_batchmax.read_file_success) {
		max_vert = get_batchmax.get_int("maxvert")[0];
	}
	else {
		get_batchmax.set_int("maxvert", { 65000 });
		get_batchmax.save_file(get_batchmax, "batchsettings");
	}

	m_batcher_renderer = std::make_unique<batcher>(max_vert);
}

void wizm::runtime_manager::render_runtime(float delta_time, std::shared_ptr<core_gl_shader> m_shader)
{
	if (engine_status == RUNTIME_STATUS)
	{

		if (!initiated_runtime) {
			init_runtime();
			initiated_runtime = true;
		}

		m_batcher_renderer->render(m_shader);
	}
	else
	{
		initiated_runtime = false;
	}
}

void wizm::runtime_manager::init_runtime()
{
	add_console_line("BATCHED", CONSOLE_SUCCESS_LOG);
	m_batcher_renderer->bacth();
}

void wizm::runtime_manager::exit_runtime()
{
}
