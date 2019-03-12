import os
from struct import *

#######################################################################
_symbols = []     # Здесь содержимое файла
_offset = 0       # Смщещение картинки относительно начала файла
_numColors = 0    # Количество цветов в картинкe
_width = 0        # Ширина картинки в пикселях
_height = 0       # Высота картинки в пикселях



########################################################################



# Читать 16 бит из массива
def ReadHalfWord(index):
    return _symbols[index] + _symbols[index + 1] * 256

# Читатать 32 бита из массива
def ReadWord(index):
    return ReadHalfWord(index) + ReadHalfWord(index + 2) * 256 * 256

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
def CalculateOffset():
    return ReadWord(0x0a)

# Расчёт количества цветов в картинке
def CalculateNumColors(index):
    colors = []                             # Список цветов
    while index < len(_symbols):
        color = _symbols[index]
        if colors.count(color) == 0:
            colors.append(color)
        index += 1
    return len(colors)

# Возвращает ширину изображения в пикселях
def GetWidthPicture():
    return ReadWord(0x12)

# Возвращает высоту изображения в пикселях
def GetHeightPicture():
    return ReadWord(0x16)



###### Start here ######

_symbols = ReadFile()

_offset = CalculateOffset()

_numColors = CalculateNumColors(_offset)

_width = GetWidthPicture()

_height = GetHeightPicture()

print("offset ", _offset)

print("number colors ", _numColors)

print("size picture ", _width, " x ", _height)
