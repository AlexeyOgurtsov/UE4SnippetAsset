// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjAssetQuickTarget : TargetRules
{
	public ProjAssetQuickTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "ProjAssetQuick" } );

		bUseUnityBuild = false;
		bUsePCHFiles = false;
	}
}