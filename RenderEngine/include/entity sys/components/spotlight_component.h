#pragma once
#include "light_component.h"

using namespace lowlevelsys;

namespace wizm {
    class spotlight_component : public light_component {
    public:
        spotlight_component(
            float constant = 1.0f,
            float linear = 0.09,
            float quadratic = 0.032f,
            float cutOff = glm::cos(glm::radians(10.0f)),
            float outerCutOff = glm::cos(glm::radians(15.0f)),
            float distance = 10,
            glm::vec3 ambient = glm::vec3(0.00f, 0.00f, 0.0f),
            glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f)
        );

        ~spotlight_component() {
            
        }

        void component_preupdate();
        void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader);
        void component_postupdate();
        core_component* _copy() const;



        //////////////////////////////////////////////////
        //////////  SERIALIZATION METHOD /////////////////
        //////////////////////////////////////////////////


        void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override {
            (void)parent_name;
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

            m_constant = save_t[index]["light"].get_float("m_constant")[0];
            m_linear = save_t[index]["light"].get_float("linear")[0];
            m_quadratic = save_t[index]["light"].get_float("m_quadratic")[0];

            m_cutOff = save_t[index]["light"].get_float("m_cutOff")[0];
            m_outerCutOff = save_t[index]["light"].get_float("m_outerCutOff")[0];


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


            spotlightindex = save_t[index]["light"].get_string("spotlightindex")[0];

            m_distance = save_t[index]["light"].get_float("m_distance")[0];

            m_brightness = save_t[index]["light"].get_float("m_brightness")[0];

            light_index = static_cast<unsigned int>(save_t[index]["light"].get_int("light_index")[0]);
        }


        void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {
            save_t["spotlight" + index]["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
            save_t["spotlight" + index]["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
            save_t["spotlight" + index]["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });
                                 
            save_t["spotlight" + index]["light"].set_float("m_constant", { m_constant });
            save_t["spotlight" + index]["light"].set_float("linear", { m_linear });
            save_t["spotlight" + index]["light"].set_float("m_quadratic", { m_quadratic });

            save_t["spotlight" + index]["light"].set_float("m_outerCutOff", { m_outerCutOff });
            save_t["spotlight" + index]["light"].set_float("m_cutOff", { m_cutOff });
                                 
            save_t["spotlight" + index]["light"].set_float("m_ambient", { m_ambient.x,m_ambient.y,m_ambient.z });
            save_t["spotlight" + index]["light"].set_float("m_diffuse", { m_diffuse.x,m_diffuse.y,m_diffuse.z });
            save_t["spotlight" + index]["light"].set_float("m_specular", { m_specular.x,m_specular.y,m_specular.z });
            save_t["spotlight" + index]["light"].set_float("m_distance", { m_distance });
            save_t["spotlight" + index]["light"].set_float("m_brightness", { m_brightness });
            save_t["spotlight" + index]["light"].set_string("spotlightindex", { spotlightindex });
            save_t["spotlight" + index]["light"].set_int("light_index", { static_cast<int>(light_index) });
        }


    public:
        unsigned int light_index = 0;

        float m_constant;
        float m_linear;
        float m_quadratic;

        glm::vec3 m_direction;
        float m_cutOff;
        float m_outerCutOff;
        float m_distance;



        std::string spotlightindex;



    };
}