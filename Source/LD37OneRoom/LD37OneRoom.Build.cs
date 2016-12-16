using UnrealBuildTool;

public class LD37OneRoom : ModuleRules
{
	public LD37OneRoom(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
