CC     := cl
CFLAGS := /W4 /WX /EHsc /nologo
SRC    := main.cpp
TARGET := rpn.exe

build:
	$(CC) $(CFLAGS) $(SRC) /Fe:$(TARGET)

run: build
	$(TARGET)

clean:
	del /Q $(TARGET) *.obj 2> NUL || ver > NUL
