#include <glad/glad.h> // 内部已引用 GL/gl.h 等opengl工具库
#include <GLFW/glfw3.h> //
#include <iostream>

// 回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 300;
const unsigned int SCR_HEIGHT = 300;

int main(int argc, char * argv[]) {
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
    glfwMakeContextCurrent(window); // 设定为当前线程主上下文
    
    // 初始化 GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ // GLAD管理opengl指针
        std::cout<< "Faild to initialized GLAD." << std::endl;
        return -1;
    }
    
    // 通知 openGL 视口信息
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // 前两个参数控制窗口左下角位置
    // 当窗口大小改变时，通知 opengl 更新视图
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    // 循环渲染 避免渲染一次窗口就关闭
    while(!glfwWindowShouldClose(window)){
        // 检测输入，并给出响应
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 状态设置
        glClear(glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT););// 状态应用 通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(BufferBit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。
        
        // TODO: 在这里输入渲染代码

        
        
        // 检查调用事件、交换缓冲
        glfwPollEvents(); // 检查是否有事件触发，并调用 callback
        glfwSwapBuffers(window); // 交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
    }
    
    glfwTerminate();
    return 0;
}

// 帧缓冲大小函数，通知 opengl 更新视口信息
void framebuffer_size_callback(GLFWwindow* window, int width, int height){ // 第一次显示也会执行
    glViewport(0, 0, width, height);
}

// 检查键盘输入事件
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE == GLFW_PRESS)){ // 如果没有按下，glfwGetKey 返回 GLFW_RELEASE
        glfwSetWindowShouldClose(window, true); // 用户按下 ESC ，第二参数为 true，确认关闭窗口
    }
}
