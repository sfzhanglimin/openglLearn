#pragma
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


/*
https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderTexture2D(GLFWwindow* window, int* shader);
void renderEBO(GLFWwindow* window, int* shader);
int* generateTexture2DShader(GLFWwindow* window);
int* generateShaderEBO(GLFWwindow* window);

int main() {

	//=========设置环境=============

	//初始化
	glfwInit();
	//设置opengl版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//明确告诉GLFW我们使用的是核心模式(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//for mac
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);




	//===================窗口操作
	//创建一个glfw window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);


	//===========glad=============

	//GLAD是用来管理OpenGL的函数指针的
	// 
	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//======================视口========================
	//OpenGL渲染窗口的尺寸大小
	glViewport(0, 0, 800, 600);

	//注册窗口变化回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//vbo
	//int* shaders = generateShaderVBO(window);

	//ebo
	int* shaders = generateShaderEBO(window);


	//循环
	//检查GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		/*监听按键指令*/
		processInput(window);

		/*渲染*/
		//renderVBO(window, shaders);
		
		renderEBO(window, shaders);

		/*
		函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		*/
		glfwSwapBuffers(window);

		//函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
		glfwPollEvents();
	}

	//释放/删除之前的分配的所有资源
	glfwTerminate();


	return 0;
}

//窗口变化回调,创新设置窗口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//按键监听操作
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int* generateTexture2DShader(GLFWwindow* window) {
	//顶点着色器转换内容,
	//将传入的第一个数据申明为一个vec3类型的aPos变量
	//将apos变量转换为opengl需要的坐标vec4
	const char* vertexShaderSource = R"(
		#version 330 core
		//定义传入的第一个值为vec3类型的aPos字段
		layout (location = 0) in vec3 aPos;
		void main()
		{
			//传出gl_Position位置
			gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
		}
	)";

	//创建一个着色器对象并将返回的ID设置为vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//着色器源码附加到gl中
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译源码
	glCompileShader(vertexShader);

	int success;
	//查看编译结果
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char log[512];
		//获取编译信息
		glGetShaderInfoLog(vertexShader, 512, NULL, log);


		std::cout << "编译顶点着色器错误:\n" << log << std::endl;
	}


	const char* frameShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		void main(){
			FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
		}
	)";

	unsigned int fragmentShader;
	//创建片段着色器器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//绑定
	glShaderSource(fragmentShader, 1, &frameShaderSource, NULL);

	//编译
	glCompileShader(fragmentShader);

	//获取是否成功
	glGetShaderiv(fragmentShader, GL_FRAGMENT_SHADER, &success);
	if (!success) {
		char logs[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, logs);
		std::cout << "片段着色器编译失败:\n" << logs << std::endl;
	}


	//将上面两个着色器合并成一个着色器程序
	//创建一个program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//附加顶点着色器
	glAttachShader(shaderProgram, vertexShader);
	//附加片段着色器
	glAttachShader(shaderProgram, fragmentShader);
	//将上面shader链接起来(应该是链接成管线)
	glLinkProgram(shaderProgram);


	//检查,用GL_LINK_STATUS  glGetProgramInfoLog
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char logs[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, logs);
		std::cout << "program链接错误:\n" << logs << std::endl;
	}

	//激活program
	//glUseProgram(shaderProgram);

	//删除着色器对象.不需要了.已经传入了.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);






	unsigned int vao;
	//创建一个顶点缓冲数组-->VAO
	glGenVertexArrays(1, &vao);

	//绑定vao
	glBindVertexArray(vao);

	//三角顶点坐标
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	GLuint vbo;
	//创建一个顶点缓冲对象-->VBO
	glGenBuffers(1, &vbo);

	//绑定vbo到opengl,
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//将顶点数据传输到opengl绑定的GL_ARRAY_BUFFER缓存对象中.(vbo)
	//GL_STATIC_DRAW 数据不经常变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//告诉opengl数据格式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//其中顶点着色器属性
	glEnableVertexAttribArray(0);

	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//解绑VAO
	glBindVertexArray(0);

	int* result;
	result = (int*)malloc(sizeof(unsigned int) * 2);
	if (result) {
		result[0] = (int)shaderProgram;
		result[1] = vao;
	}
	return result;
}



