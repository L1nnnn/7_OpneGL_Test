#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // ����ID
    unsigned int ID;

    // ��������ȡ��������ɫ��
    Shader(const char* vertexPath, const char* fragmentPath);
    // ʹ��/�������
    void use();
    // uniform���ߺ���
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, float value0, float value1) const;
    void setVec3(const std::string& name, float value0, float value1, float value2) const;
    void setVec3(const std::string& name, glm::vec3 vector) const;
    void setVec4(const std::string& name, float value0, float value1, float value2, float vlaue3) const;
    void setVec4(const std::string& name, glm::vec4 lightDir) const;
    void setMat(const std::string& name, glm::mat4 matrix) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif