-- SPDX-License-Identifier: MIT
-- Copyright (c) 2026 Julien Bernard
set_project("random")
set_version("1.0.0")

add_rules("mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "$(builddir)"})

set_policy("build.warning", true)
set_warnings("allextra", "pedantic")
set_languages("cxx20")
set_encodings("utf-8")

target("random")
    set_kind("static")
    add_files("Random.cc")
    add_headerfiles("Random.h)")
    set_license("MIT")

target("example")
    set_kind("binary")
    add_files("example.cc")
    add_deps("random")
    set_license("MIT")
