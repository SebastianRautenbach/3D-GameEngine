#pragma once
#include "camera_manager.h"
#include "audio_manager.h"
#include "camera_3d.h"
#include "other utils/common.h"

namespace wizm {
	class audio_listener {

	public:
		audio_listener(std::shared_ptr<camera_manager> cammanager, audio_manager* audio_manager) :
			m_cam_manager(cammanager), m_audio_manager(audio_manager)
		{}

		void listener_on_update(float delta_time) {
			(void)delta_time;
			if(engine_status != EDITOR_STATUS)
			{
				auto pos = m_cam_manager->m_crnt_camera->get_position();
				auto up = m_cam_manager->m_crnt_camera->get_up_vector();
				auto forward = m_cam_manager->m_crnt_camera->get_forward_vector();
				
				listener_pos.x = pos.x;
				listener_pos.y = pos.y;
				listener_pos.z = pos.z;


				// reverse the UP vector because some reason this mixes the left and right hearing direction
				listener_up.x = -up.x;
				listener_up.y = -up.y;
				listener_up.z = -up.z;
				
				listener_forward.x = forward.x;
				listener_forward.y = forward.y;
				listener_forward.z = forward.z;


				m_audio_manager->system->set3DListenerAttributes(0, &listener_pos, &listener_velocity, &listener_forward, &listener_up);
			}
			
		}


	private:
		std::shared_ptr<camera_manager> m_cam_manager;
		audio_manager* m_audio_manager;

		FMOD_VECTOR listener_pos = { 0.0f, 0.0f, 0.0f };  
		FMOD_VECTOR listener_velocity = { 0.0f, 0.0f, 0.0f };  
		FMOD_VECTOR listener_forward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR listener_up = { 0.0f, 1.0f, 0.0f };

	};
}