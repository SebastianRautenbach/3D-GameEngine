#pragma once

#ifndef SCRIPTING_FUNCTIONS
#define SCRIPTING_FUNCTIONS

#include "angelscript/include/angelscript.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include <string>
#include <iostream>
#include "scene.h"
#include "system/asset_importer.h"
#include "system/input_manager.h"
#include "system/scripting/scripting_objects.h"

/*
	These following MACROS are copied from the hpl1 engine on github https://github.com/FrictionalGames/HPL1Engine/blob/5e441bbb247a7473e75cc0f05ca8a5b62c6ec64c/include/system/Script.h#L42
	I just like the way they did it and I am also to lazy XD
*/

#ifdef __GNUC__
#ifdef __ppc__
#define __stdcall
#else
#define __stdcall __attribute__((stdcall))
#endif
#endif

#define SCRIPT_DEFINE_FUNC(return, funcname) \
			SCRIPT_FUNC_BEGIN(funcname, return, "") \
			SCRIPT_RETURN_CALL_##return funcname(); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_1(return, funcname, arg0) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_2(return, funcname, arg0, arg1) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_3(return, funcname, arg0, arg1, arg2) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_4(return, funcname, arg0, arg1, arg2, arg3) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_5(return, funcname, arg0, arg1, arg2, arg3, arg4) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_6(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_7(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_8(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6","#arg7) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_ARG_##arg7(7); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_9(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6","#arg7","#arg8) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_ARG_##arg7(7); \
			SCRIPT_ARG_##arg8(8); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8); \
			SCRIPT_FUNC_END(funcname,return)

#define SCRIPT_DEFINE_FUNC_10(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6","#arg7","#arg8","#arg9) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_ARG_##arg7(7); \
			SCRIPT_ARG_##arg8(8); \
			SCRIPT_ARG_##arg9(9); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6,_arg7,_arg8,_arg9); \
			SCRIPT_FUNC_END(funcname,return)

#define SCRIPT_DEFINE_FUNC_12(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6, \
				arg7, arg8, arg9, arg10, arg11) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6 \
				","#arg7","#arg8","#arg9","#arg10","#arg11) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_ARG_##arg7(7); \
			SCRIPT_ARG_##arg8(8); \
			SCRIPT_ARG_##arg9(9); \
			SCRIPT_ARG_##arg10(10); \
			SCRIPT_ARG_##arg11(11); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6, \
					_arg7,_arg8,_arg9,_arg10,_arg11); \
			SCRIPT_FUNC_END(funcname,return)
#define SCRIPT_DEFINE_FUNC_17(return, funcname, arg0, arg1, arg2, arg3, arg4, arg5, arg6, \
				arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16) \
			SCRIPT_FUNC_BEGIN(funcname, return, #arg0","#arg1","#arg2","#arg3","#arg4","#arg5","#arg6 \
				","#arg7","#arg8","#arg9","#arg10","#arg11","#arg12","#arg13","#arg14","#arg15","#arg16) \
			SCRIPT_ARG_##arg0(0); \
			SCRIPT_ARG_##arg1(1); \
			SCRIPT_ARG_##arg2(2); \
			SCRIPT_ARG_##arg3(3); \
			SCRIPT_ARG_##arg4(4); \
			SCRIPT_ARG_##arg5(5); \
			SCRIPT_ARG_##arg6(6); \
			SCRIPT_ARG_##arg7(7); \
			SCRIPT_ARG_##arg8(8); \
			SCRIPT_ARG_##arg9(9); \
			SCRIPT_ARG_##arg10(10); \
			SCRIPT_ARG_##arg11(11); \
			SCRIPT_ARG_##arg12(12); \
			SCRIPT_ARG_##arg13(13); \
			SCRIPT_ARG_##arg14(14); \
			SCRIPT_ARG_##arg15(15); \
			SCRIPT_ARG_##arg16(16); \
			SCRIPT_RETURN_CALL_##return funcname(_arg0,_arg1,_arg2,_arg3,_arg4,_arg5,_arg6, \
					_arg7,_arg8,_arg9,_arg10,_arg11,_arg12,_arg13,_arg14,_arg15,_arg16); \
			SCRIPT_FUNC_END(funcname,return)

