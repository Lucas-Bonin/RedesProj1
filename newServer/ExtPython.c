////
////  ExtPython.c
////  newServer
////
////  Created by Lucas Bonin on 4/12/16.
////  Copyright © 2016 Lucas Bonin. All rights reserved.
////
//
//
//#include <Python.h>
//
//#include "ExtPython.h"
//#include "server.c"
//
//
//
//static PyObject* FuncPrintHello(PyObject *self){
//    
//    dbController dataBase[MAX_LENGTH];
//    int i;
//    
//    for (i = 0; i < MAX_LENGTH; ++i)
//    {
//        dataBase[i].free = 1;
//    }
//    
//    TcpManager server;
//    initServer(&server,5000);
//    server.acceptConnection(&server);
//    
//    package pkg;
//    do {
//        pkg = server.recvMessage(&server);
//        
//        switch (pkg.control){
//            case STORE:
//                storeMessage(server, pkg, dataBase);
//                break;
//            case READ:
//                readMessage(server, dataBase);
//                break;
//            case DELETE:
//                deleteMessage(server, pkg, dataBase);
//                break;
//            case CLOSE:
//                closeConnection(server);
//                break;
//        }
//    }while (pkg.control != CLOSE);
//    
//    return Py_BuildValue("s","Hello no python 2");
//}
//
//static PyMethodDef module_teste[] = {
//    
//    {"hello",FuncPrintHello, METH_NOARGS, NULL},
//    { NULL, NULL, 0, NULL }
//    
//};
//
//
//PyMODINIT_FUNC initFuncTesteDef() {
//    Py_InitModule3("FuncTesteDef", module_teste, "docstring...");
//}