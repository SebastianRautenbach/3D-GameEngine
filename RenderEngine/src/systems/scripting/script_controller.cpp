#include "system/scripting/script_controller.h"
#include "system/scripting/scripting_functions.h"
#include "other utils/common.h"
#include "scene.h"

void script_controller::init_script(wizm::core_scene* scene, std::string path)
{
	global_scene = scene;
	m_script_engine = asCreateScriptEngine();


	module_id = "module_" + generate_unique_id();


	m_script_engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
	RegisterStdString(m_script_engine);

	engine_scripting::scripting_functions sf;
	sf.init_variables(m_script_engine);
	sf.init_scripts(m_script_engine, global_scene);

	reload_script(path);
}

script_controller::~script_controller()
{
	m_script_engine->DiscardModule(module_id.c_str());
	m_context->Release();
}

bool script_controller::reload_script(std::string path)
{
	m_path = path;

	bool cont = true;

	CScriptBuilder builder;
	int r = builder.StartNewModule(m_script_engine, module_id.c_str());
	if (r < 0) {
		cont=0;
	}

	r = builder.AddSectionFromFile(path.c_str());
	if (r < 0) {
		cont = 0;
	}



	r = builder.BuildModule();
	if (r < 0) {
		cont = 0;
	}
	
	

	asIScriptModule* mod = m_script_engine->GetModule(module_id.c_str());
	m_onstart_func = mod->GetFunctionByDecl("void on_start()");
	m_onupdate_func = mod->GetFunctionByDecl("void on_update(float)");

	if (!m_onstart_func || !m_onupdate_func) {
		cont = 0;
	}

	m_context = m_script_engine->CreateContext();
	if (!m_context) {
		cont = 0;
	}

	if (cont)
		wizm::add_console_line("Successfully compiled scripts!", CONSOLE_SUCCESS_LOG);

	

	return cont;
}

void script_controller::reset()
{
	if (m_context) {
		m_context->Release();
		m_context = nullptr;
	}

	m_onstart_func = nullptr;
	m_onupdate_func = nullptr;

	m_script_engine->DiscardModule(module_id.c_str());


	m_script_engine = asCreateScriptEngine();


	module_id = "module_" + generate_unique_id();


	m_script_engine->SetMessageCallback(asFUNCTION(MessageCallback), nullptr, asCALL_CDECL);
	RegisterStdString(m_script_engine);

	engine_scripting::scripting_functions sf;
	sf.init_variables(m_script_engine);
	sf.init_scripts(m_script_engine, global_scene);

	reload_script(m_path);
}

void script_controller::on_start()
{
	if (m_context && m_onstart_func) {
		m_context->Prepare(m_onstart_func);
		int r = m_context->Execute();
		if (r != asEXECUTION_FINISHED) {

		}
	}
}

void script_controller::on_update(float delta_time)
{
	if (m_context && m_onupdate_func) {
		m_context->Prepare(m_onupdate_func);
		m_context->SetArgFloat(0, delta_time);
		int r = m_context->Execute();
		if (r != asEXECUTION_FINISHED) {

		}
	}
}

void script_controller::MessageCallback(const asSMessageInfo* msg)
{
	std::string error;
	error += msg->section;
	error += " ";
	error += msg->message;

	wizm::add_console_line(error, msg->type);
}
