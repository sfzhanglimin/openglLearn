#pragma
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void render(GLFWwindow* window);

int main(){

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

// ��Ⱦָ��
void render(GLFWwindow* window) {
    //�����Ļ���õ���ɫ,����Ĭ����ɫ
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    //�����ɫ����
    glClear(GL_COLOR_BUFFER_BIT);
}