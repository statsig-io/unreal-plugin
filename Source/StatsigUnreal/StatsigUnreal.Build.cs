using UnrealBuildTool;
using System.IO;
using System.Linq;

public class StatsigUnreal : ModuleRules
{
	public StatsigUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDefinitions.Add("STATSIG_DISABLE_EXCEPTIONS=1");
		
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
				"StatsigUnreal/Private/compat/json",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src/public",
				"StatsigUnreal/Private/compat",
				"StatsigUnreal/Private/statsig-cpp-client-sdk/src/internal",
			}
		);

		PublicIncludePaths.AddRange(
			new string[]
			{
				Path.Combine(ModuleDirectory, "Private/compat/json"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src/public"),
				Path.Combine(ModuleDirectory, "Private/compat"),
				Path.Combine(ModuleDirectory, "Private/statsig-cpp-client-sdk/src/internal"),
			}
		);
	}
}