int* generateShaderEBO(GLFWwindow* window) {
	//顶点着色器转换内容,
	//将传入的第一个数据申明为一个vec3类型的aPos变量
	//将apos变量转换为opengl需要的坐标vec4
	const char* vertexShaderSource = R"(
		#version 330 core
		//定义传入的第一个值为vec3类型的aPos字段
		layout (location = 0) in vec3 aPos;
		void main()
		{
			//传出gl_Position位置
			gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
		}
	)";

	//创建一个着色器对象并将返回的ID设置为vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//着色器源码附加到gl中
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译源码
	glCompileShader(vertexShader);

	int success;
	//查看编译结果
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		char log[512];
		//获取编译信息
		glGetShaderInfoLog(vertexShader, 512, NULL, log);


		std::cout << "编译顶点着色器错误:\n" << log << std::endl;
	}


	const char* frameShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		void main(){
			FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
		}
	)";

	unsigned int fragmentShader;
	//创建片段着色器器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//绑定
	glShaderSource(fragmentShader, 1, &frameShaderSource, NULL);

	//编译
	glCompileShader(fragmentShader);

	//获取是否成功
	glGetShaderiv(fragmentShader, GL_FRAGMENT_SHADER, &success);
	if (!success) {
		char logs[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, logs);
		std::cout << "片段着色器编译失败:\n" << logs << std::endl;
	}


	//将上面两个着色器合并成一个着色器程序
	//创建一个program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//附加顶点着色器
	glAttachShader(shaderProgram, vertexShader);
	//附加片段着色器
	glAttachShader(shaderProgram, fragmentShader);
	//将上面shader链接起来(应该是链接成管线)
	glLinkProgram(shaderProgram);


	//检查,用GL_LINK_STATUS  glGetProgramInfoLog
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char logs[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, logs);
		std::cout << "program链接错误:\n" << logs << std::endl;
	}

	//激活program
	//glUseProgram(shaderProgram);

	//删除着色器对象.不需要了.已经传入了.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);






	unsigned int vao;
	//创建一个顶点缓冲数组-->VAO
	glGenVertexArrays(1, &vao);

	//绑定vao
	glBindVertexArray(vao);

	//四边形顶点坐标
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};

	//四边形是两个三角形组成.
	//定义两个三角形使用的数据下标
	int verticeIndexs[] = {
		//第一个三角形
		0,1,3,
		1,2,3,
	};

	GLuint ebo;
	//创建元素缓冲对象 EBO
	glGenBuffers(1, &ebo);

	GLuint vbo;
	//创建一个顶点缓冲对象-->VBO
	glGenBuffers(1, &vbo);

	//绑定vbo到opengl,
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//将顶点数据传输到opengl绑定的GL_ARRAY_BUFFER缓存对象中.(vbo)
	//GL_STATIC_DRAW 数据不经常变
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//绑定ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//缓冲数据到opengl
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticeIndexs), verticeIndexs, GL_STATIC_DRAW);

	//告诉opengl数据格式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//其中顶点着色器属性
	glEnableVertexAttribArray(0);

	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//解绑VAO
	glBindVertexArray(0);

	int* result;
	result = (int*)malloc(sizeof(unsigned int) * 2);
	if (result) {
		result[0] = (int)shaderProgram;
		result[1] = vao;
	}
	return result;
}


// 渲染指令VBO
void renderTexture2D(GLFWwindow* window, int* shader) {
	//清空屏幕所用的颜色,设置默认颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);

	int shaderProgram = shader[0];
	int vao = shader[1];

	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// 渲染指令EBO
void renderEBO(GLFWwindow* window, int* shader) {
	//清空屏幕所用的颜色,设置默认颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);

	int shaderProgram = shader[0];
	int vao = shader[1];

	glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}