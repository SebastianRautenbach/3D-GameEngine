#pragma once
#include "component_types.h"
#include <glm/glm.hpp>
#include "system/node.h"
#include "system/tag.h"
#include "gl core/core shader.h"
#include <memory>

using namespace wizm;

namespace lowlevelsys {


	class core_component : public core_node
	{
		
	public:

		virtual void component_preupdate() = 0;
		virtual void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader) = 0;
		virtual void component_postupdate() = 0;
		virtual core_component* _copy() const = 0;
		
		void remove_component();
		
		core_component();
		~core_component() override;

		


	public:

		wizm::eCompType m_component_type;

		bool m_is_active;
		bool m_is_visible;

		bool m_is_dirty = true;
		
		wizm::core_tag* component_tags;
	};
}