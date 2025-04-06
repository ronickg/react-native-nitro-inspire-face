---
sidebar_position: 3
---

# Basic Usage

This guide demonstrates how to initialize and use the core features of react-native-nitro-inspire-face. The example below shows how to:

- Initialize the SDK with a model package
- Configure face detection settings
- Process an image for face detection
- Extract facial features
- Properly manage resources

Here's a complete example that covers these basic operations:

```javascript
import {
  InspireFace,
  AssetManager,
  DetectMode,
  SearchMode,
  PrimaryKeyMode,
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
      -1,
      -1
    );

    // Configure session parameters
    session.setTrackPreviewSize(320);
    session.setFaceDetectThreshold(0.5);
    session.setFilterMinimumFacePixelSize(0);

    // Create image bitmap from file
    const bitmap = InspireFace.createImageBitmapFromFilePath(
      3,
      `${AssetManager.getBaseDirectory()}/face.jpg`
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
      console.log('Feature extracted, size:', feature.length);
    }

    // Clean up resources (Not needed but when processing lots of frames is better)
    imageStream.dispose();
    bitmap.dispose();
    session.dispose();
  } catch (err) {
    console.log('Error:', err);
  }
};
```
