import { View, StyleSheet } from 'react-native';
import InspireFace, {
  CameraRotation,
  DetectMode,
  PrimaryKeyMode,
  SearchMode,
} from 'react-native-nitro-inspire-face';
import RNFS from 'react-native-fs';
import { useEffect } from 'react';

export default function App() {
  useEffect(() => {
    const initFaceDetection = async () => {
      try {
        const fileExists = await RNFS.exists(
          RNFS.DocumentDirectoryPath + '/Pikachu'
        );
        if (!fileExists) {
          console.log('Pikachu does not exist');
          return;
        }

        InspireFace.featureHubDataEnable({
          enablePersistence: false,
          persistenceDbPath: `${RNFS.DocumentDirectoryPath}/f.db`,
          searchThreshold: 0.42,
          searchMode: SearchMode.EXHAUSTIVE,
          primaryKeyMode: PrimaryKeyMode.AUTO_INCREMENT,
        });
        InspireFace.featureHubFaceSearchThresholdSetting(0.42);

        InspireFace.launch(RNFS.DocumentDirectoryPath + '/Pikachu');
        const session = InspireFace.createSession(
          {
            enableRecognition: true,
            enableFaceQuality: true,
            enableFaceAttribute: true,
            enableInteractionLiveness: true,
            enableLiveness: true,
            enableMaskDetect: true,
            enableIrLiveness: false,
            enableDetectModeLandmark: false,
          },
          DetectMode.ALWAYS_DETECT,
          10,
          -1,
          -1
        );
        session.setTrackPreviewSize(320);
        session.setFaceDetectThreshold(0.5);
        session.setFilterMinimumFacePixelSize(0);

        const bitmap = InspireFace.createImageBitmapFromFilePath(
          3,
          RNFS.DocumentDirectoryPath + '/kun.jpg'
        );
        console.log('bitmap', new Uint8Array(bitmap.data).length);
        const imageStream = InspireFace.createImageStreamFromBitmap(
          bitmap,
          CameraRotation.ROTATION_0
        );
        // console.log('imageStream', imageStream);
        imageStream.writeImageToFile(RNFS.DocumentDirectoryPath + '/kun1.jpg');
        const multipleFaceData = session.executeFaceTrack(imageStream);
        console.log('multipleFaceData', multipleFaceData.detectedNum);
        if (multipleFaceData.detectedNum > 0 && multipleFaceData.tokens[0]) {
          const lmk = InspireFace.getFaceDenseLandmarkFromFaceToken(
            multipleFaceData.tokens[0]
          );
          console.log('lmk', lmk);
          console.log('multipleFaceData', multipleFaceData.rects);
        }
        // cosnt;
        // const data = imageStream
      } catch (err) {
        console.log('err', err);
      }
    };

    initFaceDetection();
  }, []);

  return <View style={styles.container} />;
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  error: {
    color: 'red',
    marginTop: 10,
  },
});
