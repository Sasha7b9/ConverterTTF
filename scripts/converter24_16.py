import os
from struct import *

# Читать 16 бит из массива
def ReadHalfWord(symbols, index):
    return symbols[index] + symbols[index + 1] * 256

# Читатать 32 бита из массива
def ReadWord(symbols, index):
    return ReadHalfWord(symbols, index) + ReadHalfWord(symbols, index + 2) * 256 * 256

# Чтение изображения из файла
def ReadFile():
    fileName = "../font/digitals24_16.bmp"
    input = open(fileName, "rb")
    statinfo = os.stat(fileName)
    data = input.read()
    input.close()
    symbols = unpack(str(statinfo.st_size) + "B", data)
    return symbols

# Расчёт смещения начала пикселей
def CalculateOffset(symbols):
    return ReadWord(symbols, 0x0a)

# Расчёт количества цветов в картинке
def CalculateNumColors(symbols, index):
    colors = []                             # Список цветов
    while index < len(symbols):
        color = symbols[index]
        if colors.count(color) == 0:
            colors.append(color)
        index += 1
    return len(colors)

# Возвращает ширину изображения в пикселях
def GetWidthPicture(symbols):
    return ReadWord(symbols, 0x12)

# Возвращает высоту изображения в пикселях
def GetHeightPicture(symbols):
    return ReadWord(symbols, 0x16)



###### Start here ######



symbols = ReadFile()

offset = CalculateOffset(symbols)

numColors = CalculateNumColors(symbols, offset)

width = GetWidthPicture(symbols)

height = GetHeightPicture(symbols)

print("offset ", offset)

print("number colors ", numColors)

print("size picture ", width, " x ", height)
