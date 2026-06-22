// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EchoLoop : ModuleRules
{
	public EchoLoop(ReadOnlyTargetRules Target) : base(Target)
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
			"EchoLoop",
			"EchoLoop/Variant_Platforming",
			"EchoLoop/Variant_Platforming/Animation",
			"EchoLoop/Variant_Combat",
			"EchoLoop/Variant_Combat/AI",
			"EchoLoop/Variant_Combat/Animation",
			"EchoLoop/Variant_Combat/Gameplay",
			"EchoLoop/Variant_Combat/Interfaces",
			"EchoLoop/Variant_Combat/UI",
			"EchoLoop/Variant_SideScrolling",
			"EchoLoop/Variant_SideScrolling/AI",
			"EchoLoop/Variant_SideScrolling/Gameplay",
			"EchoLoop/Variant_SideScrolling/Interfaces",
			"EchoLoop/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
