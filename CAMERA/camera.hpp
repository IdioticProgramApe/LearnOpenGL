#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// define several possible options for camera movement
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
const struct CameraValues
{
	const float YAW{ -90.0f };
	const float PITCH{ 0.0f };
	const float SPEED{ 2.5f };
	const float SENSITIVITY{ 0.1f };
	const float ZOOM{ 45.0f };  // for fov
} Cam;

class Camera
{
private:
	void updateCameraVectors()
	{
		// calculate the new front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.y = sin(glm::radians(Pitch));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);

		// also re calculate the right and up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
public:
	// camera attibutes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	float Yaw;
	float Pitch;

	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = Cam.YAW,
		float pitch = Cam.PITCH
	) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(Cam.SPEED),
		MouseSensitivity(Cam.SENSITIVITY),
		Zoom(Cam.ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalars
	Camera(
		float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw = Cam.YAW,
		float pitch = Cam.PITCH
	) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(Cam.SPEED),
		MouseSensitivity(Cam.SENSITIVITY),
		Zoom(Cam.ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// return the view matrix calculated using Euler angles and the lookAt Matrix
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
};



#endif // !CAMERA_H