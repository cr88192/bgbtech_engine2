;header, 0xBF00, header-len
dw 0xBF00, 4
;header data, width height
db 0, 8, 8, 1

;span from 32 to 127
dw 32, 127
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	;0x20 " "
db 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00	;0x21 "!"
db 0x00, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00	;0x22 "\""
db 0x00, 0x66, 0xFF, 0x66, 0x66, 0xFF, 0x66, 0x00	;0x23 "#"
db 0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x18, 0x00	;0x24 "$"
db 0x00, 0x66, 0x6C, 0x18, 0x30, 0x66, 0x46, 0x00	;0x25 "%"
db 0x1C, 0x36, 0x1C, 0x38, 0x6F, 0x66, 0x3B, 0x00	;0x26 "&"
db 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00	;0x27 "'"
db 0x00, 0x0E, 0x1C, 0x18, 0x18, 0x1C, 0x0E, 0x00	;0x28 "("
db 0x00, 0x70, 0x38, 0x18, 0x18, 0x38, 0x70, 0x00	;0x29 ")"
db 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00	;0x2A "*"
db 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00	;0x2B "+"
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30	;0x2C ","
db 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00	;0x2D "-"
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00	;0x2E "."
db 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00	;0x2F "/"
db 0x00, 0x3C, 0x66, 0x6E, 0x76, 0x66, 0x3C, 0x00	;0x30 "0"
db 0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E, 0x00	;0x31 "1"
db 0x00, 0x3C, 0x66, 0x0C, 0x18, 0x30, 0x7E, 0x00	;0x32 "2"
db 0x00, 0x7E, 0x0C, 0x18, 0x0C, 0x66, 0x3C, 0x00	;0x33 "3"
db 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x00	;0x34 "4"
db 0x00, 0x7E, 0x60, 0x7C, 0x06, 0x66, 0x3C, 0x00	;0x35 "5"
db 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00	;0x36 "6"
db 0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x00	;0x37 "7"
db 0x00, 0x3C, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00	;0x38 "8"
db 0x00, 0x3C, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00	;0x39 "9"
db 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00	;0x3A ":"
db 0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x30	;0x3B ";"
db 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00	;0x3C "<"
db 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00	;0x3D "="
db 0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00	;0x3E ">"
db 0x00, 0x3C, 0x66, 0x0C, 0x18, 0x00, 0x18, 0x00	;0x3F "?"
db 0x00, 0x3C, 0x66, 0x6E, 0x6E, 0x60, 0x3E, 0x00	;0x40 "@"
db 0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x00	;0x41 "A"
db 0x00, 0x7C, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00	;0x42 "B"
db 0x00, 0x3C, 0x66, 0x60, 0x60, 0x66, 0x3C, 0x00	;0x43 "C"
db 0x00, 0x78, 0x6C, 0x66, 0x66, 0x6C, 0x78, 0x00	;0x44 "D"
db 0x00, 0x7E, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00	;0x45 "E"
db 0x00, 0x7E, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00	;0x46 "F"
db 0x00, 0x3E, 0x60, 0x60, 0x6E, 0x66, 0x3E, 0x00	;0x47 "G"
db 0x00, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00	;0x48 "H"
db 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00	;0x49 "I"
db 0x00, 0x06, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00	;0x4A "J"
db 0x00, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x00	;0x4B "K"
db 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00	;0x4C "L"
db 0x00, 0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x00	;0x4D "M"
db 0x00, 0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x00	;0x4E "N"
db 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00	;0x4F "O"
db 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x00	;0x50 "P"
db 0x00, 0x3C, 0x66, 0x66, 0x66, 0x6C, 0x36, 0x00	;0x51 "Q"
db 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x00	;0x52 "R"
db 0x00, 0x3C, 0x60, 0x3C, 0x06, 0x06, 0x3C, 0x00	;0x53 "S"
db 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00	;0x54 "T"
db 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x00	;0x55 "U"
db 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00	;0x56 "V"
db 0x00, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00	;0x57 "W"
db 0x00, 0x66, 0x66, 0x3C, 0x3C, 0x66, 0x66, 0x00	;0x58 "X"
db 0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00	;0x59 "Y"
db 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00	;0x5A "Z"
db 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00	;0x5B "["
db 0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00	;0x5C "\\"
db 0x00, 0x78, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00	;0x5D "]"
db 0x00, 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00	;0x5E "^"
db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00	;0x5F "_"
db 0x00, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00	;0x60 "`"
db 0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00	;0x61 "a"
db 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x7C, 0x00	;0x62 "b"
db 0x00, 0x00, 0x3C, 0x60, 0x60, 0x60, 0x3C, 0x00	;0x63 "c"
db 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00	;0x64 "d"
db 0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00	;0x65 "e"
db 0x00, 0x0E, 0x18, 0x3E, 0x18, 0x18, 0x18, 0x00	;0x66 "f"
db 0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x7C	;0x67 "g"
db 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x00	;0x68 "h"
db 0x00, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3C, 0x00	;0x69 "i"
db 0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x3C	;0x6A "j"
db 0x00, 0x60, 0x60, 0x6C, 0x78, 0x6C, 0x66, 0x00	;0x6B "k"
db 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00	;0x6C "l"
db 0x00, 0x00, 0x66, 0x7F, 0x7F, 0x6B, 0x63, 0x00	;0x6D "m"
db 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00	;0x6E "n"
db 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00	;0x6F "o"
db 0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60	;0x70 "p"
db 0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06	;0x71 "q"
db 0x00, 0x00, 0x7C, 0x66, 0x60, 0x60, 0x60, 0x00	;0x72 "r"
db 0x00, 0x00, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x00	;0x73 "s"
db 0x00, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x0E, 0x00	;0x74 "t"
db 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00	;0x75 "u"
db 0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00	;0x76 "v"
db 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x3E, 0x36, 0x00	;0x77 "w"
db 0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00	;0x78 "x"
db 0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x0C, 0x78	;0x79 "y"
db 0x00, 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x7E, 0x00	;0x7A "z"
db 0x00, 0x0E, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00	;0x7B "{"
db 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00	;0x7C "|"
db 0x00, 0x70, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00	;0x7D "}"
db 0x00, 0x77, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00	;0x7E "~"
db 0x00, 0x00, 0x00, 0x08, 0x1C, 0x36, 0x63, 0x7F	;0x7F "\x7F"

;terminal
dw 0, 0
