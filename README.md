# react-native-nitro-inspire-face

A React Native library that provides face detection, recognition, and analysis capabilities using the InspireFace SDK. This library is built on top of [InspireFace](https://github.com/HyperInspire/InspireFace), a powerful cross-platform face recognition SDK developed in C/C++. Powered by [NitroModules](https://github.com/mrousavy/nitro)

## Features

All features are powered by the underlying [InspireFace C++ SDK](https://github.com/HyperInspire/InspireFace):

- Face detection and tracking
- Face recognition and comparison
- Facial landmarks detection
- Face quality assessment
- Mask detection
- Liveness detection (both silent and cooperative)
- Face attribute analysis
- Face pose estimation
- Face embedding management

## Installation

```sh
# Install both required packages
npm install react-native-nitro-modules react-native-nitro-inspire-face
# or
yarn add react-native-nitro-modules react-native-nitro-inspire-face
# or
bun add react-native-nitro-modules react-native-nitro-inspire-face
```

## Model Package Setup

This library requires model files from the InspireFace SDK to function. The models are available in different packages optimized for various use cases:

- **Pikachu** (Default) - Lightweight edge-side models (~15.8MB)
- **Megatron** - Mobile and server models (~60.2MB)

Download the model packages from the [InspireFace Releases](https://github.com/HyperInspire/InspireFace/releases) page.

### Expo Projects

For Expo projects, configure the model in your `app.json` or `app.config.js`:

```json
{
  "expo": {
    "plugins": [
      [
        "react-native-nitro-inspire-face",
        {
          "modelName": "Pikachu" // or "Megatron"
        }
      ]
    ]
  }
}
```

The plugin will automatically handle the model file placement for both iOS and Android.

### React Native CLI Projects

#### Android Setup

1. Download your chosen model file (e.g., "Pikachu")
2. Create the following directory structure in your project and place the model file:

```
android/app/src/main/assets/
└── Pikachu
```

#### iOS Setup

1. Download your chosen model file (e.g., "Pikachu")
2. In Xcode:
   - Right-click on your project in the navigator
   - Select "Add Files to [Your Project Name]"
   - Add the model file directly to your project
   - Ensure the file is included in your target's "Copy Bundle Resources" build phase

## Usage

```javascript
import {
  InspireFace,
  AssetManager,
  DetectMode,
  SearchMode,
  PrimaryKeyMode,
  CameraRotation,
} from 'react-native-nitro-inspire-face';

// Initialize the SDK with your chosen model package
InspireFace.featureHubDataEnable({
  enablePersistence: false,
  persistenceDbPath: 'fdb.db',
  searchThreshold: 0.42,
  searchMode: SearchMode.EXHAUSTIVE,
  primaryKeyMode: PrimaryKeyMode.AUTO_INCREMENT,
});
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
      },
      DetectMode.ALWAYS_DETECT,
      10,
      -1,
      -1
    );

    // Configure session parameters
    session.setTrackPreviewSize(320);
    session.setFaceDetectThreshold(0.5);

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

For more technical details, visit the [InspireFace repository](https://github.com/HyperInspire/InspireFace).

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## Resources

- [InspireFace](https://github.com/HyperInspire/InspireFace)
- [Marc Rousavy](https://github.com/mrousavy) thanks for all the examples and nitro modules
