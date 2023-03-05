


#if 0 // the SDL Library Test Demo
#include <SDL3/SDL.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 480;

SDL_Window* InitSDLWindow(const char* title);
void Wait2SecsThenExit();

SDL_Window* window;

void SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

//SDL demo main
int main(int argc, char* argv[]) {
    window = InitSDLWindow("SDL framebuffer demo");
    auto surface = SDL_GetWindowSurface(window);//��ȡ���ڻ����������
    // �ڻ�������һ���ߣ�
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        int j1 = 240, j2 = 241, j3 = 242;
        SetPixel(surface, i, j1, 0xFFFFFF);
        SetPixel(surface, i, j2, 0xFF00FF);
        SetPixel(surface, i, j3, 0x00FFFF);
    }
    SDL_UpdateWindowSurface(window);
    Wait2SecsThenExit();
    return 0;
}

SDL_Window* InitSDLWindow(const char* title) {
    SDL_Window* window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        0
    );
    return window;
}

void SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
    Uint32* const target_pixel = (Uint32*)((Uint8*)surface->pixels
        + y * surface->pitch
        + x * surface->format->BytesPerPixel);//�ƶ�ָ���Ӧÿһ�����ص��׵�ַ
    *target_pixel = pixel;//������ʵ���ص�RGBֵ
}

void Wait2SecsThenExit() {
    SDL_Delay(10 * 1000);
    SDL_DestroyWindow(window);
}
#endif


#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "GL/gl.h"
#include <iostream>
#include "shader.h"
#include "stb_image.h"
using namespace std;

void processInput(GLFWwindow* window);// key handle 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//viewport handle
float calculateSumWeight(float blurRadius);

//OpenGL Test Main
int main(int argc, char* argv[]) 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS 
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //openGL �����汾�ڶ࣬�޷��ڱ���ʱȷ�����庯���ĵ�ַ����Ҫ������ʱ��ȡ��Ӧ�����ĵ�ַ��glad�ṩ�������ķ���������glfw��ȡOpenGL����ָ���ַ�ĺ�������ʼ��glad��
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//glad get OpneGL Function address
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//register viewport update callback



    //float vertices[] =
    //{
    //    -0.5f, -0.5f, 0.0f,
    //    0.5f, -0.5f, 0.0f,
    //    0.0f,  0.5f, 0.0f
    //};

    float verticesf[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
    };

    float verticesPlane[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
             1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
    };

    unsigned int indices[] = {
        // ע��������0��ʼ! 
        // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
        // �����������±��������ϳɾ���

        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aColor;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"
        "out vec3 ourColor;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "ourColor = aColor;\n"
        "TexCoord = aTexCoord;\n"
        "}\0";


    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D ourTexture;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(ourTexture, TexCoord);\n"
        "}\n\0";
    // build and compile our shader program
// ------------------------------------
    // vertex shader
    //int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //glCompileShader(vertexShader);
    //// check for shader compile errors
    //int success;
    //char infoLog[512];
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}



    //// fragment shader
    //int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //glCompileShader(fragmentShader);
    //// check for shader compile errors
    //glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //if (!success)
    //{
    //    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    //}



    //// link shaders
    //int shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);
    //glLinkProgram(shaderProgram);
    //// check for linking errors
    //glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    //if (!success) {
    //    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    //}

    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);

    Shader shader("./shader.vs", "./shader.fs");

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);//��������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//��VBO���㻺������Դ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesf), verticesf, GL_STATIC_DRAW);//���������ݸ��Ƶ����㻺���� 
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*���Ӷ�������*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int  VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);//��������
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);//��VBO���㻺������Դ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlane), verticesPlane, GL_STATIC_DRAW);//���������ݸ��Ƶ����㻺���� 
    
    unsigned int EBO1;
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        /*���Ӷ�������*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    //����ͼƬ
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load("./ss.jpg", &width, &height, &nrChannels, 0);


    //FBO ֡������������Ⱦ������������
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // ���ɿ�����
    unsigned int texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Texture ����
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // �������ӵ���ǰ�󶨵�֡�������
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    while (!glfwWindowShouldClose(window))//render loop 
    {
        int blurRadius = 30;
        float weightValue = calculateSumWeight(blurRadius);
        processInput(window);//����
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//��Ⱦ
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        //����uniform��Ҫ�ȼ�����ɫ��
        shader.setInt("ourTexture", 0);//�������������
        
        shader.setInt("uBlurRadius", blurRadius);
        shader.setVec2("uBlurOffset", 5.0f / float(400), 0.0);
        
        shader.setFloat("uSumWeight", weightValue);
        glBindVertexArray(VAO1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        shader.use();
        shader.setInt("ourTexture", 0);//�������������
        shader.setInt("uBlurRadius", blurRadius);
        shader.setVec2("uBlurOffset", 0.0, 5.0f / float(300));
        cout << weightValue << endl;
        shader.setFloat("uSumWeight", weightValue);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);//double buffer
        glfwPollEvents();//event loop
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();//terminate glfw library 
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(window, true);
    }
}

float calculateSumWeight(float blurRadius) 
{

    if (blurRadius < 1) {
        return 1.0;
    }

    float sumWeight = 0;
    float sigma = blurRadius / 3.0f;
    for (int i = 0; i < blurRadius; i++) {
        float weight = (float)((1 / sqrt(2 * acos(-1) * sigma *sigma )) * exp(-(i * i) / (2 * sigma * sigma)));
        sumWeight += weight;
        if (i != 0) {
            sumWeight += weight;
        }
    }
    return sumWeight;
}