#define SCRIPT_FUNC_BEGIN(funcname, return, args) \
		namespace GenericScript { \
			static std::string funcname##_return = #return; \
			static std::string funcname##_arg = args; \
			void funcname##_Generic(asIScriptGeneric *gen) { \
				SCRIPT_RETURN_##return;

#define SCRIPT_FUNC_END(funcname, return) \
				SCRIPT_SET_RETURN_##return; \
			} \
		}

// Parameter Macros
#define SCRIPT_ARG_string(n) std::string _arg##n = *(std::string *)gen->GetArgObject(n)
#define SCRIPT_ARG_float(n) float _arg##n = gen->GetArgFloat(n)
#define SCRIPT_ARG_int(n) int _arg##n = gen->GetArgDWord(n)
#define SCRIPT_ARG_bool(n) bool _arg##n = gen->GetArgDWord(n)
#define SCRIPT_ARG_vec3(n) wizm_script::vec3 _arg##n = *(wizm_script::vec3*)gen->GetArgObject(n)
#define SCRIPT_ARG_vec2(n) wizm_script::vec2 _arg##n = *(wizm_script::vec2*)gen->GetArgObject(n)

// Return Value Macros
#define SCRIPT_RETURN_string std::string _ret
#define SCRIPT_RETURN_CALL_string _ret =
#define SCRIPT_SET_RETURN_string gen->SetReturnObject(&_ret)

#define SCRIPT_RETURN_vec3 wizm_script::vec3 _ret
#define SCRIPT_RETURN_CALL_vec3 _ret = 
#define SCRIPT_SET_RETURN_vec3 gen->SetReturnObject(&_ret)


#define SCRIPT_RETURN_vec2 wizm_script::vec2 _ret
#define SCRIPT_RETURN_CALL_vec2 _ret = 
#define SCRIPT_SET_RETURN_vec2 gen->SetReturnObject(&_ret)


#define SCRIPT_RETURN_bool bool _ret
#define SCRIPT_RETURN_CALL_bool _ret =
#define SCRIPT_SET_RETURN_bool gen->SetReturnDWord(_ret ? -1 : 0)

#define SCRIPT_RETURN_int int _ret
#define SCRIPT_RETURN_CALL_int _ret =
#define SCRIPT_SET_RETURN_int gen->SetReturnDWord(_ret)
#define SCRIPT_RETURN_float float _ret
#define SCRIPT_RETURN_CALL_float _ret =
#define SCRIPT_SET_RETURN_float gen->SetReturnFloat(_ret)
#define SCRIPT_RETURN_void
#define SCRIPT_RETURN_CALL_void
#define SCRIPT_SET_RETURN_void

#if defined(AS_MAX_PORTABILITY)
#define SCRIPT_REGISTER_FUNC(funcname) \
		GenericScript::funcname##_return+" "#funcname" ("+GenericScript::funcname##_arg+")", (void *)GenericScript::funcname##_Generic, asCALL_GENERIC
#else
#define SCRIPT_REGISTER_FUNC(funcname) \
		GenericScript::funcname##_return+" "#funcname" ("+GenericScript::funcname##_arg+")", (void *)funcname, asCALL_STDCALL
#endif


namespace engine_scripting
{
	wizm::core_scene* global_scene;
	
	
	static void print(std::string& msg, int lvl) {
		add_console_line(msg, lvl);
	}
	SCRIPT_DEFINE_FUNC_2(void, print, string, int);


	//------------------------------------------------- STATIC INPUT
	//--------------------------------------------------------------

	static bool has_key_pressed(int key) {
		return global_input_manager->has_key_been_pressed(key);
	}
	SCRIPT_DEFINE_FUNC_1(bool, has_key_pressed, int);

	static wizm_script::vec2 get_mouse_pos() {
		double x, y;
		global_input_manager->get_mouse_pos(x, y);
		return wizm_script::vec2(x,y);
	}
	SCRIPT_DEFINE_FUNC(vec2, get_mouse_pos);

	
	//-------------------------------------------------- STATIC MESH
	//--------------------------------------------------------------
	
	static void change_mesh(std::string entity_guid, int component_index, std::string asset_id) {
		auto entity = global_scene->get_entity_by_id(entity_guid);

		if (entity->m_components_list.size() > component_index) {
			auto sm_comp = dynamic_cast<staticmesh_component*>(entity->m_components_list[component_index]);
			sm_comp->m_mesh_asset_id = asset_id;
		}
	}
	SCRIPT_DEFINE_FUNC_3(void, change_mesh, string, int, string);

	//--------------------------------------------------	  ENTITY
	//--------------------------------------------------------------

	void destroy_entity(std::string entity_guid) {
		global_scene->delete_entity(global_scene->get_entity(entity_guid));
	}
	SCRIPT_DEFINE_FUNC_1(void, destroy_entity,string);


	//--------------------------------------------------    MATERIAL
	//--------------------------------------------------------------


	void change_material(std::string entity_guid, int component_index, std::string asset_id, int material_index) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		if (entity->m_components_list.size() > component_index) {
			auto sm_comp = dynamic_cast<staticmesh_component*>(entity->m_components_list[component_index]);
			
			if(sm_comp->m_material_asset_ids.size() > material_index)
			{
				sm_comp->m_material_asset_ids[material_index] = asset_id;				
			}
		}

	}
	SCRIPT_DEFINE_FUNC_4(void, change_material, string, int, string, int);
	
	//--------------------------------------------------   TRANSFORM
	//--------------------------------------------------------------
	// entity
	
	static void set_entity_rotation(std::string entity_guid, float p, float y, float r) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->set_rotation(glm::vec3(p, y, r));
	}
	SCRIPT_DEFINE_FUNC_4(void, set_entity_rotation, string, float, float, float);
	
	//-----------------------------------------------------------------------
	
	static void add_entity_rotation(std::string entity_guid, float p, float y, float r) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->add_rotation(glm::vec3(p, y, r));
	}
	SCRIPT_DEFINE_FUNC_4(void, add_entity_rotation, string, float, float, float);


	//-----------------------------------------------------------------------

	static wizm_script::vec3 get_entity_rotation(const std::string entity_guid) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		if (entity)
			return wizm_script::vec3(entity->get_rotation().x, entity->get_rotation().y, entity->get_rotation().z);
		else
			return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_1(vec3, get_entity_rotation, string);
	
	//-----------------------------------------------------------------------
	
	
	static void set_entity_position(std::string entity_guid, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->set_position(glm::vec3(x, y, z));
	}
	SCRIPT_DEFINE_FUNC_4(void, set_entity_position, string, float, float, float);
	
	//-----------------------------------------------------------------------
	
	static void add_entity_position(std::string entity_guid, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->add_position(glm::vec3(x, y, z));
	}
	SCRIPT_DEFINE_FUNC_4(void, add_entity_position, string, float, float, float);


	//-----------------------------------------------------------------------

	static wizm_script::vec3 get_entity_position(const std::string entity_guid) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		if (entity)
			return wizm_script::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z);
		else
			return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_1(vec3, get_entity_position, string);
	
	
	//-----------------------------------------------------------------------
	
	
	static void set_entity_scale(std::string entity_guid, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->set_scale(glm::vec3(x, y, z));
	}
	SCRIPT_DEFINE_FUNC_4(void, set_entity_scale, string, float, float, float);
	
	//-----------------------------------------------------------------------
	
		
	static void add_entity_scale(std::string entity_guid, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		entity->add_scale(glm::vec3(x, y, z));
	}
	SCRIPT_DEFINE_FUNC_4(void, add_entity_scale, string, float, float, float);


	//-----------------------------------------------------------------------

	static wizm_script::vec3 get_entity_scale(const std::string entity_guid) {
		auto entity = global_scene->get_entity_by_id(entity_guid);
		if (entity)
			return wizm_script::vec3(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
		else
			return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_1(vec3, get_entity_scale, string);

	//-----------------------------------------------------------------------

	
	// component
	
	static void set_component_rotation(std::string entity_id, int component_index, float p, float y, float r) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->set_rotation(glm::vec3(p, y, r));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, set_component_rotation, string, int, float, float, float);
	
	//-----------------------------------------------------------------------
	
	static void add_component_rotation(std::string entity_id, int component_index, float p, float y, float r) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->add_rotation(glm::vec3(p, y, r));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, add_component_rotation, string, int, float, float, float);
	
	//-----------------------------------------------------------------------


	static wizm_script::vec3 get_component_rotation(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_rotation();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_rotation, string, int);


	static wizm_script::vec3 get_component_world_rotation(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_world_rotation();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_world_rotation, string, int);


	//-----------------------------------------------------------------------
	
	
	static void set_component_position(std::string entity_id, int component_index, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->set_position(glm::vec3(x, y, z));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, set_component_position, string, int, float, float, float);
	
	
	//-----------------------------------------------------------------------
	
	
	static void add_component_position(std::string entity_id, int component_index, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->add_position(glm::vec3(x, y, z));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, add_component_position, string, int, float, float, float);
	
	
	//-----------------------------------------------------------------------

	static wizm_script::vec3 get_component_position(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_position();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_position, string, int);


	static wizm_script::vec3 get_component_world_position(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_world_position();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_world_position, string, int);
	
	
	//-----------------------------------------------------------------------
	
	
	static void set_component_scale(std::string entity_id, int component_index, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->set_scale(glm::vec3(x, y, z));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, set_component_scale, string, int, float, float, float);
	
	
	//-----------------------------------------------------------------------
	
	static void add_component_scale(std::string entity_id, int component_index, float x, float y, float z) {
		auto entity = global_scene->get_entity_by_id(entity_id);
	
		if (entity->m_components_list.size() > component_index) {
			auto comp = entity->m_components_list[component_index];
			comp->add_scale(glm::vec3(x, y, z));
		}
	}
	SCRIPT_DEFINE_FUNC_5(void, add_component_scale, string, int, float, float, float);

	//-----------------------------------------------------------------------


	static wizm_script::vec3 get_component_scale(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_scale();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_scale, string, int);


	static wizm_script::vec3 get_component_world_scale(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);
		if (entity)
		{
			if (component_index < entity->m_components_list.size()) {
				const auto& vec = entity->m_components_list[0]->get_world_scale();
				return wizm_script::vec3(vec.x, vec.y, vec.z);
			}
		}

		return wizm_script::vec3();
	}
	SCRIPT_DEFINE_FUNC_2(vec3, get_component_world_scale, string, int);


	//-----------------------------------------------------------------------

	
	//--------------------------------------------------	  LIGHTS
	//--------------------------------------------------------------

	// I needa fix this
	void set_light_brightness(const std::string entity_id, int component_index, float brightness) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto light_comp = dynamic_cast<light_component*>(entity->m_components_list[component_index]);
			if(light_comp)
				light_comp->m_brightness = brightness;
		}
	}
	SCRIPT_DEFINE_FUNC_3(void, set_light_brightness, string, int, float);


	void set_light_ambient(const std::string entity_id, int component_index,wizm_script::vec3 ambient) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto light_comp = dynamic_cast<light_component*>(entity->m_components_list[component_index]);
			if (light_comp)
				light_comp->m_ambient = glm::vec3(ambient.x, ambient.y, ambient.z);
		}
	}
	SCRIPT_DEFINE_FUNC_3(void, set_light_ambient, string, int, vec3);



	//-----------------------------------------------------------------------


	//--------------------------------------------------	  SOUNDS
	//--------------------------------------------------------------

	void play_component_sound(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto s_comp = dynamic_cast<sound_component*>(entity->m_components_list[component_index]);
			if(s_comp)
			{
				//s_comp->stop();
				s_comp->play();
			}
		}
	}
	SCRIPT_DEFINE_FUNC_2(void, play_component_sound, string, int);

	//-----------------------------------------------------------------------

	void stop_component_sound(const std::string entity_id, int component_index) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto s_comp = dynamic_cast<sound_component*>(entity->m_components_list[component_index]);
			if (s_comp)
				s_comp->stop();
		}
	}
	SCRIPT_DEFINE_FUNC_2(void, stop_component_sound, string, int);

	//-----------------------------------------------------------------------

	void set_component_sound_loop(const std::string entity_id, int component_index, bool should_loop) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto s_comp = dynamic_cast<sound_component*>(entity->m_components_list[component_index]);
			if (s_comp)
				s_comp->m_loop = should_loop;
		}
	}
	SCRIPT_DEFINE_FUNC_3(void, set_component_sound_loop, string, int, bool);

	//-----------------------------------------------------------------------

	void set_component_sound_3D(const std::string entity_id, int component_index, bool is_3d) {
		auto entity = global_scene->get_entity_by_id(entity_id);

		if (entity->m_components_list.size() > component_index) {
			auto s_comp = dynamic_cast<sound_component*>(entity->m_components_list[component_index]);
			if (s_comp)
				s_comp->m_is3d = is_3d;
		}
	}
	SCRIPT_DEFINE_FUNC_3(void, set_component_sound_3D, string, int, bool);


	//-----------------------------------------------------------------------



