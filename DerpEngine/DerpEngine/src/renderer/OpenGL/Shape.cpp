#include "Shape.h"
#include "../../core/ecs/systems/System.h"
#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/Components.h"

#include <GL/glew.h>

#include <sstream>

namespace DERP
{
	namespace SHAPE 
	{
		unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path);

		GLuint shaderID;
		std::vector<ShapesData> shapesList;

		void executeDraw(glm::mat4 View, glm::mat4 Projection)
		{
			for (auto x : shapesList)
			{
				if (x.type == POINT)
				{
					//Make 4 points
					const float t = 0.5;
					static const GLfloat g_vertex_buffer_data[] = {
						-t, t, 0,
						t, t, 0,
						-t, -t, 0,
						t, t, 0,
						t, -t, 0
						-t, -t, 0,
					};

					glm::mat4 trans = glm::mat4(1.0f);
					trans = glm::translate(trans, x.position);
					//trans = trans * glm::toMat4(t->rotation);
					//trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));

					glUseProgram(shaderID);

					GLuint ModelID = glGetUniformLocation(shaderID, "Model");
					GLuint ViewID = glGetUniformLocation(shaderID, "View");
					GLuint ProjectionID = glGetUniformLocation(shaderID, "Projection");

					glUniformMatrix4fv(ModelID, 1, GL_FALSE, &trans[0][0]);
					glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
					glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);

					//Make vertex buffer
					GLuint vertexbuffer;
					glGenBuffers(1, &vertexbuffer);
					glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
					glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glDisableVertexAttribArray(0);
				}
			}
		}

		void addShape(glm::vec3 position, Shapes type)
		{
			ShapesData s;
			s.position = position;
			s.type = type;

			shapesList.push_back(s);
		}

		void initShapes()
		{
			shaderID = compileShaders("Shape_Vertex.glsl", "Shape_Pixel.glsl");
		}

		unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path)
		{
			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			// Read the Vertex Shader code from the file
			std::string VertexShaderCode;
			std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
			if (VertexShaderStream.is_open()) {
				std::stringstream sstr;
				sstr << VertexShaderStream.rdbuf();
				VertexShaderCode = sstr.str();
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
				std::stringstream sstr;
				sstr << FragmentShaderStream.rdbuf();
				FragmentShaderCode = sstr.str();
				FragmentShaderStream.close();
			}

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			printf("Compiling shader : %s\n", vertex_file_path);
			char const* VertexSourcePointer = VertexShaderCode.c_str();
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
			char const* FragmentSourcePointer = FragmentShaderCode.c_str();
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

			return (uint32_t)ProgramID;
		}

	}
}