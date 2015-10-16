.. _tuto8-io:

===============
Input/Output
===============


~~~~ { .Cpp }

    int handle;
    handle  = IO_Open("slot0.txt","w");
    string ts = "Handle =  "+formatInt(handle,"");
    UTI_Log(ts);
    IO_WriteString(handle,"coucou");
    IO_WriteString(handle,"coucou2");
    IO_WriteInt(handle,24);
    IO_Close(handle);

~~~~

~~~~ { .Cpp }

    int handle;
    handle  = IO_Open("slot0.txt","r");
    ts = "Handle =  "+formatInt(handle,"");
    UTI_Log(ts);
    string test;
    int testInt;
    IO_ReadString(handle,test);
    UTI_Log(test);
    IO_ReadString(handle,test);
    UTI_Log(test);
    IO_ReadInt(handle,testInt);
    ts = "testInt =  "+formatInt(testInt,"");
    UTI_Log(ts);
    
    IO_Close(handle);
~~~~




