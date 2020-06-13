-- The config file for the converter.

-- vsndevts file to generate
outputFileName = "addon.vsndevts"
-- sounds folder to import
inputPath = "path_to_sounds_folder"
-- sound type to use. currently only the parameters for hlvr_default_3d are implemented
soundType = "hlvr_default_3d"
-- enable variations on sounds. currently broken, do not use!
useVariations = false
-- run a function, perFile(), perFile
perFileFunction = false
-- overide the in game name. useful only for the perfile function, elsewise it sets everything to the same name
overideInGameName = false
-- global volume for all sounds
volume = 0.5
-- global hrtf for all sounds
hrtf = 1.0
-- perFile() function. Ran if perFileFunction is true. Exposed variables are currentFilePathNoExt, and currentInGameName.
-- The Lua standard library is also included. Modifiable variables are volume, and hrtf, also inGameName if overideInGameName is set to true.
-- volume, hrtf, and inGameName are reset per file!
function perFile()
	-- code goes here
end