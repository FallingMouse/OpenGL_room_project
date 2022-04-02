#undef GLFW_DLL
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include "Libs/Shader.h"
#include "Libs/Window.h"
#include "Libs/Mesh.h"
#include "Libs/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

float yaw = 0.0f, pitch = 0.0f;

unsigned int squareTextures[4];

//Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

//Numbers of Object to draw
static int numOfDraw = 13;

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        //pos                   //TexCoord
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        0.0, -1.0f, 1.0f,       0.5f, 0.0f,
        1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2,
    };

    Mesh *obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 20, 12);
    
    // for(int i = 0; i < 2; i++) {
    //     meshList.push_back(obj1);
    // }
}

void CreateSquare()
{
    GLfloat SQvertices[] =
    {
        // up
        1.0f, 1.0f, 1.0f,       0.0f, 1.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,      1.0f, 1.0f,

        // down
        1.0f, -1.0f, 1.0f,      0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 1.0f,

        // left
        -1.0f, 1.0f, 1.0f,      0.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     1.0f, 1.0f,

        // right
        1.0f, 1.0f, 1.0f,       0.0f, 1.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,      1.0f, 1.0f,

        // front
        1.0f, 1.0f, 1.0f,       0.0f, 1.0f,
        1.0f, -1.0f, 1.0f,      0.0f, 1.0f, 
        -1.0f, -1.0f, 1.0f,     1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,      1.0f, 1.0f,

        // back
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f,
        -1.0f, 1.0f, -1.0f,     1.0f, 1.0f,
    };

    unsigned int SQindices[] = 
    {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20, 

    };

    Mesh *obj2 = new Mesh();
    obj2 -> CreateMesh(SQvertices, SQindices, 5*24, 3*12);
    for(int i = 0; i < 1 + numOfDraw; i++) {
        meshList.push_back(obj2);
    }
}


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    static float lastX = mainWindow.getBufferWidth() / 2.0f;
    static float lastY = mainWindow.getBufferHeight() / 2.0f;

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f)   pitch = 89.0f;
    if(pitch < -89.0f)  pitch = -89.0f;
}

unsigned int loadTexture(const char *textureFile){

    //texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFile, &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        //bind image with data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"Failed to load texture"<<std::endl;
    }

    stbi_image_free(data);
    return texture;
}

