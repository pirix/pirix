import os

vars = Variables()
vars.Add("ARCH", "target architecture", "i386")
vars.Add("PLATFORM", "target platform", "i386")
vars.Add("DEBUG", "set to 1 for debug", 0)
vars.Add("VERSION", "current version", "0.1")
vars.Add("PREFIX", "install prefix", "/usr")

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

host = env.Clone(
    CPPPATH=["#/include"],
)
host.Append(
    CFLAGS=["-std=gnu99"],
)

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

env.Substfile("include/config.h.in", SUBST_DICT={
    "@ARCH@": env["ARCH"],
    "@DEBUG@": env["DEBUG"],
    "@VERSION@": env["VERSION"],
    "@PLATFORM@": env["PLATFORM"],
    "@PLATFORM_LOWER@": env["PLATFORM"].lower(),
})

env.Substfile("include/arch.h.in", SUBST_DICT={
    "@ARCH@": env["ARCH"],
})

kernel = target.SConscript("kernel/SConscript", exports="target",
                           variant_dir="build/$ARCH/kernel")

libs = target.SConscript("lib/SConscript", exports="target",
                         variant_dir="build/$ARCH/lib")

servers = target.SConscript("servers/SConscript", exports="target",
                            variant_dir="build/$ARCH/servers")

host.SConscript("tools/SConscript", exports="host",
                variant_dir="build/tools")


modules = [
    "build/$ARCH/kernel/kernel",
    "build/$ARCH/servers/vfs/vfs",
    "build/$ARCH/servers/login/login"
]

cmd = "build/tools/pimg -o $TARGET -k %s" % " ".join(modules)
image = target.Command("build/$ARCH/pirix.img", "", cmd)
target.Depends(image, "build/tools/pimg")
target.Depends(image, modules)

env.Install("$PREFIX/bin", [kernel] + servers)
env.Install("$PREFIX/boot", image)
env.Install("$PREFIX/lib", libs)
env.Install("$PREFIX/include", Glob("include/pirix.h"))
env.Install("$PREFIX/include/pirix", Glob("include/pirix/*.h"))
env.Install("$PREFIX/include/sys", Glob("include/sys/*.h"))
env.Alias("install", "$PREFIX")
