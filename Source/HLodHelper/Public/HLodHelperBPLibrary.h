// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LODActor.h"
#include "HLodHelperBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UCLASS()
class UHLodHelperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|LODActor")
		static ALODActor* CreateNewEmptyCluster(const AActor* LevelActor, const int32 InLODLevel);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
		static int32 GetLODLevel(const ALODActor* LODActor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|GroupActor")
		static TArray<AActor*> GetGroupActorMembers(AGroupActor* GroupActor);

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|GroupActor")
		static void SetGroupActorLockState(AGroupActor* GroupActor, bool LockState);

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|GroupActor")
		static AActor* FindActorGroup(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|GroupActor")
		static bool GetGroupActorLockState(AGroupActor* GroupActor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
		static UStaticMesh* GetStaticMesh(ALODActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|Actor")
		static void SetActorEditorOnly(AActor* Actor, bool NewValue);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|Actor")
		static bool GetActorEditorOnly(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|Actor")
		static ALODActor* GetParentLODActor(const AActor* InActor);

	/*
	Get all base actors of selected cluster
	*/
	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
		static TArray<AActor*> GetSubActors(ALODActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|LODActor")
		static void AddSubActor(ALODActor* LODActor, AActor* NewActor);

	/*
	Returns level in which actor is located
	*/
	UFUNCTION(BlueprintPure, Category = "HLod Helper Library")
		static FString GetLevel(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|StaticMesh")
		static bool GetMeshReductionSetings(UStaticMesh* StaticMesh, int32 LODIndex, FMeshReductionSettings& ReductionSettings, float& ScreenSize);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|StaticMesh")
		static bool IsMeshAutoComputeEnabled(UStaticMesh* StaticMesh);

	//UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
	//	static bool GetHLodReductionSettings(ALODActor* Actor, FHLODMeshReductionOptions& ReductionSettings);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
		static int32 GetSubActorsTrisCount(ALODActor* Actor);

	/*
	Returns the same amount as displayed in "Reduced Triangle Count" column in HLOD Outliner
	*/
	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|LODActor")
		static int32 GetLODActorTrisCount(ALODActor* Actor);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|StaticMesh")
		static int32 GetTrisCount(UStaticMesh* StaticMesh, int32 LODIndex);

	UFUNCTION(BlueprintPure, Category = "HLod Helper Library|StaticMesh")
		static void GetLightMapResolution(const UStaticMesh* StaticMesh, int32& Resolution);

	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library|StaticMesh")
		static void SetLightMapResolution(UStaticMesh* StaticMesh, int32 Resolution);

	/*
	Wouldn't trust this tbh, but hey, it ALMOST works perfectly. Assumes editor camera FOV of 90*
	*/
	UFUNCTION(BlueprintCallable, Category = "HLod Helper Library")
		static float GetScreenSize(AActor* Actor, FVector ViewLocation);

};
