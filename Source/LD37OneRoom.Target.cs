using UnrealBuildTool;
using System.Collections.Generic;

public class LD37OneRoomTarget : TargetRules
{
	public LD37OneRoomTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.Add("LD37OneRoom");
	}
}
