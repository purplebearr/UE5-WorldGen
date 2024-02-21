// Fill out your copyright notice in the Description page of Project Settings.

#include "LandscapeGenerator.h"
#include "DiamondSquare.h"

// Sets default values
ALandscapeGenerator::ALandscapeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandscapeGenerator::BeginPlay()
{
	Super::BeginPlay();
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ALandscapeGenerator::TimerTick, TimerInterval, true);
}

void ALandscapeGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    APawn* PlayerPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (PlayerPawn)
    {
        MyCharacter = PlayerPawn->GetActorLocation();

        if (GEngine)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Player location is: %s"), *MyCharacter.ToString()));
        }

        Current.X = FMath::TruncToInt(MyCharacter.X/Scale);
        Current.Y = FMath::TruncToInt(MyCharacter.Y/Scale);

        for(int i = (Distance*-1); i <= Distance; i++)
        {
            X = i;

            for(int j = (Distance*-1); j <= Distance; j++)
            {
                Y = j;

                Visible.X = Current.X + i;
                Visible.Y = Current.Y + j;

                if (GeneratedChunks.Contains(Visible))
                {

                    //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("contains visible")));
                }
                else 
                {
                    SpawnLocation.X = Visible.X * Scale;
                    SpawnLocation.Y = Visible.Y * Scale;
                    SpawnLocation.Z = 0.0f;

                    ADiamondSquare* NewDiamondSquare = GetWorld()->SpawnActor<ADiamondSquare>(ADiamondSquare::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);

                    NewDiamondSquare->Size = this->Size;
                    NewDiamondSquare->Zmultiplier = this->Zmultiplier;
                    NewDiamondSquare->UVScale = this->UVScale;
                    NewDiamondSquare->Material = this->Material;
                    NewDiamondSquare->VertexDistance = (this->Scale)/(this->Size);
                    NewDiamondSquare->NoiseParameters = this->NoiseParameters;
                    NewDiamondSquare->Xoffset = Size * Visible.X;
                    NewDiamondSquare->Yoffset = Size * Visible.Y;
                    NewDiamondSquare->GenerateChunk();

                    //GeneratedChunkLocations.Add(Visible);
                    GeneratedChunks.Add(Visible, NewDiamondSquare);

                }
            }
        }
    }    
    
}

void ALandscapeGenerator::TimerTick()
{
    // This function will be called every 5 seconds
    // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("tick tic")));
    for (auto& Elem : GeneratedChunks)
    {
        FChunkPosition ChunkPosition = Elem.Key;
        ADiamondSquare* Chunk = Elem.Value;

        // Calculate the world position of the chunk
        FVector ChunkWorldPosition(ChunkPosition.X * Scale, ChunkPosition.Y * Scale, 0.0f);

        // Calculate the distance from the player to the chunk
        float DistanceToPlayer = FVector::Dist(MyCharacter, ChunkWorldPosition);

        // If the chunk is too far from the player, destroy it
        if (DistanceToPlayer > MaxChunkDistance)
        {
            Chunk->Destroy();
            GeneratedChunks.Remove(ChunkPosition);  // Remove the chunk from the map
        }
    }
}
