def pi_calculate(length: int) -> str:
    index = 1
    pi = mul = 2 * 10 ** (length + 10)
    while mul > 0:
        mul = mul * index // (index * 2 + 1)
        pi, index = pi + mul, index + 1
    return "3." + str(pi)[1: length + 1]


def e_calculate(length: int) -> str:
    index = 1
    e = mul = 10 ** (length + 10)
    while mul > 0:
        mul = mul // index
        e, index = e + mul, index + 1
    return "2." + str(e)[1: length + 1]
