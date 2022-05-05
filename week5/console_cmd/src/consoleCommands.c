// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"


#include "version.h"
#include "LcdDisplay.h"
#include "gyro_util.h"
#include "stm32f429i_discovery_lcd.h"
#include "game.h"
#include <string.h>

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}



static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[]);
static eCommandResult_T ConsoleCommandDrawCircle(const char buffer[]);
static eCommandResult_T ConsoleCommandGetGyroMotion(const char buffer[]);
static eCommandResult_T ConsoleCommandSetGyroTolerance(const char buffer[]);
static eCommandResult_T ConsoleCommandGetGyroSample(const char buffer[]);
static eCommandResult_T ConsoleCommandGetRandomValue(const char buffer[]);
static eCommandResult_T ConsoleCommandPrintSequence(const char buffer[]);
static eCommandResult_T ConsoleCommandIncrementSequence(const char buffer[]);

static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {";", &ConsoleCommandComment, HELP("Comment! You do need a space after the semicolon. ")},
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
    {"int", &ConsoleCommandParamExampleInt16, HELP("How to get a signed int16 from params list: int -321")},
    {"u16h", &ConsoleCommandParamExampleHexUint16, HELP("How to get a hex u16 from the params list: u16h aB12")},
    {"drawC", &ConsoleCommandDrawCircle, HELP("Draws a circle (1,2,3,4) for the playfiled")},
	{"getGM", &ConsoleCommandGetGyroMotion, HELP("Polls and returns the Gyro Motion detected")},
	{"setGT", &ConsoleCommandSetGyroTolerance, HELP("Set the Gyro Motion Tolerance")},
	{"getGSample", &ConsoleCommandGetGyroSample, HELP("Get N number of Gyro Sample separate by 10ms")},
	{"getRand", &ConsoleCommandGetRandomValue, HELP("Get Random Value between 1 and 4")},
	{"printSeq", &ConsoleCommandPrintSequence, HELP("Print the current match sequence")},
	{"incSeq", &ConsoleCommandIncrementSequence, HELP("Add to the sequence")},

	CONSOLE_COMMAND_TABLE_END // must be LAST
};


static eCommandResult_T ConsoleCommandIncrementSequence(const char buffer[]) {
	
	char incSeqStr[100];
	int seqSize;
	incMatchSequence();
	ConsoleIoSendString(STR_ENDLINE);
	seqSize = getSequenceSize();
	sprintf(incSeqStr, "Sequence size is now : %d", seqSize);
	ConsoleIoSendString(incSeqStr);
	ConsoleIoSendString(STR_ENDLINE);

	return CONSOLE_SUCCESS;
}

static eCommandResult_T ConsoleCommandPrintSequence(const char buffer[]) { 
	char seqStr[256];
	char append[4];
	int *currSeq = getMatchSequence();
	int seqSize = getSequenceSize();
	
	sprintf(seqStr, "Size: %d -- Sequence: ", seqSize);
	for (int ii=0; ii < seqSize; ii++) {
		sprintf(append,"%d ,", currSeq[ii]);
		strcat(seqStr, append);
	}
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(seqStr);
	ConsoleIoSendString(STR_ENDLINE);

	return CONSOLE_SUCCESS;

}
static eCommandResult_T ConsoleCommandGetRandomValue(const char buffer[]) {
	char str[50];
	int randomVal= getRandomValue();
	sprintf(str, "Val: %d", randomVal);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(str);
	ConsoleIoSendString(STR_ENDLINE);
	
	return CONSOLE_SUCCESS;
}
static eCommandResult_T ConsoleCommandGetGyroSample(const char buffer[]) {
	char gyroSampleStr[100];
	int16_t numSamples;
	int ii = 0;
	ConsoleReceiveParamInt16(buffer, 1, &numSamples);
	float xyzGyro[3];
	for (int ii=0; ii < numSamples; ii++) {
		getGyroSample(xyzGyro);
		sprintf(gyroSampleStr,"Sample %d -- X: %f, y: %f, z: %f", ii, xyzGyro[0], xyzGyro[1], xyzGyro[2]);
		ConsoleIoSendString(gyroSampleStr);
		ConsoleIoSendString(STR_ENDLINE);
		HAL_Delay(10);
	}

	return CONSOLE_SUCCESS;
}
static eCommandResult_T ConsoleCommandSetGyroTolerance(const char buffer[]) {
	int16_t tolerance;
	ConsoleReceiveParamInt16(buffer, 1, &tolerance);
	setGyroMotionTolerance((float)tolerance);
	ConsoleIoSendString(STR_ENDLINE);
	return CONSOLE_SUCCESS;
}

static eCommandResult_T ConsoleCommandGetGyroMotion(const char buffer[]) {

	float xyzGyro[3];
	char gyroSampleStr[100];
	HAL_Delay(500);
	ConsoleIoSendString("Waiting for Gyro Motion to Cross a Threshold");
	ConsoleIoSendString(STR_ENDLINE);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	int motionLocation = waitforGyroMotionDetection(xyzGyro);
	drawCircle(motionLocation);
	ConsoleIoSendString("Motion Detected");
	ConsoleIoSendString(STR_ENDLINE);
	sprintf(gyroSampleStr,"X: %f, y: %f, z: %f", xyzGyro[0], xyzGyro[1], xyzGyro[2]);
	ConsoleIoSendString(gyroSampleStr);
	ConsoleIoSendString(STR_ENDLINE);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandDrawCircle(const char buffer[]) {

	int16_t circleLocation;
	ConsoleReceiveParamInt16(buffer, 1, &circleLocation);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	drawCircle(circleLocation);
	ConsoleIoSendString(STR_ENDLINE);
	return COMMAND_SUCCESS;
	
}


static eCommandResult_T ConsoleCommandComment(const char buffer[])
{
	// do nothing
	IGNORE_UNUSED_VARIABLE(buffer);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		ConsoleIoSendString(mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(" : ");
		ConsoleIoSendString(mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[])
{
	int16_t parameterInt;
	eCommandResult_T result;
	result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString(STR_ENDLINE);
		ConsoleIoSendString("Parameter is ");
		ConsoleSendParamInt16(parameterInt);
		ConsoleIoSendString(" (0x");
		ConsoleSendParamHexUint16((uint16_t)parameterInt);
		ConsoleIoSendString(")");
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[])
{
	uint16_t parameterUint16;
	eCommandResult_T result;
	result = ConsoleReceiveParamHexUint16(buffer, 1, &parameterUint16);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString(STR_ENDLINE);
		ConsoleIoSendString("Parameter is 0x");
		ConsoleSendParamHexUint16(parameterUint16);
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(VERSION_STRING);
	ConsoleIoSendString(STR_ENDLINE);
	return result;
}


const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


