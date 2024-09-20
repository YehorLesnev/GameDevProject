// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Jungle : ModuleRules
{
	public Jungle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
