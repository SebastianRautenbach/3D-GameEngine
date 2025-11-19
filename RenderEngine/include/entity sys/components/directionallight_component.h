#pragma once
#include "light_component.h"

using namespace lowlevelsys;

namespace wizm {
    class directionallight_component : public light_component {
    public:
        directionallight_component(
            glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f)
        );
        ~directionallight_component();

        void component_preupdate();
        void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader);
        void component_postupdate();


        glm::mat4 computeLightSpaceMatrix(
            const glm::vec3& scene_center,
            float scene_radius,
            float ortho_size,
            float near_plane,
            float far_plane);

        core_component* _copy() const;


        //////////////////////////////////////////////////
        //////////  SERIALIZATION METHOD /////////////////
        //////////////////////////////////////////////////


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
        
            m_ambient = glm::vec3(save_t[index]["light"].get_float("m_ambient")[0],
                save_t[index]["light"].get_float("m_ambient")[1],
                save_t[index]["light"].get_float("m_ambient")[2]
            );

            m_diffuse = glm::vec3(save_t[index]["light"].get_float("m_diffuse")[0],
                save_t[index]["light"].get_float("m_diffuse")[1],
                save_t[index]["light"].get_float("m_diffuse")[2]
            );

            m_specular = glm::vec3(save_t[index]["light"].get_float("m_specular")[0],
                save_t[index]["light"].get_float("m_specular")[1],
                save_t[index]["light"].get_float("m_specular")[2]
            );
        

            m_brightness = save_t[index]["light"].get_float("m_brightness")[0];
        };


        void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {

            save_t["directionallight" + index]["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
            save_t["directionallight" + index]["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
            save_t["directionallight" + index]["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });

            save_t["directionallight" + index]["light"].set_float("m_ambient", { m_ambient.x,m_ambient.y,m_ambient.z });
            save_t["directionallight" + index]["light"].set_float("m_diffuse", { m_diffuse.x,m_diffuse.y,m_diffuse.z });
            save_t["directionallight" + index]["light"].set_float("m_specular", { m_specular.x,m_specular.y,m_specular.z });

            save_t["directionallight" + index]["light"].set_float("m_brightness", { m_brightness });

 

        }
    };
}