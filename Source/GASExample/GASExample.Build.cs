// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASExample : ModuleRules
{
	public GASExample(ReadOnlyTargetRules Target) : base(Target)
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
            "Slate",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GASExample",
			"GASExample/Variant_Platforming",
			"GASExample/Variant_Platforming/Animation",
			"GASExample/Variant_Combat",
			"GASExample/Variant_Combat/AI",
			"GASExample/Variant_Combat/Animation",
			"GASExample/Variant_Combat/Gameplay",
			"GASExample/Variant_Combat/Interfaces",
			"GASExample/Variant_Combat/UI",
			"GASExample/Variant_SideScrolling",
			"GASExample/Variant_SideScrolling/AI",
			"GASExample/Variant_SideScrolling/Gameplay",
			"GASExample/Variant_SideScrolling/Interfaces",
			"GASExample/Variant_SideScrolling/UI"
		});
        // bEnableUndefinedIdentifierWarnings = false;
        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
