// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseWrapper.h"
#include "LandscapeGenerator.h"
#include "DiamondSquare.generated.h"


class UProceduralMeshComponent;
class UMaterialInterface;
class UFastNoiseWrapper;

struct FProcMeshTangent;

UCLASS()
class PROJECTV6_API ADiamondSquare : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties

	ADiamondSquare();

	UPROPERTY(EditAnywhere)
	TArray<FNoiseProperties> NoiseParameters;

	int Size = 0;
	float Zmultiplier = 1.0f;
	float Scale = 0;
	float UVScale = 0;
	int Distance = 0;
	float VertexDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	float Xoffset = 0.0f;
	float Yoffset = 0.0f;

	UCurveFloat* CurveFloat;

	UMaterialInterface* Material;
	EFastNoise_NoiseType NoiseType;
	int Seed = 1337;	float Frequency = 0.01f;
	EFastNoise_Interp Interpolation;
	EFastNoise_FractalType FractalType;
	int FractalOctaves = 3;
	float FractalLacunarity = 2.0f;
	float FractalGain = 0.05f;
	float CellularJitter = 0.45f;
	EFastNoise_CellularDistanceFunction CellularDistanceFunction;
	EFastNoise_CellularReturnType CellularReturnType;

	void GenerateChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFastNoiseWrapper* fastNoiseWrapper = nullptr;
	float Noise2D = 0.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:

	float NoiseValueSum = 0;
	float Z = 0;

	UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY(EditAnywhere)
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<UFastNoiseWrapper*> Noises;
	

	void CreateVertices();
	void CreateTriangles();
	void OnConstruction(const FTransform& Transform) override;

};
