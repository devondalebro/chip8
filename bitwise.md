Bitwise operators

Bitwise and "&"
    Compares two bits
        1 & 1 = 1
        1 & 0 = 0
        0 & 1 = 0
        0 & 0  = 0

Bitwise or "|"
    Opposite of &

Bitwise not "~"
    Complements each bit one by one

Bitwise shift-left "<<"
    Shifts all bits to the left by the number to the right 
    i.e. first operand << second operand
    First operand: bits get shifted
    Second operand: how many bits to shift by
    Trailing bits are zeroes
    Left shifting is equivalent to multiplication by 2^(right operand)

Bitwise shift-right ">>"
    Opposite of left shift
    Leading positions are filled with zeroes
    Equivalent to divison by 2^(right operand)

Bitwise XOR operator "^"
    Think about the or "|" operator (it's inclusive)
    XOR operator is exclusive such that 1 XOR 1 = 0 (mutually exclusive)
    Basically results is 1 when two bits are different and 0 when two bits are same
    
