---
sidebar_position: 3
---

# Basic Usage

This guide demonstrates how to initialize and use the core features of react-native-nitro-inspire-face.

- Initialize the SDK with a model package
- Configure face detection settings
- Process an image for face detection
- Extract facial features
- Properly manage resources

## Loading Models

You can load models in two ways:

### Runtime Model Loading (Recommended)

Load models at runtime from a remote URL. Models are automatically cached after the first download:

```ts
import {
  launchWithRemoteModel,
  DetectMode,
  CameraRotation,
} from 'react-native-nitro-inspire-face';

// Download and launch with remote model
await launchWithRemoteModel(
  'https://github.com/HyperInspire/InspireFace/releases/download/v1.0.0/Pikachu',
  '5037ba1f49905b783a1c973d5d58b834a645922cc2814c8e3ca630a38dc24431', // SHA-256 checksum
  (progress) => {
    console.log(`Download progress: ${(progress * 100).toFixed(0)}%`);
  }
);
```

### Bundled Models

If you bundled models at build time using the Expo plugin:

```ts
import { InspireFace } from 'react-native-nitro-inspire-face';

// Load bundled model
InspireFace.launch('Pikachu');
```

## Face Detection Example

Once you've loaded a model (using either method above), you can start detecting faces:

```ts
import {
  InspireFace,
  AssetManager,
  DetectMode,
  CameraRotation,
} from 'react-native-nitro-inspire-face';

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
```
