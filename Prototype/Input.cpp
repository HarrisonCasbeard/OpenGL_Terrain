#include <btBulletDynamicsCommon.h>
#include <glfw3.h>

#include "Input.h"
#include "Command.h"
#include "Camera.h"

Input::Input()
{
	m_upArrow	 = new ApplyRBForceCommand({  0.0f, 2000.0f, 0.0f });
	m_downArrow	 = new ApplyRBForceCommand({  0.0f,-2000.0f, 0.0f });
	m_leftArrow	 = new ApplyRBForceCommand({ -2000.0f, 0.0f, 0.0f });
	m_rightArrow = new ApplyRBForceCommand({  2000.0f, 0.0f, 0.0f });

	m_escKey = new GameCloseCommand();
}

Input::~Input()
{
	m_pCamera	 = nullptr;
}
//-------------------------------------------------------------------------------------------------
void Input::initialise(Camera& camera, const double winWidth, const double winHeight)
{
	m_mouseLastX = winWidth * 0.5f;
	m_mouseLastY = winHeight * 0.5f;

	m_pCamera = &camera;
}
//-------------------------------------------------------------------------------------------------
void Input::update(GLFWwindow* window, const double deltaTime, std::vector<Command*>& vCommands)
{
	if (!window)
		return;

	handleMouseInput(window, vCommands);
	handleKeyboardInput(window, deltaTime, vCommands);
}
//-------------------------------------------------------------------------------------------------
void Input::handleMouseInput(GLFWwindow* window, std::vector<Command*>& vCommands)
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	double xOffset = xPos - m_mouseLastX;
	double yOffset = m_mouseLastY - yPos;	//Reversed because up is positive
	m_mouseLastX = xPos;
	m_mouseLastY = yPos;

	//Dampen movement
	float sensitivity = 0.4f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_pCamera->addYaw((float)xOffset);
	m_pCamera->addPitch((float)yOffset);
	m_pCamera->updateFront();

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		vCommands.push_back(m_mouseClick);
}
//-------------------------------------------------------------------------------------------------
void Input::handleKeyboardInput(GLFWwindow* window, const double deltaTime, std::vector<Command*>& vCommands)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		vCommands.push_back(m_escKey);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		vCommands.push_back(m_rightArrow);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		vCommands.push_back(m_leftArrow);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		vCommands.push_back(m_upArrow);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		vCommands.push_back(m_downArrow);

	float cameraSpeed = (float)(m_pCamera->getSpeed() * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_pCamera->movePosition(glm::normalize(m_pCamera->getFront()) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_pCamera->movePosition(-(glm::normalize(m_pCamera->getFront()) * cameraSpeed));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_pCamera->movePosition(-(glm::normalize(glm::cross(m_pCamera->getFront(), m_pCamera->getUp())) * cameraSpeed));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_pCamera->movePosition(glm::normalize(glm::cross(m_pCamera->getFront(), m_pCamera->getUp())) * cameraSpeed);
}