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
	float Zmultiplier = 1000.0f;
	float Scale = 0;
	float UVScale = 1;
	int Distance = 0;
	UPROPERTY(EditAnywhere);
	float VertexDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	float Xoffset = 0.0f;
	UPROPERTY(EditAnywhere)
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

	//float NoiseValueSum = 0;
	UPROPERTY(EditAnywhere);
	float Z = 0;

	UProceduralMeshComponent* ProceduralMesh;
	TArray<FVector> Vertices;
	TArray<FVector> outVertices;
	TArray<int> Triangles;
	TArray<int> outTriangles;
	TArray<FVector2D> UV0;
	TArray<FVector2D> outUV0;
	TArray<FVector2D> localUV0;
	TArray<FVector> Normals;
	TArray<FVector> localNormals;
	TArray<FVector> outNormals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FProcMeshTangent> outTangents;
	TArray<FProcMeshTangent> localTangents;
	TArray<UFastNoiseWrapper*> Noises;
	bool extended;
	int extendedSize;

	void CreateVertices();
	void CreateTriangles();
	void OnConstruction(const FTransform& Transform) override;

};
