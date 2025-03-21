from typing import Any

class BufferProxy:
    @property
    def parent(self) -> Any: ...
    @property
    def length(self) -> int: ...
    @property
    def raw(self) -> bytes: ...
    # possibly going to be deprecated/removed soon, in which case these
    # typestubs must be removed too
    @property
    def __array_interface__(self) -> dict[str, Any]: ...
    @property
    def __array_struct__(self) -> Any: ...
    def __init__(self, parent: Any) -> None: ...  # TODO: parent: TypedDict | Protocol
    def write(self, buffer: bytes, offset: int = 0) -> None: ...
