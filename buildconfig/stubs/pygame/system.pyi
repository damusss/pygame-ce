from typing import Optional, TypedDict

from pygame._data_classes import PowerState
from pygame.window import Window
from pygame.typing import SequenceLike

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
def show_file_dialog(
    allow_many: bool = False,
    default_location: str|None =None,
    title: str|None = None,
    parent_window: Window|None=None,
    filters:SequenceLike[SequenceLike[str]]|None=None,
) -> int: ...
def show_folder_dialog(
    allow_many: bool = False,
    default_location: str|None =None,
    title: str|None = None,
    parent_window: Window|None=None,
) -> int: ...
def show_save_dialog(
    default_location: str|None =None,
    title: str|None = None,
    parent_window: Window|None=None,
    filters:SequenceLike[SequenceLike[str]]|None=None,
) -> int: ...
