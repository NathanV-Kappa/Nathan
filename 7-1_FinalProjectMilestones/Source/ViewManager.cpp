///////////////////////////////////////////////////////////////////////////////
// viewmanager.h
// ============
// manage the viewing of 3D objects within the viewport
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "ViewManager.h"

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

// declaration of the global variables and defines
namespace {
	// Variables for window width and height
	const int WINDOW_WIDTH = 1800;
	const int WINDOW_HEIGHT = 1000;
	const char* g_ViewName = "view";
	const char* g_ProjectionName = "projection";

	// camera object used for viewing and interacting with the 3D scene
	Camera* g_pCamera = nullptr;

	// these variables are used for mouse movement processing
	float gLastX = WINDOW_WIDTH / 2.0f;
	float gLastY = WINDOW_HEIGHT / 2.0f;
	bool gFirstMouse = true;

	// time between current frame and last frame
	float gDeltaTime = 0.0f;
	float gLastFrame = 0.0f;

	// if orthographic projection is on, this value will be true
	bool bOrthographicProjection = false;
}

/***********************************************************
 *  ViewManager()
 *
 *  The constructor for the class
 ***********************************************************/
ViewManager::ViewManager(ShaderManager* pShaderManager) {
	// initialize the member variables
	m_pShaderManager = pShaderManager;
	m_pWindow = NULL;
	g_pCamera = new Camera();

	// default camera view parameters
	g_pCamera->Position = glm::vec3(0.0f, 10.5f, 20.0f); 
	g_pCamera->Front = glm::vec3(0.0f, -0.93f, -0.38f);
	g_pCamera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
	g_pCamera->Zoom = 120.0f;
}

/***********************************************************
 *  ~ViewManager()
 *
 *  The destructor for the class
 ***********************************************************/
ViewManager::~ViewManager() {
	// free up allocated memory
	m_pShaderManager = NULL;
	m_pWindow = NULL;

	if (NULL != g_pCamera) {
		delete g_pCamera;
		g_pCamera = NULL;
	}
}

/***********************************************************
 *  CreateDisplayWindow()
 *
 *  This method is used to create the main display window.
 ***********************************************************/
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle) {
	GLFWwindow* window = nullptr;

	// try to create the displayed OpenGL window
	window = glfwCreateWindow(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		windowTitle,
		NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	// this callback is used to receive mouse moving events
	glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);

	// this callback is used to receive mouse wheel scrolling events
	// used to adjust the movement speed of the camera (for zooming/travel speed control)
	glfwSetScrollCallback(window, &ViewManager::Mouse_Scroll_Wheel_Callback);

	// tell GLFW to capture all mouse events
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enable blending for supporting tranparent rendering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_pWindow = window;

	return(window);
}

/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse is moved within the active GLFW display window.
 ***********************************************************/
void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos) {
	// when the first mouse move event is received, this needs to be recorded so that 
	// all subsequent mouse moves can correctly calculate the X position offset and Y position offset for proper operation
	if (gFirstMouse) {
		gLastX = xMousePos;
		gLastY = yMousePos;
		gFirstMouse = false;
	}

	// calculate the X offset and Y offset values for moving the 3D camera accordingly
	float xOffset = xMousePos - gLastX;
	float yOffset = gLastY - yMousePos; // reversed since y-coordinates go from bottom to top

	// set the current positions into the last position variables
	gLastX = xMousePos;
	gLastY = yMousePos;

	// move the 3D camera according to the calculated offsets
	g_pCamera->ProcessMouseMovement(xOffset, yOffset);
}

/***********************************************************
 *  Mouse_Scroll_Wheel_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse wheel is scrolled within the active GLFW display window.
 *  It passes the scroll data to the Camera class which adjusts
 *  the movement speed (allowing control over how fast the camera
 *  travels/pans/zooms/orbits around the 3D scene).
 ***********************************************************/
void ViewManager::Mouse_Scroll_Wheel_Callback(GLFWwindow* window, double xOffset, double yOffset) {
	// if the camera object is null, then exit this method
	if (NULL == g_pCamera) {
		return;
	}

	// pass the vertical scroll offset (yOffset) to the camera for processing.
	// The Camera::ProcessMouseScroll typically adjusts MovementSpeed or Zoom
	// based on the scroll direction and amount. Positive yOffset usually
	// increases speed, negative decreases (exact behavior in Camera class).
	g_pCamera->ProcessMouseScroll(yOffset);
}

/***********************************************************
 *  ProcessKeyboardEvents()
 *
 *  This method is called to process any keyboard events
 *  that may be waiting in the event queue.
 ***********************************************************/
void ViewManager::ProcessKeyboardEvents() {
	// close the window if the escape key has been pressed
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_pWindow, true);
	}

	// if the camera object is null, then exit this method
	if (NULL == g_pCamera) {
		return;
	}

	// process camera zooming in and out
	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(FORWARD, gDeltaTime);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(BACKWARD, gDeltaTime);
	}

	// process camera panning left and right
	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(LEFT, gDeltaTime);
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(RIGHT, gDeltaTime);
	}

	// process camera upward and downward movement (QE keys)
	// Q key moves camera down, E key moves camera up
	if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(DOWN, gDeltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS) {
		g_pCamera->ProcessKeyboard(UP, gDeltaTime);
	}

	// Toggle between perspective (P key) and orthographic (O key) projection views.
	// This satisfies rubric item 4: user can switch at will between 3D perspective
	// and 2D-like orthographic display of the scene.
	if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS) {
		bOrthographicProjection = false;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS) {
		bOrthographicProjection = true;
	}
}

/***********************************************************
 *  PrepareSceneView()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void ViewManager::PrepareSceneView() {
	glm::mat4 view;
	glm::mat4 projection;

	// per-frame timing
	float currentFrame = glfwGetTime();
	gDeltaTime = currentFrame - gLastFrame;
	gLastFrame = currentFrame;

	// process any keyboard events that may be waiting in the event queue
	ProcessKeyboardEvents();

	// get the current view matrix from the camera
	view = g_pCamera->GetViewMatrix();

	// This implements the support for both perspective (3D) and orthographic (2D-like) displays that the user can toggle with P/O keys. 
	// Using GLM for modern OpenGL (avoids deprecated glOrtho/gluPerspective).
	// In orthographic mode the camera effectively "looks directly at" the scene with parallel projection (no perspective foreshortening). 
	// The bottom plane will appear less dominant / edge-on depending on current camera pitch; use mouse look + QE to level the view for best ortho result.
	if (bOrthographicProjection) {
		// Orthographic projection parameters
		float orthoScale = 13.0f;                    // vertical half-height in world units
		float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
		float orthoLeft = -orthoScale * aspect;
		float orthoRight = orthoScale * aspect;
		float orthoBottom = -orthoScale;
		float orthoTop = orthoScale;

		projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, 0.1f, 100.0f);
	}

	else {
		// Standard perspective projection (3D view with depth/foreshortening)
		projection = glm::perspective(glm::radians(g_pCamera->Zoom),
			(GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}

	// if the shader manager object is valid
	if (NULL != m_pShaderManager) {
		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ViewName, view);

		// set the view matrix into the shader for proper rendering
		m_pShaderManager->setMat4Value(g_ProjectionName, projection);

		// set the view position of the camera into the shader for proper rendering
		m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
	}
}