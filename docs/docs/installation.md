---
sidebar_position: 2
---

import Tabs from '@theme/Tabs'
import TabItem from '@theme/TabItem'
import useBaseUrl from '@docusaurus/useBaseUrl'

# Installation

## Prerequisites

Before installing react-native-nitro-inspire-face, you need to have [react-native-nitro-modules](https://github.com/mrousavy/react-native-nitro) set up in your project.

## Installing the library

Install [react-native-nitro-inspire-face](https://www.npmjs.com/package/react-native-nitro-inspire-face) through your preferred package manager:

<Tabs
groupId="package-manager"
defaultValue="expo"
values={[
{label: 'Expo', value: 'expo'},
{label: 'NPM', value: 'npm'},
{label: 'Yarn', value: 'yarn'},
{label: 'Bun', value: 'bun'}
]}>
<TabItem value="expo">

```bash
npx expo install react-native-nitro-modules react-native-nitro-inspire-face
```

</TabItem>
<TabItem value="npm">

```bash
npm install react-native-nitro-modules react-native-nitro-inspire-face
```

</TabItem>
<TabItem value="yarn">

```bash
yarn add react-native-nitro-modules react-native-nitro-inspire-face
```

</TabItem>
<TabItem value="bun">

```bash
bun add react-native-nitro-modules react-native-nitro-inspire-face
```

</TabItem>
</Tabs>

## Model Package Setup

This library requires model files from the InspireFace SDK to function. The models are available in different packages optimized for various use cases:

- **Pikachu** (Default) - Lightweight edge-side models (~15.8MB)
- **Megatron** - Mobile and server models (~60.2MB)

You can choose between two approaches for loading models:

### Option 1: Runtime Model Loading (Recommended)

Load models at runtime by downloading them from a remote URL (e.g., CDN, GitHub Releases, or Cloudflare R2). This approach significantly reduces your app bundle size and is recommended for most use cases.

**Setup:**
1. Host your model package on a CDN or file server
2. Calculate the SHA-256 checksum of your model file
3. Use the `launchWithRemoteModel()` function in your app code (see [Basic Usage](/docs/basic-usage) for examples)

No build-time configuration is needed for this approach.

### Option 2: Bundle Models at Build Time

Bundle the model package directly with your app. This increases your app size but ensures the model is always available offline.

**Setup:**
Download your chosen model package from the [InspireFace Releases](https://github.com/HyperInspire/InspireFace/releases) page and follow the configuration steps below.

## Project Configuration

:::info
Skip this section if you're using **Runtime Model Loading** (Option 1). The configuration below is only needed if you want to bundle models at build time.
:::


<Tabs
groupId="environment"
defaultValue="expo"
values={[
{label: 'Expo', value: 'expo'},
{label: 'React Native CLI', value: 'rn'}
]}>
<TabItem value="expo">

### Expo Setup

1. Add the plugin to your Expo config (`app.json`, `app.config.json` or `app.config.js`):

```json
{
  "expo": {
    "plugins": [
      [
        "react-native-nitro-inspire-face",
        {
          "modelName": "Pikachu", // Optional: specify model to bundle (e.g., "Pikachu" or "Megatron")
          "modelDir": "assets/models" // Optional: custom directory for model files (defaults to project root)
        }
      ]
    ]
  }
}
```

:::note
If you're using runtime model loading, you can omit the `modelName` property or leave the config empty:
```json
["react-native-nitro-inspire-face"]
```
:::

2. Place the model package in your project's root directory (or in the custom directory specified by `modelDir`).

3. Run the command to copy the model package into both the iOS and Android platforms.

```bash
npx expo prebuild
```

</TabItem>
<TabItem value="rn">

### Android Setup

Place your downloaded model package (e.g., "Pikachu") in the assets directory. If the directory doesn't exist yet, create it first:

```
android/app/src/main/assets/
└── Pikachu
```

### iOS Setup

1. Open your project in Xcode
2. Right-click on your project in the navigator
3. Select "Add Files to [Your Project Name]"
4. Add the model package (e.g., "Pikachu")
5. In the dialog that appears:
   - Ensure "Copy items if needed" is checked
   - Choose your app target
   - Click "Add"
6. Verify the model package appears in your target's "Copy Bundle Resources" build phase

</TabItem>
</Tabs>

## Next Steps

Now that you have installed react-native-nitro-inspire-face and set up the model package, you can start using the library in your project. Check out the [Basic Usage](/docs/basic-usage) guide to learn how to implement face detection and recognition features.
