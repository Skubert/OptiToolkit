// Copyright Epic Games, Inc. All Rights Reserved.

#include "HLodHelperBPLibrary.h"
#include "CoreMinimal.h"
#include "Engine/LODActor.h"
#include "Editor/GroupActor.h"
#include "GameFramework/WorldSettings.h"
#include "HierarchicalLODUtilities.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "SceneView.h"
#include "StaticMeshResources.h"
#include "HLodHelper.h"

UHLodHelperBPLibrary::UHLodHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

TArray<AActor*> UHLodHelperBPLibrary::GetGroupActorMembers(AGroupActor* GroupActor) {
	if (GroupActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetGroupActorMembers: GroupActor is null."));
		return TArray<AActor*>();
	}

	return GroupActor->GroupActors;
}

bool UHLodHelperBPLibrary::GetGroupActorLockState(AGroupActor* GroupActor) {
	if (GroupActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetGroupActorLockState: GroupActor is null."));
		return false;
	}

	return GroupActor->bLocked;
}

void UHLodHelperBPLibrary::SetGroupActorLockState(AGroupActor* GroupActor, bool LockState) {
	if (GroupActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetGroupActorLockState: GroupActor is null."));
		return;
	}

	GroupActor->bLocked = LockState;
}

AActor* UHLodHelperBPLibrary::FindActorGroup(const AActor* Actor) {
	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("FindActorGroup: Actor is null."));
		AActor *NewGroup = NULL;
		return NewGroup;
	}

	return Actor->GroupActor;

}

void UHLodHelperBPLibrary::SetActorEditorOnly(AActor* Actor, bool NewValue) {
	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetActorEditorOnly: Actor is null."));
		return;
	}
	Actor->bIsEditorOnlyActor = NewValue;
}

bool UHLodHelperBPLibrary::GetActorEditorOnly(const AActor* Actor) {
	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetActorEditorOnly: Actor is null."));
		return false;
	}
	return Actor->bIsEditorOnlyActor;
}

int32 UHLodHelperBPLibrary::GetLODLevel(const ALODActor* LODActor) {
	if (LODActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetLODLevel: LODActor is null."));
		return -1;
	}
	return LODActor->LODLevel;
}

ALODActor* UHLodHelperBPLibrary::CreateNewEmptyCluster(const AActor* LevelActor, const int32 InLODLevel) {

	UWorld* LevelWorld = LevelActor->GetLevel()->GetOuter()->GetWorld();
	AWorldSettings* WorldSettings = LevelWorld->GetWorldSettings();

	ALODActor* NewActor = LevelWorld->SpawnActor<ALODActor>(ALODActor::StaticClass(), FTransform());
	NewActor->LODLevel = InLODLevel + 1;
	NewActor->CachedNumHLODLevels = WorldSettings->GetNumHierarchicalLODLevels();
	NewActor->SetDrawDistance(0.0f);
	NewActor->SetStaticMesh(nullptr);
	NewActor->PostEditChange();

	return NewActor;
}

ALODActor* UHLodHelperBPLibrary::GetParentLODActor(const AActor* InActor) {
	ALODActor* ParentActor = nullptr;
	if (InActor)
	{
		TArray<UStaticMeshComponent*> ComponentArray;
		InActor->GetComponents<UStaticMeshComponent>(ComponentArray);
		for (auto Component : ComponentArray)
		{
			UPrimitiveComponent* ParentComponent = Component->GetLODParentPrimitive();
			if (ParentComponent)
			{
				ParentActor = CastChecked<ALODActor>(ParentComponent->GetOwner());
				break;
			}
		}
	}

	return ParentActor;
}

UStaticMesh* UHLodHelperBPLibrary::GetStaticMesh(ALODActor* Actor) {

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetStaticMesh: Actor is null."));
		UStaticMesh *NewStaticMesh = NULL;
		return NewStaticMesh;
	}

	return Actor->GetStaticMeshComponent()->GetStaticMesh();
}

TArray<AActor*> UHLodHelperBPLibrary::GetSubActors(ALODActor* Actor) {

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetSubActors: Actor is null."));
		return TArray<AActor*>();
	}

	return Actor->SubActors;
}

