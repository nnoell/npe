NPE - Named Pipes Exercise
==========================

**NPE** is an exercise that demonstrates the usage of the *Named Pipes* API for Windows using `C++11` and its `Standard Template Library`. The project consists of a *client* and a *server* that communicate each-other by sending data through one **duplex named pipe**. Essentially, the server receives requests sent by the client, and then sends a response back to the client using a custom protocol.

Since this is a server/client project, you need to run the server *before* the client. The server will wait for a client to connect when it starts, and a `Client connected!` message should be displayed when the client has been launched. The client will exit if there is no server running, and the server is meant to work only with 1 client connected at a time. If you try to run a second client, it will exit, and this is expected. Once both server and client have been executed, the user can send requests to the server using the client's command line interface.

The server is able to handle `12 types of request` sent by the client, and it replies by sending `2 types of responses` to the client. All requests can be sent in both **sync** and **async** mode.

Request types:

 - `Last`: Used to receive the last response processed by the server (only useful for receiving responses from async requests)
 - `List`: Used to receive information about all the objects that are instantiated in the server
 - `Construct`: Used to instantiate an object of a class that has been registered in the server
 - Retrieve: Used to retrieve information about an object in the server
 - `Call`: Used to call a method of an instantiated object that has been registered in the server
 - `Destroy`: Used to destroy an instantiated object in the server
 - `Ascii`: Used to send an Ascii message to the server and print it on screen
 - `Bool`: Used to send a *boolean* value to the server and print it on screen
 - `Int`: Used to send an *int* value to the server and print it on screen
 - `Char`: Used to send a *char* value to the server and print it on screen
 - `Id`: Used to send an *Id* value to the server and print it on screen
 - `Double`: Used to send a *double* value to the server and print it on screen

Responses types:

 - `Ok`: Used to notify the client that the server could process the request successfully
 - `Error`: Used to notify the client that there was an error while processing the request

If we want the *Construct* and *Call* requests to create and call our custom classes and methods, we need to first **register** them. We can easily register any kind of class by inheriting from the base class `npe::server::Object` and then using the `NPE_SERVER_REGISTER_OBJECT` macro, which will register the class using static initialization. On the other hand, the methods can be registered by adding them in the map that is passed as a parameter to the *Object* constructor from the derived class. By default the server has 2 classes registered with their respective methods: a dummy class called `npe::server::Null` that does not do anything, and a second class that holds the information of a video game called `npe::server::Game`. The user can add as many clases as he wants. See their definition for an example.

It is mandatory for the *Constructor* and *Methods* of the classes that will be registered to have exactly **2 parameters**. The first one needs to be a pointer `void *`, which will point to the args data, and the second one needs to be a `std::size_t`, which will represent the size of the args data. Even if a constructor or method do not need parameters, their signature must have those 2 parameters because otherwise you will get a compiler error when registering them. Also, note that it is mandatory for the methods to return `void`, so the only way to return a value from a registered method is using its pointer to void parameter.

The requests and responses are translated using the following protocol (in bytes):


Requests data:

    Last:      1 (code) | 1 (sync) 
    List:      1 (code) | 1 (sync)
    Construct: 1 (code) | 1 (sync) | 4 (object     type hash) | n (object args)
    Retrieve:  1 (code) | 1 (sync) | 4 (object Id)
    Call:      1 (code) | 1 (sync) | 4 (object Id) | 4 (method name hash) | n (method args)
    Destroy:   1 (code) | 1 (sync) | 4 (object Id)
    Ascii:     1 (code) | 1 (sync) | n (ascii message)
    Bool:      1 (code) | 1 (sync) | sizeof(bool) (bool value)
    Int:       1 (code) | 1 (sync) | sizeof(int) (int value)
    Char:      1 (code) | 1 (sync) | sizeof(char) (char value)
    Id         1 (code) | 1 (sync) | sizeof(Id) (Id value)
    Double:    1 (code) | 1 (sync) | sizeof(double) (double value)


Responses data:

    Ok:        1 (code) | 1 (from) | n (output args)
    Error:     1 (code) | 1 (from) | n (ascii error message)


STRUCTURE
=========

The structure of the project consists of 3 modules:

 - `npe_server`: An executable that runs the NPE server
 - `npe_client`: An executable that runs the NPE client
 - `npe_pipe`: A static library with common code used by both *npe_server* and *npe_client* executables

The namespaces of all the source files and headers follow the same structure as the folder's hierarchy. The *main* file for both server and client are `src/npeserver/main.cpp` and `npe/npeclient/main.cpp` respectively, and all public headers are under the *include* directory.


BUILD
=====

This project is meant to work on windows only, and it does not have any dependencies to run other than the `Named Pipes API`. Everything is done using the **C++ Standard Template Library**, so you need to have **CMake** and **Visual Studio** installed to build the project.

