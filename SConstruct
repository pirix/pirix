import os

#
# Build Variables
#

vars = Variables()
vars.Add("ARCH", "target architecture", "i386")
vars.Add("PLATFORM", "target platform", "i386")
vars.Add("DEBUG", "set to 1 for debug", 0)
vars.Add("VERSION", "current version", "0.1")
vars.Add("INSTALLPATH", "install path", "/")

#
# Common Environment
#

env = Environment(
    ENV=os.environ,
    variables=vars,
    tools=["default", "textfile"],
)

if env["DEBUG"]:
    env["CFLAGS"] = ["-g", "-O0"]
    env["ASFLAGS"] = ["-g"]
else:
    env["CFLAGS"] = ["-O3", "-w"]
    env["ASPPCOMSTR"] = "[AS] $TARGET"
    env["CCCOMSTR"] ="[CC] $TARGET"
    env["LINKCOMSTR"] = "[LD] $TARGET"
    env["ARCOMSTR"] = "[AR] $TARGET"
    env["RANLIBCOMSTR"] = "[RL] $TARGET"

#
# Host Environment
#

host = env.Clone(
    CPPPATH=["#/include"],
)
host.Append(
    CFLAGS=["-std=gnu99"],
)

#
# Target Environment
#

target = env.Clone(
    AS="$ARCH-elf-pirix-as",
    CC="$ARCH-elf-pirix-gcc",
    CPPPATH=["#/include", "#/servers"],
    LINK="$ARCH-elf-pirix-gcc",
)
target.Append(
    CFLAGS=["-std=gnu99"],
)

if target["ARCH"] == "arm":
    target.Append(
        CFLAGS=["-march=armv6"],
        ASFLAGS=["-march=armv6"],
    )

#
# Substitution of build variables
#

config_h = env.Substfile("include/config.h.in", SUBST_DICT={
    "@ARCH@": env["ARCH"],
    "@DEBUG@": env["DEBUG"],
    "@VERSION@": env["VERSION"],
    "@PLATFORM@": env["PLATFORM"],
    "@PLATFORM_LOWER@": env["PLATFORM"].lower(),
})

arch_h = env.Substfile("include/arch.h.in", SUBST_DICT={
    "@ARCH@": env["ARCH"],
})

doxyfile = env.Substfile("doc/Doxyfile.in", SUBST_DICT={
    "@VERSION@": env["VERSION"],
    "@BUILD_DIR@": Dir("build/").abspath,
    "@SOURCE_DIR@": Dir(".").abspath,
})

#
# SConscripts in subdirectories
#

kernel = target.SConscript(
    "kernel/SConscript",
    exports="target",
    variant_dir="build/$ARCH/kernel"
)

libs = target.SConscript(
    "lib/SConscript",
    exports="target",
    variant_dir="build/$ARCH/lib"
)

servers = target.SConscript(
    "servers/SConscript",
    exports="target",
    variant_dir="build/$ARCH/servers"
)

tools = host.SConscript(
    "tools/SConscript",
    exports="host",
    variant_dir="build/tools"
)

#
# pirix.img generation
#

modules = [
    "build/$ARCH/kernel/kernel",
    "build/$ARCH/servers/system/system",
    "build/$ARCH/servers/login/login"
]

cmd = "build/tools/pimg -o $TARGET -k %s" % " ".join(modules)
image = env.Command("build/$ARCH/pirix.img", "", cmd)
env.Depends(image, "build/tools/pimg")
env.Depends(image, modules)

#
# Doxygen documentation
#

doxygen = target.Command("build/doc/", "", "doxygen doc/Doxyfile")
env.Depends(doxygen, doxyfile)
env.AlwaysBuild(doxygen)
env.Alias("doc", doxygen)

#
# Install target
#

env.Install("$INSTALLPATH/boot", [kernel, image])
env.Install("$INSTALLPATH/sbin", servers)
env.Install("$INSTALLPATH/usr/lib", libs)
env.Install("$INSTALLPATH/usr/include", Glob("include/pirix.h"))
env.Install("$INSTALLPATH/usr/include/pirix", Glob("include/pirix/*.h"))
env.Install("$INSTALLPATH/usr/include/sys", Glob("include/sys/*.h"))
env.Alias("install", "$INSTALLPATH")

Default(kernel, libs, servers, image)
