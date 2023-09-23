#pragma
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render(GLFWwindow* window);

int main() {

	//=========���û���=============

	//��ʼ��
	glfwInit();
	//����opengl�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//��ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//for mac
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);




	//===================���ڲ���
	//����һ��glfw window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);


	//===========glad=============

	//GLAD����������OpenGL�ĺ���ָ���
	// 
	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//======================�ӿ�========================
	//OpenGL��Ⱦ���ڵĳߴ��С
	glViewport(0, 0, 800, 600);

	//ע�ᴰ�ڱ仯�ص�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//ѭ��
	//���GLFW�Ƿ�Ҫ���˳�
	while (!glfwWindowShouldClose(window))
	{
		/*��������ָ��*/
		processInput(window);

		/*��Ⱦ*/
		render(window);

		/*
		�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		*/
		glfwSwapBuffers(window);

		//���������û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬��
		glfwPollEvents();
	}

	//�ͷ�/ɾ��֮ǰ�ķ����������Դ
	glfwTerminate();


	return 0;
}

//���ڱ仯�ص�,�������ô��ڴ�С
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//������������
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int generateShader(GLFWwindow* window) {
	//������ɫ��ת������,
	//������ĵ�һ����������Ϊһ��vec3���͵�aPos����
	//��apos����ת��Ϊopengl��Ҫ������vec4
	const char* vertexShaderSource = R"(
		#version 330 core
		#���崫��ĵ�һ��ֵΪvec3���͵�aPos�ֶ�
		layout (location = 0) in vec3 aPos;
		void main()
		{
			#����gl_Positionλ��
			gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
		}
	)";

	//����һ����ɫ�����󲢽����ص�ID����ΪvertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//��ɫ��Դ�븽�ӵ�gl��
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//����Դ��
	glCompileShader(vertexShader);

	int success;
	//�鿴������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char log[512];
		//��ȡ������Ϣ
		glGetShaderInfoLog(vertexShader, 512, NULL, log);


		std::cout << "���붥����ɫ������:\n" << log << std::endl;
	}


	const char* frameShaderSource = R"(
		#version 330 core
		void main(){
			FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
		}
	)";

	unsigned int fragmentShader;
	//����Ƭ����ɫ����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//��
	glShaderSource(fragmentShader, 1, &frameShaderSource, NULL);

	//����
	glCompileShader(fragmentShader);

	//��ȡ�Ƿ�ɹ�
	glGetShaderiv(fragmentShader, GL_FRAGMENT_SHADER, &success);
	if (!success) {
		char logs[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, logs);
		std::cout << "Ƭ����ɫ������ʧ��:\n" << logs << std::endl;
	}


	//������������ɫ���ϲ���һ����ɫ������
	//����һ��program
	unsigned int shaderProgram = glCreateProgram();
	//���Ӷ�����ɫ��
	glAttachShader(shaderProgram, vertexShader);
	//����Ƭ����ɫ��
	glAttachShader(shaderProgram, fragmentShader);
	//������shader��������(Ӧ�������ӳɹ���)
	glLinkProgram(shaderProgram);


	//���,��GL_LINK_STATUS  glGetProgramInfoLog
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char logs[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, logs);
		std::cout << "program���Ӵ���:\n" << logs << std::endl;
	}

	//����program
	//glUseProgram(shaderProgram);

	//ɾ����ɫ������.����Ҫ��.�Ѿ�������.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);







	//�����ζ�������
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint vbo;
	//����һ�����㻺�����
	glGenBuffers(1, &vbo);

	//��vbo��opengl,
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//���������ݴ��䵽opengl�󶨵�GL_ARRAY_BUFFER���������.(vbo)
	//GL_STATIC_DRAW ���ݲ�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����opengl���ݸ�ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//���ж�����ɫ������
	glEnableVertexAttribArray(0);
}


// ��Ⱦָ��
void render(GLFWwindow* window) {
	//�����Ļ���õ���ɫ,����Ĭ����ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//�����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
}