#include <string>
#include <glew.h>
#include <glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "shader.h"
using namespace std;


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

struct Triangulo
{
    GLfloat* vertexBufferData;
    GLfloat* vertexColorData;
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    GLuint vertexcolor;
};
Triangulo tri;

MyShader myshader;

void initResources()
{
    //Shader
    myshader = CreateShaderProgram("//home//geronimo//programacao//linkando_com_o_clion//SimpleVertexShader.vertexshader",
                                   "//home//geronimo//programacao//linkando_com_o_clion//SimpleFragmentShader.fragmentshader");
    //Gambi pra inicializar o buffer
    static const GLfloat localData[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };
    tri.vertexBufferData = new GLfloat[9];
    memcpy(tri.vertexBufferData, localData, sizeof(localData));

    static const GLfloat localColor[] = {
            1.0f, 0.0f, 0.0, 1.0f,
            1.0f, 1.0f, 0.0, 1.0f,
            1.0f, 1.0f, 1.0, 1.0f,
    };
    tri.vertexColorData = new GLfloat[12];
    memcpy(tri.vertexColorData, localColor, sizeof(localColor));
    //cria o vertex array object e os buffers dele.
    glGenVertexArrays(1, &tri.VertexArrayID);
    glBindVertexArray(tri.VertexArrayID);
    glGenBuffers(1, &tri.vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tri.vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), const_cast<GLfloat*>(tri.vertexBufferData ), GL_STATIC_DRAW);
    glGenBuffers(1, &tri.vertexcolor);
    glBindBuffer(GL_ARRAY_BUFFER, tri.vertexcolor);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), const_cast<GLfloat*>(tri.vertexColorData), GL_STATIC_DRAW);

}

void idle()
{
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

}

void reshape(int w, int h)
{

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // Use our shader
    glUseProgram(myshader.programId);
    //Eu sei que o nome do atributo dos v?rtices ? vertexPosition_modelspace.
    //Usando o atributo da geometria.
    glEnableVertexAttribArray(myshader.attributes["vertexPosition_modelspace"]);
    glBindBuffer(GL_ARRAY_BUFFER, tri.vertexbuffer);
    glVertexAttribPointer(
            myshader.attributes["vertexPosition_modelspace"],                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );
    //Agora ? o atributo da cor
    glEnableVertexAttribArray(myshader.attributes["vertexColor"]);
    glBindBuffer(GL_ARRAY_BUFFER, tri.vertexcolor);
    glVertexAttribPointer(
            myshader.attributes["vertexColor"],                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle
    glDisableVertexAttribArray(0);

    glFlush();
    glutSwapBuffers();
}

//Fun??o principal do programa.
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("NeHe02 - First Polygons");
    //tenta iniciar a glew
    GLenum glError = glewInit();
    if (glError != GLEW_OK)
    {
        const GLubyte* _err = glewGetErrorString(glError);
        std::string str((char*)_err);
        std::cerr << "Erro na inicializa??o da GLEW:" << str << std::endl;
        return EXIT_FAILURE;
    }
    initResources();
    //seta os callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    glDeleteBuffers(1, &tri.vertexbuffer);
    glDeleteVertexArrays(1, &tri.VertexArrayID);
    glDeleteProgram(myshader.programId);

    return EXIT_SUCCESS;
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }



    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

