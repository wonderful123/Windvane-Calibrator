#include <UI/ConsoleIOHandler.h>
#include <UI/SerialIOHandler.h>
#include <UI/ConsoleOutput.h>
#include <UI/SerialOutput.h>
#include <gtest/gtest.h>

TEST(IOHandlers, Instantiate) {
    ConsoleIOHandler consoleIo;
    SerialIOHandler serialIo;
    ConsoleOutput consoleOut;
    SerialOutput serialOut;
    SUCCEED();
}
