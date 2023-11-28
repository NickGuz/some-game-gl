//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <algorithm>
//
//#include "Shader.h"
//#include "Camera.h"
//#include "stb_image.h"
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCR_WIDTH / 2;
//float lastY = SCR_HEIGHT / 2;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f; // time between current frame and last frame
//float lastFrame = 0.0f; // time of last frame
//
//// lighting
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//
//int old_main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//	// glfw window creation
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//	if (window == NULL) {
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPosCallback(window, mouse_callback);
//
//	// glad: load all OpenGL function pointers
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	// configure global opengl state
//	// -----------------------------
//	glEnable(GL_DEPTH_TEST);
//
//	// build and compile our shader program
//	Shader lightingShader("light.vert", "light.frag");
//	Shader cubeShader("cube.vert", "cube.frag");
//
//	// set up vertex data (and buffer(s)) and configure vertex attributes
//	float vertices[] = {
//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//	};
//
//	// configure the cube's VAO
//	unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	
//	// normal attribute
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	// next configure the light's VAO (VBO stays the same)
//	unsigned int lightVAO;
//	glGenVertexArrays(1, &lightVAO);
//	glBindVertexArray(lightVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	// uncomment this call to draw in wireframe polygons
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	// render loop
//	while (!glfwWindowShouldClose(window)) {
//		// per-frame time logic
//		// --------------------
//		float currentFrame = static_cast<float>(glfwGetTime());
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		// input
//		// -----
//		processInput(window);
//
//		// render
//		// ------
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// render container
//		cubeShader.use();
//		cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
//		cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
//		cubeShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
//		cubeShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
//
//		glm::mat4 view = camera.GetViewMatrix();
//		cubeShader.setMat4("view", view);
//
//		glm::mat4 projection = glm::mat4(1.0f);
//		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		cubeShader.setMat4("projection", projection);
//
//		// world transformation
//		glm::mat4 model = glm::mat4(1.0f);
//		cubeShader.setMat4("model", model);
//
//		// render cube
//		glBindVertexArray(VAO); 
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		// also draw the lamp object
//		lightingShader.use();
//		lightingShader.setMat4("projection", projection);
//		lightingShader.setMat4("view", view);
//		model = glm::mat4(1.0f);
//		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
//		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
//		model = glm::translate(model, lightPos);
//		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
//		lightingShader.setMat4("model", model);
//
//		glBindVertexArray(lightVAO);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		// glfw: swap buffers and poll IO events
//		// -------------------------------------
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	// optional: de-allocate all resources once they've outlived their purpose:
//	// ------------------------------------------------------------------------
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteVertexArrays(1, &lightVAO);
//	glDeleteBuffers(1, &VBO);
//	//glDeleteBuffers(1, &EBO);
//	cubeShader.deleteProgram();
//	lightingShader.deleteProgram();
//
//	// glfw: Terminate, clearing all previously allocated GLFW resources
//	// -----------------------------------------------------------------
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow* window) {
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//
//	// camera movement
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//		camera.ProcessKeyboard(FORWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//		camera.ProcessKeyboard(BACKWARD, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//		camera.ProcessKeyboard(LEFT, deltaTime);
//	}
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//		camera.ProcessKeyboard(RIGHT, deltaTime);
//	}
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
//	float xpos = static_cast<float>(xposIn);
//	float ypos = static_cast<float>(yposIn);
//
//	if (firstMouse) {
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
