# requires modules:
#
# pygame.color
# pygame.rect
# pygame.surface
# pygame.font
# pygame._render (to be publicized)
# pygame.gpu (to be implemented)

# NOTE: SubString definition and TextEngine subclasses could
# take great advantage from a python layer of this module
# sitting on top of a C layer that handles the TTF function calls.

# The below API is a pretty direct port of SDL3_ttf, with minimal
# pythonization improvements. Check https://wiki.libsdl.org/SDL3_ttf/CategoryAPI
# for more. (TTF_font features are excluded for this text-focused module,
# but a new and better port of TTF_font could be added to this module in the future).

from pygame import Font, Surface, Color, Rect
from pygame.typing import Point
from pygame._render import Renderer

# constants.c
SUBSTRING_TEXT_START = 0x00000100
SUBSTRING_LINE_START = 0x00000200
SUBSTRING_LINE_END = 0x00000400
SUBSTRING_TEXT_END = 0x00000800

# if this module is implemented in python, consider @dataclasses.dataclass(slots=True, frozen=True)
class SubString:
    # the SUBSTRING_* constants above. Extracted similarly to keymods,
    # f.i. substring.flags & SUBSTRING_TEXT_START
    flags: int
    # not a direct port: extract it from the TTF_SUBSTRING_DIRECTION_MASK flag
    # I think an extra attribute is better than less straight forward bitwise extraction for python users.
    direction: int
    offset: int
    length: int
    line_index: int
    cluster_index: int
    rect: Rect

class Text:
    # NOTE: draw() is missing because Text is technically backend agnostic.
    # Only Surface and Renderer backends can draw directly, GPU/GL backends
    # can only retrieve the draw data. Additionally, every draw function
    # requires different parameters, meaning a unified function is not
    # possible.

    # no engine is fine for layout calculations but will fail to be rendered.
    def __init__(self, engine: TextEngine | None, font: Font, text: str) -> None: ...
    #
    # getter makes sense as a property (and is a low-stakes operation, as
    # it's stored directly int the TTF_Text struct), setter should be a method
    # since there's multiple ways to change the text
    @property
    def text(self) -> str: ...
    #
    @property
    def engine(self) -> TextEngine | None: ...
    @engine.setter
    def engine(self, value: TextEngine | None) -> None: ...
    #
    @property
    def font(self) -> Font: ...
    @font.setter
    def font(self, value: Font) -> None: ...
    #
    @property
    def position(self) -> tuple[int, int]: ...
    @position.setter
    def position(self, value: Point) -> None: ...
    #
    @property
    def color(self) -> Color: ...
    @color.setter
    def color(self, value: Color) -> None: ...
    #
    # pygame.DIRECTION_* already exist
    @property
    def direction(self) -> int: ...
    @direction.setter
    def direction(self, value: int) -> None: ...
    #
    @property
    def wrap_width(self) -> int: ...
    @wrap_width.setter
    def wrap_width(self, value: int) -> None: ...
    #
    @property
    def wrap_whitespace_visible(self) -> bool: ...
    @wrap_whitespace_visible.setter
    def wrap_whitespace_visible(self, value: bool) -> None: ...
    #
    # hopefully pygame.font implements TTF_StringToTag and
    # TTF_TagToString related to packed script tags.
    @property
    def script(self) -> int: ...
    @script.setter
    def script(self, value: int) -> None: ...
    #
    @property
    def size(self) -> tuple[int, int]: ...
    @property
    def line_count(self) -> int: ...
    #
    def set_text(self, text: str) -> None: ...
    def append_text(self, text: str) -> None: ...
    def insert_text(self, offset: int, text: str) -> None: ...
    def delete_text(self, offset: int, length: int) -> None: ...
    #
    # renamed the TTF_GetSubstring(|ForPoint|ForLine|sForRange) with my own creativity.
    # can be changed to match TTF signatures if desired.
    def get_substring(self, offset: int) -> SubString: ...
    def get_substring_at(self, position: Point) -> SubString: ...
    def get_line_substring(self, line: int) -> SubString: ...
    def get_substrings(self, offset: int, length: int) -> list[SubString]: ...
    #
    # called automatically when text information is requested or text
    # is drawn. Added manual option because TTF allows it so why shouldn't we.
    def update(self) -> None: ...
    # called automatically when garbage collected. Added manual option
    # mimicking Window.destroy()
    def destroy(self) -> None: ...

class TextEngine:
    # same as Text(self, font, text). Not a direct port but is more pythonic.
    def create_text(self, font: Font, text: str) -> Text: ...

    # called automatically when garbage collected. Added manual option
    # mimicking Window.destroy()
    # shared name and behavior but subclasses need to override
    # this to call the proper SDL_Destroy[]TextEngine function
    def destroy(self) -> None: ...

class SurfaceTextEngine(TextEngine):
    def draw_text(self, text: Text, position: Point, surface: Surface) -> None: ...

class RendererTextEngine(TextEngine):
    def __init__(self, renderer: Renderer) -> None: ...
    def draw_text(self, text: Text, position: Point) -> None: ...

# NOTE: GPU and GL backends can be added after the first iteration of
# this module is implemented, when pygame.gpu is ready.

# consider unifying GPU/GL winding constants. Not sure why they have
# different enums in TTF if the enum values are the same with the same values!

# constants.c
GPU_WINDING_INVALID = -1
GPU_WINDING_CLOCKWISE = 0
GPU_WINDING_COUNTER_CLOCKWISE = 1

class GPUTextEngine(TextEngine):
    # would be what the SDL_GPUDevice port is in pygame.gpu
    def __init__(self, device: ...) -> None: ...

    # useful return value representing TTF_GPUAtlasDrawSequence
    def get_text_draw_data(self, text: Text) -> ...: ...
    #
    # the (GPU_?)WINDING_* constans above
    @property
    def winding(self) -> int: ...

# constants.c
GL_WINDING_INVALID = -1
GL_WINDING_CLOCKWISE = 0
GL_WINDING_COUNTERCLOCKWISE = 1

class GLTextEngine(TextEngine):
    # useful return value representing TTF_GLAtlasDrawSequence
    def get_text_draw_data(self, text: Text) -> ...: ...
    #
    # the (GL_?)WINDING_* constans above
    @property
    def winding(self) -> int: ...
