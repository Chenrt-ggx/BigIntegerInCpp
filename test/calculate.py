def add(lhs: str, rhs: str) -> str:
    return str(int(lhs) + int(rhs))


def sub(lhs: str, rhs: str) -> str:
    return str(int(lhs) - int(rhs))


def mul(lhs: str, rhs: str) -> str:
    return str(int(lhs) * int(rhs))


def div(lhs: str, rhs: str) -> str:
    return str(int(lhs) // int(rhs))


def mod(lhs: str, rhs: str) -> str:
    return str(int(lhs) % int(rhs))


def eq(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) == int(rhs) else 0


def ne(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) != int(rhs) else 0


def gt(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) > int(rhs) else 0


def ge(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) >= int(rhs) else 0


def lt(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) < int(rhs) else 0


def le(lhs: str, rhs: str) -> int:
    return 1 if int(lhs) <= int(rhs) else 0
