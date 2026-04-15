import { Image, ScrollView, StyleSheet, Text, View } from 'react-native';
import {
  AssetManager,
  CameraRotation,
  DetectMode,
  ImageFormat,
  InspireFace,
  LandmarkEngine,
  InspireFaceLogLevel,
  PrimaryKeyMode,
  SearchMode,
} from 'react-native-nitro-inspire-face';
import { useEffect, useState } from 'react';

type TestResult = {
  name: string;
  passed: boolean;
  detail: string;
};

function runTest(name: string, fn: () => string): TestResult {
  try {
    const detail = fn();
    return { name, passed: true, detail };
  } catch (err: any) {
    return { name, passed: false, detail: err.message ?? String(err) };
  }
}

export default function App() {
  const [results, setResults] = useState<TestResult[]>([]);
  const [imageUri, setImageUri] = useState<string | null>(null);

  useEffect(() => {
    const runAllTests = () => {
      const testResults: TestResult[] = [];

      // ──────────────────────────────────────────────
      // Setup
      // ──────────────────────────────────────────────
      try {
        InspireFace.setLogLevel(InspireFaceLogLevel.LOG_INFO);
        InspireFace.launch('Megatron');
        InspireFace.featureHubDataEnable({
          enablePersistence: false,
          persistenceDbPath: 'test.db',
          searchThreshold: 0.42,
          searchMode: SearchMode.EXHAUSTIVE,
          primaryKeyMode: PrimaryKeyMode.AUTO_INCREMENT,
        });
        InspireFace.featureHubFaceSearchThresholdSetting(0.42);
        AssetManager.copyAssetToFile(
          'kun.jpg',
          `${AssetManager.getFilesDirectory()}/kun.jpg`
        );
      } catch (err: any) {
        testResults.push({
          name: 'Setup',
          passed: false,
          detail: err.message ?? String(err),
        });
        setResults(testResults);
        return;
      }

      // ──────────────────────────────────────────────
      // New InspireFace functions
      // ──────────────────────────────────────────────

      testResults.push(
        runTest('isLaunched', () => {
          const launched = InspireFace.isLaunched();
          if (!launched) throw new Error('Expected SDK to be launched');
          return `isLaunched = ${launched}`;
        })
      );

      testResults.push(
        runTest('setLogLevel', () => {
          InspireFace.setLogLevel(InspireFaceLogLevel.LOG_WARN);
          InspireFace.setLogLevel(InspireFaceLogLevel.LOG_INFO);
          return 'Set to WARN then back to INFO';
        })
      );

      testResults.push(
        runTest('logDisable + re-enable', () => {
          InspireFace.logDisable();
          InspireFace.setLogLevel(InspireFaceLogLevel.LOG_INFO);
          return 'Disabled and re-enabled logging';
        })
      );

      testResults.push(
        runTest('getExtendedInformation', () => {
          const info = InspireFace.getExtendedInformation();
          if (typeof info !== 'string') throw new Error('Expected string');
          return info.length > 0 ? info.substring(0, 80) : '(empty)';
        })
      );

      testResults.push(
        runTest('switchLandmarkEngine', () => {
          InspireFace.switchLandmarkEngine(LandmarkEngine.HYPLMV2_025);
          return 'Switched to HYPLMV2_025';
        })
      );

      testResults.push(
        runTest('getSupportedDetectPixelLevels', () => {
          const levels = InspireFace.getSupportedDetectPixelLevels();
          if (!Array.isArray(levels)) throw new Error('Expected array');
          return `Levels: [${levels.join(', ')}]`;
        })
      );

      testResults.push(
        runTest(
          'version / featureLength / faceDenseLandmarkLength / faceBasicTokenLength',
          () => {
            const v = InspireFace.version;
            const fl = InspireFace.featureLength;
            const dl = InspireFace.faceDenseLandmarkLength;
            const tl = InspireFace.faceBasicTokenLength;
            return `v${v}, feat=${fl}, dense=${dl}, token=${tl}`;
          }
        )
      );

      testResults.push(
        runTest('getRecommendedCosineThreshold', () => {
          const t = InspireFace.getRecommendedCosineThreshold();
          if (typeof t !== 'number') throw new Error('Expected number');
          return `threshold = ${t}`;
        })
      );

      testResults.push(
        runTest('cosineSimilarityConvertToPercentage', () => {
          const pct = InspireFace.cosineSimilarityConvertToPercentage(0.5);
          return `0.5 -> ${pct}%`;
        })
      );

      testResults.push(
        runTest(
          'getCosineSimilarityConverter / updateCosineSimilarityConverter',
          () => {
            const cfg = InspireFace.getCosineSimilarityConverter();
            InspireFace.updateCosineSimilarityConverter(cfg);
            return `threshold=${cfg.threshold}, steepness=${cfg.steepness}`;
          }
        )
      );

      // ──────────────────────────────────────────────
      // Session creation & configuration
      // ──────────────────────────────────────────────
      let session: ReturnType<typeof InspireFace.createSession> | null = null;

      testResults.push(
        runTest('createSession', () => {
          session = InspireFace.createSession(
            {
              enableRecognition: true,
              enableFaceQuality: true,
              enableFaceAttribute: true,
              enableInteractionLiveness: true,
              enableLiveness: true,
              enableMaskDetect: true,
              enableFaceEmotion: true,
            },
            DetectMode.ALWAYS_DETECT,
            10,
            -1,
            -1
          );
          return 'Session created';
        })
      );

      testResults.push(
        runTest('session.setTrackPreviewSize + getTrackPreviewSize', () => {
          if (!session) throw new Error('No session');
          session.setTrackPreviewSize(320);
          const size = session.getTrackPreviewSize();
          if (size !== 320) throw new Error(`Expected 320 got ${size}`);
          return `previewSize = ${size}`;
        })
      );

      testResults.push(
        runTest('session.setFaceDetectThreshold', () => {
          if (!session) throw new Error('No session');
          session.setFaceDetectThreshold(0.5);
          return 'threshold = 0.5';
        })
      );

      testResults.push(
        runTest('session.setFilterMinimumFacePixelSize', () => {
          if (!session) throw new Error('No session');
          session.setFilterMinimumFacePixelSize(0);
          return 'minFacePixelSize = 0';
        })
      );

      testResults.push(
        runTest('session.setTrackModeSmoothRatio', () => {
          if (!session) throw new Error('No session');
          session.setTrackModeSmoothRatio(0.05);
          return 'smoothRatio = 0.05';
        })
      );

      testResults.push(
        runTest('session.setTrackModeNumSmoothCacheFrame', () => {
          if (!session) throw new Error('No session');
          session.setTrackModeNumSmoothCacheFrame(5);
          return 'numSmoothCacheFrame = 5';
        })
      );

      testResults.push(
        runTest('session.setTrackModeDetectInterval', () => {
          if (!session) throw new Error('No session');
          session.setTrackModeDetectInterval(20);
          return 'detectInterval = 20';
        })
      );

      testResults.push(
        runTest('session.setTrackLostRecoveryMode', () => {
          if (!session) throw new Error('No session');
          session.setTrackLostRecoveryMode(false);
          return 'trackLostRecovery = false';
        })
      );

      testResults.push(
        runTest('session.setLightTrackConfidenceThreshold', () => {
          if (!session) throw new Error('No session');
          session.setLightTrackConfidenceThreshold(0.1);
          return 'lightTrackThreshold = 0.1';
        })
      );

      // ──────────────────────────────────────────────
      // Image processing: bitmap, stream, face tracking
      // ──────────────────────────────────────────────
      const filePath = `${AssetManager.getFilesDirectory()}/kun.jpg`;
      let bitmap: ReturnType<
        typeof InspireFace.createImageBitmapFromFilePath
      > | null = null;
      let imageStream: ReturnType<
        typeof InspireFace.createImageStreamFromBitmap
      > | null = null;
      let faceToken: ArrayBuffer | null = null;
      let feature: ArrayBuffer | null = null;

      testResults.push(
        runTest('createImageBitmapFromFilePath', () => {
          bitmap = InspireFace.createImageBitmapFromFilePath(3, filePath);
          return `${bitmap.width}x${bitmap.height} ch=${bitmap.channels}`;
        })
      );

      testResults.push(
        runTest('ImageBitmap.data (zero-copy getter)', () => {
          if (!bitmap) throw new Error('No bitmap');
          const data = bitmap.data;
          const expectedSize = bitmap.width * bitmap.height * bitmap.channels;
          if (data.byteLength !== expectedSize)
            throw new Error(`Expected ${expectedSize}, got ${data.byteLength}`);
          return `${data.byteLength} bytes (${bitmap.width}x${bitmap.height}x${bitmap.channels})`;
        })
      );

      testResults.push(
        runTest('createImageBitmapFromBuffer', () => {
          if (!bitmap) throw new Error('No bitmap');
          const data = bitmap.data;
          const bmpFromBuf = InspireFace.createImageBitmapFromBuffer(
            data,
            bitmap.width,
            bitmap.height,
            bitmap.channels
          );
          const w = bmpFromBuf.width;
          const h = bmpFromBuf.height;
          const ch = bmpFromBuf.channels;
          const matches =
            w === bitmap.width && h === bitmap.height && ch === bitmap.channels;
          if (!matches) throw new Error('Dimensions mismatch');
          bmpFromBuf.dispose();
          return `Created ${w}x${h}x${ch} from buffer`;
        })
      );

      testResults.push(
        runTest('ImageBitmap.copy', () => {
          if (!bitmap) throw new Error('No bitmap');
          const bitmapCopy = bitmap.copy();
          const w = bitmapCopy.width;
          const h = bitmapCopy.height;
          const matches = w === bitmap.width && h === bitmap.height;
          if (!matches) throw new Error('Copy dimensions mismatch');
          bitmapCopy.dispose();
          return `Copied ${w}x${h}`;
        })
      );

      testResults.push(
        runTest('ImageBitmap.writeToFile', () => {
          if (!bitmap) throw new Error('No bitmap');
          const outPath = `${AssetManager.getFilesDirectory()}/test_write.jpg`;
          bitmap.writeToFile(outPath);
          return 'Wrote bitmap to file';
        })
      );

      testResults.push(
        runTest('ImageBitmap.drawRect + drawCircleF + drawCircle', () => {
          if (!bitmap) throw new Error('No bitmap');
          const bmpDraw = bitmap.copy();
          bmpDraw.drawRect(
            { x: 10, y: 10, width: 50, height: 50 },
            { r: 255, g: 0, b: 0 },
            2
          );
          bmpDraw.drawCircleF(
            { x: 50.5, y: 50.5 },
            20,
            { r: 0, g: 255, b: 0 },
            2
          );
          bmpDraw.drawCircle({ x: 100, y: 100 }, 15, { r: 0, g: 0, b: 255 }, 2);
          bmpDraw.writeToFile(
            `${AssetManager.getFilesDirectory()}/test_draw.jpg`
          );
          bmpDraw.dispose();
          return 'Drew rect (red), circleF (green), circle (blue)';
        })
      );

      testResults.push(
        runTest('createImageStreamFromBitmap', () => {
          if (!bitmap) throw new Error('No bitmap');
          imageStream = InspireFace.createImageStreamFromBitmap(
            bitmap,
            CameraRotation.ROTATION_0
          );
          imageStream.setFormat(ImageFormat.BGR);
          imageStream.setRotation(CameraRotation.ROTATION_0);
          return 'Stream created, format=BGR, rotation=0';
        })
      );

      testResults.push(
        runTest('ImageStream.setFormat (multiple formats)', () => {
          if (!imageStream) throw new Error('No stream');
          imageStream.setFormat(ImageFormat.BGR);
          imageStream.setFormat(ImageFormat.RGB);
          imageStream.setFormat(ImageFormat.BGR);
          return 'Switched BGR->RGB->BGR';
        })
      );

      testResults.push(
        runTest('ImageStream.setRotation (multiple rotations)', () => {
          if (!imageStream) throw new Error('No stream');
          imageStream.setRotation(CameraRotation.ROTATION_90);
          imageStream.setRotation(CameraRotation.ROTATION_180);
          imageStream.setRotation(CameraRotation.ROTATION_270);
          imageStream.setRotation(CameraRotation.ROTATION_0);
          return 'Cycled 90->180->270->0';
        })
      );

      testResults.push(
        runTest('ImageStream.writeImageToFile', () => {
          if (!imageStream) throw new Error('No stream');
          const outPath = `${AssetManager.getFilesDirectory()}/test_stream.jpg`;
          imageStream.writeImageToFile(outPath);
          return 'Wrote stream to file';
        })
      );

      testResults.push(
        runTest('ImageStream.createImageBitmap', () => {
          if (!imageStream) throw new Error('No stream');
          const bmpFromStream = imageStream.createImageBitmap(false, 1.0);
          if (bmpFromStream.width <= 0 || bmpFromStream.height <= 0)
            throw new Error('Invalid dimensions');
          const result = `${bmpFromStream.width}x${bmpFromStream.height}`;
          bmpFromStream.dispose();
          return result;
        })
      );

      testResults.push(
        runTest('executeFaceTrack', () => {
          if (!session || !imageStream)
            throw new Error('Missing session/stream');
          const faces = session.executeFaceTrack(imageStream);
          if (faces.length === 0) throw new Error('No faces detected');
          faceToken = faces[0]!.token;
          const f = faces[0]!;
          return `${faces.length} face(s), rect=${JSON.stringify(f.rect)}, trackId=${f.trackId}, conf=${f.detConfidence.toFixed(2)}, angles=(r=${f.angle.roll.toFixed(1)},y=${f.angle.yaw.toFixed(1)},p=${f.angle.pitch.toFixed(1)})`;
        })
      );

      testResults.push(
        runTest('session.clearTrackingFace', () => {
          if (!session || !imageStream) throw new Error('No session');
          session.clearTrackingFace();
          // Track again — should get new track IDs
          const faces = session.executeFaceTrack(imageStream);
          faceToken = faces.length > 0 ? faces[0]!.token : faceToken;
          return `Cleared, re-tracked: ${faces.length} face(s)`;
        })
      );

      // ──────────────────────────────────────────────
      // Session functions that need a face
      // ──────────────────────────────────────────────

      testResults.push(
        runTest('session.faceQualityDetect', () => {
          if (!session || !faceToken) throw new Error('Missing session/token');
          const quality = session.faceQualityDetect(faceToken);
          if (typeof quality !== 'number') throw new Error('Expected number');
          if (quality < 0 || quality > 1)
            throw new Error(`Out of range: ${quality}`);
          return `quality = ${quality.toFixed(4)}`;
        })
      );

      testResults.push(
        runTest('extractFaceFeature', () => {
          if (!session || !imageStream || !faceToken)
            throw new Error('Missing deps');
          feature = session.extractFaceFeature(imageStream, faceToken);
          if (!feature || feature.byteLength === 0)
            throw new Error('Empty feature');
          const expectedBytes = InspireFace.featureLength * 4;
          if (feature.byteLength !== expectedBytes)
            throw new Error(
              `Expected ${expectedBytes}, got ${feature.byteLength}`
            );
          return `Feature: ${feature.byteLength} bytes (${InspireFace.featureLength} floats)`;
        })
      );

      testResults.push(
        runTest('getFaceAlignmentImage', () => {
          if (!session || !imageStream || !faceToken)
            throw new Error('Missing deps');
          const alignedBitmap = session.getFaceAlignmentImage(
            imageStream,
            faceToken
          );
          const w = alignedBitmap.width;
          const h = alignedBitmap.height;
          if (w <= 0 || h <= 0) throw new Error('Invalid alignment image');
          alignedBitmap.dispose();
          return `Aligned face: ${w}x${h}`;
        })
      );

      testResults.push(
        runTest('extractFaceFeatureFromAlignmentImage', () => {
          if (!session || !imageStream || !faceToken)
            throw new Error('Missing deps');
          const alignedBitmap = session.getFaceAlignmentImage(
            imageStream,
            faceToken
          );
          const alignedStream = InspireFace.createImageStreamFromBitmap(
            alignedBitmap,
            CameraRotation.ROTATION_0
          );
          const alignedFeature =
            session.extractFaceFeatureFromAlignmentImage(alignedStream);
          if (!alignedFeature || alignedFeature.byteLength === 0)
            throw new Error('Empty aligned feature');
          // Compare with normal feature — should be similar
          if (feature) {
            const sim = InspireFace.faceComparison(feature, alignedFeature);
            alignedStream.dispose();
            alignedBitmap.dispose();
            return `${alignedFeature.byteLength} bytes, similarity to normal=${sim.toFixed(3)}`;
          }
          alignedStream.dispose();
          alignedBitmap.dispose();
          return `${alignedFeature.byteLength} bytes`;
        })
      );

      // ──────────────────────────────────────────────
      // Pipeline processing & all result getters
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('multipleFacePipelineProcess', () => {
          if (!session || !imageStream) throw new Error('Missing deps');
          const faces = session.executeFaceTrack(imageStream);
          if (faces.length === 0) throw new Error('No faces');
          const ok = session.multipleFacePipelineProcess(imageStream, faces, {
            enableFaceQuality: true,
            enableLiveness: true,
            enableMaskDetect: true,
            enableFaceAttribute: true,
            enableInteractionLiveness: true,
            enableFaceEmotion: true,
          });
          if (!ok) throw new Error('Pipeline failed');
          return `Processed ${faces.length} face(s)`;
        })
      );

      testResults.push(
        runTest('getRGBLivenessConfidence', () => {
          if (!session) throw new Error('No session');
          const vals = session.getRGBLivenessConfidence();
          if (vals.length === 0) throw new Error('Empty');
          return `[${vals.map((v) => v.toFixed(3)).join(', ')}]`;
        })
      );

      testResults.push(
        runTest('getFaceQualityConfidence', () => {
          if (!session) throw new Error('No session');
          const vals = session.getFaceQualityConfidence();
          if (vals.length === 0) throw new Error('Empty');
          return `[${vals.map((v) => v.toFixed(3)).join(', ')}]`;
        })
      );

      testResults.push(
        runTest('getFaceMaskConfidence', () => {
          if (!session) throw new Error('No session');
          const vals = session.getFaceMaskConfidence();
          if (vals.length === 0) throw new Error('Empty');
          return `[${vals.map((v) => v.toFixed(3)).join(', ')}]`;
        })
      );

      testResults.push(
        runTest('getFaceInteractionState', () => {
          if (!session) throw new Error('No session');
          const states = session.getFaceInteractionState();
          if (states.length === 0) throw new Error('Empty');
          const s = states[0]!;
          return `left=${s.left.toFixed(2)}, right=${s.right.toFixed(2)}`;
        })
      );

      testResults.push(
        runTest('getFaceInteractionActionsResult', () => {
          if (!session) throw new Error('No session');
          const actions = session.getFaceInteractionActionsResult();
          if (actions.length === 0) throw new Error('Empty');
          const a = actions[0]!;
          return `normal=${a.normal}, shake=${a.shake}, jaw=${a.jawOpen}, raise=${a.headRaise}, blink=${a.blink}`;
        })
      );

      testResults.push(
        runTest('getFaceAttributeResult', () => {
          if (!session) throw new Error('No session');
          const attrs = session.getFaceAttributeResult();
          if (attrs.length === 0) throw new Error('Empty');
          const a = attrs[0]!;
          const genders = ['Female', 'Male'];
          const races = ['Black', 'Asian', 'Latino', 'MiddleEastern', 'White'];
          return `age=${a.ageBracket}, gender=${genders[a.gender] ?? a.gender}, race=${races[a.race] ?? a.race}`;
        })
      );

      testResults.push(
        runTest('getFaceEmotionResult', () => {
          if (!session) throw new Error('No session');
          const emotions = session.getFaceEmotionResult();
          if (emotions.length === 0) throw new Error('Empty');
          const labels = [
            'Neutral',
            'Happy',
            'Sad',
            'Surprise',
            'Fear',
            'Disgust',
            'Anger',
          ];
          const e = emotions[0]!;
          return `emotion=${labels[e.emotion] ?? e.emotion}`;
        })
      );

      // ──────────────────────────────────────────────
      // FeatureHub full lifecycle
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('featureHubFaceInsert', () => {
          if (!feature) throw new Error('No feature');
          const id1 = InspireFace.featureHubFaceInsert({ id: -1, feature });
          const id2 = InspireFace.featureHubFaceInsert({ id: -1, feature });
          const id3 = InspireFace.featureHubFaceInsert({ id: -1, feature });
          return `Inserted ids: ${id1}, ${id2}, ${id3}`;
        })
      );

      testResults.push(
        runTest('featureHubGetFaceCount', () => {
          const count = InspireFace.featureHubGetFaceCount();
          if (count < 3) throw new Error(`Expected >= 3, got ${count}`);
          return `count = ${count}`;
        })
      );

      testResults.push(
        runTest('featureHubGetExistingIds', () => {
          const ids = InspireFace.featureHubGetExistingIds();
          if (ids.length < 3)
            throw new Error(`Expected >= 3, got ${ids.length}`);
          return `ids = [${ids.join(', ')}]`;
        })
      );

      testResults.push(
        runTest('featureHubFaceSearch', () => {
          if (!feature) throw new Error('No feature');
          const result = InspireFace.featureHubFaceSearch(feature);
          if (!result) throw new Error('Search returned null');
          return `Found id=${result.id}, conf=${result.confidence?.toFixed(3)}, feat=${result.feature.byteLength}b`;
        })
      );

      testResults.push(
        runTest('featureHubFaceSearchTopK', () => {
          if (!feature) throw new Error('No feature');
          const topKResults = InspireFace.featureHubFaceSearchTopK(feature, 3);
          if (topKResults.length === 0) throw new Error('Empty results');
          return topKResults
            .map((r) => `id=${r.id} conf=${r.confidence.toFixed(3)}`)
            .join(', ');
        })
      );

      testResults.push(
        runTest('featureHubGetFaceIdentity', () => {
          const ids = InspireFace.featureHubGetExistingIds();
          const identity = InspireFace.featureHubGetFaceIdentity(ids[0]!);
          if (!identity) throw new Error('Not found');
          if (identity.feature.byteLength === 0)
            throw new Error('Empty feature');
          return `id=${identity.id}, feat=${identity.feature.byteLength}b`;
        })
      );

      testResults.push(
        runTest('featureHubGetFaceIdentity (non-existent)', () => {
          const result = InspireFace.featureHubGetFaceIdentity(999999);
          if (result !== null)
            throw new Error('Expected null for non-existent id');
          return 'Correctly returned null';
        })
      );

      testResults.push(
        runTest('featureHubFaceUpdate', () => {
          if (!feature) throw new Error('No feature');
          const ids = InspireFace.featureHubGetExistingIds();
          const ok = InspireFace.featureHubFaceUpdate({ id: ids[0]!, feature });
          if (!ok) throw new Error('Update failed');
          return `Updated id=${ids[0]}`;
        })
      );

      testResults.push(
        runTest('featureHubFaceRemove', () => {
          const idsBefore = InspireFace.featureHubGetExistingIds();
          const ok = InspireFace.featureHubFaceRemove(idsBefore[0]!);
          if (!ok) throw new Error('Remove failed');
          const idsAfter = InspireFace.featureHubGetExistingIds();
          if (idsAfter.length !== idsBefore.length - 1)
            throw new Error(
              `Expected ${idsBefore.length - 1}, got ${idsAfter.length}`
            );
          return `Removed id=${idsBefore[0]}, count: ${idsBefore.length} -> ${idsAfter.length}`;
        })
      );

      testResults.push(
        runTest('faceComparison (self)', () => {
          if (!feature) throw new Error('No feature');
          const sim = InspireFace.faceComparison(feature, feature);
          if (sim < 0.99) throw new Error(`Self-comparison too low: ${sim}`);
          return `Self-similarity = ${sim.toFixed(4)}`;
        })
      );

      testResults.push(
        runTest('faceComparison (different)', () => {
          if (!feature) throw new Error('No feature');
          // Create a zeroed feature for comparison
          const zeroFeature = new Float32Array(InspireFace.featureLength)
            .buffer;
          const sim = InspireFace.faceComparison(feature, zeroFeature);
          if (sim > 0.5) throw new Error(`Zero comparison too high: ${sim}`);
          return `vs zeros = ${sim.toFixed(4)}`;
        })
      );

      // ──────────────────────────────────────────────
      // createImageStream (direct buffer) & createEmptyImageStream (zero-copy)
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('createImageStream (from buffer)', () => {
          if (!bitmap) throw new Error('No bitmap');
          const data = bitmap.data;
          const stream = InspireFace.createImageStream(
            data,
            bitmap.width,
            bitmap.height,
            ImageFormat.BGR,
            CameraRotation.ROTATION_0
          );
          if (!session) throw new Error('No session');
          const faces = session.executeFaceTrack(stream);
          stream.dispose();
          return `Buffer stream: detected ${faces.length} face(s)`;
        })
      );

      testResults.push(
        runTest('createEmptyImageStream + setBuffer (zero-copy)', () => {
          if (!bitmap || !session) throw new Error('No bitmap/session');
          const stream = InspireFace.createEmptyImageStream();
          stream.setFormat(ImageFormat.BGR);
          stream.setRotation(CameraRotation.ROTATION_0);
          const data = bitmap.data;
          stream.setBuffer(data, bitmap.width, bitmap.height);
          const faces = session.executeFaceTrack(stream);
          stream.dispose();
          return `Zero-copy stream: detected ${faces.length} face(s)`;
        })
      );

      // ──────────────────────────────────────────────
      // Stress test: zero-copy loop (memory leak check)
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('stress: 100x setBuffer loop (leak check)', () => {
          if (!bitmap || !session) throw new Error('No bitmap/session');
          const stream = InspireFace.createEmptyImageStream();
          stream.setFormat(ImageFormat.BGR);
          stream.setRotation(CameraRotation.ROTATION_0);
          const data = bitmap.data;
          const start = performance.now();
          let totalFaces = 0;
          for (let i = 0; i < 100; i++) {
            stream.setBuffer(data, bitmap.width, bitmap.height);
            const faces = session.executeFaceTrack(stream);
            totalFaces += faces.length;
          }
          const elapsed = performance.now() - start;
          stream.dispose();
          return `100 iters in ${elapsed.toFixed(0)}ms, avg ${(elapsed / 100).toFixed(1)}ms/frame, faces=${totalFaces}`;
        })
      );

      testResults.push(
        runTest('stress: 100x bitmap.data (zero-copy getData check)', () => {
          if (!bitmap) throw new Error('No bitmap');
          const start = performance.now();
          let totalBytes = 0;
          for (let i = 0; i < 100; i++) {
            const d = bitmap.data;
            totalBytes += d.byteLength;
          }
          const elapsed = performance.now() - start;
          return `100x getData in ${elapsed.toFixed(0)}ms, ${totalBytes} total bytes read`;
        })
      );

      testResults.push(
        runTest('stress: 100x old path create+dispose (comparison)', () => {
          if (!session) throw new Error('No session');
          const filePath2 = `${AssetManager.getFilesDirectory()}/kun.jpg`;
          const start = performance.now();
          let totalFaces = 0;
          for (let i = 0; i < 100; i++) {
            const bmp = InspireFace.createImageBitmapFromFilePath(3, filePath2);
            const strm = InspireFace.createImageStreamFromBitmap(
              bmp,
              CameraRotation.ROTATION_0
            );
            strm.setFormat(ImageFormat.BGR);
            strm.setRotation(CameraRotation.ROTATION_0);
            const faces = session.executeFaceTrack(strm);
            totalFaces += faces.length;
            strm.dispose();
            bmp.dispose();
          }
          const elapsed = performance.now() - start;
          return `100 iters in ${elapsed.toFixed(0)}ms, avg ${(elapsed / 100).toFixed(1)}ms/frame, faces=${totalFaces}`;
        })
      );

      // ──────────────────────────────────────────────
      // Dense landmarks & five key points
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('getFaceDenseLandmarkFromFaceToken', () => {
          if (!faceToken) throw new Error('No token');
          const lmk = InspireFace.getFaceDenseLandmarkFromFaceToken(faceToken);
          if (lmk.length === 0) throw new Error('Empty landmarks');
          return `${lmk.length} landmarks, first=(${lmk[0]!.x.toFixed(1)}, ${lmk[0]!.y.toFixed(1)})`;
        })
      );

      testResults.push(
        runTest('getFaceFiveKeyPointsFromFaceToken', () => {
          if (!faceToken) throw new Error('No token');
          const pts = InspireFace.getFaceFiveKeyPointsFromFaceToken(faceToken);
          if (pts.length !== 5)
            throw new Error(`Expected 5, got ${pts.length}`);
          return `5 points, first=(${pts[0]!.x.toFixed(1)}, ${pts[0]!.y.toFixed(1)})`;
        })
      );

      // ──────────────────────────────────────────────
      // base64 round-trip
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('fromBase64 / toBase64 roundtrip', () => {
          if (!feature) throw new Error('No feature');
          const b64 = InspireFace.toBase64(feature);
          const decoded = InspireFace.fromBase64(b64);
          if (decoded.byteLength !== feature.byteLength)
            throw new Error(
              `Size mismatch: ${decoded.byteLength} vs ${feature.byteLength}`
            );
          return `Roundtrip OK, ${b64.length} chars -> ${decoded.byteLength} bytes`;
        })
      );

      // ──────────────────────────────────────────────
      // Session reconfigure (verify it still works)
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('session.reconfigure (ALWAYS_DETECT, recognition only)', () => {
          if (!session || !imageStream) throw new Error('No session');
          session.reconfigure(
            { enableRecognition: true },
            DetectMode.ALWAYS_DETECT,
            5,
            -1,
            -1
          );
          const faces = session.executeFaceTrack(imageStream);
          if (faces.length === 0) throw new Error('No faces after reconfigure');
          return `Reconfigured, still detects ${faces.length} face(s)`;
        })
      );

      testResults.push(
        runTest('session.reconfigure (LIGHT_TRACK mode)', () => {
          if (!session || !imageStream) throw new Error('No session');
          session.reconfigure(
            { enableRecognition: true, enableFaceQuality: true },
            DetectMode.LIGHT_TRACK,
            10,
            -1,
            -1
          );
          const faces = session.executeFaceTrack(imageStream);
          return `LIGHT_TRACK mode: ${faces.length} face(s)`;
        })
      );

      testResults.push(
        runTest(
          'session.reconfigure (back to ALWAYS_DETECT, all features)',
          () => {
            if (!session || !imageStream) throw new Error('No session');
            session.reconfigure(
              {
                enableRecognition: true,
                enableFaceQuality: true,
                enableFaceAttribute: true,
                enableLiveness: true,
                enableMaskDetect: true,
                enableFaceEmotion: true,
                enableInteractionLiveness: true,
              },
              DetectMode.ALWAYS_DETECT,
              10,
              -1,
              -1
            );
            const faces = session.executeFaceTrack(imageStream);
            if (faces.length === 0) throw new Error('No faces');
            // Verify pipeline still works after reconfigure
            const ok = session.multipleFacePipelineProcess(imageStream, faces, {
              enableFaceQuality: true,
              enableLiveness: true,
            });
            if (!ok) throw new Error('Pipeline failed after reconfigure');
            const quality = session.getFaceQualityConfidence();
            return `All features re-enabled, ${faces.length} face(s), quality=${quality[0]?.toFixed(2)}`;
          }
        )
      );

      // ──────────────────────────────────────────────
      // FeatureHub disable/re-enable
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('featureHubDataDisable + re-enable', () => {
          InspireFace.featureHubDataDisable();
          InspireFace.featureHubDataEnable({
            enablePersistence: false,
            persistenceDbPath: 'test2.db',
            searchThreshold: 0.42,
            searchMode: SearchMode.EXHAUSTIVE,
            primaryKeyMode: PrimaryKeyMode.AUTO_INCREMENT,
          });
          if (!feature) throw new Error('No feature');
          // DB should be empty after re-enable
          const count = InspireFace.featureHubGetFaceCount();
          if (count !== 0) throw new Error(`Expected 0, got ${count}`);
          // Insert and verify
          const id = InspireFace.featureHubFaceInsert({ id: -1, feature });
          const countAfter = InspireFace.featureHubGetFaceCount();
          if (countAfter !== 1)
            throw new Error(`Expected 1, got ${countAfter}`);
          return `Disabled+re-enabled, count: 0 -> ${countAfter}, id=${id}`;
        })
      );

      // ──────────────────────────────────────────────
      // switchLandmarkEngine variations
      // ──────────────────────────────────────────────
      testResults.push(
        runTest('switchLandmarkEngine (all engines)', () => {
          InspireFace.switchLandmarkEngine(LandmarkEngine.HYPLMV2_050);
          InspireFace.switchLandmarkEngine(LandmarkEngine.INSIGHTFACE_2D106);
          InspireFace.switchLandmarkEngine(LandmarkEngine.HYPLMV2_025);
          return 'Cycled: 0.50 -> 2D106 -> 0.25';
        })
      );

      // ──────────────────────────────────────────────
      // Write aligned image for visual check
      // ──────────────────────────────────────────────
      try {
        if (session && imageStream && faceToken) {
          // Re-track after reconfigure
          // eslint-disable-next-line @typescript-eslint/no-explicit-any
          const faces2 = (session as any).executeFaceTrack(imageStream);
          if (faces2.length > 0) {
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            const aligned = (session as any).getFaceAlignmentImage(
              imageStream,
              faces2[0]!.token
            );
            const alignedStream = InspireFace.createImageStreamFromBitmap(
              aligned,
              CameraRotation.ROTATION_0
            );
            const outPath = `${AssetManager.getFilesDirectory()}/kun_aligned.jpg`;
            alignedStream.writeImageToFile(outPath);
            setImageUri(`file://${outPath}`);
            alignedStream.dispose();
            aligned.dispose();
          }
        }
      } catch (_) {
        // Non-critical
      }

      // ──────────────────────────────────────────────
      // Cleanup
      // ──────────────────────────────────────────────
      try {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        (imageStream as any)?.dispose();
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        (bitmap as any)?.dispose();
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        (session as any)?.dispose();
        InspireFace.featureHubDataDisable();
      } catch (_) {}

      setResults(testResults);

      const totalPassed = testResults.filter((r) => r.passed).length;
      const total = testResults.length;
      console.log(
        `\n══════════ TEST RESULTS: ${totalPassed}/${total} passed ══════════`
      );
      testResults.forEach((r) => {
        console.log(`${r.passed ? '✓' : '✗'} ${r.name}: ${r.detail}`);
      });
    };

    runAllTests();
  }, []);

  const passed = results.filter((r) => r.passed).length;
  const failed = results.filter((r) => !r.passed).length;

  return (
    <ScrollView style={styles.container} contentContainerStyle={styles.content}>
      <Text style={styles.header}>
        InspireFace Tests: {passed} passed, {failed} failed
      </Text>

      {results.map((r, i) => (
        <View key={i} style={[styles.row, !r.passed && styles.rowFailed]}>
          <Text style={r.passed ? styles.pass : styles.fail}>
            {r.passed ? '✓' : '✗'} {r.name}
          </Text>
          <Text style={styles.detail} numberOfLines={2}>
            {r.detail}
          </Text>
        </View>
      ))}

      {imageUri && (
        <View style={styles.imageContainer}>
          <Text style={styles.imageLabel}>Aligned face:</Text>
          <Image source={{ uri: imageUri }} style={styles.image} />
        </View>
      )}
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#1a1a2e',
  },
  content: {
    padding: 16,
    paddingTop: 60,
  },
  header: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#e0e0e0',
    marginBottom: 16,
    textAlign: 'center',
  },
  row: {
    backgroundColor: '#16213e',
    borderRadius: 8,
    padding: 10,
    marginBottom: 6,
    borderLeftWidth: 3,
    borderLeftColor: '#4ecca3',
  },
  rowFailed: {
    borderLeftColor: '#e74c3c',
  },
  pass: {
    color: '#4ecca3',
    fontWeight: '600',
    fontSize: 13,
  },
  fail: {
    color: '#e74c3c',
    fontWeight: '600',
    fontSize: 13,
  },
  detail: {
    color: '#a0a0a0',
    fontSize: 11,
    marginTop: 2,
  },
  imageContainer: {
    alignItems: 'center',
    marginTop: 20,
  },
  imageLabel: {
    color: '#e0e0e0',
    fontSize: 14,
    marginBottom: 8,
  },
  image: {
    width: 200,
    height: 300,
    borderRadius: 8,
  },
});
