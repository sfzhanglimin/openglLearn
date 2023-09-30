#pragma
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include <iostream>

//stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
* https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/
*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderTexture2D(GLFWwindow* window, Shader shader,unsigned int vao);

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







	int width, height, nrChannels;
	//加载图片资源.获取到宽,高,颜色,通道

	unsigned char* data = stbi_load("D:/work/other/openglLearn/src/fourth/container.jpg",
						&width, &height, &nrChannels, 0);

	if (!data) {
		std::cout << "加载图片资源错误" << std::endl;
		return 1;
	}

	Shader textureShader(
		"D:/work/other/openglLearn/src/fourth/texture_v.glsl",
		"D:/work/other/openglLearn/src/fourth/texture_f.glsl");

	float vertices[] = {
		//位置						//颜色					//纹理
		0.5,0.5,0.0                 ,1.0,0.0,0.0,			1.0,1.0,			//右上
		0.5,-0.5,0.0				,0.0,0.5,0.0,			1.0,0.0,			//右下
		-0.5,-0.5,0.0				,1.0,1.0,0.0,			0.0,0.0,			//左下
		-0.5,0.5,0.0				,1.0,1.0,1.0,			0.0,1.0				//左上
	};

	int verticesIndex[] = {
		0,1,3,
		1,2,3
	};

	unsigned int vao, vbo, ebo;
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)6);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);




	unsigned int texture;
	//创建texture
	glGenTextures(1, &texture);

	//绑定
	glBindTexture(GL_TEXTURE_2D, texture);

	//设置X轴环绕方式为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//设置Y轴环绕方式为重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//设置mip 缩小方式为线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//设置mip 放大方式为线性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//数据传输
	/*
	* 1.第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定
	* 的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）
	*
	* 2.第二个参数为纹理指定多级渐远纹理的级别，
	* 如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	*
	* 3.第三个参数告诉OpenGL我们希望把纹理储存为何种格式。
	* 我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	*
	* 4,5.第四个和第五个参数设置最终的纹理的宽度和高度
	*
	* 6.第六个参数应该总是被设为0（历史遗留的问题）
	*
	* 7,8.第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，
	* 并把它们储存为char(byte)数组，我们将会传入对应值。
	*
	* 9.最后一个参数是真正的图像数据。
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//生成mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	//释放
	stbi_image_free(data);


	


	//循环
	//检查GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		/*监听按键指令*/
		processInput(window);

		/*渲染*/
		renderTexture2D(window, textureShader, vao);

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


// 渲染指令VBO
void renderTexture2D(GLFWwindow* window, Shader shader, unsigned int vao) {
	//清空屏幕所用的颜色,设置默认颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//清除颜色缓冲
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}