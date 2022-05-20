
Exercise 8: Resource Constraints

I added the following code to a project to print out various memory addresses
```C
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
```

The code above produced the following output:

```
Initialized Global (initGlobal) 0x20000008
Static var in a function (staticVar) 0x2000000c
data start - 0x20000000, data end - 0x20000254

Uninitialized Global  (unInitGlobal) 0x200008d8
Bss start - 0x20000254, Bss end - 0x20000a1c
Variable (nonStaticVar) in a function 0x2002fd38
Stack Pointer: 0x20030000

Variable dynamically allocated(dynamicVar) - 0x2002fd3c
```

### Obversations

* Initialized global and static variables inside a function were both with the data segment memory boundaries
* Uninitialized global variable was within bss sgement
* the non-static variable in a function and dynamically alloacted variable appear relatively close to the stack pointer, so I could not draw any definitive observation about them