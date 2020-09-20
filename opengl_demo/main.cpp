#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPods;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPods.x, aPods.y, aPods.z, 1.0);\n"
"}\n\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);



int main(){
    
    // GLFW: initialized & configure & window creation ----------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // OSX 必加
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TwoTriangles", NULL, NULL); // 设置 GLFW 窗口/视口/ViewPort 大小
    if(window == NULL){
        std::cout<< "Failed to create GLFW window." << std::endl;
        glfwTerminate(); // 释放所有 window 内存
        return -1;
    }
    glfwMakeContextCurrent(window); // 将窗口上下文设为当前线程主上下文
    
    // GLAD: load GLAD to adapt APIs of gl libiary -------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600); // 设置 OpenGL 视口大小。一般小于 GLFW 视口，以显示更多内容
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 当GLFW视口变化，通知OpenGL视口变化
    
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex Shader Complied failed." << infoLog <<std::endl;
    }
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std:: cout << "Error: Fragment Shader Complied failed." << infoLog << std::endl;
    }
    
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std:: cout << "Error: Shader Program linked failed." << std:: endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    // 1. 定义顶点数据
    float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
        // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    // 2. 定义 VBO VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 绑定VAO & 设置VBOs & 配置VBO属性
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // --------- 循环渲染 ---------
    while(!glfwWindowShouldClose(window)){ // 检查窗口是否应关闭
        // 处理输入事件
        processInput(window);
        
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 0.5f); // RGBA
        glClear(GL_COLOR_BUFFER_BIT); // 清除了颜色缓冲，将使用glClearColor颜色替代
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 调用缓冲、检查事件
        glfwSwapBuffers(window); // 使用双缓冲DoubleBuffer渲染窗口
        glfwPollEvents(); // 检查是否有触发事件
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}

/**
 循环渲染时，处理输入事件
 */
void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwWindowShouldClose(window);
    }
}

/**
 作为回调，当GLFW视口有变化，通知OpenGL视口变化
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

