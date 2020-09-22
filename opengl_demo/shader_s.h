//
//  shader_s.h
//  opengl_demo
//
//  Created by 吴昊 on 2020/9/22.
//  Copyright © 2020 吴昊. All rights reserved.
//

#ifndef shader_s_h
#define shader_s_h

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID; // 程序ID
    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath){
        // 1. 从文件路径中获取 顶点着色器/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // 打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // 读取文件缓冲内容到数据流
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 关闭文件处理
            vShaderFile.close();
            fShaderFile.close();
            // 转换数据流到 stirng
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "Error: Shaders read failed." << std::endl;
        }
        // 最终将读取到的着色器 存储在 vShderCode fShaderCode (两个 string)
        const char* vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        
        int success;
        char infoLog[512];
        
        glShaderSource(vertex,1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "Error: Vertex Shader compiled failed." << infoLog << std::endl;
        }
        
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "Error: Fragment Shader compiled failed." << infoLog << std::endl;
        }
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "Error: Shader Program linked failed" << infoLog << std::endl;
        }
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // 使用/激活程序
    void use(){
        glUseProgram(ID);
    };
    // uniform 工具函数
    void setBool(const std::string &name, bool value) const{
        glUniformli(glGetUniformLocation())
    };
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, int value) const;
    
};



#endif /* Shader_h */

