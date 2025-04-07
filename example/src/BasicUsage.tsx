import {
  InspireFace,
  AssetManager,
  DetectMode,
  CameraRotation,
} from 'react-native-nitro-inspire-face';

//Load the model package of choice
InspireFace.launch('Pikachu');

// Example of face detection and feature extraction
const initFaceDetection = () => {
  try {
    // Create a session with desired features
    const session = InspireFace.createSession(
      {
        enableRecognition: true,
        enableFaceQuality: true,
        enableFaceAttribute: true,
        enableInteractionLiveness: true,
        enableLiveness: true,
        enableMaskDetect: true,
      },
      DetectMode.ALWAYS_DETECT,
      10,
      -1, // Detection resolution level (multiple of 160, e.g., 160, 320, 640; default -1 means 320)
      -1 // Frame rate for tracking mode (default -1 means 30fps)
    );

    // Configure session parameters
    session.setTrackPreviewSize(320); //Default 192
    session.setFaceDetectThreshold(0.5);
    session.setFilterMinimumFacePixelSize(0); //Default 0

    // Create image bitmap from file
    const bitmap = InspireFace.createImageBitmapFromFilePath(
      3,
      `${AssetManager.getFilesDirectory()}/face.jpg`
    );

    // Create image stream
    const imageStream = InspireFace.createImageStreamFromBitmap(
      bitmap,
      CameraRotation.ROTATION_0
    );

    // Detect faces
    const multipleFaceData = session.executeFaceTrack(imageStream);

    if (multipleFaceData.length > 0 && multipleFaceData[0]) {
      // Extract face feature
      const feature = session.extractFaceFeature(
        imageStream,
        multipleFaceData[0].token
      );
      console.log('Feature extracted, size:', new Float32Array(feature).length);
    }

    // When processing lots of frames, it's better to dispose the resources (But here it would't really be needed)
    imageStream.dispose();
    bitmap.dispose();
  } catch (err) {
    console.log('Error:', err);
  }
};

initFaceDetection();
