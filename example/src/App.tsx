import { View, StyleSheet } from 'react-native';
import InspireFace, {
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
