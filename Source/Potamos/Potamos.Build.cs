/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
using UnrealBuildTool;
using System;
using System.IO;

public class Potamos : ModuleRules
{
	public Potamos(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core"
			, "CoreUObject"
			, "Engine"
			, "InputCore"
			, "ProceduralMeshComponent"
			, "HTTP"
			, "XmlParser"
			, "RHI"
			, "RenderCore"
			, "UMG"
			, "Slate"
			, "SlateCore"
			, "Json" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
		var thisPath = ModuleDirectory;
		var depsPath = Path.Combine(thisPath,"..","..","Deps");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var windeps = Path.Combine(depsPath,"Windows");
			PublicIncludePaths.Add(windeps);
			PublicAdditionalLibraries.Add(Path.Combine(windeps,"zlibwapi.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(windeps,"openjp2.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(windeps,"whio_amalgamation.lib"));
			bEnableExceptions = true;
//			PrivateIncludePathModuleNames.Add("TextureCompressor");
//			PrivateIncludePaths.AddRange(new string[] {
//				"Developer/Windows/WindowsTargetPlatform/Private"});
		}
		else if ( Target.Platform == UnrealTargetPlatform.Linux )
		{
			var linuxdeps = Path.Combine(depsPath,"Linux");
			PublicIncludePaths.Add(linuxdeps);
			PublicAdditionalLibraries.Add("/lib/libz");
			PublicAdditionalLibraries.Add("/usr/lib64/libopenjp2.so");
			PublicAdditionalLibraries.Add(Path.Combine(linuxdeps,"libwhio_amalgamation.a"));
			PublicIncludePaths.Add("/usr/include");
			PublicIncludePaths.Add("/usr/include/openjpeg-2.1");
			
		} 
		else if ( Target.Platform == UnrealTargetPlatform.Mac )
		{
			var osxdeps = Path.Combine(depsPath,"Mac");
			PublicIncludePaths.Add(osxdeps);
			PublicAdditionalLibraries.Add(Path.Combine(osxdeps,"libwhio_amalgamation.a"));
			PublicAdditionalLibraries.Add("/usr/lib/libz.dylib");
			PublicAdditionalLibraries.Add("/usr/local/lib/libopenjp2.dylib");
			PublicIncludePaths.Add("/usr/local/include/openjpeg-2.1");
		} // additional platforms
	}
}
