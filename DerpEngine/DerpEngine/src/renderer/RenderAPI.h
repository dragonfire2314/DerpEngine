#pragma once

namespace DERP {

	//Abstract class that is overrided by a graphics class
	//Example openGL class
	class RenderAPI
	{
	private:
	public:

		virtual void Render() = 0;
		virtual void ClearScreen() = 0;
	};

}