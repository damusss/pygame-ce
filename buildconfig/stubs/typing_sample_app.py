"""
Sample app run by mypy to ensure typing.py aliases work as expected
"""
from pygame import typing
import pygame

# validate SequenceLike
class MySequence:
    def __getitem__(self, index):
        if index > 20:
            raise IndexError()
        if index % 2 == 0:
            return 1
        return 0
    
    def __len__(self):
        return 20
    
def validator_SequenceLike(sequence: typing.SequenceLike) -> int:
    return 0

# must pass
validator_SequenceLike(MySequence())
validator_SequenceLike([0, 1, 2, 3])
validator_SequenceLike((0, 1, 2, 3))
validator_SequenceLike(pygame.Rect(-10, 10, 40, 40))
validator_SequenceLike(pygame.Vector2())
validator_SequenceLike("1234567890")

# validate PathLike
class MyPath:
    def __fspath__(self) -> str:
        return "file.py"
    
def validator_PathLike(path: typing.PathLike) -> int:
    return 0

# must pass
validator_PathLike("file.py")
validator_PathLike(b"file.py")
validator_PathLike(MyPath())

# validate Coordinate, IntCoordinate

def validator_Coordinate(coordinate: typing.Coordinate) -> int:
    return 0

def validator_IntCoordinate(coordinate: typing.IntCoordinate) -> int:
    return 0

# must pass
validator_Coordinate((1, 2))
validator_Coordinate([3, -4])
validator_Coordinate((5, -6.5))
validator_Coordinate((-6.7, 8.9))
validator_Coordinate(pygame.Vector2())

validator_IntCoordinate((3, 4))
validator_IntCoordinate([-4, -3])

# validate RGBATuple, ColorLike
def validator_RGBATuple(rgba: typing.RGBATuple) -> int:
    return 0

def validator_ColorLike(color: typing.ColorLike) -> int:
    return 0

# must pass
validator_RGBATuple((100, 200, 50, 20))
validator_ColorLike("green")
validator_ColorLike(1)
validator_ColorLike((255, 255, 255, 30))
validator_ColorLike(pygame.Color(100, 100, 100, 100))

# validate RectLike
class MyObject1:
    def __init__(self):
        self.rect = pygame.Rect(10, 10, 20, 20)
    
class MyObject2:
    def __init__(self):
        self.rect = lambda: pygame.Rect(5, 5, 10, 10)
    
def validator_RectLike(rect: typing.RectLike) -> int:
    return 0

# must pass
validator_RectLike((10, 10, 10, 10))
validator_RectLike(((5, 5), (30, 30)))
validator_RectLike(pygame.Rect(1, 2, 3, 4))
validator_RectLike(MyObject1())
validator_RectLike(MyObject2())