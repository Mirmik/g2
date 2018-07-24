#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.libs
from licant.cxx_modules import application
from licant.modules import submodule

licant.libs.include("gxx")
licant.libs.include("g1")
licant.libs.include("g2")

application("target",
	sources = ["main.cpp"],
	include_paths = ["../.."],
	include_modules = [
		("gxx", "posix"),
		("gxx.inet", "posix"),
		("gxx.print", "cout"),
		("gxx.dprint", "cout"),
		("gxx.log2", "impl"),
		("gxx.syslock", "mutex"),

		("g1"),
		("g1.allocator", "malloc"),
		("g1.time", "chrono"),
		("g1.udpgate"),

		("g2"),		
	],
)

licant.ex("target")