from typing import Optional, Protocol, Union, final

from pygame.color import Color
from pygame.rect import Rect
from pygame.surface import Surface
from pygame.typing import ColorLike, IntPoint, Point, RectLike, SequenceLike
from pygame.window import Window
from typing_extensions import deprecated  # added in 3.13

class _DrawableClass(Protocol):
    # Object that has the draw method that accepts area and dest arguments
    def draw(
        self, area: Optional[RectLike] = None, dest: Optional[RectLike] = None
    ): ...

@final
class Renderer:
    def __init__(
        self,
        window: Window,
        index: int = -1,
        accelerated: int = -1,
        vsync: bool = False,
        target_texture: bool = False,
    ) -> None: ...
    def blit(
        self,
        source: Union["Texture", "Image", _DrawableClass],
        dest: Optional[RectLike] = None,
        area: Optional[RectLike] = None,
        special_flags: int = 0,
    ) -> Rect: ...
    def clear(self) -> None: ...
    def draw_line(self, p1: Point, p2: Point) -> None: ...
    def draw_lines(self, points: SequenceLike[Point], *, closed: bool = False) -> None:
        """Draw multiple contiguous straight line segments

        Draw a sequence of contiguous line segments from a sequence of at least 2 points
        on the renderer using the :meth:`Renderer.draw_color`. Effectively draws
        a polygon if the closed argument is True.

        :param points: a sequence of 2 or more (x, y) coordinates
        :type points: :data:`pygame.typing.SequenceLike` of :data:`pygame.typing.Point`
        :param bool closed: When True, add an extra segment from the last point
           to the first one, closing the shape.

        :raises ValueError: if less than two points are provided.

        .. versionadded:: 2.5.4
        """

    def draw_point(self, point: Point) -> None: ...
    def draw_quad(self, p1: Point, p2: Point, p3: Point, p4: Point) -> None: ...
    def draw_rect(self, rect: RectLike) -> None: ...
    def draw_triangle(self, p1: Point, p2: Point, p3: Point) -> None: ...
    def fill_quad(self, p1: Point, p2: Point, p3: Point, p4: Point) -> None: ...
    def fill_convex_poly(self, points: SequenceLike[Point]) -> None:
        """Draw a filled convex polygon

        Draw a filled polygon from a sequence of at least 3 points on the renderer using the
        :meth:`Renderer.draw_color`. The points are expected to form a convex shape,
        the only kind supported by the triangle fan algorithm used.

        :param points: a sequence of 3 or more (x, y) coordinates
        :type points: :data:`pygame.typing.SequenceLike` of :data:`pygame.typing.Point`

        :raises ValueError: if less than three points are provided.

        .. versionadded:: 2.5.4
        """

    def fill_rect(self, rect: RectLike) -> None: ...
    def fill_triangle(self, p1: Point, p2: Point, p3: Point) -> None: ...
    def get_viewport(self) -> Rect: ...
    def present(self) -> None: ...
    def set_viewport(self, area: Optional[RectLike]) -> None: ...
    def to_surface(
        self, surface: Optional[Surface] = None, area: Optional[RectLike] = None
    ) -> Surface: ...
    @property
    def draw_blend_mode(self) -> int: ...
    @draw_blend_mode.setter
    def draw_blend_mode(self, value: int) -> None: ...
    @property
    def draw_color(self) -> Color: ...
    @draw_color.setter
    def draw_color(self, value: ColorLike) -> None: ...
    @property
    def logical_size(self) -> tuple[int, int]: ...
    @logical_size.setter
    def logical_size(self, value: IntPoint) -> None: ...
    @property
    def scale(self) -> tuple[float, float]: ...
    @scale.setter
    def scale(self, value: Point) -> None: ...
    @property
    def target(self) -> "Texture": ...
    @target.setter
    def target(self, value: "Texture") -> None: ...
    @classmethod
    def compose_custom_blend_mode(
        cls, color_mode: SequenceLike[int], alpha_mode: SequenceLike[int]
    ) -> int: ...
    @classmethod
    def from_window(cls, window: Window) -> Renderer: ...

@final
class Texture:
    pass

@final
class Image:
    pass
