// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class ShiftQueueEditorPlugin : ModuleRules
    {
        public ShiftQueueEditorPlugin(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateIncludePaths.Add("ShiftQueueEditorPlugin/Private");
            PublicIncludePaths.Add("ShiftQueueEditorPlugin/Public");

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Slate",
                    "SlateCore",
                    "Engine",
                    "UnrealEd",
                    "ShiftQueue"
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "EditorStyle"
                }
            );
        }
    }
}
