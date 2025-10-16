// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OCEditorTarget : TargetRules
{
	public OCEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        CppStandard = CppStandardVersion.Cpp20; 
		//BuildEnvironment = TargetBuildEnvironment.Unique;
        ExtraModuleNames.AddRange( new string[] { "OC" } );
	}
}
