from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.env import Environment, VirtualBuildEnv, VirtualRunEnv
from conan.tools.files import copy, get
from conan.tools.gnu import Autotools, AutotoolsToolchain
from conan.tools.layout import basic_layout


class LibOpenMPTConan(ConanFile):
    name = "libopenmpt"
    version = "0.7.11"
    package_type = "library"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True, "sdl/*:wayland":False, "sdl/*:alsa":False, "sdl/*:pulse":False}
    requires = ["sdl/2.30.8"]

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        basic_layout(self)

    def source(self):
        get(self, url="https://lib.openmpt.org/files/libopenmpt/src/libopenmpt-{}+release.autotools.tar.gz".format(self.version), strip_root=True)

    def generate(self):
        tc = AutotoolsToolchain(self)
        yes_no = lambda v: "yes" if v else "no"
        tc.configure_args.extend([
            "--with-pic={}".format(yes_no(self.options.fPIC)),
            "--enable-shared={}".format(yes_no(self.options.shared)),
            "--enable-static={}".format(yes_no(not self.options.shared)),
            "--without-zlib",
            "--without-mpg123",
            "--without-ogg",
            "--without-vorbis",
            "--without-vorbisfile",
            "--without-pulseaudio",
            "--without-portaudio",
            "--without-portaudiocpp",
            "--without-sndfile",
            "--without-flac",
            "--with-sdl2"])
        tc.generate()

        env = Environment()
        sdl_info = self.dependencies["sdl"].cpp_info
        env.define("SDL2_CFLAGS", "-isystem{}".format(sdl_info.includedirs[0] + "/SDL2"))
        env.define("SDL2_LIBS", "-L{} -lSDL2".format(sdl_info.libdirs[0]))
        env.vars(self).save_script("sdl_env_vars")

    def build(self):
        autotools = Autotools(self)
        autotools.configure()
        autotools.make()

    def package(self):
        autotools = Autotools(self)
        autotools.install()

    def package_info(self):
        self.cpp_info.libs = ["openmpt"]