bool add_script_func(asIScriptEngine* script_engine, const std::string& func_decl, void* function, int call_conv) {

	if (script_engine->RegisterGlobalFunction(func_decl.c_str(),
		asFUNCTION(function), call_conv) < 0)
	{
		std::cerr << "Couldn't add func: " << func_decl.c_str() << "\n";
		return false;
	}

	return true;
}

class scripting_functions {
public:
	
	void init_scripts(asIScriptEngine* script_engine, wizm::core_scene* scene) {
		if (scene == NULL) {
			//stop
			return;
		}
		global_scene = scene;

		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(print));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(has_key_pressed));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_mouse_pos));

		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(change_mesh));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(change_material));

		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(destroy_entity));

		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_entity_rotation));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_entity_rotation));
		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_entity_position));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_entity_position));
		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_entity_scale));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_entity_scale));
		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_component_rotation));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_component_rotation));
		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_component_position));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_component_position));
		
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_component_scale));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(add_component_scale));

		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_entity_rotation));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_entity_position));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_entity_scale));


		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_rotation));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_position));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_scale));

		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_world_rotation));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_world_position));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(get_component_world_scale));


		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_light_brightness));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_light_ambient));


		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(play_component_sound));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(stop_component_sound));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_component_sound_loop));
		add_script_func(script_engine, SCRIPT_REGISTER_FUNC(set_component_sound_3D));


	}
	

	void init_variables(asIScriptEngine* script_engine) {
		//---------------------------------------------------------------------------------------------------- ENUMS
		//----------------------------------------------------------------------------------------------------------

		script_engine->RegisterEnum("texture_codes");
		script_engine->RegisterEnumValue("texture_codes", "eDiffuseTex", 0);
		script_engine->RegisterEnumValue("texture_codes", "eSpecularTex", 1);

		script_engine->RegisterEnum("key_codes");
		script_engine->RegisterEnumValue("key_codes", "eKEY_W", eKEY_W);
		script_engine->RegisterEnumValue("key_codes", "eKEY_S", eKEY_S);
		script_engine->RegisterEnumValue("key_codes", "eKEY_A", eKEY_A);
		script_engine->RegisterEnumValue("key_codes", "eKEY_D", eKEY_D);
		script_engine->RegisterEnumValue("key_codes", "eKEY_Q", eKEY_Q);
		script_engine->RegisterEnumValue("key_codes", "eKEY_E", eKEY_E);
		script_engine->RegisterEnumValue("key_codes", "eKEY_R", eKEY_R);
		script_engine->RegisterEnumValue("key_codes", "eKEY_T", eKEY_T);
		script_engine->RegisterEnumValue("key_codes", "eKEY_Y", eKEY_Y);
		script_engine->RegisterEnumValue("key_codes", "eKEY_U", eKEY_U);
		script_engine->RegisterEnumValue("key_codes", "eKEY_I", eKEY_I);
		script_engine->RegisterEnumValue("key_codes", "eKEY_O", eKEY_O);
		script_engine->RegisterEnumValue("key_codes", "eKEY_P", eKEY_P);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F", eKEY_F);
		script_engine->RegisterEnumValue("key_codes", "eKEY_G", eKEY_G);
		script_engine->RegisterEnumValue("key_codes", "eKEY_H", eKEY_H);
		script_engine->RegisterEnumValue("key_codes", "eKEY_J", eKEY_J);
		script_engine->RegisterEnumValue("key_codes", "eKEY_K", eKEY_K);
		script_engine->RegisterEnumValue("key_codes", "eKEY_L", eKEY_L);
		script_engine->RegisterEnumValue("key_codes", "eKEY_Z", eKEY_Z);
		script_engine->RegisterEnumValue("key_codes", "eKEY_X", eKEY_X);
		script_engine->RegisterEnumValue("key_codes", "eKEY_C", eKEY_C);
		script_engine->RegisterEnumValue("key_codes", "eKEY_V", eKEY_V);
		script_engine->RegisterEnumValue("key_codes", "eKEY_B", eKEY_B);
		script_engine->RegisterEnumValue("key_codes", "eKEY_N", eKEY_N);
		script_engine->RegisterEnumValue("key_codes", "eKEY_M", eKEY_M);
		script_engine->RegisterEnumValue("key_codes", "eKEY_SPACE", eKEY_SPACE);
		script_engine->RegisterEnumValue("key_codes", "eKEY_ENTER", eKEY_ENTER);
		script_engine->RegisterEnumValue("key_codes", "eKEY_ESCAPE", eKEY_ESCAPE);
		script_engine->RegisterEnumValue("key_codes", "eKEY_LEFT", eKEY_LEFT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_RIGHT", eKEY_RIGHT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_UP", eKEY_UP);
		script_engine->RegisterEnumValue("key_codes", "eKEY_DOWN", eKEY_DOWN);
		script_engine->RegisterEnumValue("key_codes", "eKEY_TAB", eKEY_TAB);
		script_engine->RegisterEnumValue("key_codes", "eKEY_BACKSPACE", eKEY_BACKSPACE);
		script_engine->RegisterEnumValue("key_codes", "eKEY_DELETE", eKEY_DELETE);
		script_engine->RegisterEnumValue("key_codes", "eKEY_LEFT_SHIFT", eKEY_LEFT_SHIFT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_RIGHT_SHIFT", eKEY_RIGHT_SHIFT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_LEFT_CTRL", eKEY_LEFT_CTRL);
		script_engine->RegisterEnumValue("key_codes", "eKEY_RIGHT_CTRL", eKEY_RIGHT_CTRL);
		script_engine->RegisterEnumValue("key_codes", "eKEY_LEFT_ALT", eKEY_LEFT_ALT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_RIGHT_ALT", eKEY_RIGHT_ALT);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F1", eKEY_F1);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F2", eKEY_F2);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F3", eKEY_F3);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F4", eKEY_F4);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F5", eKEY_F5);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F6", eKEY_F6);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F7", eKEY_F7);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F8", eKEY_F8);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F9", eKEY_F9);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F10", eKEY_F10);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F11", eKEY_F11);
		script_engine->RegisterEnumValue("key_codes", "eKEY_F12", eKEY_F12);
		script_engine->RegisterEnumValue("key_codes", "eKEY_0", eKEY_0);
		script_engine->RegisterEnumValue("key_codes", "eKEY_1", eKEY_1);
		script_engine->RegisterEnumValue("key_codes", "eKEY_2", eKEY_2);
		script_engine->RegisterEnumValue("key_codes", "eKEY_3", eKEY_3);
		script_engine->RegisterEnumValue("key_codes", "eKEY_4", eKEY_4);
		script_engine->RegisterEnumValue("key_codes", "eKEY_5", eKEY_5);
		script_engine->RegisterEnumValue("key_codes", "eKEY_6", eKEY_6);
		script_engine->RegisterEnumValue("key_codes", "eKEY_7", eKEY_7);
		script_engine->RegisterEnumValue("key_codes", "eKEY_8", eKEY_8);
		script_engine->RegisterEnumValue("key_codes", "eKEY_9", eKEY_9);


		//----------------------------------------------------------------------------------------------------- VEC2
		//----------------------------------------------------------------------------------------------------------

		script_engine->RegisterObjectType("vec2", sizeof(wizm_script::vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
		script_engine->RegisterObjectProperty("vec2", "float x", offsetof(wizm_script::vec2, x));
		script_engine->RegisterObjectProperty("vec2", "float y", offsetof(wizm_script::vec2, y)); 
		script_engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR([](void* memory) { new(memory) wizm_script::vec2(); }, (void*), void), asCALL_CDECL_OBJFIRST);
		script_engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTIONPR([](void* memory, float x, float y) { new(memory) wizm_script::vec2(x, y); }, (void*, float, float), void), asCALL_CDECL_OBJFIRST);

		//----------------------------------------------------------------------------------------------------- VEC3
		//----------------------------------------------------------------------------------------------------------

		script_engine->RegisterObjectType("vec3", sizeof(wizm_script::vec3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
		script_engine->RegisterObjectProperty("vec3", "float x", offsetof(wizm_script::vec3, x)); 
		script_engine->RegisterObjectProperty("vec3", "float y", offsetof(wizm_script::vec3, y)); 
		script_engine->RegisterObjectProperty("vec3", "float z", offsetof(wizm_script::vec3, z));
		script_engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR([](void* memory) { new(memory) wizm_script::vec3(); }, (void*), void), asCALL_CDECL_OBJFIRST);
		script_engine->RegisterObjectBehaviour("vec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTIONPR([](void* memory, float x, float y, float z) { new(memory) wizm_script::vec3(x, y, z); }, (void*, float, float, float), void), asCALL_CDECL_OBJFIRST);

	}	
};


}



#endif


