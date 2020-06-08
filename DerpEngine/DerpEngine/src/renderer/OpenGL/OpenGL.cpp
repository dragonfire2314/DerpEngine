#include "OpenGL.h"

#include "../../../external/glm/glm.hpp"
#include "../../../external/glm/gtc/quaternion.hpp"
#include "../../../external/glm/gtx/quaternion.hpp"

#include "../../core/ecs/ComponentManager.h"
#include "../../core/ecs/systems/System.h"

#include "../../core/Animation/AnimationManager.h"

#include "Shape.h"

#include <unordered_map>
#include <GL/glew.h>

#include <sstream>

namespace DERP {

	unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint shaderNum;
	GLuint postProcessing;

	GLuint standardVertexVAO;
	GLuint framebufferRenderVAO;

	void OpenGL::Render()
	{
		//printf("Start\n");

		for (auto x : sys_cameras->Entities) { //Render screen once per camera
			//TODO - Allow cameras to choose render method (screen space, or texture)
			setupCameraForFrame(x);
			
			RenderToTexture();

			renderFrameBufferToScreen();
		}
	}

	void OpenGL::setupCameraForFrame(uint32_t camera)
	{
		//Setup veiw matrix before call
		Transform* t = CM::GetComponent<Transform>(camera);
		glm::vec3 forward = t->rotation * glm::vec3(0, 0, -1);
		//glm::vec3 euler = glm::eulerAngles(t->rotation) * 180.0f / 3.14159f;
		View = glm::lookAt(t->position, t->position + forward, glm::vec3(0, 1, 0));

		mainCamPos = t->position;

		light.setCameraPosition(mainCamPos);
	}

	void* OpenGL::RenderToTexture()
	{
		setupCameraForFrame(mainCamera);

		setupFrameBuffer();

		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(standardVertexVAO);

		Entity* rootEntity = EM::getEntity(0);
		RenderMainCamera(rootEntity);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return &renderedTexture;
	}

	//Recursively renders objects in entity tree
	void OpenGL::RenderMainCamera(Entity* baseEntity)
	{
		//Must render all mesh components

		//Loop though entities
		for (auto x : baseEntity->childern)
		{
			if (CM::IsComponent<Mesh>(x) && CM::IsComponent<Material>(x)) {
				Mesh* mesh = CM::GetComponent<Mesh>(x);
				Material* mat = CM::GetComponent<Material>(x);

				//Check if mesh is valid
				if (mesh->mesh == nullptr) continue;

				//Generate to world matrix
				Transform* t = CM::GetComponent<Transform>(x);
				Transform* parentT = CM::GetComponent<Transform>(baseEntity->ID);

				//Apply transforms to the parents matrix
				glm::mat4 trans = parentT->toWorld;
				trans = glm::translate(trans, t->position);
				trans = trans * glm::toMat4(t->rotation);
				trans = glm::scale(trans, t->scale);
				//Multiply with parents to world matrix
				//Store new toWorld matrix in entity trnasform
				t->toWorld = trans;// * parentT->toWorld;  //Can probably change this later to be B * A instead of A * B if the trans starts at 1

				//Render code
				{

					glUseProgram(shaderNum);

					// Our ModelViewProjection
					GLuint ModelID = glGetUniformLocation(shaderNum, "Model");
					GLuint ViewID = glGetUniformLocation(shaderNum, "View");
					GLuint ProjectionID = glGetUniformLocation(shaderNum, "Projection");

					glUniformMatrix4fv(ModelID, 1, GL_FALSE, &trans[0][0]);
					glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
					glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);

					//Tells the shader if it should use a texture or not (Will have to make the sytem use a differnt shader in this event)
					GLuint isDiffuseID = glGetUniformLocation(shaderNum, "isDiffuse");
					glUniform1i(isDiffuseID, mat->mat->isDiffuseTexture);

					//Tell shader if it should use animationBones or not
					GLuint isAnimatedID = glGetUniformLocation(shaderNum, "isAnimated");
					glUniform1i(isAnimatedID, mesh->mesh->isAnimation);

					if (mesh->mesh->isAnimation) {
						updateAnimations(shaderNum, x);
					}

					//Give shader a color for the objects
					GLuint diffuseColorID = glGetUniformLocation(shaderNum, "diffuseColor");
					glUniform3f(diffuseColorID, mat->mat->Kd.X, mat->mat->Kd.Y, mat->mat->Kd.Z);

					//Lighting Stuff
					//light.UpdateDirectionalLight(shader.getShader(x));
					light.UpdatePointLight(shaderNum);

					//GLuint viewPosID = glGetUniformLocation(shaderNum, "viewPos");
					//glUniform3f(viewPosID, mainCamPos.x, mainCamPos.y, mainCamPos.z);

					

					//Bind stuff
					//Vertex Buffer
					glBindVertexBuffer(0, VBManager->getVertexBuffer(x), 0, 16 * sizeof(float));
					//Index Buffer
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBManager->getIndexBuffer(x));
					//Texture
					glBindTexture(GL_TEXTURE_2D, textureManagerGL.getTexture(x));
					//Draw Call
					glDrawElements(
						GL_TRIANGLES,                      // mode
						mesh->mesh->Indices.size(),        // count
						GL_UNSIGNED_INT,                   // type
						(void*)0                           // element array buffer offset
					);
				}
			}

