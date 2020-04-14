#pragma once

#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class AMyActor : public AActor
{
	GENERATED_BODY()

public:
	AMyActor();

private:
	// ~RootComponent Begin
	void InitializeRootComponent();
	// ~RootComponent End	

	// ~StaticMesh Begin
	UStaticMesh* LoadStaticMesh();
	void InitializeStaticMesh();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMesh = nullptr;
	// ~StaticMesh End

	// ~Materials Begin
	void LoadMyMaterial();
	// ~Materials End
};