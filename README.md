# This repository contains our third assignment in the 'Advanced programming' course

## We were told to create the following program:
### The program shall set up a server and client(s), then after assuring the two connected successfully, they will be connected in a new thread ans start  their work together. The server sends a menu to the client and the client need to respond his choice, then the server make the order.


**_Our way of work_**:

First thing we did was to set up a client and server and make sure they can communicate properly, afterwards we started working on the logic on the server side (how to break down the message from the user in order to get the data we need to find the type nearest (according to the requested distance-calculating algorithm) to the vector the client is sending.

in the course of dealing with the input we had some trouble dealing with an invalid input (for instace if the last element the user sent us wasn't a number, the function we used to convert the string to a number (stoi()) raised an exception that crashed the server) but we managed to overcome the stated obstacle :)

Once we got that out of the way, we proceeded to assure the client can quit and close the connection, and once it does that we are open to serve a new client, and that was the final piece of the puzzle.

After being able to make the program with 1 client , we started working on the multi-threading of the project.

**_User Guide_**:

In order to make things easier for the user, we added a makefile for the project. so all the user needs to do is type "make some" (when (s)he's on the currect directory) and then all is left is to execute Server.out and Client.out

_Server_:

In order to set up a server the user needs to type: "./Server.out [port number to listen to]" That's all for the server, the rest is on our beloved client


_Client_:

In order to set up the client, the user needs to type "./Client.out [ip address of the server] [port number the server listens to]" Afterwards the user of the client needs to input a message to send to the server.

After the connection is done , we will go through the menu:

![image](https://user-images.githubusercontent.com/118102450/214791213-26bc17dc-68a1-4569-bc94-299afe72275d.png)

**_option 1 : Upload!!_**

in this option , the server would request the client to upload a local train file ( a CSV file with K columns , K-1 are numbers and the last column is a name of the type) , if thr file is invalid it would return "invalid input" and return to the menu . if the file is valid , it will write "Upload complete" and will request the client to upload local test file (again ,  a CSV file with K-1 columns , all are numbers) , the same as the train file.

_Example_ :

![image](https://user-images.githubusercontent.com/118102450/214792662-3090db21-a10e-4e44-bfe2-5673df0d939f.png)

_Example for invalid option_ :

![image](https://user-images.githubusercontent.com/118102450/214793000-782a3d71-afd1-448c-b517-2a393f374904.png)

**_option 2 : Settings!!_**

In this option , the server sends to client the current K and Distance function that setted. The default setting are K=5 and distance = AUC.
If you don't want to change them, just press ENTER. To change the setting you should write one line that contains a valid positive number K , a space and then a Metric function name (AUC/MIN/MAN/CAN/CHB). format : {positive number K} {AUC/MIN/MAN/CAN/CHB}. If your input is not valid , the settings won't change.

_Example for not changing_ :

![image](https://user-images.githubusercontent.com/118102450/214794332-6c7e5252-d099-49f3-9640-ee405e650c80.png)

_Example for changing_ :

![image](https://user-images.githubusercontent.com/118102450/214794508-bd63e1bd-f07c-4ccf-9faf-61c1b5b1526e.png)

_Example for invalid option_ :

![image](https://user-images.githubusercontent.com/118102450/214794655-d93622d5-b64d-4da2-a2ee-c2eb4b4aa5aa.png)

**_option 3 : Classify! (the main thing 🤙 )_**

In this option , the server would classify the test file that the client enterd. If there is no data, the server asks the client to upload data.
If everything ok the server sends complete message.

_Example for valid classify_ :

![image](https://user-images.githubusercontent.com/118102450/214795485-4eba43a8-8cb2-4e10-861f-f07555477c56.png)

_Example for not valid_ :

![image](https://user-images.githubusercontent.com/118102450/214795637-d1231081-3146-435b-a223-9c98022a8440.png)

**_option 4 : Displaying the results!_**

In this option , the Client asks the server to display the results. the results would be printed on Client's shell at the same order like the test file.
Each vector is printed like this : {index}. {type classified}.
if there is no data , it would say : "please upload data"
if the data was not classified : "please classify the data"

_Example for no data_ : 

![image](https://user-images.githubusercontent.com/118102450/214797046-c715d876-bb80-4c43-84a2-89dc8d6c6f64.png)

_Example for unclassified_ :

![image](https://user-images.githubusercontent.com/118102450/214797183-944f825b-84c8-4cd4-add7-0de716fa8d86.png)

_Example for valid_ :

![image](https://user-images.githubusercontent.com/118102450/214797263-ebb47306-9b43-41b9-abe0-7e104e956a9d.png)

**_option 5 : Download!_**

This option is pretty similar to option 4, but it will be printed on a file the Client would ask the server to edit.
the Server asks the Client to send a file path.
The download made on a different thread , so the client gets back to the menu while the download is being done.
The format on the file will be the same as displaying the results.

**_option 8 : Exit!_**

If the client pressing 8 , the client proccess will be closed , and the server stays on.

**_any other option : inalid input!_**

And that's all, now you can know which types your test vectours are closest to BUT with a server and client, like a boss.

