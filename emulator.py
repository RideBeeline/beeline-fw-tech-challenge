import sys
import ctypes
import numpy as np
import pygame as pg
import subprocess

from pathlib import Path
from ctypes import POINTER, c_char, c_int, c_bool, c_uint32, byref

repo_path = Path(__file__).parent
emulator_path = repo_path / "emulator" 

# Set REBUILD to True to rebuild the firmware before running the emulator
REBUILD = True

if sys.platform == "win32":
    lib_ending = ".dll"
    platform = "WINDOWS"
    # we assume a docker based build for windows
    # build_command = ['make', 'PLATFORM=WINDOWS']
    build_command = ['docker', 'run', '--rm', '-v', f"{repo_path}:/beeline", '-w', '/beeline/emulator', 'beeline-fw-crosscompile', 'make', 'PLATFORM=WINDOWS']
elif sys.platform == "darwin":
    lib_ending = ".dylib"
    build_command = ['make', 'PLATFORM=MACOS']
else:
    lib_ending = ".so"
    build_command = ['make', 'PLATFORM=LINUX']


fmw = None
def load_lib():
    """Set up cytpes bindings to the firmware library"""
    global fmw
    fmw = ctypes.cdll.LoadLibrary(str(emulator_path / "build" / f"libbeeline{lib_ending}"))

    fmw.ei_lcd_get_screen.argtypes = [POINTER(c_int), POINTER(c_int)]   
    fmw.ei_lcd_get_screen.restype = POINTER(c_char)
    fmw.ei_loop.argtypes = []
    fmw.ei_loop.restype = None
    fmw.ei_increment_ms_since_boot.argtypes = [c_uint32]
    fmw.ei_increment_ms_since_boot.restype = None
    fmw.ei_get_ms_since_boot.argtypes = []
    fmw.ei_get_ms_since_boot.restype = c_uint32
    fmw.ei_set_connected.argtypes = [c_bool]
    fmw.ei_set_connected.restype = None


class App():
    def __init__(self):
        pg.init()
        pg.display.set_caption("Beeline Emulator")
        self.capture_screen()

        self.resolution = (400, 400)
        self.display_surf = pg.display.set_mode(self.resolution)
        
        self.background_surface = pg.Surface(self.resolution)
        self.background_surface.fill(pg.Color('#505050'))
        
        
        self.screen = pg.Surface(self.screen_dimensions)
        self.screen_rect = self.screen.get_rect()
        self.screen_rect.center = self.display_surf.get_rect().center

        self.frame = pg.image.load(emulator_path/"img"/"velo2_frame.png").convert_alpha()

        self.clock = pg.time.Clock()
        self.gui_fps = 25
        self.fw_fps = 25

        self.is_connected = False
        

    def __del__(self):
        pg.quit()

    def toggle_connected(self):
        self.is_connected = not self.is_connected
        fmw.ei_set_connected(self.is_connected)
    
    def capture_screen(self):
        """Grab the framebuffer from the firmware"""
        height = c_int()
        width = c_int()
        p_screen = fmw.ei_lcd_get_screen(byref(height), byref(width))

        screen = np.ctypeslib.as_array(
            p_screen, shape=(height.value, width.value, 3))
        screen.dtype = np.uint8
        self.screen_dimensions = (width.value, height.value)
        return screen

    def loop(self):
        for event in pg.event.get():
            if event.type == pg.QUIT:
                sys.exit()
            # get mouse clicks
            if event.type == pg.MOUSEBUTTONDOWN:
                if event.button == 1:
                    self.toggle_connected()

        # Some time is taken by waiting for the SPIM transfer. Assume the rest of the time is taken 
        # up by the rendering and other fw tasks. 
        ms_pre_fmw = fmw.ei_get_ms_since_boot()
        fmw.ei_loop()
        ms_passed = fmw.ei_get_ms_since_boot() - ms_pre_fmw
        ms_expected = round(1000/self.fw_fps)
        assert ms_passed <= ms_expected, f"Frame took {ms_passed}ms, expected {ms_expected}ms"

        # Sync the firmware time with the GUI time
        fmw.ei_increment_ms_since_boot(ms_expected - ms_passed)

        # Display the framebuffer
        framebuf = np.transpose(self.capture_screen(), axes=(1,0,2))
        self.display_surf.blit(self.background_surface, (0, 0))
        pg.surfarray.blit_array(self.screen, framebuf)
        self.display_surf.blit(self.screen, self.screen_rect)
        frame_rect = self.frame.get_rect()
        frame_rect.center = self.screen_rect.center
        self.display_surf.blit(self.frame, frame_rect)
        pg.display.flip()

        # Throttle the GUI to the desired framerate
        self.clock.tick(self.gui_fps)
        
        

if __name__ == "__main__":
    
    if REBUILD:
        subprocess.run(build_command, cwd=emulator_path, check=True)
    load_lib()
    app = App()
  
    try:
        while True:
            app.loop()
            
    except KeyboardInterrupt:
        del app


            
    