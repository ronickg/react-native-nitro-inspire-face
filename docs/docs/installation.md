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

Download your chosen model package from the [InspireFace Releases](https://github.com/HyperInspire/InspireFace/releases) page.

## Project Configuration

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
          "modelName": "Pikachu" // or "Megatron"
        }
      ]
    ]
  }
}
```

2. Place the model package in your project's root directory.

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