int32 UHLodHelperBPLibrary::GetSubActorsTrisCount(ALODActor* Actor) {

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetSubActorsTrisCount: StaticMesh is null."));
		return -1;
	}

	return Actor->NumTrianglesInSubActors;
}

int32 UHLodHelperBPLibrary::GetLODActorTrisCount(ALODActor* Actor) {

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetSubActorsTrisCount: StaticMesh is null."));
		return -1;
	}

	return Actor->NumTrianglesInMergedMesh;
}

void UHLodHelperBPLibrary::AddSubActor(ALODActor* LODActor, AActor* NewActor) {

	if (LODActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AddSubActor: Actor is null."));
		return;
	}

	if (NewActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AddSubActor: NewActor is null."));
		return;
	}

	LODActor->AddSubActor(NewActor);
	return;
}

FString UHLodHelperBPLibrary::GetLevel(AActor* Actor) {
	
	if (Actor == nullptr) {
		return "";
	}
	
	return Actor->GetLevel()->GetOuter()->GetName();
}

 bool UHLodHelperBPLibrary::GetMeshReductionSetings(UStaticMesh* StaticMesh, int32 LODIndex, FMeshReductionSettings& ReductionSettings, float& ScreenSize) {

	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetMeshReductionSetings: StaticMesh is null."));
		return false;
	}

	ReductionSettings = StaticMesh->GetReductionSettings(LODIndex);
	ScreenSize = StaticMesh->GetSourceModel(LODIndex).ScreenSize.Default;

	return true;
}

 bool UHLodHelperBPLibrary::IsMeshAutoComputeEnabled(UStaticMesh* StaticMesh) {
	 if (StaticMesh == nullptr)
	 {
		 UE_LOG(LogTemp, Error, TEXT("IsMeshAutoComputeEnabled: StaticMesh is null."));
		 return false;
	 }
	
	 return StaticMesh->bAutoComputeLODScreenSize;
}

/*bool UHLodHelperBPLibrary::GetHLodReductionSettings(ALODActor* Actor, FHLODMeshReductionOptions& ReductionSettings) {

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetHLodReductionSettings: Actor is null."));
		return true;
	}

	ReductionSettings = Actor->ReductionOptions;

	return true;
}*/

int32 UHLodHelperBPLibrary::GetTrisCount(UStaticMesh* StaticMesh, int32 LODIndex) {

	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetMeshTrisCount: StaticMesh is null."));
		return -1;
	}

	FStaticMeshLODResources& res = StaticMesh->RenderData->LODResources[LODIndex];

	return res.GetNumTriangles();
}

float UHLodHelperBPLibrary::GetScreenSize(AActor* Actor, FVector ViewLocation) {

	if (Actor == nullptr) {
		return -1;
	}

	int32 Width, Height;

	Width = GEditor->GetActiveViewport()->GetSizeXY().X;
	Height = GEditor->GetActiveViewport()->GetSizeXY().Y;

	float BoundingRadius = Actor->GetRootComponent()->Bounds.SphereRadius;
	float Dist = FVector::Dist(Actor->GetActorLocation(), ViewLocation);

	return (( BoundingRadius / Dist) * (( Width / Height ))) * 2; //honestly don't know why, but *2 is needed ¯\_(ツ)_/¯

}

void UHLodHelperBPLibrary::GetLightMapResolution(const UStaticMesh* StaticMesh, int32& Resolution)
{
	TGuardValue<bool> UnattendedScriptGuard(GIsRunningUnattendedScript, true);

	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetLightMapResolution: StaticMesh is null."));
		return;
	}

	Resolution = StaticMesh->LightMapResolution;
}

void UHLodHelperBPLibrary::SetLightMapResolution(UStaticMesh* StaticMesh, int32 Resolution)
{
	TGuardValue<bool> UnattendedScriptGuard(GIsRunningUnattendedScript, true);

	if (StaticMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetLightMapResolution: StaticMesh is null."));
		return;
	}

	StaticMesh->LightMapResolution = Resolution;
}


