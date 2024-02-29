/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */

using UnrealBuildTool;
using System.Collections.Generic;
using System.Linq;

public class AvinationViewerTarget : TargetRules
{
	public AvinationViewerTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.Add("AvinationViewer");
        

	}
	
}
