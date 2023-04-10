set_languages("c++20")
add_rules("mode.debug", "mode.release")

local libs = { "fmt", "gtest", "prepucio", "cxxopts" }

add_includedirs("include")
add_requires(table.unpack(libs))

target("interpreter-lib")
  set_kind("static")
  add_files("source/**/*.cpp")
  add_packages(table.unpack(libs))

target("interpreter")
  set_kind("binary")
  add_files("standalone/main.cpp")
  add_packages(table.unpack(libs))
  add_deps("interpreter-lib")

target("test")
  set_kind("binary")
  add_files("test/*.cpp")
  add_packages(table.unpack(libs))
  add_deps("interpreter-lib")

-- Post build copy assets folder
after_build(function(target)
    os.cp("assets", target:targetdir())
end)
