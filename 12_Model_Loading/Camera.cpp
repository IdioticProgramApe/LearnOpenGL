#include "camera.h"


void Camera::updateCameraVectors()
{
	// calculate the new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_front = glm::normalize(front);

	// also re calculate the right and up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: m_position(position)
	, m_worldUp(up)
	, m_yaw(yaw)
	, m_pitch(pitch)
{
	updateCameraVectors();
}

// constructor with scalars
Camera::Camera(
	float posX, float posY, float posZ,
	float upX, float upY, float upZ,
	float yaw, float pitch
) : m_position(glm::vec3(posX, posY, posZ))
  , m_worldUp(glm::vec3(upX, upY, upZ))
  , m_yaw(yaw)
  , m_pitch(pitch)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = m_movementSpeed * deltaTime;
	switch (direction)
	{
	case CameraMovement::FORWARD:
		m_position += velocity * m_front;
		break;
	case CameraMovement::BACKWARD:
		m_position -= velocity * m_front;
		break;
	case CameraMovement::LEFT:
		m_position -= velocity * m_right;
		break;
	case CameraMovement::RIGHT:
		m_position += velocity * m_right;
		break;
	case CameraMovement::UP:
		m_position += velocity * m_worldUp;
		break;
	case CameraMovement::DOWN:
		m_position -= velocity * m_worldUp;
		break;
	default:
		break;
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw   += xoffset;
	m_pitch += yoffset;

	if (m_constrainPitch)
	{
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
	{
		m_zoom -= yoffset;
	}
	else if (m_zoom <= 1.0f)
	{
		m_zoom = 1.0f;
	}
	else if (m_zoom >= 45.0f)
	{
		m_zoom = 45.0f;
	}
}

void Camera::setConstrainPitchStatus(bool constrainPitch)
{
	m_constrainPitch = constrainPitch;
}

const float Camera::getZoom() const
{
	return m_zoom;
}

const glm::vec3 Camera::getPosition() const
{
	return m_position;
}

const glm::vec3 Camera::getFront() const
{
	return m_front;
}
