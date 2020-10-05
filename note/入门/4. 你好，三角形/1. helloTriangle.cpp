#include <glad/glad.h> // 内部已引用 GL/gl.h 等opengl工具库
#include <GLFW/glfw3.h> //
#include <iostream>

// 回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main(int argc, char * argv[]) {
    // ---------- 配置 GLFW 窗口 -----------
    // 初始化 GLFW
    glfwInit();
    // 配置 GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MAC OSX 必加
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // 使用 GLFW 创建窗口上下文
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Fail to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    // 设定为当前线程主上下文
    glfwMakeContextCurrent(window);
    // 通知 openGL 视口信息
    //        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // 前两个参数控制窗口左下角位置
    // 当窗口大小改变时，通知 opengl 更新视图
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // ----------- 初始化 GLAD ------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ // GLAD管理opengl指针
        std::cout << "Faild to initialized GLAD." << std::endl;
        return -1;
    }
    
    // ------- 编写并编译 vertex-shader 顶点着色器 ------
    // 创建顶点着色器存储 ID，声明类型
    int vertexShader;
    // 创建 vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 装载 vertex shader 源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 着色器，源码字符串数量，源码，？？
    // 开始编译 vertex shader
    glCompileShader(vertexShader);
    // 定义编译成功标志
    int success;
    // 定义 errorlog 容器
    char infoLog[512];
    // 获取编译结果
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // 如果编译失败，打印错误信息
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // --------- 编写并编译 fragment-shader 片段着色器 ---------
    int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << std::endl;
    }
    
    // ------- 将 vertex-shader 和 fragment-shader 链接为一个着色器程序 shader-program ---------
    // 声明一个着色器程序类型
    int shaderProgram;
    // 创建一个 shader-program
    shaderProgram = glCreateProgram();
    // 着色器程序附加到 shader-program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 链接，此过程后 shaderProgram 是一个程序对象
    glLinkProgram(shaderProgram);
    // 获取日志
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // --------- 删除已使用了的着色器 ---------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // ------- 创建 vertex-data 顶点数据 ----------
    // 定义 3D 坐标
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    // --------- 创建 VAO VBO --------
    // 声明顶点数组对象VertexArrayObject-ID，顶点缓冲对象VertexBufferObject-ID
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO); // 根据 buffer-size 和 VAO-ID 生成 VAO
    glGenBuffers(1, &VBO); // 根据 buffer-size 和 VBO-ID 生成 VBO
    // bind Vertex-Array-Object
    glBindVertexArray(VAO); // 绑定 VBO 到 VAO
    // then bind and set Vertex-Buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定 VBO 数据类型
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把 vertices 数据复制到GL_ARRAY_BUFFER缓冲即VBO中
    
    // ----------- 链接顶点属性 ---------
    // then configure Vertex-Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 链接顶点属性到 OpenGL
    glEnableVertexAttribArray(0); // 启用顶点属性
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    // 循环渲染
    while(!glfwWindowShouldClose(window)){
        // 检测输入，并给出响应
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置 RGBA
        glClear(GL_COLOR_BUFFER_BIT); // 状态应用
        
        // 激活 shaderProgram 程序对象
        glUseProgram(shaderProgram);
        
        // TODO: 在这里输入渲染代码
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // GL_LINE_STRIP
        
        // 检查调用事件、交换缓冲
        glfwSwapBuffers(window); // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwPollEvents(); // 检查是否有事件触发，并调用 callback
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}

// 帧缓冲大小函数，通知 opengl 更新视口信息
void framebuffer_size_callback(GLFWwindow* window, int width, int height){ // 第一次显示也会执行
    glViewport(0, 0, width, height);
}

// 检查键盘输入事件
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // 如果没有按下，glfwGetKey 返回 GLFW_RELEASE
        glfwSetWindowShouldClose(window, true); // 用户按下 ESC ，第二参数为 true，确认关闭窗口
    }
}
