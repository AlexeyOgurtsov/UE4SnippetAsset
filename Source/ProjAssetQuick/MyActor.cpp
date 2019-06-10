#include "MyActor.h"

#include "Util/Core/LogUtilLib.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

AMyActor::AMyActor()
{
	InitializeRootComponent();
	auto M = LoadMyMaterial();
	InitializeStaticMesh();
}

UStaticMesh* AMyActor::LoadStaticMesh()
{
	UE_LOG(MyLog, Log, TEXT("LoadStaticMesh..."));
	UE_LOG(MyLog, Log, TEXT("Not yet impl"));
	ConstructorHelpers::FObjectFinderOptional<UStaticMesh> Finder { TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Box.Shape_Box'") };
	if(false == Finder.Succeeded())
	{
		UE_LOG(MyLog, Error, TEXT("Finder: returned FAILED"));
	}
	UE_LOG(MyLog, Log, TEXT("LoadStaticMesh DONE"));
	return Finder.Get();
}

void AMyActor::InitializeRootComponent()
{
	UE_LOG(MyLog, Log, TEXT("InitializeRootComponent..."));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MyRootComponent"));
	UE_LOG(MyLog, Log, TEXT("InitializeRootComponent DONE"));
}

void AMyActor::InitializeStaticMesh()
{
	UE_LOG(MyLog, Log, TEXT("InitializeStaticMesh..."));
	checkf(RootComponent, TEXT("InitializeStaticMesh: Function must be called only AFTER RootComponent is assigned!"));

	UStaticMesh* const M = LoadStaticMesh();	
	
	UStaticMeshComponent* Comp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if(M)
	{
		Comp->SetStaticMesh(M);
	}	
	else
	{
		UE_LOG(MyLog, Warning, TEXT("LoadStaticMesh returned nullptr"));
	}
	Comp->SetupAttachment(RootComponent);

	StaticMesh = Comp;

	UE_LOG(MyLog, Log, TEXT("InitializeStaticMesh DONE"));
}

void AMyActor::LoadMyMaterial()
{
	UE_LOG(MyLog, Log, TEXT("LoadMyMaterial..."));
	UE_LOG(MyLog, Warning, TEXT("NotYetImpl"));
	UE_LOG(MyLog, Log, TEXT("LoadMyMaterial DONE"));
}