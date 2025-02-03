from typing import Optional, TypedDict

from pygame._data_classes import PowerState
import pygame

class _InstructionSets(TypedDict):
    ALTIVEC: bool
    MMX: bool
    SSE: bool
    SSE2: bool
    SSE3: bool
    SSE41: bool
    SSE42: bool
    AVX: bool
    AVX2: bool
    AVX512F: bool
    NEON: bool
    ARMSIMD: bool
    LSX: bool
    LASX: bool

# dict at runtime, TypedDict exists solely for the typechecking benefits
class _Locale(TypedDict):
    language: str
    country: Optional[str]

def get_cpu_instruction_sets() -> _InstructionSets: ...
def get_total_ram() -> int: ...
def get_pref_path(org: str, app: str) -> str: ...
def get_pref_locales() -> list[_Locale]: ...
def get_power_state() -> Optional[PowerState]: ...


class _TrayEntryManager(list[TrayButton]):
    """
    Custom list subclass with insert, append and pop operations overriden to
    align with the underlying SDL operations SDL_InsertEntryAt and SDL_RemoveEntry
    """

    ...

class TrayButton:
    def __init__(self, label: str, enabled: bool): ...
    @property
    def parent(self) -> Tray | TraySubMenu | None: ...
    @property
    def label(self) -> str: ...  # SDL_GetTrayEntryLabel
    @label.setter
    def label(self, value: str): ...  # SDL_SetTrayEntryLabel
    @property
    def enabled(self) -> bool: ...  # SDL_GetTrayEntryEnabled
    @enabled.setter
    def enabled(self, value: bool): ...  # SDL_SetTrayEntryEnabled
    def click(self) -> None: ...  # SDL_ClickTrayEntry

class TrayCheckbox(TrayButton):
    @property
    def checked(self) -> bool: ...  # SDL_GetTrayEntryChecked
    @checked.setter
    def checked(self, value: bool): ...  # SDL_SetTrayEntryChecked

class TraySubMenu(TrayButton):
    @property
    def items(self) -> _TrayEntryManager: ...  # read only, but mutable

class Tray:
    # no userdata for SDL_Tray, gotta hold references to objects ourselves
    def __init__(
        self, icon: pygame.Surface | None = None, tooltip: str = ""
    ): ...  # SDL_CreateTray
    @property
    def icon(self) -> pygame.Surface | None: ...  # cached ref like window surface
    @icon.setter
    def icon(self, value: pygame.Surface | None): ...  # SDL_SetTrayIcon
    @property
    def tooltip(self) -> str: ...  # cached
    @tooltip.setter
    def tooltip(self, value: str): ...  # SDL_SetTrayTooltip
    @property
    def items(self) -> _TrayEntryManager: ...  # read only, but mutable

    # SDL_DestroyTray on dealloc
