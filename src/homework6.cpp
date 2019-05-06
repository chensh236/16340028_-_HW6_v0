#include "Initial.h"
#include "Shader.h"
#include <vector>
#define phongType 0
#define gouraudType 1
typedef int modelType;
// ************************�趨�õĲ���************************************
const unsigned int windowsWidth = 1000;
const unsigned int windowsHeight = 1000;
const char* head = "Draw square";
float ambientStrength = 0.2f;
float specularStrength = 1.0f;
float shininess = 4000.0f;
float diffuseFactor = 1.0f;
float radius = 4.0f;

/**
* @brief ����Esc�˳�
* @param window ����Ĵ���ָ��
* @param deltaTime ��ӵ��²���
*/
void escapePress(GLFWwindow *window, float& deltaTime);

int main()
{
    // ��ʼ������
    GLFWwindow* window = NULL;
    if (initialWindow::initial(window, windowsWidth, windowsHeight, head) == -1) return -1;
    //// ��ʼ��rgbֵ
    //Shader shader("rgb.vs", "rgb.frag");
    /**
    * IMGUI
    */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // ������ɫ��
    Shader shader("shader.vs", "shader.frag");
    Shader lampShader("light.vs", "light.frag");
    Shader gouraudShader("gouraud.vs", "gouraud.frag");

    // ���������Լ��ֹ���ӵķ���������
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // �󶨣�����׸��
    unsigned int cubeVAO, VBO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���й���ģ�͵�ѡ��
    modelType currentModel = phongType;
    // �ж��Ƿ���й�Դ���ƶ�
    bool isMove = true;
    // ��Դλ��
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    // �۲�λ��
    glm::vec3 viewPos = glm::vec3(5.0f, 1.0f, 10.0f);

    while (!glfwWindowShouldClose(window))
    {
        // ����Esc�˳�
        float WORTHLESS;
        escapePress(window, WORTHLESS);

        // ������ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /**
        * ʹ��IMGUI
        */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Attributes");
        ImGui::Text("Choose the model");
        ImGui::RadioButton("phong", &currentModel, phongType);
        ImGui::RadioButton("gouraud", &currentModel, gouraudType);
        ImGui::Separator();
        ImGui::Checkbox("Move the light", &isMove);

        ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 3.0f);
        ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 5.0f);
        ImGui::SliderFloat("Shininess", &shininess, 1.0f, 4000.0f);
        ImGui::SliderFloat("Diffuse Factor", &diffuseFactor, 0.0f, 1.0f);
        ImGui::End();

        glm::mat4 projection(1.0f);
        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        
        view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        /*projection = glm::perspective(20.0f, 1.0f, 0.01f, 100.0f);*/
        projection = glm::perspective(glm::radians(45.0f), (float)windowsWidth / (float)windowsHeight, 0.1f, 50.0f);
        // ����
        if (currentModel == phongType) {
            shader.use();
            // ������ɫ
            shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            // �������
            shader.setVec3("viewPos", viewPos);
            shader.setVec3("lightPos", lightPos);
            shader.setFloat("ambientStrength", ambientStrength);
            shader.setFloat("specularStrength", specularStrength);
            shader.setFloat("shininess", shininess);
            shader.setFloat("diffuseFactor", diffuseFactor);
            // ת����
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
        }
        else {
            gouraudShader.use();
            // ������ɫ
            shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            // �������
            gouraudShader.setVec3("viewPos", viewPos);
            gouraudShader.setVec3("lightPos", lightPos);
            gouraudShader.setFloat("ambientStrength", ambientStrength);
            gouraudShader.setFloat("specularStrength", specularStrength);
            gouraudShader.setFloat("shininess", shininess);
            shader.setFloat("diffuseFactor", diffuseFactor);
            // ת����
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            gouraudShader.setMat4("model", model);
            gouraudShader.setMat4("view", view);
            gouraudShader.setMat4("projection", projection);
        }
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // �ƶ���Դλ��
        if (isMove) {
            float camPosX = sin(glfwGetTime() * 0.5) * (float)radius;
            float camPosZ = cos(glfwGetTime() * 0.5) * (float)radius;
            lightPos = glm::vec3(camPosX, lightPos.y, camPosZ);
        }
        lampShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("model", model);
        lampShader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // render 
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

/**
* @brief ���м��̰����ļ��
* @param window ����ָ��
* @param deltaTime ��������ʱ��
*/
void escapePress(GLFWwindow *window, float& deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //// ���WSAD����
    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    Camera::getInstance()->ProcessKeyboard(FORWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    Camera::getInstance()->ProcessKeyboard(BACKWARD, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    Camera::getInstance()->ProcessKeyboard(LEFT, deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    Camera::getInstance()->ProcessKeyboard(RIGHT, deltaTime);
}