- [Named Pipes API](https://docs.microsoft.com/en-us/windows/desktop/ipc/named-pipes)
- [Visual Studio](https://visualstudio.microsoft.com)
- [CMake](https://cmake.org)

Instructions:

 - Open `CMakeLists.txt` with Visual Studio
 - Select the `npe_server.exe` and `npe_client.exe` targets and build them using the *build* dropdown menu

Already built binaries for `npeserver.exe` and `npeclient.exe` can be found under the *releases* directory.


USAGE
=====

Essentially, the basic usage of this project is the following:

 - Run `npe_server.exe`, it will wait for a client to connect
 - Run `npe_client.exe`, it will wait for the user to send requests via its command line interface.

The client has 3 basic commands:

 - `h`: Prints the help
 - `q`: Exits the client (and so the server)
 - `v`: Shows the version
 - `r`: Sends a request to the server (see below for examples and syntax)

Request command syntax:

    r <type> <sync=1|async=0> [param1] ... [paramN]

Request command syntax by type:

    Last:      r 0 <1|0>
    List:      r 1 <1|0>
    Construct: r 2 <1|0> <object_name> [arg1 t=v] ... [argN t=v]
    Request:   r 3 <1|0> <object_id>
    Call:      r 4 <1|0> <object_id> <method_id> [args t=v] ... [argN t=v]
    Destroy:   r 5 <1|0> <object_id>
    Ascii:     r 6 <1|0> <ascii_value>
    Bool:      r 7 <1|0> <bool_value>
    Int:       r 8 <1|0> <int_value>
    Char:      r 9 <1|0> <char_value>
    Id:        r a <1|0> <id_value>
    Double:    r b <1|0> <double_value>

The args values for the *Construct* and *Call* request are a `type=value` pair, where type can be one of the following characters:

 - `a`: for an array of `255 characters`. If you need to send more than 255 characters, use multiple arrays.
 - `b`: for `boolean` values
 - `i`: for `int` values
 - `c`: for `char` values
 - `k`: for `Id` values
 - `d`: for `double` values

Usage example:

 - Send a "Construct" request (code = '2') synchronically (code = 1) to construct a "Null" object on the server. The response will be the Id of the newly created object, which is 0 in this case because it is the first object created on the server:

 ```>> r 2 1 Null```

 - Send another sync "Construct" request to construct a "Game" object on the server. Note that the "Game" constructor needs 2 parameters, and they need to be provided as a type-value pair: 'string' (code = 'a') for the name, and 'int' (code = 'i') for the year. After sending the request, the server will respond with the Id of the newly created object, which will be 1:

 ```>> r 2 1 Game a=System_Shock i=1994```

 - Send an async (code = 0) request to construct another Null object. Note that the server will process the request, but will not respond because the request has been send asynchronically:

 ```>> r 2 0 Null```

 - Wait and get the response of the last request by sending a "Last" request (code = '0') synchronically. The response will be the Id of the second Null object, which is 2:

 ```>> r 0 1```

 - List all the objects on the server by sending a "List" request (code = '1') synchronically:

 ```>> r 1 1```

 - You can also retrive one object by just sending a "Retrieve" request (code = '3') synchronically, which needs the Id of the object to be retrieve. Let's retrieve the Game object with Id 1:

 ```>> r 3 1 1```

 - Let's call the method "SetName" on the Game object (with Id = 1) by sending a "Call" request (code = '4') synchronically. Note that the "SetName" method needs a string as a parameter:

 ```>> r 4 1 1 SetName a=Perfect_Dark```

 - Let's get the year of the game by calling the "GetYear" method. The method has an output `int` parameter and it needs to be specified:

 ```>> r 4 1 1 GetYear i=0```

 - Set the year of the game object using the "SetYear" method:

 ```>> r 4 1 1 SetYear i=2000```

 - List again all the objects on the server and check if the values have been updated

 ```>> r 1 1```

 - Destroy the first Null object (with Id = 0) by sending a "Destroy" request (code = 5):

 ```>> r 5 1 0```

 - Call the method "DoNothing" without args of the remaining Null object with Id = 2:

 ```>> r 4 1 2 DoNothing```

 - Destroy the remaining Null object

 ```>> r 5 1 2```

 - Destroy the remaining Game object

 ```>> r 5 1 1```

 - Send an *ascii* request so it will show up on the server's screen

 ```>> r 6 1 Hello_World```

 - Do the same with a *bool* request

 ```>> r 7 1 1```

 - Do the same with an *int* request

 ```>> r 8 1 87```

 - Do the same with a *char* request

 ```>> r 9 1 ?```

 - Do the same with an *Id* request

 ```>> r a 1 123456```

 - Do the same with a *double* request

 ```>> r b 1 3.1416```


AUTHOR'S NOTES
==============

 - This project has only been tested on `Windows 10` with `Visual Studio Professional 2017`
 - Email to `nnoell3@gmail.com` if you find bugs or you need help
