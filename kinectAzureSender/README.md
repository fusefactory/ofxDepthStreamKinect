# exampleKinectSender

This is the sender for kinect azure.

## Recorder

There is the possibility to record the depth image of kinect azure using the GUI (toggle record). 

The recordings are made up of raw images, to use them (see example KinectRecorder) you need to convert them into a video using ffmpeg.

For example:
`ffmpeg -framerate 30 -i %06d.png -b:v 2000000k _output.mov`