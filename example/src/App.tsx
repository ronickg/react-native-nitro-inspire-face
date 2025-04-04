import { Image, StyleSheet, View } from 'react-native';
import {
  AssetManager,
  CameraRotation,
  DetectMode,
  ImageFormat,
  InspireFace,
  PrimaryKeyMode,
  SearchMode,
  type FaceFeatureIdentity,
  type SessionCustomParameter,
} from 'react-native-nitro-inspire-face';
import { useEffect } from 'react';

InspireFace.featureHubDataEnable({
  enablePersistence: false,
  persistenceDbPath: 'bob.db',
  searchThreshold: 0.42,
  searchMode: SearchMode.EXHAUSTIVE,
  primaryKeyMode: PrimaryKeyMode.AUTO_INCREMENT,
});
InspireFace.featureHubFaceSearchThresholdSetting(0.42);
InspireFace.launch('Pikachu');

AssetManager.copyAssetToFile(
  'kun.jpg',
  `${AssetManager.getFilesDirectory()}/kun.jpg`
);

export default function App() {
  useEffect(() => {
    const initFaceDetection = () => {
      try {
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
        session.setFilterMinimumFacePixelSize(0);

        for (let j = 0; j < 1; j++) {
          const bitmap = InspireFace.createImageBitmapFromFilePath(
            3,
            `${AssetManager.getFilesDirectory()}/kun.jpg`
          );
          const imageStream = InspireFace.createImageStreamFromBitmap(
            bitmap,
            CameraRotation.ROTATION_90
          );
          imageStream.setFormat(ImageFormat.BGR);
          imageStream.setRotation(CameraRotation.ROTATION_0);
          const multipleFaceData = session.executeFaceTrack(imageStream);
          console.log('multipleFaceData', multipleFaceData.length);
          if (multipleFaceData.length > 0 && multipleFaceData[0]) {
            console.log(multipleFaceData[0].rect);
            const lmk = InspireFace.getFaceDenseLandmarkFromFaceToken(
              multipleFaceData[0].token
            );
            console.log('lmk', lmk.length);
            const feature = session.extractFaceFeature(
              imageStream,
              multipleFaceData[0].token
            );
            for (let i = 0; i < 10; i++) {
              const result = InspireFace.featureHubFaceInsert({
                id: -1,
                feature,
              });
              console.log('result', result);
            }
            console.log('Feature size: ', feature.byteLength);
            const searched = InspireFace.featureHubFaceSearch(feature);
            if (searched) {
              console.log(
                'searched',
                searched.id,
                'confidence',
                searched.confidence
              );
            }
            const topKResults = InspireFace.featureHubFaceSearchTopK(
              feature,
              10
            );
            console.log('topKResults', topKResults.length);
            topKResults.forEach((result) => {
              console.log(
                'TopK id: ',
                result.id,
                'Confidence: ',
                result.confidence
              );
            });
            // const newFeature = new Array(InspireFace.featureLength).fill(0);
            const newFeature = new Float32Array(InspireFace.featureLength)
              .buffer;

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
            const start = performance.now();
            let queryIdentity = InspireFace.featureHubGetFaceIdentity(4);
            if (queryIdentity) {
              console.log('Query identity: ', queryIdentity.id);
            } else {
              console.log('Query identity failed');
            }
            queryIdentity = InspireFace.featureHubGetFaceIdentity(2);
            if (queryIdentity) {
              console.log('strFt', queryIdentity.feature.byteLength);
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
            const end = performance.now();
            console.log(`Time taken: ${end - start} milliseconds`);
            const imageStream1 = InspireFace.createImageStreamFromBitmap(
              session.getFaceAlignmentImage(
                imageStream,
                multipleFaceData[0].token
              ),
              CameraRotation.ROTATION_0
            );
            imageStream1.writeImageToFile(
              `${AssetManager.getFilesDirectory()}/kun_circle.jpg`
            );
          }
          // const imageBitmap = imageStream.createImageBitmap(false, 1);
          // imageBitmap.drawRect(
          //   { x: 100, y: 100, width: 100, height: 100 },
          //   { r: 255, g: 0, b: 0 },
          //   4
          // );
          // imageBitmap.drawCircle(
          //   { x: 150, y: 100 },
          //   200,
          //   { r: 0, g: 255, b: 0 },
          //   20
          // );
          // imageBitmap.writeToFile(
          //   `${AssetManager.getFilesDirectory()}/kun_circle1.jpg`
          // );

          // InspireFace.
          // console.log('imageBitmap', imageBitmap.width);

          // InspireFace.setAppleCoreMLInferenceMode(AppleCoreMLInferenceMode.CPU);

          console.log(InspireFace.getRecommendedCosineThreshold());

          imageStream.dispose();
          bitmap.dispose();
        }
        session.dispose();
      } catch (err) {
        console.log('err', err);
      }
    };
    initFaceDetection();
  }, []);

  return (
    <View style={styles.container}>
      <Image
        source={{
          uri: `file://${AssetManager.getFilesDirectory()}/kun_circle.jpg`,
        }}
        style={{ width: 200, height: 300 }}
      />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  error: {
    color: 'red',
    marginTop: 10,
  },
});
