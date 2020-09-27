#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include </Users/cregskin/code/c/opengl_demo/opengl_demo/shader_s.h> // 强制使用绝对路径

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 650;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void toNDC(float* vertices, int length, int SCR_WIDTH, int SCR_HEIGHT);


int main(){
    // GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "VAO&VBOtoTriangles", NULL, NULL);
    if(!window){
        std:: cout << "Error: GLFW window initialized failed." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std:: cout << "Error: GLAD initialized failed." << std::endl;
        return -1;
    }
    
    Shader ourShader("/Users/cregskin/code/c/opengl_demo/opengl_demo/shader.vs", "/Users/cregskin/code/c/opengl_demo/opengl_demo/shader.fs");
    
    float vertices[] = {
        500.0, 0, 0.0f,
        -500.0, 0, 0.0f,
    };
    
    toNDC(vertices, sizeof(vertices), SCR_WIDTH, SCR_HEIGHT);
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 顶点位置值 顶点大小 数据类型 是否标准化 步长 数据在缓冲起始位置
    glEnableVertexAttribArray(0); // 顶点属性位置值为参数，启用顶点属性
    
    
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, 2); // 数据起始位置 点数
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(2, &VAO);
    glDeleteBuffers(2, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height); // 前两个参数表示 GLFW 窗口左下角位置
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

/**
 按标准坐标 等比缩小
 */
void toNDC(float* vertices, int size, int SCR_WIDTH, int SCR_HEIGHT){
    int verticesNum = size / 4; // 顶点总数
    float halfSCR_WIDTH = SCR_WIDTH / 2.0;
    float halfSCR_HEIGHT = SCR_HEIGHT / 2.0;
    for(int i = 0; i < verticesNum; i += 3){
//        std::cout << "from " <<vertices[i] << "  " << vertices[i+1] << "  " << vertices[i+2] << std::endl;
        vertices[i] = float(vertices[i] / halfSCR_WIDTH); // x
        vertices[i+1] = float(vertices[i+1] / halfSCR_HEIGHT); // y
        vertices[i+2] = 0.0f; // z
//        std::cout << "to " << vertices[i] << "  " << vertices[i+1] << "  " << vertices[i+2] << std::endl;
    }
}
