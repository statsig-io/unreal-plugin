using UnrealBuildTool;
using System.IO;
using System.Linq;

public class StatsigUnreal : ModuleRules
{
	public StatsigUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"StatsigUnreal/Private",
				"StatsigUnreal/Private/sdk",
				"StatsigUnreal/Private/sdk/include",
				"StatsigUnreal/Private/sdk/src"
			}
		);

		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "Private"),
				Path.Combine(ModuleDirectory, "Private/sdk"),
				Path.Combine(ModuleDirectory, "Private/sdk/include"),
				Path.Combine(ModuleDirectory, "Private/sdk/src"),
			}
		);

	}
}