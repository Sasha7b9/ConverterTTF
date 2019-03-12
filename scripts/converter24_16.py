import os
from struct import *

#######################################################################
_symbols = []       # Здесь содержимое файла
_offset = 0         # Смщещение картинки относительно начала файла
_numColors = 0      # Количество цветов в картинкe
_width = 0          # Ширина картинки в пикселях
_height = 0         # Высота картинки в пикселях
_colors = []        # Цвета изображения. Они идут в таком порядке : цвет промежутков между глифами, цвет пустоты в глифах, цвет символа
_dX = 0             # Смещение между левыми верхними углами глефов по горизонтали
_dY = 0             # Смещение между левыми верхними углами глефов по вертикали


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
    while index < len(_symbols):
        color = _symbols[index]
        if _colors.count(color) == 0:
            _colors.append(color)
        index += 1
    return len(_colors)

# Возвращает ширину изображения в пикселях
def GetWidthPicture():
    return ReadWord(0x12)

# Возвращает высоту изображения в пикселях
def GetHeightPicture():
    return ReadWord(0x16)

# Возвращает значение пикселя в точке x, y
def GetPoint(x, y):
    return _symbols[_offset + x + y * _width]

# Расчёт смещений между глефами
def CalculateOffsetX():
    x1 = 0;
    x2 = 0;
    for x in range(_width):
        if GetPoint(x, 149) == _colors[1] or GetPoint(x, 149) == _colors[2]:    # Первое вхождение в глиф
            x1 = x
            print("find x1 ", x1)
            break
    for x in range(x1, _width):
        if GetPoint(x, 149) == _colors[0]:    # Вышли за глиф
            x2 = x
            print("find first x2 ", x2)
            break
    for x in range(x2, _width):
        if GetPoint(x, 149) == _colors[1] or GetPoint(x, 149) == _colors[2]:    # Вошли в следующий глиф
            x2 = x
            print("find second x2 ", x2)
            break
    return x2 - x1

def Dump(sizeX, sizeY):
    for y in range(sizeY):
        for x in range(sizeX):
            color = GetPoint(x, y)
            if color == _colors[0]:
                print('*', end = '')
            if color == _colors[1]:
                print('.', end = '')
            if color == _colors[2]:
                print(' ', end = '')
        print()


###### Start here ######



_symbols = ReadFile()

_offset = CalculateOffset()

_numColors = CalculateNumColors(_offset)

_width = GetWidthPicture()

_height = GetHeightPicture()

print("offset ", _offset)

print("number colors ", _numColors)

print("size picture ", _width, " x ", _height)

print("colors - ", _colors[0], " ", _colors[1], " ", _colors[2])

_dX = CalculateOffsetX()

print("dX = ", _dX, ", dY = ", _dY)

Dump(70, 70)

