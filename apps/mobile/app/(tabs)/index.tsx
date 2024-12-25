import { CameraView, CameraType, useCameraPermissions } from 'expo-camera';
import { BarcodeScanningResult } from 'expo-camera/build/Camera.types';
import { useState, useCallback } from 'react';
import { Button, StyleSheet, Text, TouchableOpacity, View } from 'react-native';

export default function App() {
  const [facing, setFacing] = useState<CameraType>('back');
  const [scanned, setScanned] = useState(false);
  const [qrValue, setQrValue] = useState<string>('');
  const [permission, requestPermission] = useCameraPermissions();

  const handleBarCodeScanned = useCallback(({ type, data }: BarcodeScanningResult) => {
    if (!scanned) {
      setScanned(true);
      setQrValue(data);
      alert(`QR Code scanned: ${data}`);
    }
  }, [scanned]);

  const resetScanner = useCallback(() => {
    setScanned(false);
    setQrValue('');
  }, []);

  if (!permission) {
    return <View />;
  }

  if (!permission.granted) {
    return (
      <View style={styles.container}>
        <Text style={styles.message}>We need your permission to show the camera</Text>
        <Button onPress={requestPermission} title="grant permission" />
      </View>
    );
  }

  return (
    <View style={styles.container}>
      <CameraView 
        style={styles.camera} 
        facing={facing}
        onBarcodeScanned={handleBarCodeScanned}
      >
        <View style={styles.scannerOverlay}>
          <View style={styles.scannerFrame} />
        </View>
        
        <View style={styles.overlay}>
          {qrValue ? (
            <Text style={styles.qrText}>Scanned Value: {qrValue}</Text>
          ) : (
            <Text style={styles.qrText}>Align QR Code within frame</Text>
          )}
        </View>
        
        <View style={styles.buttonContainer}>
          <TouchableOpacity style={styles.button} onPress={() => setFacing(current => (current === 'back' ? 'front' : 'back'))}>
            <Text style={styles.text}>Flip Camera</Text>
          </TouchableOpacity>
          {scanned && (
            <TouchableOpacity 
              style={styles.scanAgainButton} 
              onPress={resetScanner}
            >
              <Text style={styles.text}>Scan Again</Text>
            </TouchableOpacity>
          )}
        </View>
      </CameraView>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
  },
  message: {
    textAlign: 'center',
    paddingBottom: 10,
  },
  camera: {
    flex: 1,
  },
  scannerOverlay: {
    position: 'absolute',
    top: 0,
    left: 0,
    right: 0,
    bottom: 0,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: 'rgba(0,0,0,0.5)',
  },
  scannerFrame: {
    width: 250,
    height: 250,
    borderWidth: 2,
    borderColor: '#fff',
    backgroundColor: 'transparent',
  },
  overlay: {
    position: 'absolute',
    top: '40%',
    left: 0,
    right: 0,
    alignItems: 'center',
  },
  qrText: {
    fontSize: 18,
    color: 'white',
    backgroundColor: 'rgba(0,0,0,0.7)',
    padding: 10,
    borderRadius: 5,
    marginTop: 160,
  },
  buttonContainer: {
    flex: 1,
    flexDirection: 'row',
    backgroundColor: 'transparent',
    margin: 64,
  },
  button: {
    flex: 1,
    alignSelf: 'flex-end',
    alignItems: 'center',
  },
  scanAgainButton: {
    flex: 1,
    alignSelf: 'flex-end',
    alignItems: 'center',
  },
  text: {
    fontSize: 24,
    fontWeight: 'bold',
    color: 'white',
  },
});