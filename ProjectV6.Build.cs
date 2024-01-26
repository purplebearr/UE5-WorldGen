// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectV6 : ModuleRules
{
    public ProjectV6(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ProceduralMeshComponent", "FastNoiseGenerator", "FastNoise" });
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}