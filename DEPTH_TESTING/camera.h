#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


// define possible camera movements
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW			= -90.0f;  // make sure the camera look at into screen
const float PITCH		=  0.0f;    
const float SPEED		=  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM		=  45.0f;   // the basic FOV value

class Camera
{
private:
	// calculate the front, the up and the right vector from updated euler angles
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		// always remember to normalize
		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up	= glm::normalize(glm::cross(m_right, m_front));
	}

public:
	// camera attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	// euler angles
	float m_yaw;
	float m_pitch;

	// camera options
	float m_movementSpeed;
	float m_mouseSensitivity;
	float m_zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)
		, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f)
		, float yaw = YAW
		, float pitch = PITCH)
		: m_front(glm::vec3(0.0f, 0.0f, -1.0f))
		, m_movementSpeed(SPEED)
		, m_mouseSensitivity(SENSITIVITY)
		, m_zoom(ZOOM)
	{
		m_position = position;
		m_worldUp = glm::normalize(worldUp);
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}

	// Constructor with scalars
	Camera(float posX, float posY, float posZ
		, float worldUpX, float worldUpY, float worldUpZ
		, float yaw
		, float pitch)
		: m_front(glm::vec3(0.0f, 0.0f, -1.0f))
		, m_movementSpeed(SPEED)
		, m_mouseSensitivity(SENSITIVITY)
		, m_zoom(ZOOM)
	{
		m_position = glm::vec3(posX, posY, posZ);
		m_worldUp = glm::normalize(glm::vec3(worldUpX, worldUpY, worldUpZ));
		m_yaw = yaw;
		m_pitch = pitch;
		updateCameraVectors();
	}
};




#endif // !CAMERA_H

