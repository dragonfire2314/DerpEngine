#include "OpenGL.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtc/quaternion.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/systems/System.h"

#include "Shape.h"

#include <unordered_map>
#include <GL/glew.h>

#include <sstream>

namespace DERP {

	unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint shaderNum;

	void OpenGL::Render()
	{
		//printf("Start\n");

		for (auto x : sys_cameras->Entities) { //Render screen once per camera
			//TODO - Allow cameras to choose render method (screen space, or texture)
			//Setup veiw matrix before call
			Transform* t = CM::GetComponent<Transform>(x);
			glm::vec3 forward = t->rotation * glm::vec3(0, 0, -1);
			//glm::vec3 euler = glm::eulerAngles(t->rotation) * 180.0f / 3.14159f;
			View = glm::lookAt(t->position, t->position + forward, glm::vec3(0, 1, 0));

			mainCamPos = t->position;

			light.setCameraPosition(mainCamPos);

			//t->print();

			RenderMainCamera();
		}
	}

	void OpenGL::RenderMainCamera() 
	{
		//Must render all mesh components

		//Loop though meshes
		for (auto x : sys_renderer->Entities)
		{
			Mesh* mesh = CM::GetComponent<Mesh>(x);
			Material* mat = CM::GetComponent<Material>(x);

			//Check if mesh is valid
			if (mesh->mesh == nullptr) continue;

			//Move child
 			Transform* t = CM::GetComponent<Transform>(x);

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, t->position);
			trans = trans * glm::toMat4(t->rotation);
			trans = glm::scale(trans, t->scale);

			//Makes sure childern move wiht parents
			{
				Entity* entityTemp = EM::getEntity(x)->parent;
				while (entityTemp != nullptr) {
					uint32_t p = entityTemp->ID;
					Transform* pt = CM::GetComponent<Transform>(p);

					trans = glm::translate(trans, pt->position);
					trans = trans * glm::toMat4(pt->rotation);
					trans = glm::scale(trans, pt->scale);

					entityTemp = EM::getEntity(entityTemp->ID);
					if (entityTemp == nullptr) break;
					else entityTemp = entityTemp->parent;
				}
			}

			// Our ModelViewProjection
			GLuint ModelID =      glGetUniformLocation(shaderNum, "Model");
			GLuint ViewID =       glGetUniformLocation(shaderNum, "View");
			GLuint ProjectionID = glGetUniformLocation(shaderNum, "Projection");

			glUniformMatrix4fv(ModelID, 1, GL_FALSE, &trans[0][0]);
			glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
			glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);

			//Tells the shader if it should use a texture or not (Will have to make the sytem use a differnt shader in this event)
			GLuint isDiffuseID = glGetUniformLocation(shaderNum, "isDiffuse");
			glUniform1i(isDiffuseID, mat->mat->isDiffuseTexture);

			//Give shader a color for the objects
			GLuint diffuseColorID = glGetUniformLocation(shaderNum, "diffuseColor");
			glUniform3f(diffuseColorID, mat->mat->Kd.X, mat->mat->Kd.Y, mat->mat->Kd.Z);

			//Lighting Stuff
			//light.UpdateDirectionalLight(shader.getShader(x));
			light.UpdatePointLight(shaderNum);

			//GLuint viewPosID = glGetUniformLocation(shaderNum, "viewPos");
			//glUniform3f(viewPosID, mainCamPos.x, mainCamPos.y, mainCamPos.z);

			glUseProgram(shaderNum);

			//Bind stuff
			//Vertex Buffer
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, VBManager->getVertexBuffer(x));
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			//Normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			//Tex Coord
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			//Index Buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBManager->getIndexBuffer(x));
			//Texture
			glBindTexture(GL_TEXTURE_2D, textureManagerGL.getTexture(x));
			glBindVertexArray(VertexArrayID);

			//Draw Call
			//glDrawArrays(GL_TRIANGLES, 0, x.second->mesh->Vertices.size());
			glDrawElements(
				GL_TRIANGLES,                      // mode
				mesh->mesh->Indices.size(),        // count
				GL_UNSIGNED_INT,                   // type
				(void*)0                           // element array buffer offset
			);
			//Unbind stuff
			glDisableVertexAttribArray(0); 
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			//Draw Shapes on top of everyting
		}
	}

	void OpenGL::RenderNonMainCameras() 
	{
		
	}

	void OpenGL::ClearScreen()
	{
		glClearColor(0.2, 0.1, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::SetUp()
	{
		shaderNum = compileShaders("vertex_shadow.glsl", "pixel_shadow.glsl");

		VBManager = new VertexBufferManger();

		/*******************
		*	Depth
		********************/
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		/*******************
		*	VBA and shaders
		********************/
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
		//Load the shaders
		shader.loadShaders();
		//Load the vertexBuffers
		VBManager->makeVertexBuffers();


		/*******************
		*	Projection and Camera
		********************/
		Projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);

		View = glm::lookAt(
			glm::vec3(0, 0, 4),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0) 
		);

		/*******************
		*	Texture
		********************/
		//Wrap mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//Scaling filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Mip map img filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Load all textures that are needed
		textureManagerGL.LoadTextures();

		/*******************
		*	Lighting
		********************/
		light.InitLights(shaderNum);
	}

	void OpenGL::updateMesh(uint32_t entityID)
	{
		VBManager->updateVetexBuffer(entityID);
	}

	void OpenGL::updateShader(uint32_t entityID)
	{
		shader.updateShader(entityID);
		////textureManagerGL.LoadTextures();
	}

	void OpenGL::updateTexture(uint32_t entityID) 
	{
		textureManagerGL.updateTexture(entityID);
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