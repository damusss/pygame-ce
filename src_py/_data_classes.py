from dataclasses import dataclass
from typing import Optional


@dataclass(frozen=True)
class PowerState:
    battery_percent: Optional[int]
    battery_seconds: Optional[int]
    on_battery: bool
    no_battery: bool
    charging: bool
    charged: bool
    plugged_in: bool
    has_battery: bool


@dataclass(frozen=True)
class PixelFormat:
    format: int
    name: str
    bitsize: int
    bytesize: int
    masks: tuple[int, int, int, int]
    shifts: tuple[int, int, int, int]
    alpha: bool

    def __eq__(self, other: "PixelFormat"):
        return self.format == other.format

    def __ne__(self, other: "PixelFormat"):
        return self.format != other.format