int main()
{
    mainWindow = Window(WIDTH, HEIGHT, 3, 3);
    mainWindow.initialise();

    CreateSquare();

    CreateShaders();

    GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
    
    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPos = glm::vec3(1.0f, 0.5f, 2.0f);
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, -0.3f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraDirection = glm::normalize(cameraTarget - cameraPos);

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

    glfwSetCursorPosCallback(mainWindow.getWindow(), mouse_callback);
    glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    unsigned int textureContainer = loadTexture("Textures/container.jpg");
    unsigned int textureCloth = loadTexture("Textures/cloth.jpg");
    unsigned int textureWall = loadTexture("Textures/wall.jpg");
    unsigned int textureFloor = loadTexture("Textures/floor.jpg");
    unsigned int textureWoodPlate1 = loadTexture("Textures/woodPlate1.jpg");
    unsigned int textureWoodPlate2 = loadTexture("Textures/woodPlate2.jpg");


    //Loop until window closed
    while (!mainWindow.getShouldClose())
    {
        //Get + Handle user input events
        glfwPollEvents();

        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraDirection = glm::normalize(direction);

        cameraRight = glm::normalize(glm::cross(cameraDirection, up));
        cameraUp = glm::normalize(glm::cross(cameraDirection, -cameraRight));

        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraDirection * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraDirection * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= cameraRight * 0.01f;
        if(glfwGetKey(mainWindow.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += cameraRight * 0.01f;

        //Clear window
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearColor(186.0f/255.0f, 228.0f/255.0f, 228.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw here
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetUniformLocation("model");
        uniformView = shaderList[0].GetUniformLocation("view");
        uniformProjection = shaderList[0].GetUniformLocation("projection");


        glm::vec3 squarePositions[] =
        {
            //         Positions            // i    description
            // wall
            glm::vec3(1.0f, 1.0f, 0.0f),    // 0    wall 1
            glm::vec3(1.0f, 0.0f, 1.0f),    // 1    floor
            glm::vec3(0.0f, 1.0f, 1.0f),    // 2    wall 2

            // top, bottom plate wall 2
            glm::vec3(3.0f, 79.0f, 1.0f),   // 3    top
            glm::vec3(3.0f, 2.0f, 1.0f),    // 4    bottom

            // top, bottom plate wall 1
            glm::vec3(1.0f, 79.0f, 3.0f),   // 5    top
            glm::vec3(1.0f, 2.0f, 3.0f),    // 6    bottom

            // middle plate wall 2
            glm::vec3(3.0f, 21.0f, 1.0f),   // 7    middle-top
            glm::vec3(3.0f, 9.0f, 1.0f),    // 8    middle-bottom

            // middle plate wall 1
            glm::vec3(1.0f, 21.0f, 3.0f),   // 9    middle-top

            //vertical plate wall 2
            glm::vec3(3.0f, 1.0f, 23.0f),   // 10   vertical-left
            glm::vec3(3.0f, 1.0f, 9.0f),    // 11   vertical-right

            //vertical plate wall 1
            glm::vec3(9.0f, 1.0f, 3.0f),    // 12   vertical-left

            //corner plate
            glm::vec3(2.0f, 1.0f, 2.0f)     // 13   corner
            
        };
        
        glm::mat4 view (1.0f);

        glm::mat4 cameraPosMat (1.0f);
        cameraPosMat[0][3] = -cameraPos.x;
        cameraPosMat[1][3] = -cameraPos.y;
        cameraPosMat[2][3] = -cameraPos.z;

        glm::mat4 cameraRotateMat(1.0f);
        cameraRotateMat[0] = glm::vec4(cameraRight.x, cameraUp.x, -cameraDirection.x, 0.0f);
        cameraRotateMat[1] = glm::vec4(cameraRight.y, cameraUp.y, -cameraDirection.y, 0.0f);
        cameraRotateMat[2] = glm::vec4(cameraRight.z, cameraUp.z, -cameraDirection.z, 0.0f);
        
        // view = cameraRotateMat * glm::transpose(cameraPosMat);
        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, up);

        //Object
        glm::mat4 model(1.0f);

        for (int i = 0; i < 1 + numOfDraw; i++)
        {
            glm::mat4 model (1.0f);

            // wall
            if(i == 0)  // wall 1
            {
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.02f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWall;
            }
            else if(i == 1) // floor
            {
                model = glm::scale(model, glm::vec3(1.0f, 0.02f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureFloor;
            }
            else if(i == 2) // wall 2
            {
                model = glm::scale(model, glm::vec3(0.02f, 1.0f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWall;
            }
            // wood plate
            else if(i <= 4) // (top/bottom) plate wall 2
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.025f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate2;
            }
            else if(i <= 6) // (top/bottom) plate wall 1
            { 
                model = glm::scale(model, glm::vec3(1.0f, 0.025f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate2;
            }
            else if(i <= 8) // middle plate wall 2
            {
                model = glm::scale(model, glm::vec3(0.01f, 0.075f, 1.0f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate2;
            }
            else if(i == 9) // middle-top plate wall 1
            {
                model = glm::scale(model, glm::vec3(1.0f, 0.075f, 0.01f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate2;
            }
            else if(i <= 11) // vertical plate wall 2
            {
                model = glm::scale(model, glm::vec3(0.015f, 1.0f, 0.075f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate1;
            }
            else if(i == 12) // vertical plate wall 1
            {
                model = glm::scale(model, glm::vec3(0.075f, 1.0f, 0.015f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate1;
            }
            else if(i == 13) // corner plate
            {
                model = glm::scale(model, glm::vec3(0.025f, 1.0f, 0.025f));
                model = glm::translate(model, squarePositions[i]);
                squareTextures[i] = textureWoodPlate1;
            }

            
            // model = glm::translate(model, squarePositions[i]);
            // model = glm::translate(model, skyboxPosition[i]);
            // model = glm::rotate(model, glm::radians(2.0f * i) ,glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, squareTextures[i]);
            meshList[i]->RenderMesh();
        }

        glUseProgram(0);
        //end draw

        mainWindow.swapBuffers();
    }

    return 0;
}
