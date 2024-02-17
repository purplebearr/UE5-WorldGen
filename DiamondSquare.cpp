#include "DiamondSquare.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "FastNoiseWrapper.h"
#include "LandscapeGenerator.h"



// Sets default values
ADiamondSquare::ADiamondSquare()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
    ProceduralMesh->SetupAttachment(GetRootComponent());

    fastNoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseWrapper"));

}

void ADiamondSquare::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    Vertices.Reset();
    Triangles.Reset();
    UV0.Reset();

}

// Called when the game starts or when spawned
void ADiamondSquare::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ADiamondSquare::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADiamondSquare::CreateVertices()
{
    //NoiseValueSum = 0.0f;
    for (int Xindex = -1; Xindex <= (Size+1); ++Xindex)
    {
        //NoiseValueSum = 0.0f;

        for (int Yindex = -1; Yindex <= (Size+1); ++Yindex)
        {
            float NoiseValueSum = 0.0f;
            for (int32 NoiseIndex2 = 0; NoiseIndex2 < NoiseParameters.Num(); ++NoiseIndex2) {
                FNoiseProperties& NoiseProperties2 = NoiseParameters[NoiseIndex2];
                UFastNoiseWrapper* CurrentNoise2 = Noises[NoiseIndex2];

                float tempX = Xindex + 0.1 + Xoffset;
                float tempY = Yindex + 0.1 + Yoffset;
                float tempNoiseValue = CurrentNoise2->GetNoise2D(tempX, tempY);
                float tempFloatValue = NoiseProperties2.CurveFloat->GetFloatValue(tempNoiseValue);

                NoiseValueSum = tempFloatValue + NoiseValueSum;

               
            }

            Z = NoiseValueSum * Zmultiplier;



            Vertices.Add(FVector((((Size / -2) + Xindex) * VertexDistance), (((Size / -2) + Yindex) * VertexDistance), Z));

            UV0.Add(FVector2D(((Size / -2) + Xindex) * UVScale, ((Size / -2) + Yindex) * UVScale));

        }
    }
}

void ADiamondSquare::CreateTriangles()
{
    int Vertex = 0;

    if (extended == true) {
        extendedSize = Size + 2;
    }
    else {
        extendedSize = Size;
    }

    //extendedSize = Size + 2;

    for (int X = 0; X < (extendedSize); X++) //changed size to extended size
    {
        for (int Y = 0; Y < (extendedSize); Y++)
        {
            Triangles.Add(Vertex); //bottom left
            Triangles.Add(Vertex + 1); //bottom right
            Triangles.Add(Vertex + extendedSize + 1); //top left //changed to extended size isntead of size
            Triangles.Add(Vertex + 1); //bottom right
            Triangles.Add(Vertex + extendedSize + 2); //top right
            Triangles.Add(Vertex + extendedSize + 1); //top left

            ++Vertex;
            

        }
        ++Vertex;
    }

}

void ADiamondSquare::GenerateChunk()
{
      for (int32 NoiseIndex = 0; NoiseIndex < NoiseParameters.Num(); ++NoiseIndex)
    {
        Noises.Add(fastNoiseWrapper);

        //UFastNoiseWrapper* CurrentNoise = Noises[NoiseIndex];

        FNoiseProperties& CurrentNoiseProperty = NoiseParameters[NoiseIndex];

        NoiseType = CurrentNoiseProperty.NoiseType;
        Seed = CurrentNoiseProperty.Seed;
        Frequency = CurrentNoiseProperty.Frequency;
        Interpolation = CurrentNoiseProperty.Interpolation;
        FractalType = CurrentNoiseProperty.FractalType;
        FractalOctaves = CurrentNoiseProperty.FractalOctaves;
        FractalLacunarity = CurrentNoiseProperty.FractalLacunarity;
        FractalGain = CurrentNoiseProperty.FractalGain;
        CellularJitter = CurrentNoiseProperty.CellularJitter;
        CellularDistanceFunction = CurrentNoiseProperty.CellularDistanceFunction;
        CellularReturnType = CurrentNoiseProperty.CellularReturnType;
        CurveFloat = CurrentNoiseProperty.CurveFloat;

        Noises[NoiseIndex]->SetupFastNoise(NoiseType, Seed, Frequency, Interpolation, FractalType, FractalOctaves, FractalLacunarity, FractalGain, CellularJitter, CellularDistanceFunction, CellularReturnType);

    }
      extended = true;
      CreateVertices();
      CreateTriangles();

      UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);

      localNormals = Normals;
      localTangents = Tangents;
      int localIndex = 0;

      for (int localX = -1; localX <= (Size+1); localX++) {
          for (int localY = -1; localY <= (Size+1); localY++) {
              if (localX > -1 && localX <= Size && localY > -1 && localY <= Size){
                  outVertices.Add(Vertices[localIndex]);
                  outNormals.Add(localNormals[localIndex]);
                  outUV0.Add(UV0[localIndex]);
                  outTangents.Add(localTangents[localIndex]);
                  localIndex++;

              }
              else {
                  localIndex++;
              }

          }
      }

      extended = false;
      Triangles.Empty();
      CreateTriangles();

      ProceduralMesh->CreateMeshSection(0, outVertices, Triangles, outNormals, outUV0, TArray<FColor>(), outTangents, true);
      ProceduralMesh->SetMaterial(0, Material);

      //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("chunk generated")));
}
