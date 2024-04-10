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
				"HTTP",
				"Json"
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
			new string[]
			{
				"StatsigUnreal/Private/statsig_compatibility/json",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/include",
				"StatsigUnreal/Private/statsig_compatibility",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src",
			}
		);

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "Private/statsig_compatibility/json"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/include"),
				Path.Combine(ModuleDirectory, "Private/statsig_compatibility"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src"),
			}
		);
	}
}