#!/usr/bin/env python
"""pygame.examples.font_viewer
Scroll through your system fonts from a list of surfaces or one huge buffer.

This example exhibits:
* iterate over available fonts using font.get_fonts and font.SysFont()
* click and drag using mouse input
* scrolling with the scroll wheel
* save a surface to disk
* work with a very large surface
* simple mouse and keyboard scroll speed acceleration

By default this example uses the fonts returned by pygame.font.get_fonts()
and opens them using pygame.font.SysFont().
Alternatively, you may pass a path to the command line. The TTF files found
in that directory will be used instead.

Mouse Controls:
* Use the mouse wheel or click and drag to scroll

Keyboard Controls:
* Press up or down to scroll
* Press escape to exit
"""

import os
import sys

import pygame

use_big_surface = False  # draw into large buffer and save png file


class FontViewer:
    """
    This example is encapsulated by the fontviewer class
    It initializes the pygame window, handles input, and draws itself
    to the screen.
    """

    KEY_SCROLL_SPEED = 10
    MOUSE_SCROLL_SPEED = 50

    def __init__(self):
        pygame.init()

        # create a window that uses 80 percent of the screen
        info = pygame.display.Info()
        self.screen_size = (int(info.current_w * 0.8), int(info.current_h * 0.8))
        pygame.display.set_mode(self.screen_size)
        self.font_size = self.screen_size[1] // 16

        self.clock = pygame.Clock()
        self.y_offset = 0
        self.grabbed = False
        self.render_fonts("&N abcDEF789")

        if use_big_surface or "big" in sys.argv:
            self.render_surface()
            self.display_surface()
            self.save_png()
        else:
            self.display_fonts()

    def get_font_list(self):
        """
        Generate a font list using font.get_fonts() for system fonts or
        from a path from the command line.
        """
        path = ""
        if len(sys.argv) > 1 and os.path.exists(sys.argv[1]):
            path = os.path.join(sys.argv[1], "")
        fonts = []
        if os.path.exists(path):
            fonts = [font for font in os.listdir(path) if font.endswith(".ttf")]
        return fonts or pygame.font.get_fonts(), path

    def render_fonts(self, text="A display of font &N"):
        """
        Build a list that includes a surface and the running total of their
        height for each font in the font list. Store the largest width and
        other variables for later use.
        """
        font_size = self.font_size
        color = (255, 255, 255)
        instruction_color = (255, 255, 0)
        self.back_color = (0, 0, 0)

        fonts, path = self.get_font_list()
        font_surfaces = []
        total_height = 0
        max_width = 0

        load_font = pygame.Font if path else pygame.font.SysFont

        # display instructions at the top of the display
        font = pygame.font.SysFont(pygame.font.get_default_font(), font_size)
        font.align = pygame.FONT_CENTER
        instructions = (
            "Use the scroll wheel or click and drag to scroll up "
            "and down.  Fonts that don't use the Latin Alphabet "
            "might render incorrectly.  Here are your "
            f"{len(fonts)} fonts"
        )
        surf = font.render(
            instructions,
            True,
            instruction_color,
            self.back_color,
            self.screen_size[0] - 20,
        )
        font_surfaces.append((surf, total_height))
        total_height += surf.get_height()
        max_width = max(max_width, surf.get_width())

        # render all the fonts and store them with the total height
        for name in sorted(fonts):
            try:
                font = load_font(path + name, font_size)
            except OSError:
                continue
            line = text.replace("&N", name)
            try:
                surf = font.render(
                    line, True, color, self.back_color, self.screen_size[0] - 20
                )
            except pygame.error as e:
                print(e)
                break

            max_width = max(max_width, surf.get_width())
            font_surfaces.append((surf, total_height))
            total_height += surf.get_height()

        # store variables for later usage
        self.total_height = total_height
        self.max_width = max_width
        self.font_surfaces = font_surfaces
        self.max_y = total_height - pygame.display.get_surface().get_height()

    def display_fonts(self):
        """
        Display the visible fonts based on the y_offset value(updated in
        handle_events) and the height of the pygame window.
        """
        pygame.display.set_caption("Font Viewer")
        display = pygame.display.get_surface()
        clock = pygame.Clock()
        center = display.get_width() // 2

        while True:
            # draw visible surfaces
            display.fill(self.back_color)
            for surface, top in self.font_surfaces:
                bottom = top + surface.get_height()
                if (
                    bottom >= self.y_offset
                    and top <= self.y_offset + display.get_height()
                ):
                    x = center - surface.get_width() / 2
                    display.blit(surface, (x, top - self.y_offset))
            # get input and update the screen
            if not self.handle_events():
                break
            pygame.display.flip()
            clock.tick(30)

    def render_surface(self):
        """
        Note: this method uses twice the memory and is only called if
        big_surface is set to true or big is added to the command line.

        Optionally generates one large buffer to draw all the font surfaces
        into. This is necessary to save the display to a png file and may
        be useful for testing large surfaces.
        """

        large_surface = pygame.surface.Surface(
            (self.max_width, self.total_height)
        ).convert()
        large_surface.fill(self.back_color)
        print("scrolling surface created")

        # display the surface size and memory usage
        byte_size = large_surface.get_bytesize()
        total_size = byte_size * (self.max_width * self.total_height)
        print(
            "Surface Size = {}x{} @ {}bpp: {:,.3f}mb".format(
                self.max_width, self.total_height, byte_size, total_size / 1000000.0
            )
        )

        y = 0
        center = int(self.max_width / 2)
        for surface, top in self.font_surfaces:
            w = surface.get_width()
            x = center - int(w / 2)
            large_surface.blit(surface, (x, y))
            y += surface.get_height()
        self.max_y = (
            large_surface.get_height() - pygame.display.get_surface().get_height()
        )
        self.surface = large_surface

    def display_surface(self, time=10):
        """
        Display the large surface created by the render_surface method. Scrolls
        based on the y_offset value(set in handle_events) and the height of the
        pygame window.
        """
        screen = pygame.display.get_surface()

        # Create a Rect equal to size of screen. Then we can just change its
        # top attribute to draw the desired part of the rendered font surface
        # to the display surface
        rect = pygame.rect.Rect(
            0,
            0,
            self.surface.get_width(),
            min(self.surface.get_height(), screen.get_height()),
        )

        x = int((screen.get_width() - self.surface.get_width()) / 2)
        going = True
        while going:
            if not self.handle_events():
                going = False
            screen.fill(self.back_color)
            rect.top = self.y_offset
            screen.blit(self.surface, (x, 0), rect)
            pygame.display.flip()
            self.clock.tick(20)

    def save_png(self, name="font_viewer.png"):
        pygame.image.save(self.surface, name)
        file_size = os.path.getsize(name) // 1024
        print(f"font surface saved to {name}\nsize: {file_size:,}Kb")

    def handle_events(self):
        """
        This method handles user input. It returns False when it receives
        a pygame.QUIT event or the user presses escape. The y_offset is
        changed based on mouse and keyboard input. display_fonts() and
        display_surface() use the y_offset to scroll display.
        """
        events = pygame.event.get()
        for e in events:
            if e.type == pygame.QUIT:
                return False
            elif e.type == pygame.KEYDOWN:
                if e.key == pygame.K_ESCAPE:
                    return False
            elif e.type == pygame.MOUSEWHEEL:
                self.y_offset += e.y * self.MOUSE_SCROLL_SPEED * -1
            elif e.type == pygame.MOUSEBUTTONDOWN:
                # enter dragging mode on mouse down
                self.grabbed = True
                pygame.event.set_grab(True)
            elif e.type == pygame.MOUSEBUTTONUP:
                # exit drag mode on mouse up
                self.grabbed = False
                pygame.event.set_grab(False)

        # allow simple accelerated scrolling with the keyboard
        keys = pygame.key.get_pressed()
        if keys[pygame.K_UP]:
            self.key_held += 1
            self.y_offset -= int(self.KEY_SCROLL_SPEED * (self.key_held // 10))
        elif keys[pygame.K_DOWN]:
            self.key_held += 1
            self.y_offset += int(self.KEY_SCROLL_SPEED * (self.key_held // 10))
        else:
            self.key_held = 20

        # set the y_offset for scrolling and keep it between 0 and max_y
        y = pygame.mouse.get_rel()[1]
        if y and self.grabbed:
            self.y_offset -= y

        self.y_offset = min((max(self.y_offset, 0), self.max_y))
        return True


if __name__ == "__main__":
    viewer = FontViewer()
    pygame.quit()
