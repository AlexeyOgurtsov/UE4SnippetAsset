#pragma once

#include "GameFramework/Actor.h"
#include "MyAssetLoader.generated.h"

UCLASS()
class AMyAssetLoader : public AActor
{
	GENERATED_BODY()

public:
	AMyAssetLoader();

	virtual void PostInitProperties() override;

private:
	// ~TestObjectFinder Begin
	void TestObjectFinder();

	void TestObjectFinder_NotInConstructor();
	void TestObjectFinder_LoadObject();	

	void TestObjectFinder_NonexistentPath();
	void TestObjectFinder_BackslashPath();
	void TestObjectFinder_OtherClassArgument();
	void TestObjectFinder_BaseClassArgument();
	// ~TestObjectFinder End

	// ~TestClassFinder Begin
	void TestClassFinder_TypeAsBlueprint();
	void TestClassFinder_TypeAsActor();
	void TestClassFinder_TypeAsUObject();
	void TestClassFinder_TypeAsClass();
	void TestClassFinder_TypeAsBlueprintGeneratedClass();

	void TestClassFinder();
	void TestClassFinder_TesteeActor();

	void TestClassFinder_ObjectDotObjectURI_IsWrong();
	void TestClassFinder_CPostfix_InPath();
	void TestClassFinder_NoClassPrefix();

	void TestClassFinder_BPInterface();
	void TestClassFinder_BPLibrary();
	void TestClassFinder_AnimBlueprint();
	void TestClassFinder_Widget();
	void TestClassFinder_BehaviourTree();
	void TestClassFinder_Blackboard();

	void TestObjectFinder_LoadClass();
	void TestObjectFinder_LoadObjectClass();	
	void Test_LoadObjectClass();
	// ~TestObjectFinder End
};
