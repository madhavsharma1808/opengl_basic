#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<ctime>
#include<fstream>
#include<string>
#include<sstream> 

#define ASSERT(x) if (!(x))  __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(#x,__FILE__ ,__LINE__))


static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);   
}

static bool GLCheckError(const char*function,const char* file,int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGl Error " <<( error)<<" "<<file
            <<" "<<function<<" : "<<line<<std::endl;
        return false;
    }
    return true;
}
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }

        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(),ss[1].str() };
}
static unsigned int CompileShader(unsigned int type,const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); //copies the src code into shader object
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //will ewruen the compile status
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program=glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    //deleting bcz once attached to the progrma they are not needed
    return program;
}
 
int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    
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


    float position[] = {
       
       0.5f,-0.5f,   //0
       0.5f,0.5f,    //1
       -0.5f,0.5f,   //2
       -0.5f,-0.5f,  //3
       
    };
    unsigned int ind_buffer[] =
    {
        0,1,3,
        2,3,1

    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1,&vao));
    GLCall(glBindVertexArray(vao));

    float red[] = { 1.0,1.0,0.0,1.0 };
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 12* sizeof(float),position,GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(float), ind_buffer, GL_STATIC_DRAW);

    

    

    std::string path = "resources/shaders/Basic.shader";
    ShaderProgramSource source = ParseShader(path);
    std::cout << source.VertexSource << source.FragmentSource;
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    GLint location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    float r = 0.0f;
    float inc = 0.05;
    
    GLCall(glBindVertexArray(0));
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
   
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glClearBufferfv(GL_COLOR, 0, red);
        glUseProgram(shader);
        GLCall(glUniform4f(location, r, 0.5f, 0.7f, 1.0f));
        GLCall(glBindVertexArray(vao));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
       

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
       

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
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}