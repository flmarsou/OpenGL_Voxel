#include "Camera.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Camera::Init(const u32 width, const u32 height, const glm::vec3 pos)
{
	this->width = width;
	this->height = height;
	this->Position = pos;
}

// ========================================================================== //
//    Getters                                                                 //
// ========================================================================== //

u8		Camera::GetFacingDirection() const
{
	return (this->_facing);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Camera::Matrix(float nearPlane, float farPlane, u32 shader, const char *uniform)
{
	this->View = glm::lookAt(
		this->Position,
		this->Position + this->Direction,
		this->Angle
	);

	this->Proj = glm::perspective(
		glm::radians(CAMERA_FOV),
		this->width / this->height,
		nearPlane,
		farPlane
	);

	glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(this->Proj * this->View));

	if (fabs(this->Direction.x) > fabs(this->Direction.z))
		if (this->Direction.x > 0)
			this->_facing = EAST;
		else
			this->_facing = WEST;
	else
		if (this->Direction.z > 0)
			this->_facing = SOUTH;
		else
			this->_facing = NORTH;
}

void	Camera::Input(GLFWwindow *window)
{
	// --- Positional Movements ---
	const glm::vec3	zMovement = glm::normalize(glm::vec3(Direction.x, 0.0f, Direction.z));
	const glm::vec3	xMovement = glm::normalize(glm::cross(zMovement, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Foward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->Position += CAMERA_SPEED * zMovement;

	// Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->Position -= CAMERA_SPEED * xMovement;

	// Backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->Position -= CAMERA_SPEED * zMovement;

	// Left
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->Position += CAMERA_SPEED * xMovement;

	// Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->Position += CAMERA_SPEED * glm::vec3(0.0f, 1.0f, 0.0f);

	// Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		this->Position += CAMERA_SPEED * glm::vec3(0.0f, -1.0f, 0.0f);

	// --- Directional Movement ---
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		double	mouseX;
		double	mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float	rotX = CAMERA_SENSITIVITY * (float)(mouseY - this->height / 2);
		float	rotY = CAMERA_SENSITIVITY * (float)(mouseX - this->width / 2);

		this->_yaw += rotY;
		this->_pitch -= rotX;

		if (this->_pitch > 89.0f)
			this->_pitch = 89.0f;
		if (this->_pitch < -89.0f)
			this->_pitch = -89.0f;

		glm::vec3	newDirection;
		newDirection.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
		newDirection.y = sin(glm::radians(this->_pitch));
		newDirection.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));

		this->Direction = glm::normalize(newDirection);

		glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}
}
