



#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "GL/gl.h"
#include <iostream>
#include "shader.h"
#include "stb_image.h"
#include "camera.h"
using namespace std;

void processInput(GLFWwindow* window);// key handle 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//viewport handle

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

    //openGL 驱动版本众多，无法在编译时确定具体函数的地址，需要在运行时获取对应函数的地址。glad提供了这样的方法，传入glfw获取OpenGL函数指针地址的函数，初始化glad。
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
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    float verticesPlane[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

	float vertices[] = {
		        // positions          // normals           // texture coords
			   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
				0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
				0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

    unsigned int indices[] = {
        // 注意索引从0开始! 
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    //多个立方体位置
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.2f,  0.0f, 0.0f),
        glm::vec3(-1.5f, -1.0f, -2.0f),
        glm::vec3(-1.0f, -2.0f, 0.0f),
        glm::vec3(1.0f,  -0.8f,  -1.3f),
        glm::vec3(-0.7f, -1.3f, -2.0f),
        glm::vec3(1.0f,  1.5f, -1.0f),
        glm::vec3(0.0f,  0.06f, -2.4f)
    };

    //多个点光源位置
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f,  0.2f,  1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f,  1.0f, 1.0f),
        glm::vec3(-3.0f,  0.0f, -1.4f)

    };

    Shader shader("./shader.vs", "./shader.fs");
    Shader lightShader("./lightShader.vs", "./lightShader.fs");
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    unsigned int VBO;
    glGenBuffers(1, &VBO);//创建对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定VBO顶点缓冲对象，显存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将顶点数据复制到顶点缓存区 

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*链接顶点属性*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//绑定VBO顶点缓冲对象，显存中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //加载图片
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load("./ss.jpg", &width, &height, &nrChannels, 0);

    //Texture 纹理
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
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


    //Box Texture 材质纹理(光照贴图:漫反射/环境光贴图)
    unsigned int boxTexture;
    glGenTextures(1, &boxTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boxTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    //camera
    data = stbi_load("./boxTex.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Specular Texture 材质纹理(光照贴图:镜面高光贴图)
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    //camera
    data = stbi_load("./specularTex.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    Camera camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

    //Projection
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::vec3 lightPos(0.0f,0.0f,2.0f);
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))//render loop 
    {
        int blurRadius = 30;
        processInput(window);//输入
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//渲染
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        shader.setInt("ourTexture", 0);//设置纹理采样器
//        //MVP
//        //Model

        //model =  glm::translate(model, glm::vec3(0.0f,0.0f,-3.0f));
        /*glfwGetTime()**/
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
       
        //View
        //glm::lookAt(Position, Position + Front, Up);
        //摄像机世界坐标，实际通过观察矩阵转化为观察坐标（0,0,0）
        glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 4.0f);
        glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //view = glm::translate(view, glm::vec3(0.2f, 0.0f, -2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
       
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("viewPos", viewPos.x, viewPos.y, viewPos.z);
        /*
            光照-投光物
            定向光（Directional Light）/点光源（Point Light）/聚光灯（Spotlight）
                a.平行光：模拟当光源很远的情况下，类似现实生活的太阳光照，这个时候与物体位置和观察位置无光，是一个平行的光线，光照可以给一个光照的方向，而不是
                          具体的光源位置，可以通过齐次坐标vec4的w分量区分，如果是0，不能进行偏移转换，代表方向，如果是1，代表是点可以进行偏移转换，shader中可以
                          进行区分，另外光的方向如果是从光源指向物体，shader中进行取反操作，默认认为漫反射光照处理是物体指向光源。
        
                b.点光源： 点光源会朝着所有方向发光，但光线会随着距离逐渐衰减，衰减公式为Fatt=1.0/（Kc+Kl∗d+Kq∗d*d),其中d为距离，随着距离衰减，距离短的时候光照
                           下降速度较快，距离远的时候下降较慢。kc,k1,kq为常量，kc一般为1.0，保证分母大于1.0.类似电灯泡或者火把。
                c.聚光源： 聚光是位于环境中某个位置的光源，它只朝一个特定方向而不是所有方向照射光线。这样的结果就是只有在聚光方向的特定半径内的物体才会被照亮，
                           其它的物体都会保持黑暗。聚光很好的例子就是路灯或手电筒。
                           
        */
        //平行光 （模拟当光源很远的情况下，类似现实生活的太阳光）,设置linght.lightDir(vec4 变量)，齐次坐标0代表向量（光的方向），齐次坐标1代表点（光的点位置）。
        glm::vec4 lightDir(0.0f, 0.0f, 5.0f, 0.0f);
        shader.setVec4("dlight.lightDir", lightDir);
        shader.setVec3("dlight.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("dlight.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("dlight.specular", 1.0f, 1.0f, 1.0f);
        ////设置点光源衰减常数
        shader.setFloat("plight[0].constant", 1.0f);
        shader.setFloat("plight[0].linear", 0.09f);
        shader.setFloat("plight[0].quadratic", 0.032f);
        shader.setVec3("plight[0].ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("plight[0].diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("plight[0].specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("plight[0].lightPos", glm::vec3(0.0f, 0.0f, 2.0f));
        shader.setFloat("plight[1].constant", 1.0f);
        shader.setFloat("plight[1].linear", 0.09f);
        shader.setFloat("plight[1].quadratic", 0.032f);
        shader.setVec3("plight[1].ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("plight[1].diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("plight[1].specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("plight[1].lightPos", pointLightPositions[1]);
        shader.setFloat("plight[2].constant", 1.0f);
        shader.setFloat("plight[2].linear", 0.09f);
        shader.setFloat("plight[2].quadratic", 0.032f);
        shader.setVec3("plight[2].ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("plight[2].diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("plight[2].specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("plight[2].lightPos", pointLightPositions[2]);
        shader.setFloat("plight[3].constant", 1.0f);
        shader.setFloat("plight[3].linear", 0.09f);
        shader.setFloat("plight[3].quadratic", 0.032f);
        shader.setVec3("plight[3].ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("plight[3].diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("plight[3].specular", 1.0f, 1.0f, 1.0f);
        shader.setVec3("plight[3].lightPos", pointLightPositions[3]);
        //聚光灯
        shader.setVec3("flight.lightPos", viewPos);
        shader.setVec3("flight.direction", glm::vec3(0.0f, 0.0f, -2.0f));
        shader.setFloat("flight.cutOff", glm::cos(glm::radians(20.5f)));
        shader.setFloat("flight.outerCutOff", glm::cos(glm::radians(25.5f)));
        shader.setVec3("flight.ambient", 0.2f, 0.2f, 0.2f);
        shader.setVec3("flight.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("flight.specular", 1.0f, 1.0f, 1.0f);
        //应用光照纹理贴图后，不设置自然光和漫反射光以及镜面反射高光的颜色分量，从纹理中采样每个片段的颜色分量。
        //shader.setVec3("material.ambient", 0.0f, 0.5f, 0.35f);
        //shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.35f);
        //shader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);

        //设置材质纹理贴图(漫反射光贴图)
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, boxTexture);
        //设置漫反射光贴图纹理采样器
        shader.setInt("material.diffuse", 1);
        //设置材质纹理贴图(镜面反射高光贴图)
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, specularTexture);
        //设置漫反射光贴图纹理采样器
        shader.setInt("material.specular", 2);
        shader.setFloat("material.shininess", 32);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 8; i++)
        {
            //glm::mat4 model;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            //float angle = 20.0f * i;
           // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//;
//
        //shader.setMat("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, lightPos);
        model1 = glm::scale(model1, glm::vec3(0.2f));
        lightShader.use();
        lightShader.setMat("model", model1);
        lightShader.setMat("projection", projection);
        lightShader.setMat("view", view);
        glBindVertexArray(LightVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);//double buffer
        glfwPollEvents();//event loop
    }
    glDeleteVertexArrays(1, &LightVAO);
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

