---
title: SessionCustomParameter
---

# SessionCustomParameter

Custom parameters for configuring a face recognition session. This type holds various flags to enable or disable specific features in the face recognition context, such as face recognition, liveness detection, mask detection, quality assessment, and attribute prediction.

```typescript
type SessionCustomParameter = {
  enableRecognition?: boolean;
  enableLiveness?: boolean;
  enableIrLiveness?: boolean;
  enableMaskDetect?: boolean;
  enableFaceQuality?: boolean;
  enableFaceAttribute?: boolean;
  enableInteractionLiveness?: boolean;
  enableDetectModeLandmark?: boolean;
};
```

## Properties

| Property                    | Type      | Description                                                                        |
| --------------------------- | --------- | ---------------------------------------------------------------------------------- |
| `enableRecognition`         | `boolean` | _(Optional)_ Enable face recognition feature. Default to `false`                   |
| `enableLiveness`            | `boolean` | _(Optional)_ Enable RGB liveness detection feature. Default to `false`             |
| `enableIrLiveness`          | `boolean` | _(Optional)_ Enable IR (Infrared) liveness detection feature. Default to `false`   |
| `enableMaskDetect`          | `boolean` | _(Optional)_ Enable mask detection feature. Default to `false`                     |
| `enableFaceQuality`         | `boolean` | _(Optional)_ Enable face quality assessment feature. Default to `false`            |
| `enableFaceAttribute`       | `boolean` | _(Optional)_ Enable face attribute prediction feature. Default to `false`          |
| `enableInteractionLiveness` | `boolean` | _(Optional)_ Enable interaction for liveness detection feature. Default to `false` |
| `enableDetectModeLandmark`  | `boolean` | _(Optional)_ Enable landmark detection in detection mode. Default to `false`       |
