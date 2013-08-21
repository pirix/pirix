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
    AS="$ARCH-elf-pirix-gcc",
    CC="$ARCH-elf-pirix-gcc",
    CPPPATH=["#/include", "#/build/$ARCH/include"],
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
    variant_dir="build/$ARCH/kernel",
    duplicate=False
)

libs = target.SConscript(
    "lib/SConscript",
    exports="target",
    variant_dir="build/$ARCH/lib",
    duplicate=False
)

servers = target.SConscript(
    "servers/SConscript",
    exports="target",
    variant_dir="build/$ARCH/servers",
    duplicate=False
)

#
# Provide architecture headers
#

def SymLink(target, source, env):
    src = os.path.abspath(str(source[0]))
    tgt = os.path.abspath(str(target[0]))
    os.symlink(src, tgt)

headers = target.Command("build/$ARCH/include/arch",
                         "#/include/$ARCH",
                         SymLink)
Requires(kernel, headers)

if target["ARCH"] == "arm":
    platform = target.Command("build/$ARCH/include/platform.h",
                              "#/include/$ARCH/platforms/${PLATFORM.lower()}.h",
                              SymLink)
    Requires(kernel, platform)

#
# Doxygen documentation
#

doxygen = env.Command("build/doc/", "", "doxygen doc/Doxyfile")
env.Depends(doxygen, doxyfile)
env.AlwaysBuild(doxygen)
env.Alias("doc", doxygen)

target.Depends(libs, kernel)
target.Depends(servers, kernel)
Default(kernel, libs, servers)
