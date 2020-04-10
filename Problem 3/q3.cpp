

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "utility/readfile.hpp"
#include "utility/shader.hpp"
#include "Textures/texture.hpp"
#include "utility/camera.hpp"

#include "utility/glm/glm/glm.hpp"
#include "utility/glm/glm/gtc/matrix_transform.hpp"
#include "utility/glm/glm/gtc/type_ptr.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(std::vector<std::string> faces);
void verticesfind(float *vertices, int no);
// settings
float deltaTime= 0.0f;
float lastframe=0.0f;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
bool blinn = false;
bool blinnKeyPressed = false;

int lighting = 1;
bool lightingkeypress = false;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX =SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT/2.0f;

float totvertices[14*36];

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cube", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    glewInit();
    
    float vertices1[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    };   
    float vertices2[] = {
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    };
    float vertices3[] = {
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    };
    float vertices4[] = {
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    };
    float vertices5[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    };
    float vertices6[] = {
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 0.6f, -3.3f, -3.0f),
        glm::vec3(-2.0f,  2.0f, 3.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    GLfloat skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };

    verticesfind(vertices1, 0);
    verticesfind(vertices2, 1);
    verticesfind(vertices3, 2);
    verticesfind(vertices4, 3);
    verticesfind(vertices5, 4);
    verticesfind(vertices6, 5);
    
    glEnable(GL_DEPTH_TEST);

    Shader *skyboxshdr = new Shader("shaders/shadsky.vs", "shaders/shadsky.fs");
    
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray( skyboxVAO );

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid * ) 0);
    glEnableVertexAttribArray(0);

     std::string arr[] = {
    "Images/skybox/right.jpg",
    "Images/skybox/left.jpg",
    "Images/skybox/top.jpg",
    "Images/skybox/bottom.jpg",
    "Images/skybox/front.jpg",
    "Images/skybox/back.jpg"
    };
    std::vector<std::string> faces(arr, arr + sizeof(arr)/ sizeof(std::string));
    unsigned int cubemapTexture = loadCubemap(faces);   

    skyboxshdr->use();
    skyboxshdr->setInt("skybox", 0);

    Shader *lightingshdr = new Shader("shaders/lighting.vs","shaders/lighting.fs");
    Shader *lightsrcshdr = new Shader("shaders/lightsrc.vs", "shaders/lightsrc.fs");
    Texture *txt = new Texture("Images/seamine.jpg");
    Texture *txt1 = new Texture("Images/seamine1.jpg");
    Texture *txt2 = new Texture("Images/seaminenorm.jpg");
    Texture *txt3 = new Texture("Images/seamine2.jpg");
    GLuint VBO, contVAO;
    glGenVertexArrays(1, &contVAO);
    glGenBuffers( 1, &VBO );

    glBindVertexArray( contVAO );
    
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( totvertices ), totvertices, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) (3* sizeof( GLfloat )));
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) (6* sizeof( GLfloat )));
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) (8* sizeof( GLfloat )));
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) (11* sizeof( GLfloat )));
    glEnableVertexAttribArray( 4 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray(0);

    GLuint lightsrcVAO;
    glGenVertexArrays(1, &lightsrcVAO);

    glBindVertexArray( lightsrcVAO );

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 14* sizeof( GLfloat ), (GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        
        float currentframe = glfwGetTime();
        deltaTime=currentframe-lastframe;
        lastframe=currentframe;
        // input
        // -----
        processInput(window);
         
        // render
        // ------
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        lightingshdr->use();
        lightingshdr->setInt("material.diffuse",0);
        lightingshdr->setInt("material.specular",1);
        lightingshdr->setInt("normalMap",2);
        lightingshdr->setInt("material.albedo", 3);
        lightingshdr->setFloat("material.shininess", 8.0f);
        lightingshdr->setVec3("viewPos", camera.Position);
        lightingshdr->setInt("blinn", blinn);
        lightingshdr->setVec3("lightPos", lightPos);

        lightingshdr->setInt("lighting", lighting);
        txt->use(0);
        txt1->use(1);
        txt2->use(2);
        txt3->use(3);

        if(lighting == 0)
        {
            lightingshdr->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingshdr->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            lightingshdr->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            lightingshdr->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        }
        
        if(lighting == 1)
        {
        // point light 1
            lightingshdr->setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingshdr->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
            lightingshdr->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
            lightingshdr->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            lightingshdr->setFloat("pointLights[0].constant", 1.0f);
            lightingshdr->setFloat("pointLights[0].linear", 0.09);
            lightingshdr->setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
            lightingshdr->setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingshdr->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
            lightingshdr->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
            lightingshdr->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            lightingshdr->setFloat("pointLights[1].constant", 1.0f);
            lightingshdr->setFloat("pointLights[1].linear", 0.09);
            lightingshdr->setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
            lightingshdr->setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingshdr->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
            lightingshdr->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
            lightingshdr->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
            lightingshdr->setFloat("pointLights[2].constant", 1.0f);
            lightingshdr->setFloat("pointLights[2].linear", 0.09);
            lightingshdr->setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
            lightingshdr->setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingshdr->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            lightingshdr->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            lightingshdr->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
            lightingshdr->setFloat("pointLights[3].constant", 1.0f);
            lightingshdr->setFloat("pointLights[3].linear", 0.09);
            lightingshdr->setFloat("pointLights[3].quadratic", 0.032);
        }
        if(lighting == 2)
        {
        // spotLight
            lightingshdr->setVec3("spotLight.position", camera.Position);
            lightingshdr->setVec3("spotLight.direction", camera.Front);
            lightingshdr->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingshdr->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingshdr->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingshdr->setFloat("spotLight.constant", 1.0f);
            lightingshdr->setFloat("spotLight.linear", 0.09);
            lightingshdr->setFloat("spotLight.quadratic", 0.032);
            lightingshdr->setFloat("spotLight.cutOff", glm::cos(glm::radians(6.5f)));
            lightingshdr->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.0f)));
        }
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingshdr->setMat4("projection",projection);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        lightingshdr->setMat4("view",view);

        glBindVertexArray(contVAO);
        
        glm::mat4 model = glm::mat4(1.0f);
        lightingshdr->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << " ";
        std::cout <<lighting<<std::endl;
        
        lightsrcshdr->use();
        lightsrcshdr->setMat4("view", view);
        lightsrcshdr->setMat4("projection",projection);
        
        if(lighting == 1)
        {
            glBindVertexArray(lightsrcVAO);
            for (unsigned int i = 0; i < 4; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
                lightsrcshdr->setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
         // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxshdr->use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxshdr->setMat4("view", view);
        skyboxshdr->setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays( 1, &contVAO );
    glDeleteVertexArrays(1, &lightsrcVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = deltaTime*5.0f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
        blinnKeyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lightingkeypress)
       { lighting = (lighting+1)%3;
        lightingkeypress = true;}
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
        lightingkeypress = false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}  
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
} 

void verticesfind(float *vertices, int no)
{
        glm::vec3 pos1(vertices[0], vertices[1], vertices[2]);
        glm::vec3 pos2(vertices[8], vertices[9], vertices[10]);
        glm::vec3 pos3(vertices[16], vertices[17], vertices[18]);
        glm::vec3 pos4(vertices[32], vertices[33], vertices[34]);
        // texture coordinates
        glm::vec2 uv1(vertices[6], vertices[7]);
        glm::vec2 uv2(vertices[14], vertices[15]);
        glm::vec2 uv3(vertices[22], vertices[23]);  
        glm::vec2 uv4(vertices[38], vertices[39]);
        // normal vector
        glm::vec3 nm(vertices[3],vertices[4], vertices[5]);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent1 = glm::normalize(bitangent1);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent2 = glm::normalize(bitangent2);


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        for (int i = 0; i < 14*6; ++i)
        {
            /* code */
            totvertices[no*14*6 + i] = quadVertices[i];
        }
}