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
#include "matrix_util.h"
#include <string.h>
#include <stdlib.h>

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}
extern int debugMode;

int unInitGlobal;
int initGlobal = 9;


static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[]);
static eCommandResult_T ConsoleCommandDrawGraphic(const char buffer[]);
static eCommandResult_T ConsoleCommandGetGyroMotion(const char buffer[]);
static eCommandResult_T ConsoleCommandSetGyroTolerance(const char buffer[]);
static eCommandResult_T ConsoleCommandGetGyroSample(const char buffer[]);
static eCommandResult_T ConsoleCommandGetRandomValue(const char buffer[]);
static eCommandResult_T ConsoleCommandPrintSequence(const char buffer[]);
static eCommandResult_T ConsoleCommandIncrementSequence(const char buffer[]);
static eCommandResult_T ConsoleCommandPlaySequence(const char buffer[]);
static eCommandResult_T ConsoleCommandCompareSequence(const char buffer[]);
static eCommandResult_T ConsoleCommandSetGraphicsMode(const char buffer[]);
static eCommandResult_T ConsoleCommandSetDebugMode(const char buffer[]);
static eCommandResult_T ConsoleCommandDebugPrint(const char buffer[]);

static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {";", &ConsoleCommandComment, HELP("Comment! You do need a space after the semicolon. ")},
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
    {"drawG", &ConsoleCommandDrawGraphic, HELP("Draws a graphic (direction 1,2,3,4) for the playfiled")},
	{"getGM", &ConsoleCommandGetGyroMotion, HELP("Polls and returns the Gyro Motion detected")},
	{"setGT", &ConsoleCommandSetGyroTolerance, HELP("Set the Gyro Motion Tolerance")},
	{"getGSample", &ConsoleCommandGetGyroSample, HELP("Get N number of Gyro Sample separate by 10ms")},
	{"getRand", &ConsoleCommandGetRandomValue, HELP("Get Random Value between 1 and 4")},
	{"printSeq", &ConsoleCommandPrintSequence, HELP("Print the current match sequence")},
	{"incSeq", &ConsoleCommandIncrementSequence, HELP("Add to the sequence")},
	{"playSeq", &ConsoleCommandPlaySequence, HELP("Play the sequence")},
	{"cmpSeq", &ConsoleCommandCompareSequence, HELP("Compare user input to the sequence")},
	{"setModeG", &ConsoleCommandSetGraphicsMode, HELP("Set the Graphics Mode (1- LCD, 2 - LED MATRIX ")},
	{"setD", &ConsoleCommandSetDebugMode, HELP("Set the Debug Mode (1- Debug On, 0 - Debug Off ")},
	{"dp", &ConsoleCommandDebugPrint, HELP("Print Some debug information (memory addresses) ")},
	

	CONSOLE_COMMAND_TABLE_END // must be LAST
};

static eCommandResult_T ConsoleCommandDebugPrint(const char buffer[]) {
	int nonStaticVar = 4;
	static int staticVar = 2;
	char stack_info[60];
	char bss_info[100];
	char data_info[100];
	char initGlobal_info[60];
	char uniInitGlobal_info[60];
	char staticVar_info[60];
	char localVar_info[60];
	char dynamicVar_info[60];
	char *dynamic_var = malloc(4 * sizeof(char));
	// register uintptr_t sp asm ("sp");
	extern int _estack, __bss_start__, __bss_end__, _sdata, _edata;
	sprintf(bss_info, "Bss start - %p, Bss end - %p ", &__bss_start__, &__bss_end__);
    sprintf(stack_info, "Stack Pointer: %p", &_estack);
	sprintf(data_info, "data start - %p, data end - %p ", &_sdata, &_edata);

	sprintf(initGlobal_info,"Initialized Global (initGlobal) %p", &initGlobal);
	sprintf(uniInitGlobal_info,"Uninitialized Global  (unInitGlobal) %p", &unInitGlobal);
	sprintf(staticVar_info,"Static var in a function (staticVar) %p", &staticVar);
	sprintf(localVar_info,"Variable (nonStaticVar) in a function %p", &nonStaticVar);
	sprintf(dynamicVar_info,"Variable dynamically allocated(dynamicVar) - %p", &dynamic_var);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(initGlobal_info);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(staticVar_info);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(data_info);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(STR_ENDLINE);

	ConsoleIoSendString(uniInitGlobal_info);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(bss_info);
	ConsoleIoSendString(STR_ENDLINE);
	
	
	ConsoleIoSendString(localVar_info);
	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(stack_info);
	ConsoleIoSendString(STR_ENDLINE);

	ConsoleIoSendString(STR_ENDLINE);
	ConsoleIoSendString(dynamicVar_info);
	ConsoleIoSendString(STR_ENDLINE);
	
	return CONSOLE_SUCCESS;
}
static eCommandResult_T ConsoleCommandSetDebugMode(const char buffer[]) {
	int16_t mode;
	char debugStr[50];
	ConsoleReceiveParamInt16(buffer, 1, &mode);
	debugMode = mode;
	sprintf(debugStr, "Setting Debug to %d", debugMode);
	ConsoleIoSendString(debugStr);
	ConsoleIoSendString(STR_ENDLINE);
	return CONSOLE_SUCCESS;
}
static eCommandResult_T ConsoleCommandSetGraphicsMode(const char buffer[]) {
	int16_t mode;
	ConsoleReceiveParamInt16(buffer, 1, &mode);
	setGraphicsMode(mode);
	ConsoleIoSendString(STR_ENDLINE);
	return CONSOLE_SUCCESS;
}

static eCommandResult_T ConsoleCommandCompareSequence(const char buffer[]) {
	char matchStr[50];
	int matches = 0;
	HAL_Delay(500);
	ConsoleIoSendString("Start Playing!");
	matches = compareSequence();

	ConsoleIoSendString(STR_ENDLINE);
	sprintf(matchStr, "Compare Complete. Matches : %d", matches);
	ConsoleIoSendString(matchStr);
	ConsoleIoSendString(STR_ENDLINE);
	
	return CONSOLE_SUCCESS;

}
static eCommandResult_T ConsoleCommandPlaySequence(const char buffer[]) {
	
	char playStr[50];
	int seqSize;
	seqSize = getSequenceSize();
	playSequence();
	ConsoleIoSendString(STR_ENDLINE);
	sprintf(playStr, "Play Complete. Sequence size is : %d", seqSize);
	ConsoleIoSendString(playStr);
	ConsoleIoSendString(STR_ENDLINE);

	return CONSOLE_SUCCESS;
}

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
	drawCircle(motionLocation, 1);
	ConsoleIoSendString("Motion Detected");
	ConsoleIoSendString(STR_ENDLINE);
	sprintf(gyroSampleStr,"X: %f, y: %f, z: %f", xyzGyro[0], xyzGyro[1], xyzGyro[2]);
	ConsoleIoSendString(gyroSampleStr);
	ConsoleIoSendString(STR_ENDLINE);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandDrawGraphic(const char buffer[]) {

	int16_t circleLocation;
	enum GraphicsMode graphicsMode;
	graphicsMode = getGraphicsMode();
	ConsoleReceiveParamInt16(buffer, 1, &circleLocation);
	if (graphicsMode == LCD_SCREEN) {
		BSP_LCD_Clear(LCD_COLOR_WHITE);
		drawCircle(circleLocation, 1);
	}

	if (graphicsMode == LED_MATRIX) {
		drawGraphic(circleLocation);		
	}

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


