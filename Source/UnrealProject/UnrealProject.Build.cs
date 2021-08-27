// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class UnrealProject : ModuleRules
{
    public UnrealProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Public 폴더 참조
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
            "UMG", "NavigationSystem", "AIModule", "GamePlayTasks" });

        // Private 폴더 참조
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealProjectSetting" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
