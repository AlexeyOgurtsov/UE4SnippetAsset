#pragma once

#include "GameFramework/Actor.h"
#include "MyAudioHolder.generated.h"

class UAudioComponent;

UCLASS()
class AMyAudioHolder : public AActor
{
	GENERATED_BODY()

public:
	AMyAudioHolder();

private:
	void TestLoadSoundCue();
	void TestLoadSoundWave();

	void InitAudioComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess = true))
	UAudioComponent* AudioComponent = nullptr;
};
