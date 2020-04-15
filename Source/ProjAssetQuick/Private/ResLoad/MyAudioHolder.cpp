#include "MyAudioHolder.h"
#include "Util/Core/LogUtilLib.h"

#include "Kismet/GameplayStatics.h"

AMyAudioHolder::AMyAudioHolder()
{
	{
		TestLoadSoundCue();
		TestLoadSoundWave();
	}

	InitAudioComponent();
}

void AMyAudioHolder::TestLoadSoundCue()
{
}

void AMyAudioHolder::TestLoadSoundWave()
{
}

void AMyAudioHolder::InitAudioComponent()
{
}
