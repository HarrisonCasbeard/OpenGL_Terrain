#ifndef _INPUT_H_
#define _INPUT_H_

#include <vector>

class Camera;
class Command;
struct GLFWwindow;

class Input
{
private:
	void handleMouseInput(GLFWwindow* window, std::vector<Command*>& vCommands);
	void handleKeyboardInput(GLFWwindow* window, const double deltaTime, std::vector<Command*>& vCommands);

	Camera*	m_pCamera;

	double m_mouseLastX;
	double m_mouseLastY;

	Command* m_upArrow;
	Command* m_downArrow;
	Command* m_leftArrow;
	Command* m_rightArrow;
	Command* m_escKey;
	Command* m_mouseClick;

public:
	 Input();
	~Input();

	void initialise	(Camera& camera, const double winWidth, const double winHeight);
	void update		(GLFWwindow* window, const double deltaTime, std::vector<Command*>& vCommands);
};

#endif

