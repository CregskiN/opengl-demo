#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
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
    
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    
    Shader ourShader("/Users/cregskin/code/c/opengl_demo/opengl_demo/shader.vs", "/Users/cregskin/code/c/opengl_demo/opengl_demo/shader.fs");
    
    
    
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 3D位置属性
    // 顶点位置值 顶点大小 数据类型 是否标准化 步长 数据在缓冲起始位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 顶点属性位置值为参数，启用顶点属性
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    // 纹理 1
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1); // 定义纹理数据空间
    glBindTexture(GL_TEXTURE_2D, texture1); // 绑定纹理空间
    // 配置 2D 纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 配置 S 轴纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 配置 T 轴纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 纹理缩小，使用 linear 过滤纹理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 纹理放大，使用 linear 过滤纹理
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    std::string inputPath = "/Users/cregskin/code/c/opengl_demo/opengl_demo/container.jpg";
    // 文件路径 宽度 高度 信道数量
    unsigned char* data = stbi_load(inputPath.c_str(), &width, &height, &nrChannels, 0);
    if(data) {
        // 纹理目标(处于opengl) 多级渐远纹理级别0为基本级别 纹理储存的格式 固定0历史遗留问题 原图格式和数据类型 原数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Error: Faile to load image 1." << std::endl;
    }
    stbi_image_free(data); // 释放 data 数据
    
    // 纹理 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    inputPath = "/Users/cregskin/code/c/opengl_demo/opengl_demo/awesomeface.jpg";
    data = stbi_load(inputPath.c_str(), &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Error: Faile to load image 2" << std::endl;
    }
    stbi_image_free(data);
    
    // 使用 shader // 在此后可传递 Uniform 在此前可获取 Uniform location
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    
    
    
    // 循环渲染
    while(!glfwWindowShouldClose(window)){
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader.use();
        
        // 定义三种变换矩阵：模型矩阵modelMatrix，观察矩阵viewMatrix，投影矩阵projectionMatrix
        glm::mat4 view = glm::mat4(1.0f);;
        glm::mat4 projection = glm::mat4(1.0f);;

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // 单位矩阵 + 位移向量 = 位移矩阵
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
          glm::mat4 model;
          model = glm::translate(model, cubePositions[i]);
//          float angle = 20.0f * i;
          model = glm::rotate(model, (float)glfwGetTime()*((i+1) % 3), glm::vec3(1.0f, 0.3f, 0.5f));
          ourShader.setMat4("model", model);

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
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
