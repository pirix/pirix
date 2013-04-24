import os

vars = Variables()
vars.Add("ARCH", "target architecture", "i386")
vars.Add("PLATFORM", "target platform", "i386")
vars.Add("DEBUG", "set to 1 for debug", 0)
vars.Add("VERSION", "current version", "0.1")

env = Environment(
    ENV=os.environ,
    variables=vars,
    tools=["default", "textfile"],
)

if env["DEBUG"]:
    env["CFLAGS"] = ["-g"]
else:
    env["ASPPCOMSTR"] = "[AS] $TARGET"
    env["CCCOMSTR"] ="[CC] $TARGET"
    env["LINKCOMSTR"] = "[LD] $TARGET"
    env["ARCOMSTR"] = "[AR] $TARGET"
    env["RANLIBCOMSTR"] = "[RL] $TARGET"

host = env.Clone(
    CFLAGS=["-std=gnu99"],
    CPPPATH=["#/include"],
)

target = env.Clone(
    AS="$ARCH-elf-pirix-as",
    CC="$ARCH-elf-pirix-gcc",
    CFLAGS=["-ffreestanding", "-fno-stack-protector", "-std=gnu99"],
    CPPPATH=["#/include", "#/servers"],
    LINK="$ARCH-elf-pirix-gcc",
    LINKFLAGS="-T kernel/$ARCH/link.ld -nostartfiles -nodefaultlibs",
)

env.Substfile("include/config.h.in", SUBST_DICT={
    "@DEBUG@": env["DEBUG"],
    "@VERSION@": env["VERSION"],
    "@PLATFORM@": env["PLATFORM"],
    "@PLATFORM_LOWER@": env["PLATFORM"].lower(),
})

env.Substfile("include/arch.h.in", SUBST_DICT={
    "@ARCH@": env["ARCH"],
})

target.SConscript("kernel/SConscript", exports="target",
                  variant_dir="build/$ARCH/kernel")

target.SConscript("lib/SConscript", exports="target",
                  variant_dir="build/$ARCH/lib")

target.SConscript("servers/SConscript", exports="target",
                  variant_dir="build/$ARCH/servers")

host.SConscript("tools/SConscript", exports="host",
                variant_dir="build/tools")


modules = [
    "build/$ARCH/kernel/kernel",
    "build/$ARCH/servers/vfs/vfs",
    "build/$ARCH/servers/devmgr/devmgr"
]

cmd = "build/tools/pimg -o $TARGET -k %s" % " ".join(modules)
image = target.Command("build/$ARCH/pirix.img", "", cmd)
target.Depends(image, "build/tools/pimg")
target.Depends(image, modules)
