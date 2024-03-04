// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "FastNoiseWrapper.h"
#include "Math/Vector.h"
#include "Misc/Crc.h"
#include "Curves/CurveFloat.h"
#include "Multithreaded.h"
#include "MultithreadedLibrary.h"
#include"PCGComponent.h"

#include "LandscapeGenerator.generated.h"

class UGameplayStatistics;
class ADiamondSquare;


USTRUCT(BlueprintType)
struct FNoiseProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EFastNoise_NoiseType NoiseType;
	
	UPROPERTY(EditAnywhere)
	int Seed = 1337;

	UPROPERTY(EditAnywhere)
	float Frequency = 0.01f;

	UPROPERTY(EditAnywhere)
	EFastNoise_Interp Interpolation;

	UPROPERTY(EditAnywhere)
	EFastNoise_FractalType FractalType;

	UPROPERTY(EditAnywhere)
	int FractalOctaves = 3;

	UPROPERTY(EditAnywhere)
	float FractalLacunarity = 2.0f;

	UPROPERTY(EditAnywhere)
	float FractalGain = 0.05f;

	UPROPERTY(EditAnywhere)
	float CellularJitter = 0.45f;

	UPROPERTY(EditAnywhere)
	EFastNoise_CellularDistanceFunction CellularDistanceFunction;

	UPROPERTY(EditAnywhere)
	EFastNoise_CellularReturnType CellularReturnType;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	UPCGGraphInterface* PCGGraph;

	FNoiseProperties()
	{
	}
};

USTRUCT(BlueprintType)
struct FChunkPosition
{
	GENERATED_BODY()

	int32 X;
	int32 Y;

	FChunkPosition(int32 InX = 0, int32 InY = 0) : X(InX), Y(InY) {}

	bool operator==(const FChunkPosition& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

FORCEINLINE uint32 GetTypeHash(const FChunkPosition& Thing)
{
    return (std::hash<int32>()(Thing.X) * 397) ^ (std::hash<int32>()(Thing.Y) * 17);
}

UCLASS()
class PROJECTV6_API ALandscapeGenerator : public AActor, public IMultithreaded
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandscapeGenerator();

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000000001), Category = "Noise Variables")
	int Distance = 0;

	//Chunk Variables

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category = "Noise Variables")
	int Size = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0), Category="Noise Variables")
	float Zmultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000000001), Category="Noise Variables")
	float Scale = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000000001), Category="Noise Variables")
	float UVScale = 0;

	UPROPERTY(EditAnywhere)
	TArray<FNoiseProperties> NoiseParameters;

	TMap<FChunkPosition, ADiamondSquare*> GeneratedChunks;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FChunkPosition ChunkLocation;
	FChunkPosition Current;
	FChunkPosition Visible;

	TArray<FChunkPosition> ChunksToHide;


	TSet<FChunkPosition> GeneratedChunkLocations;

	float CurrentX = 0;
	int CurrentY = 0;

	int X = 0;
	int Y = 0;

	UPROPERTY(EditAnywhere)
	float MaxChunkDistance = 100000.0f;

	FVector MyCharacter;

	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnInfo;

	TSubclassOf<class ADiamondSquare> MyBlueprintClass;

	FTimerHandle TimerHandle;
	UPROPERTY(EditAnywhere)
	float TimerInterval = 5.0f;
	void TimerTick();
};
