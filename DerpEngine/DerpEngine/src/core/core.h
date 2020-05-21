#pragma once

//Only suports windows compiling
#ifdef DERP_WINDOWS
	#ifdef DERP_DLL
		#define DERP_API __declspec(dllexport) 
	#elif DERP_STATIC
		#define DERP_API
	#else
		#define DERP_API __declspec(dllimport) 
	#endif
#endif // DERP_WINDOWS