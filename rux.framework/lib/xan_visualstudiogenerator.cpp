#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <xan_visualstudiogenerator.h>
#include <xan_path.h>
#include <xan_keyvaluepair.h>
#include <xan_directory.h>
#include <xan_uint64.h>
begin_implement_rux_class_ns( VisualStudioConfiguration , rux::generator );
end_implement_rux_class();
namespace rux
{
	namespace generator
	{
		rux::String& XVisualStudioGenerator::GenerateVS2010CppProject( rux::XArray< XVisualStudioConfiguration >& configurations , rux::XArray< rux::XString >& sources , const rux::XString& project_directory )
		{	
			rux::XString vs2010_project , vs2010_project_directory , string0 , CLCompile , CLInclude , YASM , ResourceCompile;
			vs2010_project_directory.set_ByRef( rux::io::XPath::ExcludeRelative( project_directory ) );
			vs2010_project += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			vs2010_project += "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
			vs2010_project += " <ItemGroup Label=\"ProjectConfigurations\">\n";
			rux::uint32 index0 = 0 , index1 = 0 , index2 = 0;
			for( index0 = 0 ; index0 < configurations.Count() ; index0++ )
				vs2010_project += configurations[ index0 ]._project_configuration;
			vs2010_project += " </ItemGroup>\n";
			for( index0 = 0 ; index0 < sources.Count() ; index0++ )
			{
				string0.set_ByRef( rux::io::XPath::GetFileName( sources[ index0 ] ) );
				for( index1 = 0 ; index1 < configurations.Count() ; index1++ )
				{
					for( index2 = 0 ; index2 < configurations[ index1 ]._excluded_sources.Count() ; index2++ )
					{
						if( configurations[ index1 ]._excluded_sources[ index2 ] == string0 )
							break;
					}
					if( index2 < configurations[ index1 ]._excluded_sources.Count() )
						break;
				}
				if( index1 >= configurations.Count() )
				{
					string0.set_ByRef( rux::io::XPath::GetFileNameExtension( sources[ index0 ] ) );
					string0.set_ByRef( string0.ToLower() );
					if( string0.Equals( "cpp" , 3 ) )
					{
						if( CLCompile.Length() == 0 )
							CLCompile += " <ItemGroup>\n";
						string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , sources[ index0 ] ) );
						if( string0.IndexOf( "./" ) == 0 )
							string0.RemoveRange( 0 , 2 );
						CLCompile += "  <ClCompile Include=\"" + string0 + "\"";
						string0.set_ByRef( rux::io::XPath::GetFileName( sources[ index0 ] ) );
						for( index1 = 0 ; index1 < configurations.Count() ; index1++ )
						{
							for( index2 = 0 ; index2 < configurations[ index1 ]._excluded_sources.Count() ; index2++ )
							{
								if( configurations[ index1 ]._excluded_sources[ index2 ] == string0 )
								{
									if( CLCompile.get_UTF8Char( CLCompile.Length() - 1 ) == '"' )
										CLCompile += ">\n";
									CLCompile += "   <ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[ index1 ]._release_or_debug + "|" + configurations[ index1 ]._architecture + "'\">true</ExcludedFromBuild>\n";
									break;
								}
							}					
						}
						if( CLCompile.get_UTF8Char( CLCompile.Length() - 1 ) == '"' )
							CLCompile += " />\n";
						else
							CLCompile += "  </CLCompile>\n";
					}	
					else if( string0.Equals( "h" , 1 ) )
					{
						if( CLInclude.Length() == 0 )
							CLInclude += " <ItemGroup>\n";
						string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , sources[ index0 ] ) );
						if( string0.IndexOf( "./" ) == 0 )
							string0.RemoveRange( 0 , 2 );
						CLInclude += "  <ClInclude Include=\"" + string0 + "\"";
						string0.set_ByRef( rux::io::XPath::GetFileName( sources[ index0 ] ) );
						for( index1 = 0 ; index1 < configurations.Count() ; index1++ )
						{
							for( index2 = 0 ; index2 < configurations[ index1 ]._excluded_sources.Count() ; index2++ )
							{
								if( configurations[ index1 ]._excluded_sources[ index2 ] == string0 )
								{
									if( CLInclude.get_UTF8Char( CLInclude.Length() - 1 ) == '"' )
										CLInclude += ">\n";
									CLInclude += "   <ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[ index1 ]._release_or_debug + "|" + configurations[ index1 ]._architecture + "'\">true</ExcludedFromBuild>\n";
									break;
								}
							}					
						}
						if( CLInclude.get_UTF8Char( CLInclude.Length() - 1 ) == '"' )
							CLInclude += " />\n";
						else
							CLInclude += "  </CLInclude>\n";
					}	
					else if( string0.Equals( "rc" , 2 ) )
					{
						if( ResourceCompile.Length() == 0 )
							ResourceCompile += " <ItemGroup>\n";
						string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , sources[ index0 ] ) );
						if( string0.IndexOf( "./" ) == 0 )
							string0.RemoveRange( 0 , 2 );
						ResourceCompile += "  <ResourceCompile Include=\"" + string0 + "\"";
						string0.set_ByRef( rux::io::XPath::GetFileName( sources[ index0 ] ) );
						for( index1 = 0 ; index1 < configurations.Count() ; index1++ )
						{
							for( index2 = 0 ; index2 < configurations[ index1 ]._excluded_sources.Count() ; index2++ )
							{
								if( configurations[ index1 ]._excluded_sources[ index2 ] == string0 )
								{
									if( ResourceCompile.get_UTF8Char( ResourceCompile.Length() - 1 ) == '"' )
										ResourceCompile += ">\n";
									ResourceCompile += "   <ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[ index1 ]._release_or_debug + "|" + configurations[ index1 ]._architecture + "'\">true</ExcludedFromBuild>\n";
									break;
								}
							}					
						}
						if( ResourceCompile.get_UTF8Char( ResourceCompile.Length() - 1 ) == '"' )
							ResourceCompile += " />\n";
						else
							ResourceCompile += "  </ResourceCompile>\n";
					}
					else if( string0.Equals( "s" , 1 ) )
					{
						if( YASM.Length() == 0 )
							YASM += " <ItemGroup>\n";
						string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , sources[ index0 ] ) );
						if( string0.IndexOf( "./" ) == 0 )
							string0.RemoveRange( 0 , 2 );
						YASM += "  <YASM Include=\"" + string0 + "\">\n";
						YASM += "   <FileType>Document</FileType>\n";
						string0.set_ByRef( rux::io::XPath::GetFileName( sources[ index0 ] ) );
						for( index1 = 0 ; index1 < configurations.Count() ; index1++ )
						{
							for( index2 = 0 ; index2 < configurations[ index1 ]._excluded_sources.Count() ; index2++ )
							{
								if( configurations[ index1 ]._excluded_sources[ index2 ] == string0 )
								{
									YASM += "   <ExcludedFromBuild Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[ index1 ]._release_or_debug + "|" + configurations[ index1 ]._architecture + "'\">true</ExcludedFromBuild>\n";
									break;
								}
							}					
						}
						YASM += "  </YASM>\n";
					}	
				}
			}
			if( CLCompile.Length() > 0 )
			{
				CLCompile += " </ItemGroup>\n";
				vs2010_project += CLCompile;
			}
			if( CLInclude.Length() > 0 )
			{
				CLInclude += " </ItemGroup>\n";
				vs2010_project += CLInclude;
			}
			if( ResourceCompile.Length() > 0 )
			{
				ResourceCompile += " </ItemGroup>\n";
				vs2010_project += ResourceCompile;
			}
			if( YASM.Length() > 0 )
			{
				YASM += " </ItemGroup>\n";
				vs2010_project += YASM;
			}
			vs2010_project += " <PropertyGroup Label=\"Globals\">\n";
			vs2010_project += "  <ProjectGuid>{956C360E-FDD6-4A65-90EC-35EE651F81E5}</ProjectGuid>\n";
			vs2010_project += "  <Keyword>Win32Proj</Keyword>\n";
			vs2010_project += "  <RootNamespace>ruxbasis1</RootNamespace>\n";
			vs2010_project += " </PropertyGroup>\n";
			vs2010_project += " <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n";
			for( index0 = 0 ; index0 < configurations.Count() ; index0++ )
				vs2010_project += configurations[ index0 ]._configuration;
			vs2010_project += " <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n";
			vs2010_project += " <ImportGroup Label=\"ExtensionSettings\">\n";
			if( YASM.Length() > 0 )
				vs2010_project += "  <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\vsyasm.props\" />\n";
			vs2010_project += " </ImportGroup>\n";
			for( index0 = 0 ; index0 < configurations.Count() ; index0++ )
			{
				vs2010_project += " <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='" + configurations[ index0 ]._release_or_debug + "|" + configurations[ index0 ]._architecture + "'\">\n";
				vs2010_project += "  <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n";
				vs2010_project += " </ImportGroup>\n";
			}
			vs2010_project += " <PropertyGroup Label=\"UserMacros\" />\n";
			for( index0 = 0 ; index0 < configurations.Count() ; index0++ )
				vs2010_project += configurations[ index0 ]._out_int_dir_target_name;
			for( index0 = 0 ; index0 < configurations.Count() ; index0++ )
				vs2010_project += configurations[ index0 ]._item_definition_group;
			vs2010_project += " <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n";
			vs2010_project += " <ImportGroup Label=\"ExtensionTargets\">\n";
			if( YASM.Length() > 0 )
				vs2010_project += "  <Import Project=\"$(VCTargetsPath)\\BuildCustomizations\\vsyasm.targets\" />\n";
			vs2010_project += " </ImportGroup>\n";
			vs2010_project += "</Project>\n";
			return vs2010_project++;
		};
		VisualStudioConfiguration& XVisualStudioGenerator::GenerateVS2010CppProjectConfiguration( const rux::XString& platform , const rux::XString& configuration , rux::XArray< rux::XString >& excluded_sources , XEnum_OutputFile output_file_type_enum , const rux::XString& output_file , const rux::XString& project_directory , rux::XArray< rux::XString >& preprocessors , rux::XArray< rux::XString >& include_paths , rux::XArray< rux::XString >& libs , const rux::XString& post_build , const rux::XString& pre_build , rux::XArray< rux::XString >& lib_paths , rux::uint8 require_administrator )
		{
			rux::XString vs2010_project_directory;
			vs2010_project_directory.set_ByRef( rux::io::XPath::ExcludeRelative( project_directory ) );
			XVisualStudioConfiguration vs2010_configuration;
			vs2010_configuration()->_excluded_sources = excluded_sources;
			vs2010_configuration()->_architecture = platform;
			vs2010_configuration()->_release_or_debug = configuration;
			vs2010_configuration()->_architecture.set_ByRef( vs2010_configuration()->_architecture.ToLower() );
			if( vs2010_configuration()->_architecture.Equals( "x86" , 3 ) 
				|| vs2010_configuration()->_architecture.Equals( "win32" , 5 ) )
				vs2010_configuration()->_architecture = "Win32";
			else
				vs2010_configuration()->_architecture = "x64";
			vs2010_configuration()->_release_or_debug.set_ByRef( vs2010_configuration()->_release_or_debug.ToLower() );
			if( vs2010_configuration()->_release_or_debug.Equals( "release" , 7 ) )
				vs2010_configuration()->_release_or_debug = "Release";
			else
				vs2010_configuration()->_release_or_debug = "Debug";
			vs2010_configuration()->_project_configuration += "  <ProjectConfiguration Include=\"" + vs2010_configuration()->_release_or_debug + "|" + vs2010_configuration()->_architecture + "\">\n";
			vs2010_configuration()->_project_configuration += "   <Configuration>" + vs2010_configuration()->_release_or_debug + "</Configuration>\n";
			vs2010_configuration()->_project_configuration += "   <Platform>" + vs2010_configuration()->_architecture + "</Platform>\n";
			vs2010_configuration()->_project_configuration += "  </ProjectConfiguration>\n";	
			vs2010_configuration()->_configuration += " <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='" + vs2010_configuration()->_release_or_debug + "|" + vs2010_configuration()->_architecture + "'\" Label=\"Configuration\">\n";
			if( output_file_type_enum == XEnum_OutputFile_StaticLibrary )
			{
				vs2010_configuration()->_configuration += "  <ConfigurationType>StaticLibrary</ConfigurationType>\n";
				if( vs2010_configuration()->_release_or_debug == "Debug" )
					vs2010_configuration()->_configuration += "  <UseDebugLibraries>true</UseDebugLibraries>\n";
				else
					vs2010_configuration()->_configuration += "  <UseDebugLibraries>false</UseDebugLibraries>\n";
			}
			else if( output_file_type_enum == XEnum_OutputFile_DynamicLibrary )
				vs2010_configuration()->_configuration += "  <ConfigurationType>DynamicLibrary</ConfigurationType>\n";
			else if( output_file_type_enum == XEnum_OutputFile_Console )
			{
				vs2010_configuration()->_configuration += "  <ConfigurationType>Application</ConfigurationType>\n";
				vs2010_configuration()->_configuration += "  <WholeProgramOptimization>false</WholeProgramOptimization>\n";
			}	
			vs2010_configuration()->_configuration += "  <CharacterSet>NotSet</CharacterSet>\n";	
			vs2010_configuration()->_configuration += " </PropertyGroup>\n";
			rux::XString OutDir , string0;
			OutDir.set_ByRef( rux::io::XPath::GetDirectoryName( output_file ) );
			OutDir.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , OutDir ) );
			if( OutDir.IndexOf( "./" ) == 0 )
				OutDir.set_ByRef( OutDir.ReplaceFirst( "./" , "$(ProjectDir)/" ) );
			OutDir.set_ByRef( rux::io::XDirectory::TransformToRuxDirectoryName( OutDir ) );
			OutDir += "/";
			vs2010_configuration()->_out_int_dir_target_name += " <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='" + vs2010_configuration()->_release_or_debug + "|" + vs2010_configuration()->_architecture + "'\">\n";
			vs2010_configuration()->_out_int_dir_target_name += "  <OutDir>" + OutDir + "</OutDir>\n";
			vs2010_configuration()->_out_int_dir_target_name += "  <TargetName>" + rux::io::XPath::GetFileNameWithoutExtension( output_file ) + "</TargetName>\n";	
			if( output_file_type_enum == XEnum_OutputFile_Console 
				|| output_file_type_enum == XEnum_OutputFile_DynamicLibrary )
			{
				vs2010_configuration()->_out_int_dir_target_name += "  <IntDir>" + vs2010_configuration()->_architecture + "\\$(Configuration)\\</IntDir>\n";
				vs2010_configuration()->_out_int_dir_target_name += "  <LinkIncremental>false</LinkIncremental>\n";
			}
			vs2010_configuration()->_out_int_dir_target_name += " </PropertyGroup>\n";
			rux::uint32 index0 = 0;
			vs2010_configuration()->_item_definition_group += " <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='" + vs2010_configuration()->_release_or_debug + "|" + vs2010_configuration()->_architecture + "'\">\n";     		
			vs2010_configuration()->_item_definition_group += "  <ClCompile>\n";
			if( vs2010_configuration()->_release_or_debug == "Release" )
				vs2010_configuration()->_item_definition_group += "   <IntrinsicFunctions>false</IntrinsicFunctions>\n";
			if( output_file_type_enum != XEnum_OutputFile_StaticLibrary
				&& vs2010_configuration()->_release_or_debug == "Release" )
				vs2010_configuration()->_item_definition_group += "   <WholeProgramOptimization>false</WholeProgramOptimization>\n";
			vs2010_configuration()->_item_definition_group += "   <PrecompiledHeader>\n";
			vs2010_configuration()->_item_definition_group += "   </PrecompiledHeader>\n";
			vs2010_configuration()->_item_definition_group += "   <WarningLevel>Level4</WarningLevel>\n";
			vs2010_configuration()->_item_definition_group += "   <Optimization>Disabled</Optimization>\n";
			vs2010_configuration()->_item_definition_group += "   <PreprocessorDefinitions>";
			if( vs2010_configuration()->_release_or_debug == "Debug" )
				vs2010_configuration()->_item_definition_group += "_DEBUG;";
			else
				vs2010_configuration()->_item_definition_group += "NDEBUG;";
			if( vs2010_configuration()->_architecture == "Win32" )
				vs2010_configuration()->_item_definition_group += "WIN32;";
			else
				vs2010_configuration()->_item_definition_group += "WIN64;";
			for( index0 = 0 ; index0 < preprocessors.Count() ; index0++ )
				vs2010_configuration()->_item_definition_group += preprocessors[ index0 ] + ";";
			vs2010_configuration()->_item_definition_group += "%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
			vs2010_configuration()->_item_definition_group += "   <AdditionalIncludeDirectories>\n";
			for( index0 = 0 ; index0 < include_paths.Count() ; index0++ )
			{
				string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , include_paths[ index0 ] ) );
				if( string0.IndexOf( "./" ) == 0 )
					string0.set_ByRef( string0.ReplaceFirst( "./" , "$(ProjectDir)/" ) );
				vs2010_configuration()->_item_definition_group += string0 + ";";
			}
			vs2010_configuration()->_item_definition_group += "</AdditionalIncludeDirectories>\n";
			if( vs2010_configuration()->_release_or_debug == "Debug" )
				vs2010_configuration()->_item_definition_group += "   <MinimalRebuild>false</MinimalRebuild>\n";
			vs2010_configuration()->_item_definition_group += "   <ExceptionHandling>Sync</ExceptionHandling>\n";
			if( vs2010_configuration()->_release_or_debug == "Debug" )
				vs2010_configuration()->_item_definition_group += "   <RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>\n";
			else
				vs2010_configuration()->_item_definition_group += "   <RuntimeLibrary>MultiThreaded</RuntimeLibrary>\n";
			if( output_file_type_enum == XEnum_OutputFile_StaticLibrary )
				vs2010_configuration()->_item_definition_group += "   <ProgramDataBaseFileName>$(TargetDir)\\" + rux::io::XPath::GetFileNameWithoutExtension( output_file ) + "</ProgramDataBaseFileName>\n";
			vs2010_configuration()->_item_definition_group += "   <FavorSizeOrSpeed>Size</FavorSizeOrSpeed>\n";
			vs2010_configuration()->_item_definition_group += "   <StringPooling>false</StringPooling>\n";
			if( vs2010_configuration()->_release_or_debug == "Debug" )
				vs2010_configuration()->_item_definition_group += "   <BasicRuntimeChecks>Default</BasicRuntimeChecks>\n";
			vs2010_configuration()->_item_definition_group += "   <FunctionLevelLinking>true</FunctionLevelLinking>\n";
			vs2010_configuration()->_item_definition_group += "   <PrecompiledHeaderFile>\n";
			vs2010_configuration()->_item_definition_group += "   </PrecompiledHeaderFile>\n";
			vs2010_configuration()->_item_definition_group += "   <PrecompiledHeaderOutputFile>\n";
			vs2010_configuration()->_item_definition_group += "   </PrecompiledHeaderOutputFile>\n";
			if( vs2010_configuration()->_release_or_debug == "Debug" )
				vs2010_configuration()->_item_definition_group += "   <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\n";
			vs2010_configuration()->_item_definition_group += "  </ClCompile>\n";
			vs2010_configuration()->_item_definition_group += "  <Link>\n";
			if( output_file_type_enum != XEnum_OutputFile_StaticLibrary )
			{
				vs2010_configuration()->_item_definition_group += "   <AdditionalDependencies>";
				for( index0 = 0 ; index0 < libs.Count() ; index0++ )
					vs2010_configuration()->_item_definition_group += libs[ index0 ] + ";";
				vs2010_configuration()->_item_definition_group += "%(AdditionalDependencies)</AdditionalDependencies>\n";
				vs2010_configuration()->_item_definition_group += "   <OutputFile>" + OutDir + rux::io::XPath::GetFileName( output_file ) + "</OutputFile>\n";
				vs2010_configuration()->_item_definition_group += "   <AdditionalLibraryDirectories>";
				for( index0 = 0 ; index0 < lib_paths.Count() ; index0++ )
				{
					string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , lib_paths[ index0 ] ) );
					if( string0.IndexOf( "./" ) == 0 )
						string0.set_ByRef( string0.ReplaceFirst( "./" , "$(ProjectDir)/" ) );
					vs2010_configuration()->_item_definition_group += string0 + ";";
				}
				vs2010_configuration()->_item_definition_group += "%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\n";
				if( vs2010_configuration()->_architecture == "Win32" )
					vs2010_configuration()->_item_definition_group += "   <TargetMachine>MachineX86</TargetMachine>\n";
				else
					vs2010_configuration()->_item_definition_group += "   <TargetMachine>MachineX64</TargetMachine>\n";
				vs2010_configuration()->_item_definition_group += "   <LinkTimeCodeGeneration>Default</LinkTimeCodeGeneration>\n";
			}
			if( output_file_type_enum == XEnum_OutputFile_Console )
			{
				if( require_administrator == 1 )
					vs2010_configuration()->_item_definition_group += "   <UACExecutionLevel>RequireAdministrator</UACExecutionLevel>\n";
				else
					vs2010_configuration()->_item_definition_group += "   <UACExecutionLevel>AsInvoker</UACExecutionLevel>\n";			
				vs2010_configuration()->_item_definition_group += "   <SubSystem>Console</SubSystem>\n";
			}
			else
				vs2010_configuration()->_item_definition_group += "   <SubSystem>Windows</SubSystem>\n";
			if( output_file_type_enum == XEnum_OutputFile_DynamicLibrary )
			{
				vs2010_configuration()->_item_definition_group += "   <ModuleDefinitionFile>\n";
				vs2010_configuration()->_item_definition_group += "   </ModuleDefinitionFile>\n";
				vs2010_configuration()->_item_definition_group += "   <ImportLibrary>$(OutDir)$(TargetName).lib</ImportLibrary>\n";
				if( vs2010_configuration()->_release_or_debug == "Debug" )
				{      
					vs2010_configuration()->_item_definition_group += "   <ProgramDatabaseFile>$(TargetDir)$(TargetName).pdb</ProgramDatabaseFile>\n";
					vs2010_configuration()->_item_definition_group += "   <IgnoreAllDefaultLibraries>false</IgnoreAllDefaultLibraries>\n";
				}
			}	
			vs2010_configuration()->_item_definition_group += "   <GenerateDebugInformation>true</GenerateDebugInformation>\n";
			vs2010_configuration()->_item_definition_group += "   <EnableCOMDATFolding>true</EnableCOMDATFolding>\n";
			vs2010_configuration()->_item_definition_group += "   <OptimizeReferences>true</OptimizeReferences>\n";
			vs2010_configuration()->_item_definition_group += "  </Link>\n";
			if( output_file_type_enum == XEnum_OutputFile_StaticLibrary )
			{
				vs2010_configuration()->_item_definition_group += "  <Lib>\n";
				vs2010_configuration()->_item_definition_group += "   <AdditionalDependencies>";
				for( index0 = 0 ; index0 < libs.Count() ; index0++ )
					vs2010_configuration()->_item_definition_group += libs[ index0 ] + ";";
				vs2010_configuration()->_item_definition_group += "%(AdditionalDependencies)</AdditionalDependencies>\n";
				vs2010_configuration()->_item_definition_group += "   <OutputFile>" + OutDir + rux::io::XPath::GetFileName( output_file ) + "</OutputFile>\n";
				vs2010_configuration()->_item_definition_group += "   <AdditionalLibraryDirectories>";
				for( index0 = 0 ; index0 < lib_paths.Count() ; index0++ )
				{
					string0.set_ByRef( rux::io::XPath::CalculateRelative( vs2010_project_directory , lib_paths[ index0 ] ) );
					if( string0.IndexOf( "./" ) == 0 )
						string0.set_ByRef( string0.ReplaceFirst( "./" , "$(ProjectDir)/" ) );
					vs2010_configuration()->_item_definition_group += string0 + ";";
				}
				vs2010_configuration()->_item_definition_group += "%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>\n";
				if( vs2010_configuration()->_architecture == "Win32" )
					vs2010_configuration()->_item_definition_group += "   <TargetMachine>MachineX86</TargetMachine>\n";
				else
					vs2010_configuration()->_item_definition_group += "   <TargetMachine>MachineX64</TargetMachine>\n";
				if( vs2010_configuration()->_release_or_debug == "Debug" )
				{
					vs2010_configuration()->_item_definition_group += "   <ForceSymbolReferences>\n";
					vs2010_configuration()->_item_definition_group += "   </ForceSymbolReferences>\n";
					vs2010_configuration()->_item_definition_group += "   <IgnoreAllDefaultLibraries>false</IgnoreAllDefaultLibraries>\n";
				}
				vs2010_configuration()->_item_definition_group += "  </Lib>\n";
			}
			if( post_build.Length() > 0 )
			{
				vs2010_configuration()->_item_definition_group += "  <PostBuildEvent>\n";
				vs2010_configuration()->_item_definition_group += "   <Command>" + post_build + "</Command>\n";
				vs2010_configuration()->_item_definition_group += "  </PostBuildEvent>\n";
			}
			if( pre_build.Length() > 0 )
			{
				vs2010_configuration()->_item_definition_group += "  <PreBuildEvent>\n";
				vs2010_configuration()->_item_definition_group += "   <Command>" + pre_build + "</Command>\n";
				vs2010_configuration()->_item_definition_group += "  </PreBuildEvent>\n";
			}
			vs2010_configuration()->_item_definition_group += " </ItemDefinitionGroup>\n";
			return vs2010_configuration++;
		};
	};
};