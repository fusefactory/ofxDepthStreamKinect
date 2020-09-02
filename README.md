# ofxDepthStreamKinect

OpenFrameworks addon to send data from kinect over the network.

# Compatibility

This addons is compatible with the following kinect:

- [Kinect V2](https://developer.microsoft.com/en-us/windows/kinect/) 

- [Azure Kinect](https://azure.microsoft.com/en-gb/services/kinect-dk/)


|  OS        | Sender | Receiver |
|------------|--------|----------|
| Windows 10 | Yes    | Yes      |
| Mac OS     | No     | Yes      |

The sender binary (java) for KinectV2 is available on [here](https://drive.google.com/file/d/1G8OlAO5aRGtnPxaKppEEuxYKKH0UME4D/view?usp=sharing).

# Protocol

A communication tcp/ip is established from the server and the client. A single message has the following data:

![alt text](docs/messageFrame.png)

**Header**: int value of N, number of bytes of the message
**Message**: byte array of data compressed.

The steps to get the single message are:

1) retrive data from the kinect
2) put data on byte array (1 or 2 bytes per pixel)
3) compress data using zlib
4) retrive compressed data size
5) Send raw bytes. First 4 bytes (int) are the size of the message.

Every new frame a message is send.


# Examples

## kinectAzureSender

Using [ofxAzureKinect](https://github.com/fusefactory/ofxAzureKinect/tree/develop) retrive depth data of [AzureKinect](https://docs.microsoft.com/en-us/azure/kinect-dk/) and it starts a server stream.


## kinectReceiver

Add ofxDepthStreamKinectReceiver to the preprocessor to enable opencv dependancies.

```c++
#define ofxDepthStreamKinectReceiver
```

Generic stream receiver for kinect (Kinect V2 and KinectAzure) depth map data.