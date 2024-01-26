// Fill out your copyright notice in the Description page of Project Settings.

#include "LandscapeGenerator.h"
#include "DiamondSquare.h"

// Sets default values
ALandscapeGenerator::ALandscapeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Visible.X = 0;
    Visible.Y = 0;
}

// Called when the game starts or when spawned
void ALandscapeGenerator::BeginPlay()
{
	Super::BeginPlay();
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

                Visible.X = Current.X + X;
                Visible.Y = Current.Y + Y;

                if (GeneratedChunkLocations.Contains(Visible))
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("contains visible")));
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("GeneratedChunkLocations Size: %d"), GeneratedChunkLocations.Num()));
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("not generating chunk")));

                    if (Visible == FChunkPosition()) {
                        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("visible has default values")));
                    }
                    else {
                        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("visible does not have default values ")));
                    }
                }
                else 
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("generating chunk")));
                    SpawnLocation.X = Visible.X * Scale;
                    SpawnLocation.Y = Visible.Y * Scale;
                    SpawnLocation.Z = 0;

                    ADiamondSquare* NewDiamondSquare = GetWorld()->SpawnActor<ADiamondSquare>(ADiamondSquare::StaticClass(), SpawnLocation, SpawnRotation, SpawnInfo);
                    GEngine->AddOnScreenDebugMessage(-1, 9999.f, FColor::Red, FString::Printf(TEXT("Generated Chunk: %d %d"), Visible.X, Visible.Y));

                    NewDiamondSquare->Size = this->Size;
                    NewDiamondSquare->Zmultiplier = this->Zmultiplier;
                    NewDiamondSquare->UVScale = this->UVScale;
                    NewDiamondSquare->Material = this->Material;
                    NewDiamondSquare->VertexDistance = (this->Scale)/(this->Size);
                    NewDiamondSquare->NoiseParameters = this->NoiseParameters;
                    NewDiamondSquare->Xoffset = (this->Size)/(this->Visible.X);
                    NewDiamondSquare->Yoffset = (this->Size)/(this->Visible.Y);

                    NewDiamondSquare->GenerateChunk();

                    //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("chunk generated")));

                    GeneratedChunkLocations.Add(Visible);

                    //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("GeneratedChunkLocations Size: %d"), GeneratedChunkLocations.Num()));


                }
            }
        }
    }
}