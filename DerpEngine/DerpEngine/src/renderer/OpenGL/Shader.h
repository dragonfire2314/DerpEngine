#pragma once

#include <unordered_map>

namespace DERP {

	class Shader
	{
	private:
		//Key - entityID
		//Value - ShaderID
		std::unordered_map<uint32_t, uint32_t> programs;

		unsigned int compileShaders(const char* vertex_file_path, const char* fragment_file_path);
	public:
		void loadShaders();
		unsigned int getShader(uint32_t entityID);
	};

}