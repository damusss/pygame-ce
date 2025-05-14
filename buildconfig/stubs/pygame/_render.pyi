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
class GeometryMesh:
    """Pygame object storing vertices and indices of a 2D mesh for fast rendering

    Meant to be used with :meth:`Renderer.render_geometry`. Parses the input
    and stores it as C arrays to render a 2D mesh without wasting performance.
    The texture, if one is used, can be provided to the Renderer at the time of
    rendering. When updating the data, it is faster to call :meth:`GeometryMesh.update`
    if the length of data is the same.
    Vertices can represent any kind of 2D mesh. Usage of this object requires knowledge
    related to GPU rendering and is only meant as a fast wrapper of SDL functionality.

    :param vertices: A sequence of vertices. Each vertex should be a 3-element
                     sequence. The first item should be a Point-like representing the vertex
                     position in Renderer coordinates. The second item should be
                     a valid pygame color value. The third item should be a Point-like
                     representing the normalized texture coordinates the vertex will
                     use.
    :param indices: An optional sequence of integers representing indices of the vertices.
                    Commonly used to reduce the amount of vertices when they overlap. If
                    not specified, all vertices will be drawn in order. The indices must be
                    in bounds of the vertices and must be a valid amount to form complete
                    triplets of vertices, otherwise SDL will raise errors when rendering.

    .. note:: Creating an instance every frame defies the purpose of this object. Store it
              and update it only when necessary.

    .. versionadded:: 2.5.4
    """

    def __init__(
        self,
        vertices: SequenceLike[tuple[Point, ColorLike, Point]],
        indices: SequenceLike[int] | None = None,
    ) -> None: ...
    def update(
        self,
        vertices: SequenceLike[tuple[Point, ColorLike, Point]],
        indices: SequenceLike[int] | None = None,
    ) -> None:
        """Update the stored vertices and indices with new data

        If the amount of vertices and indices are the same, the previous array is used,
        making it faster than re-instancing the object. Otherwise, behaves the same.

        Check the constructor paramters of :class:`GeometryMesh` to see what data
        to provide.

        .. versionadded:: 2.5.4
        """

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
    def draw_point(self, point: Point) -> None: ...
    def draw_quad(self, p1: Point, p2: Point, p3: Point, p4: Point) -> None: ...
    def draw_rect(self, rect: RectLike) -> None: ...
    def draw_triangle(self, p1: Point, p2: Point, p3: Point) -> None: ...
    def fill_quad(self, p1: Point, p2: Point, p3: Point, p4: Point) -> None: ...
    def fill_rect(self, rect: RectLike) -> None: ...
    def fill_triangle(self, p1: Point, p2: Point, p3: Point) -> None: ...
    def get_viewport(self) -> Rect: ...
    def present(self) -> None: ...
    def set_viewport(self, area: Optional[RectLike]) -> None: ...
    def render_geometry(
        self, mesh: GeometryMesh, texture: "Texture|None" = None
    ) -> None:
        """Render a static 2D mesh

        This method provides a fully flexible wrapper around ``SDL_RenderGeometry``
        to allow the rendering of any 2D mesh, not limited to primitive shapes.
        Only use this functionality if you are familiar with GPU rendering, as it
        is not intuitive compared to standard pygame rendering.

        :param mesh: The mesh to render. It must be a :class:`GeometryMesh` instance
                     to avoid re-parsing the input every frame, guaranteeing maximum
                     performance.
        :type mesh: :class:`GeometryMesh`
        :param texture: An optional :class:`Texture` instance that the mesh will be
                        rendered with. The texture coordinates of the vertices should
                        be normalized to represent portions of the texture.
        :type texture: :class:`Texture`|None

        .. versionadded:: 2.5.4
        """

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
