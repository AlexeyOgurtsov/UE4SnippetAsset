#include "MyAssetLoader.h"
#include "Util/Core/LogUtilLib.h"

#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"

using ReferenceResourceType = UStaticMesh;
using OtherClassType = UMaterial;
using BaseClassType = UObject;

// ~ObjectFinder Begin
static const TCHAR* const VALID_REFERENCE = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
static const TCHAR* const REFERENCE_BACKSLASH = TEXT("StaticMesh'\\Game\\StarterContent\\Shapes\\Shape_Cube.Shape_Cube'");

static const TCHAR* const NON_EXISTENT_REFERENCE = TEXT("StaticMesh'\\Game\\StarterContent\\Shapes\\NonExistent.NonExistent'");
static const TCHAR* const OTHER_CLASS_REFERENCE = TEXT("SkeletalMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
// ~ObjectFinder End

// ~ClassFinder Begin
static const TCHAR* const VALID_BP_CLASS_REFERENCE = TEXT("Blueprint'/Game/Res/Blueprint/BP_MyAct'");
// This form can be used to load object as classes
static const TCHAR* const VALID_BP_CLASS_C_POSTFIX = TEXT("Blueprint'/Game/Res/Blueprint/BP_MyAct.BP_MyAct_C'");
static const TCHAR* const WRONG_BP_CLASS_NO_CLASS_PREFIX = TEXT("/Game/Res/Blueprint/BP_MyAct'");

static const TCHAR* const WRONG_BP_CLASS_REFERENCE_DOT_DOT = TEXT("Blueprint'/Game/Res/Blueprint/BP_MyAct.BP_MyAct'");

static const TCHAR* const VALID_BP_TESTEE_CLASS_REFERENCE = TEXT("Blueprint'/Game/Res/Blueprint/BP_TesteeActor'");

static const TCHAR* const VALID_BP_INTERFACE_CLASS_REFERENCE = TEXT("Blueprint'/Game/Res/Blueprint/BPI_Interface'");
static const TCHAR* const VALID_BP_LIB_CLASS_REFERENCE = TEXT("Blueprint'/Game/Res/Blueprint/BPL_Lib'");
static const TCHAR* const VALID_BP_MACRO_LIB_CLASS_REFERENCE = TEXT("Blueprint'/Game/Res/Blueprint/BPM_MacroLib'");
static const TCHAR* const VALID_BP_STRUCT_REFERENCE = TEXT("UserDefinedStruct'/Game/Res/Blueprint/BPS_Struct'");
static const TCHAR* const VALID_BP_ENUM_REFERENCE = TEXT("UserDefinedEnum'/Game/Res/Blueprint/BPE_MyEnum'");

static const TCHAR* const VALID_ANIM_BP_CLASS_REFERENCE = TEXT("AnimBlueprint'/Game/Res/Anim/AnimBP'");
static const TCHAR* const VALID_WIDGET_BP_CLASS_REFERENCE = TEXT("WidgetBlueprint'/Game/Res/UI/WidgetBlueprint'");
static const TCHAR* const VALID_BEHAV_TREE_BP_CLASS_REFERENCE = TEXT("BehaviorTree'/Game/Res/AI/MyBehaviorTree.MyBehaviorTree'");
static const TCHAR* const VALID_BLACKBOARD_DATA_BP_CLASS_REFERENCE = TEXT("BlackboardData'/Game/Res/AI/MyBlackboard'");

AMyAssetLoader::AMyAssetLoader()
{
	M_LOGFUNC();

	{
		{
			TestObjectFinder();
			TestObjectFinder_NonexistentPath();
			TestObjectFinder_BackslashPath();
			TestObjectFinder_OtherClassArgument();
			TestObjectFinder_BaseClassArgument();

			// Loading object dynamically can be correctly done from constructor also!
			TestObjectFinder_LoadObject();		
			TestObjectFinder_LoadObjectClass();
		}

		{
			TestClassFinder_TypeAsBlueprint();
			TestClassFinder_TypeAsActor();
			TestClassFinder_TypeAsUObject();
			TestClassFinder_TypeAsClass();
			TestClassFinder_TypeAsBlueprintGeneratedClass();

			TestClassFinder_ObjectDotObjectURI_IsWrong();
			TestClassFinder_CPostfix_InPath();
			TestClassFinder_NoClassPrefix();

			TestClassFinder();
			TestClassFinder_TesteeActor();
			TestClassFinder_BPInterface();
			TestClassFinder_BPLibrary();
			TestClassFinder_AnimBlueprint();
			TestClassFinder_Widget();
			TestClassFinder_BehaviourTree();
			TestClassFinder_Blackboard();

			// Loading class dynamically can be correctly done from constructor also!	
			TestObjectFinder_LoadObjectClass();
			TestObjectFinder_LoadClass();
		}
	}
}

void AMyAssetLoader::PostInitProperties()
{
	Super::PostInitProperties();
	TestObjectFinder_NotInConstructor();
	Test_LoadObjectClass();
	TestObjectFinder_LoadObject();
}

void AMyAssetLoader::TestClassFinder_TypeAsUObject()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ VALID_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_TypeAsClass()
{
	// Note we cannot mark actor-derived blueprint class as UClass
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UClass> ClassFinder{ VALID_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded() == false, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_TypeAsBlueprintGeneratedClass()
{
	// Note we cannot mark actor-derived blueprint class as blueprint-generated-class
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UClass> ClassFinder{ VALID_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded() == false, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_TypeAsBlueprint()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UBlueprint> ClassFinder{ VALID_BP_CLASS_REFERENCE };
	checkf(!ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_TypeAsActor()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<AActor> ClassFinder{ VALID_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UObject> ClassFinder { VALID_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_TesteeActor()
{
	M_LOGFUNC();
	class ATesteeClass;
	// FAILS TO COMPILE:
	// We MUST include class DECLARATION to use it inside FClassFinder, NOT only fwd declare IT!!!	

	//ConstructorHelpers::FClassFinder<ATesteeClass> ClassFinder{ VALID_BP_TESTEE_CLASS_REFERENCE };
	//checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	//checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	//ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_ObjectDotObjectURI_IsWrong()
{
	M_LOGFUNC();
	// It's wrong to specify the class resource as ResourceName.ResourceName
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ WRONG_BP_CLASS_REFERENCE_DOT_DOT };
	checkf( ! ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_CPostfix_InPath()
{
	M_LOGFUNC();
	// We DO can use form ResourceName.ResourceName_C
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ VALID_BP_CLASS_C_POSTFIX };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_NoClassPrefix()
{
	M_LOGFUNC();	
	// We cannot OMIT specifying the class prefix (like Blueprint'' before the path)
	// ASSERTS!!!
	//ConstructorHelpers::FClassFinder<UObject> ClassFinder{ WRONG_BP_CLASS_NO_CLASS_PREFIX };
	//checkf(!ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	//checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestClassFinder_BPInterface()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UObject> ClassFinder { VALID_BP_INTERFACE_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_BPLibrary()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ VALID_BP_LIB_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_AnimBlueprint()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UAnimInstance> ClassFinder{ VALID_ANIM_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_Widget()
{
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UUserWidget> ClassFinder{ VALID_WIDGET_BP_CLASS_REFERENCE };
	checkf(ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class != nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_BehaviourTree()
{
	// It's wrong to load the behaviour tree like a class!
	M_LOGFUNC();
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ VALID_BEHAV_TREE_BP_CLASS_REFERENCE };
	checkf( ! ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::TestClassFinder_Blackboard()
{
	M_LOGFUNC();
	// It's wrong to load blackboard like class!!!
	ConstructorHelpers::FClassFinder<UObject> ClassFinder{ VALID_BLACKBOARD_DATA_BP_CLASS_REFERENCE };
	checkf( ! ClassFinder.Succeeded(), TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	checkf(ClassFinder.Class == nullptr, TEXT("Inside function \"%s\" class finder must succeed"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("ClassFinder.Class"), ClassFinder.Class);
}

void AMyAssetLoader::Test_LoadObjectClass()
{
	// WARN!!! WARN!!! WARN!!
	// THE ONLY valid way to load a class!!!
	// Passing Reference in form Blueprint'/Game/../BP_Name.BP_Name_C'
	// (Note _C at the end)
	{
		UClass* ActClass = LoadObject<UClass>(this, VALID_BP_CLASS_C_POSTFIX);
		checkf(ActClass != nullptr, TEXT("Inside function \"%s\" LoadObject(AActor) must SUCCEED"), TEXT(__FUNCTION__));
	}

	{
		UClass* ActClass = LoadObject<UClass>(this, VALID_BP_CLASS_REFERENCE);
		checkf(ActClass == nullptr, TEXT("Inside function \"%s\" LoadObject(AActor) must fail"), TEXT(__FUNCTION__));
	}

	{
		AActor* ActClass = LoadObject<AActor>(this, VALID_BP_CLASS_REFERENCE);
		checkf(ActClass == nullptr, TEXT("Inside function \"%s\" LoadObject(AActor) must fail"), TEXT(__FUNCTION__));
	}

	{
		AActor* ActClass = LoadObject<AActor>(this, VALID_BP_CLASS_C_POSTFIX);
		checkf(ActClass == nullptr, TEXT("Inside function \"%s\" LoadObject(AActor) must fail"), TEXT(__FUNCTION__));
	}	
}

void AMyAssetLoader::TestObjectFinder_LoadClass()
{
	M_LOGFUNC();
	// WRONG: Compilation fails be doen (AActor cannot be used instead of UClass!)
	//AActor* Act = LoadClass<AActor>(this, VALID_BP_CLASS_REFERENCE);

	// This must fail
	// It's wrong to specify path '/Game/Res/Blueprint/BP_MyAct' for LoadClass
	{
		//[2020.04.15 - 07.16.48:736][0]LogLinker: Warning: Failed to load '/Game/Res/Blueprint/BP_MyAct'': Can't find file.
		//[2020.04.15 - 07.16.48:737][0]LogPackageName : Error : DoesPackageExist : DoesPackageExist FAILED : '/Game/Res/Blueprint/BP_MyAct'' is not a standard unreal filename or a long path name. Reason: Name may not contain the following characters: '''
		//[2020.04.15 - 07.16.48:738][0]LogLinker : Warning : Failed to load '/Game/Res/Blueprint/BP_MyAct'': Can't find file.
		//[2020.04.15 - 07.16.48:738][0]LogUObjectGlobals : Warning : Failed to find object 'Class /Script/ProjAssetQuick.Default__MyAssetLoader./Game/Res/Blueprint/BP_MyAct'
		//Assertion failed : ActClass != nullptr[File:C:\U4L\Snip\Asset\ProjAssetQuick\Source\ProjAssetQuick\Private\AssetLoad\MyAssetLoader.cpp][Line:248]
		//Inside function "AMyAssetLoader::TestObjectFinder_LoadClass::<lambda_02046282aac8f9d5ab2c7959224b5ba7>::operator ()" class finder must succeed
		//UE4Editor - Win64 - DebugGame.exe вызвал срабатывание точки останова.

		UClass* ActClass = LoadClass<UClass>(this, VALID_BP_CLASS_REFERENCE);
		checkf(ActClass == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	}

	{		
		// This must fail
		// It's wrong to specify path '/Game/Res/Blueprint/BP_MyAct.BP_MyAct_C' for LoadClass		
		UClass* ActClass = LoadClass<UClass>(this, VALID_BP_CLASS_C_POSTFIX);
		checkf(ActClass == nullptr, TEXT("Inside function \"%s\" class finder must FAIL"), TEXT(__FUNCTION__));
	}
}

void AMyAssetLoader::TestObjectFinder()
{
	M_LOGFUNC();

	ConstructorHelpers::FObjectFinderOptional<ReferenceResourceType> ObjFinder { VALID_REFERENCE };
	checkf(ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must succeed"), TEXT(__FUNCTION__));
	checkf(ObjFinder.Get() != nullptr, TEXT("Inside function \"%s\" object finder Get() must get NON-null"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestObjectFinder_NotInConstructor()
{
	M_LOGFUNC();
	// This will fail with ASSERT!
	//ConstructorHelpers::FObjectFinderOptional<ReferenceResourceType> ObjFinder{ VALID_REFERENCE };
	//checkf(!ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestObjectFinder_LoadObject()
{
	M_LOGFUNC();
	// Can correctly work ever from constructor
	// WARN!!! The SECOND argument is REFERENCE to OBJECT!!!
	ReferenceResourceType* Obj = LoadObject<ReferenceResourceType>(this, VALID_REFERENCE );
	checkf(Obj != nullptr, TEXT("Inside function \"%s\" object finder must SUCCEED"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("LoadedObject"), Obj);
}

void AMyAssetLoader::TestObjectFinder_LoadObjectClass()
{
	M_LOGFUNC();
	ConstructorHelpers::FObjectFinderOptional<UClass> ObjFinder{ VALID_BP_CLASS_C_POSTFIX };
	checkf( ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must SUCCEED"), TEXT(__FUNCTION__));
	checkf( ObjFinder.Get() != nullptr, TEXT("Inside function \"%s\" object finder must SUCCEED"), TEXT(__FUNCTION__));
	ULogUtilLib::LogKeyedNameClassSafeC(TEXT("LoadedObject"), ObjFinder.Get());
}

void AMyAssetLoader::TestObjectFinder_NonexistentPath()
{
	M_LOGFUNC();
	ConstructorHelpers::FObjectFinderOptional<ReferenceResourceType> ObjFinder { NON_EXISTENT_REFERENCE };
	checkf( ! ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestObjectFinder_BackslashPath()
{
	M_LOGFUNC();
	ConstructorHelpers::FObjectFinderOptional<ReferenceResourceType> ObjFinder { REFERENCE_BACKSLASH };
	checkf( ! ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must FAIL"), TEXT(__FUNCTION__));
}

void AMyAssetLoader::TestObjectFinder_OtherClassArgument()
{
	M_LOGFUNC();
	ConstructorHelpers::FObjectFinderOptional<OtherClassType> ObjFinder { VALID_REFERENCE };
	checkf( ! ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must FAIL"), TEXT(__FUNCTION__));
	checkf( ObjFinder.Get() == nullptr, TEXT("Inside function \"%s\" object finder must FAIL"), TEXT(__FUNCTION__) );
}

void AMyAssetLoader::TestObjectFinder_BaseClassArgument()
{
	M_LOGFUNC();
	ConstructorHelpers::FObjectFinderOptional<BaseClassType> ObjFinder { VALID_REFERENCE };
	checkf( ObjFinder.Succeeded(), TEXT("Inside function \"%s\" object finder must SUCCEED"), TEXT(__FUNCTION__));
	checkf(ObjFinder.Get() != nullptr, TEXT("Inside function \"%s\" object finder must SUCCEED"), TEXT(__FUNCTION__));
}

