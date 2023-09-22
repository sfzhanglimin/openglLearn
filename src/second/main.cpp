#pragma
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render(GLFWwindow* window);

int main(){

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


    //循环
    //检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        /*监听按键指令*/
        processInput(window);

        /*渲染*/
        render(window);

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

// 渲染指令
void render(GLFWwindow* window) {
    //清空屏幕所用的颜色,设置默认颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    //清除颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT);
}