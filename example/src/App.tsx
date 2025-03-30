import { View, StyleSheet } from 'react-native';
import InspireFace, {
  CameraRotation,
  DetectMode,
  PrimaryKeyMode,
  SearchMode,
  type FaceFeature,
  type FaceFeatureIdentity,
  type SessionCustomParameter,
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
            // enableIrLiveness: false,
            // enableDetectModeLandmark: false,
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
          console.log('lmk', lmk.length);
          const feature = session.extractFaceFeature(
            imageStream,
            multipleFaceData.tokens[0]
          );

          for (let i = 0; i < 10; i++) {
            const result = InspireFace.featureHubFaceInsert({
              id: -1,
              feature,
            });
            console.log('result', result);
          }

          console.log('Feature size: ', new Float32Array(feature.data).length);

          const searched = InspireFace.featureHubFaceSearch(feature);
          if (searched) {
            console.log(
              'searched',
              searched.id,
              'confidence',
              searched.confidence
            );
          }

          const topKResults = InspireFace.featureHubFaceSearchTopK(feature, 10);
          console.log('topKResults', topKResults.length);
          topKResults.forEach((result) => {
            console.log(
              'TopK id: ',
              result.id,
              'Confidence: ',
              result.confidence
            );
          });

          const newFeature: FaceFeature = {
            size: InspireFace.getFeatureLength(),
            data: new Float32Array(feature.data).buffer,
          };
          const identity: FaceFeatureIdentity = {
            id: 8,
            feature: newFeature,
          };
          const updateSuccess = InspireFace.featureHubFaceUpdate(identity);
          if (updateSuccess) {
            console.log('Update feature success: ' + 8);
          } else {
            console.log('Update feature failed: ' + 8);
          }
          const removeSuccess = InspireFace.featureHubFaceRemove(4);
          if (removeSuccess) {
            console.log('Remove feature success: ' + 4);
          } else {
            console.log('Remove feature failed: ' + 4);
          }
          const topkAgn = InspireFace.featureHubFaceSearchTopK(feature, 10);
          topkAgn.forEach((result) => {
            console.log(
              'Agn TopK id:',
              result.id,
              'Confidence: ',
              result.confidence
            );
          });

          let queryIdentity = InspireFace.featureHubGetFaceIdentity(4);
          if (queryIdentity) {
            console.log('Query identity: ', queryIdentity.id);
          } else {
            console.log('Query identity failed');
          }
          queryIdentity = InspireFace.featureHubGetFaceIdentity(2);
          if (queryIdentity) {
            console.log(
              'strFt',
              new Float32Array(queryIdentity.feature.data).length
            );
            console.log('query id: ', queryIdentity.id);

            const comp = InspireFace.faceComparison(
              queryIdentity.feature,
              feature
            );
            console.log('comp', comp);
          } else {
            console.log('Query identity failed');
          }

          const pipelineNeedParam: SessionCustomParameter = {
            enableFaceQuality: true,
            enableLiveness: true,
            enableMaskDetect: true,
            enableFaceAttribute: true,
            enableInteractionLiveness: true,
          };
          const succPipe = session.multipleFacePipelineProcess(
            imageStream,
            multipleFaceData,
            pipelineNeedParam
          );
          if (succPipe) {
            console.log('pipeline success');
            const rgbLivenessConfidence = session.getRGBLivenessConfidence();
            console.log('rgbLivenessConfidence', rgbLivenessConfidence);
            const faceQualityConfidence = session.getFaceQualityConfidence();
            console.log('faceQualityConfidence', faceQualityConfidence);
            const faceMaskConfidence = session.getFaceMaskConfidence();
            console.log('faceMaskConfidence', faceMaskConfidence);
            const faceInteractionState = session.getFaceInteractionState();
            console.log('faceInteractionState', faceInteractionState);
            const faceInteractionActionsResult =
              session.getFaceInteractionActionsResult();
            console.log(
              'faceInteractionActionsResult',
              faceInteractionActionsResult
            );
            const faceAttributeResult = session.getFaceAttributeResult();
            console.log('faceAttributeResult', faceAttributeResult);
          }
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
