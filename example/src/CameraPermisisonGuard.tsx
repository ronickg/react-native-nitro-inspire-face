import { Button, Linking, Text, View } from 'react-native';
import { useCallback, useState } from 'react';
import { useCameraPermission } from 'react-native-vision-camera';
import type { CameraDevice } from 'react-native-vision-camera';
import type { PropsWithChildren } from 'react';

type CameraPermissionGuardProps = {
  device: CameraDevice | undefined;
};

const CameraPermissionGuard = ({
  children,
  device,
}: PropsWithChildren<CameraPermissionGuardProps>) => {
  const { hasPermission, requestPermission } = useCameraPermission();
  const [isRequestingPermission, setIsRequestingPermission] = useState(false);
  const [permissionDenied, setPermissionDenied] = useState(false);

  const handleRequestPermission = useCallback(async () => {
    setIsRequestingPermission(true);
    const newPermission = await requestPermission();
    setIsRequestingPermission(false);
    if (!newPermission) {
      console.log('Camera permission denied');
      setPermissionDenied(true);
    } else {
      setPermissionDenied(false);
    }
  }, [requestPermission]);

  const handleOpenSettings = useCallback(() => {
    Linking.openSettings();
  }, []);

  if (!hasPermission) {
    return (
      <View>
        <Text>Camera Permission Required</Text>
        <Text>
          {permissionDenied
            ? 'Camera access was denied. Please enable it in settings to use this feature.'
            : 'We need your permission to use the camera for this feature.'}
        </Text>
        <Button
          onPress={
            permissionDenied ? handleOpenSettings : handleRequestPermission
          }
          title={
            isRequestingPermission
              ? 'Requesting...'
              : permissionDenied
                ? 'Open Settings'
                : 'Grant Permission'
          }
          disabled={isRequestingPermission}
        />

        {permissionDenied && (
          <Button onPress={handleRequestPermission} title="Try Again" />
        )}
      </View>
    );
  }

  if (!device) {
    return (
      <View>
        <Text>No camera detected</Text>
      </View>
    );
  }

  return children;
  // return <View className="flex-1">{children}</View>;
};

export default CameraPermissionGuard;
