#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    
    
    // 定义 3D顶点坐标、颜色RGB、2D纹理坐标
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形：右上、右下、左下
        1, 2, 3  // 第二个三角形：右下、左下、左上
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 3D位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 顶点位置值 顶点大小 数据类型 是否标准化 步长 数据在缓冲起始位置
    glEnableVertexAttribArray(0); // 顶点属性位置值为参数，启用顶点属性
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    unsigned int texture;
    glGenTextures(1, &texture); // 定义纹理数据空间
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理空间
    // 配置 2D 纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 配置 S 轴纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 配置 T 轴纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 纹理缩小，使用 linear 过滤纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 纹理放大，使用 linear 过滤纹理
    
    int width, height, nrChannels;
    std::string inputPath = "/Users/cregskin/code/c/opengl_demo/opengl_demo/container.jpg";
    unsigned char* data = stbi_load(inputPath.c_str(),&width,&height,&nrChannels, 0); // filename width height numberOfChannels
    if(data){
        // 纹理目标(处于opengl) 多级渐远纹理级别0为基本级别 纹理储存的格式 固定0历史遗留问题 原图格式和数据类型 原数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Error: Faile to load image." << std::endl;
    }
    stbi_image_free(data); // 释放 data 数据
    
    
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, texture);
        
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制模式 绘制顶点个数 索引类型 EBO中的偏移量
        
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
