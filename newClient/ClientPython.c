//
//  ClientPython.c
//  newClient
//
//  Created by Lucas Bonin on 4/13/16.
//  Copyright © 2016 Lucas Bonin. All rights reserved.
//

#include <Python/Python.h>

#include "newClient.c"

TcpManager client;


static PyObject* PyInitClient(PyObject* self, PyObject* args){
    
    char* hostName;
    int port;
    
    if (!PyArg_ParseTuple(args, "si", &hostName, &port)) {
        printf("Nao conseguiu parsear parametro\n");
        Py_RETURN_NONE;
    }

    printf("Variaveis %i, %s\n",port,hostName);
    
    initClient(&client, hostName, port);
    //initClient(&client, "localhost", 5000);
    Py_RETURN_NONE;

}

static PyObject* PyStoreMessage(PyObject* self, PyObject* args){
    
    double temp, hum;
    
    if (!PyArg_ParseTuple(args, "dd", &temp, &hum)) {
        printf("Nao conseguiu parsear parametro\n");
        Py_RETURN_NONE;
    }
    
    storeMessage(client, temp, hum);
    
    Py_RETURN_NONE;
}

static PyObject* PyCloseMessage(PyObject* self){
    
    closeMessage(client);
    
    Py_RETURN_NONE;

}

static PyMethodDef defModule[] = {

    {"initClient",(PyCFunction)PyInitClient, METH_VARARGS, NULL},
    {"storeMessage",(PyCFunction)PyStoreMessage, METH_VARARGS, NULL},
    {"closeMessage",(PyCFunction)PyCloseMessage, METH_NOARGS, NULL},
    { NULL, NULL, 0, NULL }

};


PyMODINIT_FUNC initclient() {
    Py_InitModule3("client", defModule, "docstring...");
}