from typing import Dict, List, Optional, Tuple, Union, overload, Literal
from typing_extensions import deprecated # added in 3.13

from pygame.constants import FULLSCREEN
from pygame.surface import Surface

from pygame._sdl2 import Window

from pygame.typing import (
    ColorLike,
    CoordinateLike,
    IntCoordinateLike,
    RectLike,
    RGBALike,
    SequenceLike,
)

class _VidInfo:
    hw: int
    wm: int
    video_mem: int
    bitsize: int
    bytesize: int
    masks: RGBALike
    shifts: RGBALike
    losses: RGBALike
    blit_hw: int
    blit_hw_CC: int
    blit_hw_A: int
    blit_sw: int
    blit_sw_CC: int
    blit_sw_A: int
    current_h: int
    current_w: int
    pixel_format: str

def init() -> None: ...
def quit() -> None: ...
def get_init() -> bool: ...
def set_mode(
    size: CoordinateLike = (0, 0),
    flags: int = 0,
    depth: int = 0,
    display: int = 0,
    vsync: int = 0,
) -> Surface: ...
def get_surface() -> Surface: ...
def flip() -> None: ...
@overload
def update(
    rectangle: Optional[Union[RectLike, SequenceLike[Optional[RectLike]]]] = None, /
) -> None: ...
@overload
def update(x: int, y: int, w: int, h: int, /) -> None: ...
@overload
def update(xy: CoordinateLike, wh: CoordinateLike, /) -> None: ...
def get_driver() -> str: ...
def Info() -> _VidInfo: ...
def get_wm_info() -> Dict[str, int]: ...
def list_modes(
    depth: int = 0,
    flags: int = FULLSCREEN,
    display: int = 0,
) -> List[Tuple[int, int]]: ...
def mode_ok(
    size: IntCoordinateLike,
    flags: int = 0,
    depth: int = 0,
    display: int = 0,
) -> int: ...
def gl_get_attribute(flag: int, /) -> int: ...
def gl_set_attribute(flag: int, value: int, /) -> None: ...
def get_active() -> bool: ...
def iconify() -> bool: ...
def toggle_fullscreen() -> int: ...
@deprecated("since 2.1.4. Removed in SDL3")
def set_gamma(red: float, green: float = ..., blue: float = ..., /) -> int: ...
@deprecated("since 2.1.4. Removed in SDL3")
def set_gamma_ramp(
    red: SequenceLike[int], green: SequenceLike[int], blue: SequenceLike[int], /
) -> int: ...
def set_icon(surface: Surface, /) -> None: ...
def set_caption(title: str, icontitle: Optional[str] = None, /) -> None: ...
def get_caption() -> Tuple[str, str]: ...
def set_palette(palette: SequenceLike[ColorLike], /) -> None: ...
def get_num_displays() -> int: ...
def get_window_size() -> Tuple[int, int]: ...
def get_window_position() -> Tuple[int, int]:...
def set_window_position(position: CoordinateLike) -> None:...
def get_allow_screensaver() -> bool: ...
def set_allow_screensaver(value: bool = True) -> None: ...
def get_desktop_sizes() -> List[Tuple[int, int]]: ...
def is_fullscreen() -> bool: ...
def is_vsync() -> bool: ...
def get_current_refresh_rate() -> int: ...
def get_desktop_refresh_rates() -> List[int]: ...
def message_box(
    title: str,
    message: Optional[str] = None,
    message_type: Literal["info", "warn", "error"] = "info",
    parent_window: Optional[Window] = None,
    buttons: SequenceLike[str] = ("OK",),
    return_button: int = 0,
    escape_button: Optional[int] = None,
) -> int: ...
