#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

// define several possible options for camera movement
enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// default camera values
namespace CameraDefaultValues
{
	constexpr const float YAW         { -90.0f };
	constexpr const float PITCH       { 0.0f };
	constexpr const float SPEED       { 2.5f };
	constexpr const float SENSITIVITY { 0.1f };
	constexpr const float ZOOM        { 45.0f };  // for fov
};

class Camera
{
private:
	void updateCameraVectors();

private:
	// camera attributes
	glm::vec3 m_position { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_worldUp  { 0.0f, 1.0f, 0.0f };
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	// Euler angles
	float m_yaw              = CameraDefaultValues::YAW;
	float m_pitch            = CameraDefaultValues::PITCH;

	// camera options
	float m_movementSpeed    = CameraDefaultValues::SPEED;
	float m_mouseSensitivity = CameraDefaultValues::SENSITIVITY;
	float m_zoom             = CameraDefaultValues::ZOOM;

	bool m_constrainPitch    = true;

public:
	// constructor with default values
	Camera() { updateCameraVectors(); }

	// constructor with vectors
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

	// constructor with scalars
	Camera(float posX, float posY, float posZ,
		   float upX, float upY, float upZ,
		   float yaw, float pitch);

	virtual ~Camera() = default;

public:
	// return the view matrix calculated using Euler angles and the lookAt Matrix
	glm::mat4 getViewMatrix();

	// process input received from any keyboard-like input system
	// accepts input parameter in the form of camera defined ENUM
	void processKeyboard(CameraMovement direction, float deltaTime);

	// process input received from a mouse input system.
	// expect the offset value in both the x and y direction
	void processMouseMovement(float xoffset, float yoffset);

	// process input received from a mouse scroll-wheel event
	// only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);

	void setConstrainPitchStatus(bool constrainPitch);

public:
	const float     getZoom()     const;
	const glm::vec3 getPosition() const;
	const glm::vec3 getFront()    const;

};

#endif // !CAMERA_H