import {
  SearchMode,
  PrimaryKeyMode,
  AppleCoreMLInferenceMode,
  DetectMode,
  CameraRotation,
  ImageFormat,
} from '../enums';

/**
 * These enum values are passed straight through to the InspireFace C API as
 * integers (HF_DETECT_MODE_LIGHT_TRACK == 1, HF_STREAM_BGR == 1, ...). If anyone
 * reorders a member, native silently misbehaves with zero compile error. Pin
 * the numbers to the C ABI so a reorder fails loudly here instead.
 */
describe('enum values are ABI-locked to the InspireFace C SDK', () => {
  it('SearchMode', () => {
    expect(SearchMode.EAGER).toBe(0);
    expect(SearchMode.EXHAUSTIVE).toBe(1);
  });

  it('PrimaryKeyMode', () => {
    expect(PrimaryKeyMode.AUTO_INCREMENT).toBe(0);
    expect(PrimaryKeyMode.MANUAL_INPUT).toBe(1);
  });

  it('AppleCoreMLInferenceMode', () => {
    expect(AppleCoreMLInferenceMode.CPU).toBe(0);
    expect(AppleCoreMLInferenceMode.GPU).toBe(1);
    expect(AppleCoreMLInferenceMode.ANE).toBe(2);
  });

  it('DetectMode', () => {
    expect(DetectMode.ALWAYS_DETECT).toBe(0);
    expect(DetectMode.LIGHT_TRACK).toBe(1);
    expect(DetectMode.TRACK_BY_DETECTION).toBe(2);
  });

  it('CameraRotation (0/90/180/270 -> 0..3)', () => {
    expect(CameraRotation.ROTATION_0).toBe(0);
    expect(CameraRotation.ROTATION_90).toBe(1);
    expect(CameraRotation.ROTATION_180).toBe(2);
    expect(CameraRotation.ROTATION_270).toBe(3);
  });

  it('ImageFormat (must match HF_STREAM_* order used in the C++ switch)', () => {
    expect(ImageFormat.RGB).toBe(0);
    expect(ImageFormat.BGR).toBe(1);
    expect(ImageFormat.RGBA).toBe(2);
    expect(ImageFormat.BGRA).toBe(3);
    expect(ImageFormat.YUV_NV12).toBe(4);
    expect(ImageFormat.YUV_NV21).toBe(5);
    expect(ImageFormat.I420).toBe(6);
    expect(ImageFormat.GRAY).toBe(7);
  });
});