			RenderMainCamera(EM::getEntity(x));
		}
	}

	void OpenGL::renderFrameBufferToScreen()
	{
		//Return to sceen space rendering
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, renderWidth, renderHeight);
		glDisable(GL_DEPTH_TEST);

		//Draw the texture to the window
		glUseProgram(postProcessing);
		//Bind stuff
		glBindVertexArray(framebufferRenderVAO);
		//Vertex Buffer
		glBindVertexBuffer(0, postQuadVerts, 0, 5 * sizeof(float));
		//Index Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postQuadInds);
		//Texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		//Draw Call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	}

	void OpenGL::setupFrameBuffer()
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			assert(0);
		// Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, renderWidth, renderHeight);
		ClearScreen();
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
		Projection = glm::perspective(glm::radians(45.0f), (float)renderWidth / (float)renderHeight, 0.1f, 100.0f);

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

		/*******************
		*	Frame
		********************/
		//Set up the frmae buffer
		FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		// The texture we're going to render to
		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderWidth, renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// The depth buffer
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
		//Need a program to display the framebuffer
		postProcessing = compileShaders("post_vertex.glsl", "post_pixel.glsl");
		//Need a quad to render on
		struct tempVertex {
			objl::Vector3 Position;
			objl::Vector2 TextureCoordinate;
		};
		std::vector<tempVertex> postQuad;
		tempVertex v0; tempVertex v1; tempVertex v2; tempVertex v3;
		v0.Position = objl::Vector3(-1, 1, 0); v0.TextureCoordinate = objl::Vector2(0, 1);
		v1.Position = objl::Vector3(1, 1, 0);  v1.TextureCoordinate = objl::Vector2(1, 1);
		v2.Position = objl::Vector3(-1, -1, 0);v2.TextureCoordinate = objl::Vector2(0, 0);
		v3.Position = objl::Vector3(1, -1, 0); v3.TextureCoordinate = objl::Vector2(1, 0);
		postQuad.push_back(v0); postQuad.push_back(v1); postQuad.push_back(v2); postQuad.push_back(v3);
		std::vector<uint32_t> indecies;
		indecies.push_back(0); indecies.push_back(1); indecies.push_back(2);
		indecies.push_back(1); indecies.push_back(3); indecies.push_back(2);
		glGenBuffers(1, &postQuadVerts);
		glBindBuffer(GL_ARRAY_BUFFER, postQuadVerts);
		glBufferData(GL_ARRAY_BUFFER, postQuad.size() * sizeof(objl::Vertex),
			postQuad.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &postQuadInds);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postQuadInds);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int),
			indecies.data(), GL_STATIC_DRAW);


		/*******************
		*	VAO's
		********************/
		int vertexBindingPoint = 0;

		glGenVertexArrays(1, &standardVertexVAO);
		glBindVertexArray(standardVertexVAO);

		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, vertexBindingPoint);
		glEnableVertexAttribArray(0);
		//Normals
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		glVertexAttribBinding(1, vertexBindingPoint);
		glEnableVertexAttribArray(1);
		//Tex Coord
		glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
		glVertexAttribBinding(2, vertexBindingPoint);
		glEnableVertexAttribArray(2);
		//BoneIDs
		glVertexAttribFormat(3, 4, GL_UNSIGNED_INT, GL_FALSE, 8 * sizeof(float));
		glVertexAttribBinding(3, vertexBindingPoint);
		glEnableVertexAttribArray(3);
		//Bone weights
		glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float));
		glVertexAttribBinding(4, vertexBindingPoint);
		glEnableVertexAttribArray(4);

		//Frame BufferRenderVAO
		glGenVertexArrays(1, &framebufferRenderVAO);
		glBindVertexArray(framebufferRenderVAO);

		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, vertexBindingPoint);
		glEnableVertexAttribArray(0);
		//Tex Coord
		glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		glVertexAttribBinding(2, vertexBindingPoint);
		glEnableVertexAttribArray(2);

		/*******************
		*	Main Camera Temp
		********************/
		mainCamera = sys_cameras->Entities[0];
	}

	void OpenGL::updateFrameBuffer()
	{
		//Delete old stuff
		glDeleteTextures(1, &renderedTexture);
		glDeleteFramebuffers(1, &FramebufferName);
		glDeleteRenderbuffers(1, &depthrenderbuffer);

		//Set up the frmae buffer
		FramebufferName = 0;
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		// The texture we're going to render to
		glGenTextures(1, &renderedTexture);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderWidth, renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// The depth buffer
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		//Fix projection
		Projection = glm::perspective(glm::radians(45.0f), (float)renderWidth / (float)renderHeight, 0.1f, 100.0f);
	}

	void OpenGL::updateMesh(uint32_t entityID)
	{
		VBManager->updateVetexBuffer(entityID);
	}

	void OpenGL::updateShader(uint32_t entityID)
	{
		shader.updateShader(entityID);
		//textureManagerGL.LoadTextures();
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





//#include "OpenGL.h"
//
//#include "../../../external/glm/glm.hpp"
//#include "../../../external/glm/gtc/quaternion.hpp"
//#include "../../../external/glm/gtx/quaternion.hpp"
//
//#include "../../core/ecs/ComponentManager.h"
//#include "../../core/ecs/systems/System.h"
//
//#include "../../core/Animation/AnimationManager.h"
//
//#include "Shape.h"
//
//#include <unordered_map>
//#include <GL/glew.h>
//
//#include <sstream>
//
//namespace DERP {
//
//	unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path);
//	GLuint shaderNum;
//	GLuint postProcessing;
//
//	void OpenGL::Render()
//	{
//		//printf("Start\n");
//
//		for (auto x : sys_cameras->Entities) { //Render screen once per camera
//			//TODO - Allow cameras to choose render method (screen space, or texture)
//			//Setup veiw matrix before call
//			Transform* t = CM::GetComponent<Transform>(x);
//			glm::vec3 forward = t->rotation * glm::vec3(0, 0, -1);
//			//glm::vec3 euler = glm::eulerAngles(t->rotation) * 180.0f / 3.14159f;
//			View = glm::lookAt(t->position, t->position + forward, glm::vec3(0, 1, 0));
//
//			mainCamPos = t->position;
//
//			light.setCameraPosition(mainCamPos);
//
//			// Set "renderedTexture" as our colour attachement #0
//			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
//			// Set the list of draw buffers.
//			GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
//			glDrawBuffers(1, DrawBuffers);
//			// Always check that our framebuffer is ok
//			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//				assert(0);
//			// Render to our framebuffer
//			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//			glViewport(0, 0, 1024, 768);
//			ClearScreen();
//
//			glEnable(GL_DEPTH_TEST);
//			RenderToTexture();
//
//			//Return to sceen space rendering
//			glBindFramebuffer(GL_FRAMEBUFFER, 0);
//			glViewport(0, 0, 1024, 768);
//			glDisable(GL_DEPTH_TEST);
//
//			//Draw the texture to the window
//			glUseProgram(postProcessing);
//			//Bind stuff
//			//Vertex Buffer
//			glEnableVertexAttribArray(0);
//			glBindBuffer(GL_ARRAY_BUFFER, postQuadVerts);
//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
//			//Normals
//			glEnableVertexAttribArray(1);
//			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
//			//Tex Coord
//			glEnableVertexAttribArray(2);
//			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(6 * sizeof(float)));
//			//BoneIDs
//			glEnableVertexAttribArray(3);
//			glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
//			//Bone weights
//			glEnableVertexAttribArray(4);
//			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
//			//Index Buffer
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postQuadInds);
//			//Texture
//			glBindTexture(GL_TEXTURE_2D, renderedTexture);
//			glBindVertexArray(VertexArrayID);
//			//Draw Call
//			glDrawElements(
//				GL_TRIANGLES,                      // mode
//				6,								   // count
//				GL_UNSIGNED_INT,                   // type
//				(void*)0                           // element array buffer offset
//			);
//			//Unbind stuff
//			glDisableVertexAttribArray(0);
//			glDisableVertexAttribArray(1);
//			glDisableVertexAttribArray(2);
//			glDisableVertexAttribArray(3);
//			glDisableVertexAttribArray(4);
//		}
//	}
//
//	void* OpenGL::RenderToTexture()
//	{
//		Entity* rootEntity = EM::getEntity(0);
//		RenderMainCamera(rootEntity);
//
//		return &renderedTexture;
//	}
//
//	//Recursively renders objects in entity tree
//	void OpenGL::RenderMainCamera(Entity* baseEntity)
//	{
//		//Must render all mesh components
//
//		//Loop though entities
//		for (auto x : baseEntity->childern)
//		{
//			if (CM::IsComponent<Mesh>(x) && CM::IsComponent<Material>(x)) {
//				Mesh* mesh = CM::GetComponent<Mesh>(x);
//				Material* mat = CM::GetComponent<Material>(x);
//
//				//Check if mesh is valid
//				if (mesh->mesh == nullptr) continue;
//
//				//Generate to world matrix
//				Transform* t = CM::GetComponent<Transform>(x);
//				Transform* parentT = CM::GetComponent<Transform>(baseEntity->ID);
//
//				//Apply transforms to the parents matrix
//				glm::mat4 trans = parentT->toWorld;
//				trans = glm::translate(trans, t->position);
//				trans = trans * glm::toMat4(t->rotation);
//				trans = glm::scale(trans, t->scale);
//				//Multiply with parents to world matrix
//				//Store new toWorld matrix in entity trnasform
//				t->toWorld = trans;// * parentT->toWorld;  //Can probably change this later to be B * A instead of A * B if the trans starts at 1
//
//				//Render code
//				{
//
//					glUseProgram(shaderNum);
//
//					// Our ModelViewProjection
//					GLuint ModelID = glGetUniformLocation(shaderNum, "Model");
//					GLuint ViewID = glGetUniformLocation(shaderNum, "View");
//					GLuint ProjectionID = glGetUniformLocation(shaderNum, "Projection");
//
//					glUniformMatrix4fv(ModelID, 1, GL_FALSE, &trans[0][0]);
//					glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
//					glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
//
//					//Tells the shader if it should use a texture or not (Will have to make the sytem use a differnt shader in this event)
//					GLuint isDiffuseID = glGetUniformLocation(shaderNum, "isDiffuse");
//					glUniform1i(isDiffuseID, mat->mat->isDiffuseTexture);
//
//					//Tell shader if it should use animationBones or not
//					GLuint isAnimatedID = glGetUniformLocation(shaderNum, "isAnimated");
//					glUniform1i(isAnimatedID, mesh->mesh->isAnimation);
//
//					if (mesh->mesh->isAnimation) {
//						updateAnimations(shaderNum, x);
//					}
//
//					//Give shader a color for the objects
//					GLuint diffuseColorID = glGetUniformLocation(shaderNum, "diffuseColor");
//					glUniform3f(diffuseColorID, mat->mat->Kd.X, mat->mat->Kd.Y, mat->mat->Kd.Z);
//
//					//Lighting Stuff
//					//light.UpdateDirectionalLight(shader.getShader(x));
//					light.UpdatePointLight(shaderNum);
//
//					//GLuint viewPosID = glGetUniformLocation(shaderNum, "viewPos");
//					//glUniform3f(viewPosID, mainCamPos.x, mainCamPos.y, mainCamPos.z);
//
//
//
//					//Bind stuff
//					//Vertex Buffer
//					glEnableVertexAttribArray(0);
//					glBindBuffer(GL_ARRAY_BUFFER, VBManager->getVertexBuffer(x));
//					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
//					//Normals
//					glEnableVertexAttribArray(1);
//					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
//					//Tex Coord
//					glEnableVertexAttribArray(2);
//					glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(6 * sizeof(float)));
//					//BoneIDs
//					glEnableVertexAttribArray(3);
//					glVertexAttribPointer(3, 4, GL_UNSIGNED_INT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
//					//Bone weights
//					glEnableVertexAttribArray(4);
//					glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
//					//Index Buffer
//					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBManager->getIndexBuffer(x));
//					//Texture
//					glBindTexture(GL_TEXTURE_2D, textureManagerGL.getTexture(x));
//					glBindVertexArray(VertexArrayID);
//
//					//Draw Call
//					//glDrawArrays(GL_TRIANGLES, 0, x.second->mesh->Vertices.size());
//					glDrawElements(
//						GL_TRIANGLES,                      // mode
//						mesh->mesh->Indices.size(),        // count
//						GL_UNSIGNED_INT,                   // type
//						(void*)0                           // element array buffer offset
//					);
//					//Unbind stuff
//					glDisableVertexAttribArray(0);
//					glDisableVertexAttribArray(1);
//					glDisableVertexAttribArray(2);
//					glDisableVertexAttribArray(3);
//					glDisableVertexAttribArray(4);
//
//				}
//			}
//
//			RenderMainCamera(EM::getEntity(x));
//		}
//	}
//
//	void OpenGL::RenderNonMainCameras()
//	{
//
//	}
//
//	void OpenGL::ClearScreen()
//	{
//		glClearColor(0.2, 0.1, 0.2, 1.0);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//
//	void OpenGL::SetUp()
//	{
//		shaderNum = compileShaders("vertex_shadow.glsl", "pixel_shadow.glsl");
//
//		VBManager = new VertexBufferManger();
//
//		/*******************
//		*	Depth
//		********************/
//		glEnable(GL_DEPTH_TEST);
//		glDepthFunc(GL_LESS);
//
//		/*******************
//		*	VBA and shaders
//		********************/
//		glGenVertexArrays(1, &VertexArrayID);
//		glBindVertexArray(VertexArrayID);
//		//Load the shaders
//		shader.loadShaders();
//		//Load the vertexBuffers
//		VBManager->makeVertexBuffers();
//
//
//		/*******************
//		*	Projection and Camera
//		********************/
//		Projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);
//
//		View = glm::lookAt(
//			glm::vec3(0, 0, 4),
//			glm::vec3(0, 0, 0),
//			glm::vec3(0, 1, 0)
//		);
//
//		/*******************
//		*	Texture
//		********************/
//		//Wrap mode
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//		//Scaling filter
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		//Mip map img filter
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		//Load all textures that are needed
//		textureManagerGL.LoadTextures();
//
//		/*******************
//		*	Lighting
//		********************/
//		light.InitLights(shaderNum);
//
//		/*******************
//		*	Frame
//		********************/
//		//Set up the frmae buffer
//		FramebufferName = 0;
//		glGenFramebuffers(1, &FramebufferName);
//		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
//		// The texture we're going to render to
//		glGenTextures(1, &renderedTexture);
//		glBindTexture(GL_TEXTURE_2D, renderedTexture);
//		// Give an empty image to OpenGL ( the last "0" )
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
//		// Poor filtering. Needed !
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//		// The depth buffer
//		glGenRenderbuffers(1, &depthrenderbuffer);
//		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
//		//Need a program to display the framebuffer
//		postProcessing = compileShaders("post_vertex.glsl", "post_pixel.glsl");
//		//Need a quad to render on
//		std::vector<objl::Vertex> postQuad;
//		objl::Vertex v0; objl::Vertex v1; objl::Vertex v2; objl::Vertex v3;
//		v0.Position = objl::Vector3(-1, 1, 0); v0.TextureCoordinate = objl::Vector2(0, 1);
//		v1.Position = objl::Vector3(1, 1, 0);  v1.TextureCoordinate = objl::Vector2(1, 1);
//		v2.Position = objl::Vector3(-1, -1, 0); v2.TextureCoordinate = objl::Vector2(0, 0);
//		v3.Position = objl::Vector3(1, -1, 0); v3.TextureCoordinate = objl::Vector2(1, 0);
//		postQuad.push_back(v0); postQuad.push_back(v1); postQuad.push_back(v2); postQuad.push_back(v3);
//		std::vector<uint32_t> indecies;
//		indecies.push_back(0); indecies.push_back(1); indecies.push_back(2);
//		indecies.push_back(1); indecies.push_back(3); indecies.push_back(2);
//		glGenBuffers(1, &postQuadVerts);
//		glBindBuffer(GL_ARRAY_BUFFER, postQuadVerts);
//		glBufferData(GL_ARRAY_BUFFER, postQuad.size() * sizeof(objl::Vertex),
//			postQuad.data(), GL_STATIC_DRAW);
//		glGenBuffers(1, &postQuadInds);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, postQuadInds);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int),
//			indecies.data(), GL_STATIC_DRAW);
//	}
//
//	void OpenGL::updateMesh(uint32_t entityID)
//	{
//		VBManager->updateVetexBuffer(entityID);
//	}
//
//	void OpenGL::updateShader(uint32_t entityID)
//	{
//		shader.updateShader(entityID);
//		////textureManagerGL.LoadTextures();
//	}
//
//	void OpenGL::updateTexture(uint32_t entityID)
//	{
//		textureManagerGL.updateTexture(entityID);
//	}
//
//	unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path)
//	{
//		// Create the shaders
//		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
//		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//
//		// Read the Vertex Shader code from the file
//		std::string VertexShaderCode;
//		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
//		if (VertexShaderStream.is_open()) {
//			std::stringstream sstr;
//			sstr << VertexShaderStream.rdbuf();
//			VertexShaderCode = sstr.str();
//			VertexShaderStream.close();
//		}
//		else {
//			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
//			getchar();
//			return 0;
//		}
//
//		// Read the Fragment Shader code from the file
//		std::string FragmentShaderCode;
//		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
//		if (FragmentShaderStream.is_open()) {
//			std::stringstream sstr;
//			sstr << FragmentShaderStream.rdbuf();
//			FragmentShaderCode = sstr.str();
//			FragmentShaderStream.close();
//		}
//
//		GLint Result = GL_FALSE;
//		int InfoLogLength;
//
//		// Compile Vertex Shader
//		printf("Compiling shader : %s\n", vertex_file_path);
//		char const* VertexSourcePointer = VertexShaderCode.c_str();
//		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
//		glCompileShader(VertexShaderID);
//
//		// Check Vertex Shader
//		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
//		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//		if (InfoLogLength > 0) {
//			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
//			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
//			printf("%s\n", &VertexShaderErrorMessage[0]);
//		}
//
//		// Compile Fragment Shader
//		printf("Compiling shader : %s\n", fragment_file_path);
//		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
//		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
//		glCompileShader(FragmentShaderID);
//
//		// Check Fragment Shader
//		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
//		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//		if (InfoLogLength > 0) {
//			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
//			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
//			printf("%s\n", &FragmentShaderErrorMessage[0]);
//		}
//
//		// Link the program
//		printf("Linking program\n");
//		GLuint ProgramID = glCreateProgram();
//		glAttachShader(ProgramID, VertexShaderID);
//		glAttachShader(ProgramID, FragmentShaderID);
//		glLinkProgram(ProgramID);
//
//		// Check the program
//		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
//		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//		if (InfoLogLength > 0) {
//			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
//			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
//			printf("%s\n", &ProgramErrorMessage[0]);
//		}
//
//		glDetachShader(ProgramID, VertexShaderID);
//		glDetachShader(ProgramID, FragmentShaderID);
//
//		glDeleteShader(VertexShaderID);
//		glDeleteShader(FragmentShaderID);
//
//		return (uint32_t)ProgramID;
//	}
//
//}