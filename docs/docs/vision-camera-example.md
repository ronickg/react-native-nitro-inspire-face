---
sidebar_position: 4
---

import useBaseUrl from '@docusaurus/useBaseUrl'

# Vision Camera Example

This guide demonstrates how to integrate react-native-nitro-inspire-face with [react-native-vision-camera](https://mrousavy.com/react-native-vision-camera/) to perform real-time face detection and landmark tracking. The example also uses:

<div style={{ float: 'right', marginLeft: "10px" }}>
  <img src={useBaseUrl("img/vision-camera-example.png")} style={{ width: 200, height: 'auto' }} alt="Vision Camera Example" />
</div>

- [vision-camera-resize-plugin](https://github.com/mrousavy/vision-camera-resize-plugin) to efficiently resize frames for optimal processing performance
- [@shopify/react-native-skia](https://github.com/Shopify/react-native-skia) for high-performance drawing of facial landmarks on the camera preview

The example shows how to:

- Process camera frames in real-time
- Detect faces and draw facial landmarks
- Properly manage resources in a frame processor

## Example Implementation

Here's a complete example that demonstrates real-time face detection and landmark visualization using Vision Camera:

```typescript
import CameraPermissionGuard from "@/components/guards/CameraPermissionGuard";
import { Skia } from "@shopify/react-native-skia";
import { useRef } from "react";
import { View } from "react-native";
import {
  BoxedInspireFace,
  CameraRotation,
  DetectMode,
  InspireFace,
} from "react-native-nitro-inspire-face";
import { NitroModules } from "react-native-nitro-modules";
import {
  Camera,
  Templates,
  useCameraDevice,
  useCameraFormat,
  useSkiaFrameProcessor,
} from "react-native-vision-camera";
import { useResizePlugin } from "vision-camera-resize-plugin";

//Launch the model package
InspireFace.launch("Pikachu");

export default function Example() {
  let device = useCameraDevice("back");
  const camera = useRef<Camera>(null);
  const { resize } = useResizePlugin();

  const format = useCameraFormat(device, Templates.FrameProcessing);

  const paint = Skia.Paint();
  paint.setColor(Skia.Color("blue"));

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
  session.setTrackPreviewSize(320);
  session.setFaceDetectThreshold(0.5);

  const frameProcessor = useSkiaFrameProcessor((frame) => {
    "worklet";

    // Draw the frame to the canvas
    frame.render();

    const size = 320;
    const frameWidth = frame.height; // 720
    const scaleX = frameWidth / size; // Scale based on processed width
    const cropOffset = (frame.width - frame.height) / 2; // Adjust for cropping

    // Resize frame for processing
    const resized = resize(frame, {
      scale: {
        width: size,
        height: size,
      },
      rotation: "90deg",
      pixelFormat: "bgr",
      dataType: "uint8",
      mirror: true,
    });

    // Unbox InspireFace instance for frame processor
    const unboxedInspireFace = BoxedInspireFace.unbox();

    // Create image bitmap from frame buffer
    const bitmap = unboxedInspireFace.createImageBitmapFromBuffer(
      resized.buffer as ArrayBuffer,
      size,
      size,
      3
    );

    // Create image stream for face detection
    const imageStream = unboxedInspireFace.createImageStreamFromBitmap(
      bitmap,
      CameraRotation.ROTATION_0
    );

    // Unbox session and execute face detection
    const unboxedSession = session.unbox();
    const faces = unboxedSession.executeFaceTrack(imageStream);

    // Draw facial landmarks for each detected face
    for (let i = 0; i < faces.length; i++) {
      const lmk = unboxedInspireFace.getFaceDenseLandmarkFromFaceToken(
        faces[i].token
      );
      const path = Skia.Path.Make();

      // Draw landmark points
      lmk.forEach((point) => {
        path.addCircle(point.y * scaleX + cropOffset, point.x * scaleX, 3);
      });

      // Draw landmarks to canvas
      frame.drawPath(path, paint);
    }

    // Clean up resources
    imageStream.dispose();
    bitmap.dispose();
  }, []);

  //The CameraPermissionGuard is just a wrapper to check for permissions
  return (
    <View style={{ flex: 1 }}>
      <CameraPermissionGuard device={device}>
        <Camera
          ref={camera}
          style={{ flex: 1 }}
          device={device!}
          isActive={true}
          format={format}
          frameProcessor={frameProcessor}
        />
      </CameraPermissionGuard>
    </View>
  );
}
```
