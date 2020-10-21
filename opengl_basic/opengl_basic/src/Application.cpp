#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<ctime>
#include<fstream>
#include<string>
#include<sstream>
#include "Render.h"
#include "Vertexbuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

 
int main()
{
   
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    else
        std::cout << "heyy";
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

     


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "ERROR";
    {

    float position[] = {

       0.5f,-0.5f, 0.0f, 0.0f,  //0
       0.5f, 0.5f, 1.0f, 0.0f,  //1
       -0.5f,0.5f, 1.0f, 1.0f,  //2
       -0.5f,-0.5f, 0.0f ,1.0f  //3

    };
    unsigned int ind_buffer[] =
    {
        0,1,3,
        2,3,1

    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA));  //To blend alpha pixels , The first one is the source and the second one is the destination
   

    
    Vertexbuffer vb(position, 4 * 4 * sizeof(float)); //Bcz We have 8 values
    VertexBufferLayout layout;
    layout.Push<float>(2.0f);
    layout.Push<float>(2.0f);
    VertexArray va;
    va.AddBuffer(vb, layout);

    float red[] = { 1.0,1.0,0.0,1.0 };

    Indexbuffer ib(ind_buffer, 6);
  
    Shader shader("resources/shaders/Basic.shader");
    shader.Bind();
    shader.SetSetUniform("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    Texture texture("resources/textures/messi.jpg");
    texture.Bind();
    shader.SetSetUniform1i("u_Texture", 0);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.Unbind();
    float r = 0.0f;
    float inc = 0.05;
    Renderer renderer;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        //glClearBufferfv(GL_COLOR, 0, red);
       shader.Bind();
       shader.SetSetUniform("u_Color", r, 0.3f, 0.8f, 1.0f);  //Will change on adding materials
       
       renderer.Draw(va, ib, shader);

        if (r > 1.0f)
            inc = -0.05;
        else if (r < 0.0f)
            inc = inc + 0.05;
        r = r + inc;


        /* Swap front and back buffers */

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    }
    glfwTerminate();
    return 0;
}