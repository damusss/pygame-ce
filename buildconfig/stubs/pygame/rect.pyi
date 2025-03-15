import sys
from collections.abc import Callable, Collection, Iterator
from typing import (
    ClassVar,
    Literal,
    SupportsIndex,
    TypeVar,
    Union,
    overload,
    Optional,
)
from typing_extensions import deprecated  # added in 3.13

from pygame.typing import Point, RectLike, SequenceLike

if sys.version_info >= (3, 11):
    from typing import Self
else:
    from typing_extensions import Self

# 'ellipsis' existed in typeshed pre 3.10, now we use EllipsisType which is
# the modern standard library equivalent.
if sys.version_info >= (3, 10):
    from types import EllipsisType
else:
    EllipsisType = ellipsis

_N = TypeVar("_N", int, float)
_K = TypeVar("_K")
_V = TypeVar("_V")
_T = TypeVar("_T")

_RectTypeCompatible_co = TypeVar("_RectTypeCompatible_co", bound=RectLike, covariant=True)

class _GenericRect(Collection[_N]):
    @property
    def x(self) -> _N: ...
    @x.setter
    def x(self, value: float) -> None: ...
    @property
    def y(self) -> _N: ...
    @y.setter
    def y(self, value: float) -> None: ...
    @property
    def top(self) -> _N: ...
    @top.setter
    def top(self, value: float) -> None: ...
    @property
    def left(self) -> _N: ...
    @left.setter
    def left(self, value: float) -> None: ...
    @property
    def bottom(self) -> _N: ...
    @bottom.setter
    def bottom(self, value: float) -> None: ...
    @property
    def right(self) -> _N: ...
    @right.setter
    def right(self, value: float) -> None: ...
    @property
    def topleft(self) -> tuple[_N, _N]: ...
    @topleft.setter
    def topleft(self, value: Point) -> None: ...
    @property
    def bottomleft(self) -> tuple[_N, _N]: ...
    @bottomleft.setter
    def bottomleft(self, value: Point) -> None: ...
    @property
    def topright(self) -> tuple[_N, _N]: ...
    @topright.setter
    def topright(self, value: Point) -> None: ...
    @property
    def bottomright(self) -> tuple[_N, _N]: ...
    @bottomright.setter
    def bottomright(self, value: Point) -> None: ...
    @property
    def midtop(self) -> tuple[_N, _N]: ...
    @midtop.setter
    def midtop(self, value: Point) -> None: ...
    @property
    def midleft(self) -> tuple[_N, _N]: ...
    @midleft.setter
    def midleft(self, value: Point) -> None: ...
    @property
    def midbottom(self) -> tuple[_N, _N]: ...
    @midbottom.setter
    def midbottom(self, value: Point) -> None: ...
    @property
    def midright(self) -> tuple[_N, _N]: ...
    @midright.setter
    def midright(self, value: Point) -> None: ...
    @property
    def center(self) -> tuple[_N, _N]: ...
    @center.setter
    def center(self, value: Point) -> None: ...
    @property
    def centerx(self) -> _N: ...
    @centerx.setter
    def centerx(self, value: float) -> None: ...
    @property
    def centery(self) -> _N: ...
    @centery.setter
    def centery(self, value: float) -> None: ...
    @property
    def size(self) -> tuple[_N, _N]: ...
    @size.setter
    def size(self, value: Point) -> None: ...
    @property
    def width(self) -> _N: ...
    @width.setter
    def width(self, value: float) -> None: ...
    @property
    def height(self) -> _N: ...
    @height.setter
    def height(self, value: float) -> None: ...
    @property
    def w(self) -> _N: ...
    @w.setter
    def w(self, value: float) -> None: ...
    @property
    def h(self) -> _N: ...
    @h.setter
    def h(self, value: float) -> None: ...
    __hash__: ClassVar[None]  # type: ignore[assignment]
    @property
    def __safe_for_unpickling__(self) -> Literal[True]: ...
    @overload
    def __init__(
        self, left: float, top: float, width: float, height: float
    ) -> None: ...
    @overload
    def __init__(self, left_top: Point, width_height: Point) -> None: ...
    @overload
    def __init__(self, single_arg: RectLike) -> None: ...
    @overload
    def __init__(self) -> None: ...
    def __len__(self) -> Literal[4]: ...
    def __iter__(self) -> Iterator[_N]: ...
    @overload
    def __getitem__(self, i: SupportsIndex) -> _N: ...
    @overload
    def __getitem__(self, s: Union[slice, EllipsisType]) -> list[_N]: ...
    @overload
    def __setitem__(self, key: int, value: float) -> None: ...
    @overload
    def __setitem__(
        self, key: Union[slice, EllipsisType], value: Union[float, RectLike]
    ) -> None: ...
    def __copy__(self) -> Self: ...
    def copy(self) -> Self: ...
    @overload
    def move(self, x: float, y: float, /) -> Self: ...
    @overload
    def move(self, move_by: Point, /) -> Self: ...
    @overload
    def move_ip(self, x: float, y: float, /) -> None: ...
    @overload
    def move_ip(self, move_by: Point, /) -> None: ...
    def move_to(self, **kwargs: Union[float, Point]) -> Self: ...
    @overload
    def inflate(self, x: float, y: float, /) -> Self: ...
    @overload
    def inflate(self, inflate_by: Point, /) -> Self: ...
    @overload
    def inflate_ip(self, x: float, y: float, /) -> None: ...
    @overload
    def inflate_ip(self, inflate_by: Point, /) -> None: ...
    @overload
    def scale_by(self, x: float, y: float = ...) -> Self: ...
    @overload
    def scale_by(self, scale_by: Point) -> Self: ...
    @overload
    def scale_by_ip(self, x: float, y: float = ...) -> None: ...
    @overload
    def scale_by_ip(self, scale_by: Point) -> None: ...
    @overload
    def update(self, left: float, top: float, width: float, height: float, /) -> None: ...
    @overload
    def update(self, left_top: Point, width_height: Point, /) -> None: ...
    @overload
    def update(self, single_arg: RectLike, /) -> None: ...
    @overload
    def clamp(self, rect: RectLike, /) -> Self: ...
    @overload
    def clamp(self, left_top: Point, width_height: Point, /) -> Self: ...
    @overload
    def clamp(self, left: float, top: float, width: float, height: float, /) -> Self: ...
    @overload
    def clamp_ip(self, rect: RectLike, /) -> None: ...
    @overload
    def clamp_ip(self, left_top: Point, width_height: Point, /) -> None: ...
    @overload
    def clamp_ip(
        self, left: float, top: float, width: float, height: float, /
    ) -> None: ...
    @overload
    def clip(self, rect: RectLike, /) -> Self: ...
    @overload
    def clip(self, left_top: Point, width_height: Point, /) -> Self: ...
    @overload
    def clip(self, left: float, top: float, width: float, height: float, /) -> Self: ...
    @overload
    def clipline(
        self, x1: float, x2: float, x3: float, x4: float, /
    ) -> Union[tuple[tuple[_N, _N], tuple[_N, _N]], tuple[()]]: ...
    @overload
    def clipline(
        self, first_point: Point, second_point: Point, /
    ) -> Union[tuple[tuple[_N, _N], tuple[_N, _N]], tuple[()]]: ...
    @overload
    def clipline(
        self, rect_arg: RectLike, /
    ) -> Union[tuple[tuple[_N, _N], tuple[_N, _N]], tuple[()]]: ...
    @overload
    def union(self, rect: RectLike, /) -> Self: ...
    @overload
    def union(self, left_top: Point, width_height: Point, /) -> Self: ...
    @overload
    def union(self, left: float, top: float, width: float, height: float, /) -> Self: ...
    @overload
    def union_ip(self, rect: RectLike, /) -> None: ...
    @overload
    def union_ip(self, left_top: Point, width_height: Point, /) -> None: ...
    @overload
    def union_ip(
        self, left: float, top: float, width: float, height: float, /
    ) -> None: ...
    def unionall(self, rect: SequenceLike[_RectTypeCompatible_co], /) -> Self: ...
    def unionall_ip(self, rect_SequenceLike: SequenceLike[_RectTypeCompatible_co], /) -> None: ...
    @overload
    def fit(self, rect: RectLike, /) -> Self: ...
    @overload
    def fit(self, left_top: Point, width_height: Point, /) -> Self: ...
    @overload
    def fit(self, left: float, top: float, width: float, height: float, /) -> Self: ...
    def normalize(self) -> None: ...
    def __contains__(self, rect: Union[RectLike, _N], /) -> bool: ...  # type: ignore[override]
    @overload
    def contains(self, rect: RectLike, /) -> bool: ...
    @overload
    def contains(self, left_top: Point, width_height: Point, /) -> bool: ...
    @overload
    def contains(
        self, left: float, top: float, width: float, height: float, /
    ) -> bool: ...
    @overload
    def collidepoint(self, x: float, y: float, /) -> bool: ...
    @overload
    def collidepoint(self, x_y: Point, /) -> bool: ...
    @overload
    def colliderect(self, rect: RectLike, /) -> bool: ...
    @overload
    def colliderect(self, left_top: Point, width_height: Point, /) -> bool: ...
    @overload
    def colliderect(
        self, left: float, top: float, width: float, height: float, /
    ) -> bool: ...
    def collidelist(self, rect_list: SequenceLike[_RectTypeCompatible_co], /) -> int: ...
    def collidelistall(self, rect_list: SequenceLike[_RectTypeCompatible_co], /) -> list[int]: ...
    def collideobjectsall(
        self, objects: SequenceLike[_T], key: Optional[Callable[[_T], RectLike]] = None
    ) -> list[_T]: ...
    def collideobjects(
        self, objects: SequenceLike[_T], key: Optional[Callable[[_T], RectLike]] = None
    ) -> Optional[_T]: ...
    @overload
    def collidedict(
        self, rect_dict: dict[_RectTypeCompatible_co, _V], values: Literal[False] = False
    ) -> Optional[tuple[_RectTypeCompatible_co, _V]]: ...
    @overload
    def collidedict(
        self, rect_dict: dict[_K, _RectTypeCompatible_co], values: Literal[True]
    ) -> Optional[tuple[_K, _RectTypeCompatible_co]]: ...
    @overload
    def collidedictall(
        self, rect_dict: dict[_RectTypeCompatible_co, _V], values: Literal[False] = False
    ) -> list[tuple[_RectTypeCompatible_co, _V]]: ...
    @overload
    def collidedictall(
        self, rect_dict: dict[_K, _RectTypeCompatible_co], values: Literal[True]
    ) -> list[tuple[_K, _RectTypeCompatible_co]]: ...

# Rect confirms to the Collection ABC, since it also confirms to
# Sized, Iterable and Container ABCs
class Rect(_GenericRect[int]):
    ...

class FRect(_GenericRect[float]):
    ...

@deprecated("Use `Rect` instead (RectType is an old alias)")
class RectType(Rect): ...

@deprecated("Use `FRect` instead (FRectType is an old alias)")
class FRectType(FRect): ...
