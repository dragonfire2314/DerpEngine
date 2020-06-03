#include "AudioManager.h"

#include "../ecs/systems/System.h"
#include "../ecs/ComponentManager.h"
#include "../ecs/Components.h"

#include <irrKlang.h>
#include <unordered_map>

namespace DERP
{
	namespace AUDIO
	{
		irrklang::ISoundEngine* engine;

		std::unordered_map<uint32_t, irrklang::ISound*> soundMap;

		bool initAudio()
		{
			//ISoundEngine* engine = createIrrKlangDevice();
			engine = irrklang::createIrrKlangDevice();

			if (!engine)
				return false;

			return true;
		}

		void updateAudio() 
		{
			for (auto x : sys_audio->Entities) 
			{
				AudioSource* a = CM::GetComponent<AudioSource>(x);
				//Did the audio source update?
				if (!a->getUpdateStatus()) continue;

				//Check if audio is playing
				if (soundMap.count(x)) {
					//Exists
					if (a->getPlayingStatus() == false) { //Turn off sound
						soundMap[x]->drop();
						soundMap.erase(x);
						continue;
					}

					//Update settings of audios
					//soundMap[x]->setVolume(a->getVolume());
					//soundMap[x]->setIsLooped(a->getLoopStatus());
				}
				else 
				{
					//Doesn't exsit

					if (a->getPlayingStatus()) { //Turn on sound

						irrklang::ISound* snd = engine->play2D(a->getFile().c_str()); //RETURNS A NULL POINTER?? WHY??

						soundMap.insert({ x, snd });
					}
				}

				//Tell the soucre the update was handled
				a->UpdateHandled();
			}
		}
	}
}