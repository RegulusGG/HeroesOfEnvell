// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HeroesOfEnvell : ModuleRules
{
	public HeroesOfEnvell(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HeroesOfEnvell",
			"HeroesOfEnvell/Variant_Platforming",
			"HeroesOfEnvell/Variant_Platforming/Animation",
			"HeroesOfEnvell/Variant_Combat",
			"HeroesOfEnvell/Variant_Combat/AI",
			"HeroesOfEnvell/Variant_Combat/Animation",
			"HeroesOfEnvell/Variant_Combat/Gameplay",
			"HeroesOfEnvell/Variant_Combat/Interfaces",
			"HeroesOfEnvell/Variant_Combat/UI",
			"HeroesOfEnvell/Variant_SideScrolling",
			"HeroesOfEnvell/Variant_SideScrolling/AI",
			"HeroesOfEnvell/Variant_SideScrolling/Gameplay",
			"HeroesOfEnvell/Variant_SideScrolling/Interfaces",
			"HeroesOfEnvell/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
