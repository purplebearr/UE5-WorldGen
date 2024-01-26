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
    for (int X = 0; X <= Size; ++X)
    {
        for (int Y = 0; Y <= Size; ++Y)
        {
            NoiseValueSum = 0.0f;
            for (int32 NoiseIndex2 = 0; NoiseIndex2 < NoiseParameters.Num(); ++NoiseIndex2) {
                FNoiseProperties& NoiseProperties2 = NoiseParameters[NoiseIndex2];
                UFastNoiseWrapper* CurrentNoise2 = Noises[NoiseIndex2];

                NoiseValueSum = NoiseProperties2.CurveFloat->GetFloatValue(CurrentNoise2->GetNoise2D((X + 0.1 + Xoffset), (Y + 0.1 + Yoffset))) + NoiseValueSum;
            }

            Z = NoiseValueSum * Zmultiplier;

            Vertices.Add(FVector(((Size / -2) + X) * VertexDistance, ((Size / -2) + Y) * VertexDistance, Z));

            UV0.Add(FVector2D(((Size / -2) + X) * UVScale, ((Size / -2) + Y) * UVScale));
           
        }
    }
}

void ADiamondSquare::CreateTriangles()
{
    int Vertex = 0;

    for (int X = 0; X < Size; X++)
    {
        for (int Y = 0; Y < Size; Y++)
        {
            Triangles.Add(Vertex); //bottom left
            Triangles.Add(Vertex + 1); //bottom right
            Triangles.Add(Vertex + Size + 1); //top left
            Triangles.Add(Vertex + 1); //bottom right
            Triangles.Add(Vertex + Size + 2); //top right
            Triangles.Add(Vertex + Size + 1); //top left

            ++Vertex;
            //DrawDebugSphere(GetWorld(), FVector(X * Scale, Y * Scale, 0), 25.0f, 16, FColor::Red, true, -1.0f, 0U, 0.0f);

        }
        ++Vertex;
    }

}

void ADiamondSquare::GenerateChunk()
{
      for (int32 NoiseIndex = 0; NoiseIndex < NoiseParameters.Num(); ++NoiseIndex)
    {
        //fastNoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseWrapper"));
        Noises.Add(fastNoiseWrapper);

        UFastNoiseWrapper* CurrentNoise = Noises[NoiseIndex];

        if (NoiseIndex >= 0 && NoiseIndex < NoiseParameters.Num())
        {
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
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("error")));
        }
        
        CurrentNoise->SetupFastNoise(NoiseType, Seed, Frequency, Interpolation, FractalType, FractalOctaves, FractalLacunarity, FractalGain, CellularJitter, CellularDistanceFunction, CellularReturnType);
    }
      CreateVertices();
      CreateTriangles();

      UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);

      ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
      ProceduralMesh->SetMaterial(0, Material);

      //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("chunk generated")));
}
