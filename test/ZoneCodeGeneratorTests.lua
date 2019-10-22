ZoneCodeGeneratorTests = {}

function ZoneCodeGeneratorTests:include()
    
end

function ZoneCodeGeneratorTests:link()
    
end

function ZoneCodeGeneratorTests:project()
	local folder = TestFolder();

	project "ZoneCodeGeneratorTests"
        targetdir(TargetDirectoryTest)
		location "%{wks.location}/test/%{prj.name}"
		kind "SharedLib"
		language "C#"
		
		files {
			path.join(folder, "ZoneCodeGeneratorTests/**.cs")
		}

        vpaths { ["*"] = "test/ZoneCodeGeneratorTests" }

        nuget {
            "Moq:4.13.1",
			"MSTest.TestFramework:2.0.0",
			"MSTest.TestAdapter:2.0.0"
        }
        
        links {
            "System",
            "System.Core",
			"System.Data",
			"Moq",
			"MSTest.TestFramework"
		}
		
		ZoneCodeGenerator:link()
end
